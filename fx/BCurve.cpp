#include "BCurve.hpp"

namespace pt {
namespace fx {

bool BCurve::p_Enable = false;

std::atomic_int BCurve::p_cB[256];

int BCurve::p_B[256];

cv::Mat BCurve::p_Image;

int BCurve::p_Log = 110, BCurve::p_Amp = 100, BCurve::p_Tune = 100;

void BCurve::toggle() {
    if (p_Enable)
        cv::destroyWindow("Blue Curve");
    else {
        cv::namedWindow("Blue Curve");
        cv::createTrackbar("Log x0.01", "Blue Curve", nullptr, 200, p_Tracker, &p_Log);
        cv::createTrackbar("Amp x0.01", "Blue Curve", nullptr, 200, p_Tracker, &p_Amp);
        cv::createTrackbar("Tune x0.01", "Blue Curve", nullptr, 300, p_Tracker, &p_Tune);
        cv::setTrackbarMin("Log x0.01", "Blue Curve", 101);
        cv::setTrackbarMin("Amp x0.01", "Blue Curve", 20);
        cv::setTrackbarMin("Tune x0.01", "Blue Curve", 1);
        cv::setTrackbarPos("Log x0.01", "Blue Curve", 110);
        cv::setTrackbarPos("Amp x0.01", "Blue Curve", 100);
        cv::setTrackbarPos("Tune x0.01", "Blue Curve", 100);
        cv::setMouseCallback("Blue Curve", p_Mouse, p_B);
        for (int i = 0; i < 256; ++i)
            p_B[i] = i;
        p_Image = cv::Mat(cv::Size(280, 280), CV_8UC3);
    }
    p_Enable = !p_Enable;
}

bool BCurve::isEnabled() {
    return p_Enable;
}

void BCurve::effect(cv::Mat& io) {
    if (!p_Enable)
        return;
    for (int i = 0; i < 256; ++i)
        p_cB[i].store(0);
    io.forEach<cv::Vec3b>([&](const cv::Vec3b& px, const int pos[]) {
        p_cB[px[0]].fetch_add(1);
    });
    double base = p_Log * 0.01, amp = p_Amp * 0.01;
    p_Image.forEach<cv::Vec3b>([&](cv::Vec3b& px, const int pos[]) {
        px[0] = px[1] = px[2] = 0;
        if (12 <= pos[0] && pos[0] < 268 && 12 <= pos[1] && pos[1] < 268) {
            int len = log(p_cB[pos[1] - 12].load() + 1) / log(base) * amp;
            if (268 - len <= pos[0] && pos[0] < 268)
                px[0] = px[1] = px[2] = 255;
        }
    });
    io.forEach<cv::Vec3b>([&](cv::Vec3b& px, const int pos[]) {
        px[0] = std::max(0.0, std::min(p_B[px[0]] * p_Tune * 0.01, 255.0));
    });
    for (int i = 1; i < 256; ++i)
        cv::line(p_Image, cv::Point(i + 11, 268 - p_B[i - 1]), cv::Point(i + 12, 268 - p_B[i]), cv::Scalar(0, 0, 255));
    cv::imshow("Blue Curve", p_Image);
}

void BCurve::p_Tracker(int val, void* userdata) {
    *((int*)userdata) = val;
}

void BCurve::p_Mouse(int event, int x, int y, int flags, void* userdata) {
    if (flags & cv::EVENT_FLAG_LBUTTON) {
        if (12 <= x && x < 268 && 12 <= y && y < 268)
            p_B[x - 12] = 268 - y;
    }
}

}
}
