#include "SCurve.hpp"

namespace pt {
namespace fx {

bool SCurve::p_Enable = false;

std::atomic_int SCurve::p_cS[256];

int SCurve::p_S[256];

cv::Mat SCurve::p_Image;

int SCurve::p_Log = 110, SCurve::p_Amp = 100, SCurve::p_Tune = 100;

void SCurve::toggle() {
    if (p_Enable)
        cv::destroyWindow("Saturation Curve");
    else {
        cv::namedWindow("Saturation Curve");
        cv::createTrackbar("Log x0.01", "Saturation Curve", nullptr, 200, p_Tracker, &p_Log);
        cv::createTrackbar("Amp x0.01", "Saturation Curve", nullptr, 200, p_Tracker, &p_Amp);
        cv::createTrackbar("Tune x0.01", "Saturation Curve", nullptr, 300, p_Tracker, &p_Tune);
        cv::setTrackbarMin("Log x0.01", "Saturation Curve", 101);
        cv::setTrackbarMin("Amp x0.01", "Saturation Curve", 20);
        cv::setTrackbarMin("Tune x0.01", "Saturation Curve", 1);
        cv::setTrackbarPos("Log x0.01", "Saturation Curve", 110);
        cv::setTrackbarPos("Amp x0.01", "Saturation Curve", 100);
        cv::setTrackbarPos("Tune x0.01", "Saturation Curve", 100);
        cv::setMouseCallback("Saturation Curve", p_Mouse, p_S);
        for (int i = 0; i < 256; ++i)
            p_S[i] = i;
        p_Image = cv::Mat(cv::Size(280, 280), CV_8UC3);
    }
    p_Enable = !p_Enable;
}

bool SCurve::isEnabled() {
    return p_Enable;
}

void SCurve::effect(cv::Mat& io) {
    if (!p_Enable)
        return;
    for (int i = 0; i < 256; ++i)
        p_cS[i].store(0);
    io.forEach<cv::Vec3b>([&](const cv::Vec3b& px, const int pos[]) {
        double r = px[2] / 255.0, g = px[1] / 255.0, b = px[0] / 255.0;
        double i = (r + g + b) / 3.0;
        double s = 1.0 - (fabs(i) < 1e-8 ? 1.0 : std::min({r, g, b}) / i);
        p_cS[int(s * 255)].fetch_add(1);
    });
    double base = p_Log * 0.01, amp = p_Amp * 0.01;
    p_Image.forEach<cv::Vec3b>([&](cv::Vec3b& px, const int pos[]) {
        px[0] = px[1] = px[2] = 0;
        if (12 <= pos[0] && pos[0] < 268 && 12 <= pos[1] && pos[1] < 268) {
            int len = log(p_cS[pos[1] - 12].load() + 1) / log(base) * amp;
            if (268 - len <= pos[0] && pos[0] < 268)
                px[0] = px[1] = px[2] = 255;
        }
    });
    const double pi3 = CV_PI / 3;
    io.forEach<cv::Vec3b>([&](cv::Vec3b& px, const int pos[]) {
        // Normalize
        double r = px[2] / 255.0, g = px[1] / 255.0, b = px[0] / 255.0;
        // Intensity
        double i = (r + g + b) / 3;
        // Saturation
        double s = 1.0 - (fabs(i) < 1e-8 ? 1.0 : std::min({r, g, b}) / i);
        s *= 255;
        int tl = trunc(s);
        int tr = tl + 1;
        if (tr < 256)
            s = (s - tl) * (p_S[tr] - p_S[tl]) + p_S[tl];
        s /= 255;
        s = std::max(0.0, std::min(s * p_Tune * 0.01, 1.0));
        // Hue
        float h = 0;
        if (fabs(r - g) > 1e-8 || fabs(r - b) > 1e-8 || fabs(g - b) > 1e-8)
            h = acos(0.5 * (r * 2 - g - b) / sqrt(pow(r - g, 2) + (r - b) * (g - b)));
        // Rebuild
        if (h < pi3 * 2) {
            b = i * (1 - s);
            r = i * (1 + s * cos(h) / cos(pi3 - h));
            g = 3 * i - b - r;
        }
        else if (h < pi3 * 4) {
            h -= pi3 * 2;
            r = i * (1 - s);
            g = i * (1 + s * cos(h) / cos(pi3 - h));
            b = 3 * i - g - r;
        }
        else {
            h -= pi3 * 4;
            g = i * (1 - s);
            b = i * (1 + s * cos(h) / cos(pi3 - h));
            r = 3 * i - b - g;
        }
        px[0] = std::max(0.0, std::min(round(b * 255), 255.0));
        px[1] = std::max(0.0, std::min(round(g * 255), 255.0));
        px[2] = std::max(0.0, std::min(round(r * 255), 255.0));
    });
    for (int i = 1; i < 256; ++i)
        cv::line(p_Image, cv::Point(i + 11, 268 - p_S[i - 1]), cv::Point(i + 12, 268 - p_S[i]), cv::Scalar(0, 0, 255));
    cv::imshow("Saturation Curve", p_Image);
}

void SCurve::p_Tracker(int val, void* userdata) {
    *((int*)userdata) = val;
}

void SCurve::p_Mouse(int event, int x, int y, int flags, void* userdata) {
    if (flags & cv::EVENT_FLAG_LBUTTON) {
        if (12 <= x && x < 268 && 12 <= y && y < 268)
            p_S[x - 12] = 268 - y;
    }
}

}
}
