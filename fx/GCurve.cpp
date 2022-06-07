#include "GCurve.hpp"

namespace pt {
namespace fx {

bool GCurve::p_Enable = false;

std::atomic_int GCurve::p_cG[256];

int GCurve::p_G[256];

cv::Mat GCurve::p_Image;

int GCurve::p_Log = 110, GCurve::p_Amp = 100, GCurve::p_Tune = 100;

void GCurve::toggle() {
    if (p_Enable)
        cv::destroyWindow("Green Curve");
    else {
        cv::namedWindow("Green Curve");
        cv::createTrackbar("Log x0.01", "Green Curve", nullptr, 200, p_Tracker, &p_Log);
        cv::createTrackbar("Amp x0.01", "Green Curve", nullptr, 200, p_Tracker, &p_Amp);
        cv::createTrackbar("Tune x0.01", "Green Curve", nullptr, 300, p_Tracker, &p_Tune);
        cv::setTrackbarMin("Log x0.01", "Green Curve", 101);
        cv::setTrackbarMin("Amp x0.01", "Green Curve", 20);
        cv::setTrackbarMin("Tune x0.01", "Green Curve", 1);
        cv::setTrackbarPos("Log x0.01", "Green Curve", 110);
        cv::setTrackbarPos("Amp x0.01", "Green Curve", 100);
        cv::setTrackbarPos("Tune x0.01", "Green Curve", 100);
        cv::setMouseCallback("Green Curve", p_Mouse, p_G);
        for (int i = 0; i < 256; ++i)
            p_G[i] = i;
        p_Image = cv::Mat(cv::Size(280, 280), CV_8UC3);
    }
    p_Enable = !p_Enable;
}

bool GCurve::isEnabled() {
    return p_Enable;
}

void GCurve::effect(cv::Mat& io) {
    if (!p_Enable)
        return;
    for (int i = 0; i < 256; ++i)
        p_cG[i].store(0);
    io.forEach<cv::Vec3b>([&](const cv::Vec3b& px, const int pos[]) {
        p_cG[px[1]].fetch_add(1);
    });
    double base = p_Log * 0.01, amp = p_Amp * 0.01;
    p_Image.forEach<cv::Vec3b>([&](cv::Vec3b& px, const int pos[]) {
        px[0] = px[1] = px[2] = 0;
        if (12 <= pos[0] && pos[0] < 268 && 12 <= pos[1] && pos[1] < 268) {
            int len = log(p_cG[pos[1] - 12].load() + 1) / log(base) * amp;
            if (268 - len <= pos[0] && pos[0] < 268)
                px[0] = px[1] = px[2] = 255;
        }
    });
    io.forEach<cv::Vec3b>([&](cv::Vec3b& px, const int pos[]) {
        px[1] = std::max(0.0, std::min(p_G[px[1]] * p_Tune * 0.01, 255.0));
    });
    for (int i = 1; i < 256; ++i)
        cv::line(p_Image, cv::Point(i + 11, 268 - p_G[i - 1]), cv::Point(i + 12, 268 - p_G[i]), cv::Scalar(0, 0, 255));
    cv::imshow("Green Curve", p_Image);
}

void GCurve::p_Tracker(int val, void* userdata) {
    *((int*)userdata) = val;
}

void GCurve::p_Mouse(int event, int x, int y, int flags, void* userdata) {
    if (flags & cv::EVENT_FLAG_LBUTTON) {
        if (12 <= x && x < 268 && 12 <= y && y < 268)
            p_G[x - 12] = 268 - y;
    }
}

}
}
