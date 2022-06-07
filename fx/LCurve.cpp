#include "LCurve.hpp"

namespace pt {
namespace fx {

bool LCurve::p_Enable = false;

std::atomic_int LCurve::p_cY[256];

int LCurve::p_L[256];

cv::Mat LCurve::p_Image;

int LCurve::p_Log = 110, LCurve::p_Amp = 100, LCurve::p_Tune = 100;

void LCurve::toggle() {
    if (p_Enable)
        cv::destroyWindow("Lightness Curve");
    else {
        cv::namedWindow("Lightness Curve");
        cv::createTrackbar("Log x0.01", "Lightness Curve", nullptr, 200, p_Tracker, &p_Log);
        cv::createTrackbar("Amp x0.01", "Lightness Curve", nullptr, 200, p_Tracker, &p_Amp);
        cv::createTrackbar("Tune x0.01", "Lightness Curve", nullptr, 300, p_Tracker, &p_Tune);
        cv::setTrackbarMin("Log x0.01", "Lightness Curve", 101);
        cv::setTrackbarMin("Amp x0.01", "Lightness Curve", 20);
        cv::setTrackbarMin("Tune x0.01", "Lightness Curve", 1);
        cv::setTrackbarPos("Log x0.01", "Lightness Curve", 110);
        cv::setTrackbarPos("Amp x0.01", "Lightness Curve", 100);
        cv::setTrackbarPos("Tune x0.01", "Lightness Curve", 100);
        cv::setMouseCallback("Lightness Curve", p_Mouse, p_L);
        for (int i = 0; i < 256; ++i)
            p_L[i] = i;
        p_Image = cv::Mat(cv::Size(280, 280), CV_8UC3);
    }
    p_Enable = !p_Enable;
}

bool LCurve::isEnabled() {
    return p_Enable;
}

void LCurve::effect(cv::Mat& io) {
    if (!p_Enable)
        return;
    for (int i = 0; i < 256; ++i)
        p_cY[i].store(0);
    io.forEach<cv::Vec3b>([&](const cv::Vec3b& px, const int pos[]) {
        int Y = 0.299 * px[2] + 0.587 * px[1] + 0.114 * px[0];
        p_cY[Y].fetch_add(1);
    });
    double base = p_Log * 0.01, amp = p_Amp * 0.01;
    p_Image.forEach<cv::Vec3b>([&](cv::Vec3b& px, const int pos[]) {
        px[0] = px[1] = px[2] = 0;
        if (12 <= pos[0] && pos[0] < 268 && 12 <= pos[1] && pos[1] < 268) {
            int len = log(p_cY[pos[1] - 12].load() + 1) / log(base) * amp;
            if (268 - len <= pos[0] && pos[0] < 268)
                px[0] = px[1] = px[2] = 255;
        }
    });
    io.forEach<cv::Vec3b>([&](cv::Vec3b& px, const int pos[]) {
        double Y = 0.299 * px[2] + 0.587 * px[1] + 0.114 * px[0];
        double Cb = -0.168736 * px[2] - 0.331264 * px[1] + 0.5 * px[0];
        double Cr = 0.5 * px[2] - 0.418688 * px[1] - 0.081312 * px[0];
        int l = Y;
        int r = l + 1;
        if (r < 256) 
            Y = ((p_L[r] - p_L[l]) * (Y - l) + p_L[l]) * p_Tune * 0.01;
        Y = std::min(std::max(0.0, Y), 255.0);
        int R = round(Y + 1.402 * Cr);
        int G = round(Y - 0.344136 * Cb - 0.714136 * Cr);
        int B = round(Y + 1.772 * Cb);
        px[2] = std::min(std::max(0, R), 255);
        px[1] = std::min(std::max(0, G), 255);
        px[0] = std::min(std::max(0, B), 255);
    });
    for (int i = 1; i < 256; ++i)
        cv::line(p_Image, cv::Point(i + 11, 268 - p_L[i - 1]), cv::Point(i + 12, 268 - p_L[i]), cv::Scalar(0, 0, 255));
    cv::imshow("Lightness Curve", p_Image);
}

void LCurve::p_Tracker(int val, void* userdata) {
    *((int*)userdata) = val;
}

void LCurve::p_Mouse(int event, int x, int y, int flags, void* userdata) {
    if (flags & cv::EVENT_FLAG_LBUTTON) {
        if (12 <= x && x < 268 && 12 <= y && y < 268)
            p_L[x - 12] = 268 - y;
    }
}

}
}
