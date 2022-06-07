#include "LHistogram.hpp"

namespace pt {
namespace parade {

bool LHistogram::p_Enabled = false;

std::atomic_int LHistogram::p_cY[256];

cv::Mat LHistogram::p_Image;

int LHistogram::p_Log = 110, LHistogram::p_Amp = 100;

void LHistogram::toggle() {
    if (p_Enabled)
        // Destroy window
        cv::destroyWindow("Lightness Histogram");
    else {
        // Create window
        cv::namedWindow("Lightness Histogram");
        cv::createTrackbar("Log x0.01", "Lightness Histogram", nullptr, 200, p_Tracker, &p_Log);
        cv::createTrackbar("Amp x0.01", "Lightness Histogram", nullptr, 200, p_Tracker, &p_Amp);
        cv::setTrackbarMin("Log x0.01", "Lightness Histogram", 101);
        cv::setTrackbarMin("Amp x0.01", "Lightness Histogram", 20);
        cv::setTrackbarPos("Log x0.01", "Lightness Histogram", 110);
        cv::setTrackbarPos("Amp x0.01", "Lightness Histogram", 100);
        p_Image = cv::Mat(cv::Size(280, 280), CV_8UC1);
    }
    p_Enabled = !p_Enabled;
}

bool LHistogram::isEnabled() {
    return p_Enabled;
}

void LHistogram::init() {
    if (!p_Enabled)
        return;
    for (int i = 0; i < 256; ++i)
        p_cY[i].store(0);
}

void LHistogram::staticstic(const cv::Vec3b& px) {
    if (!p_Enabled)
        return;
    int Y = 0.299 * px[2] + 0.587 * px[1] + 0.114 * px[0];
    p_cY[Y].fetch_add(1);
}

void LHistogram::show() {
    if (!p_Enabled)
        return;
    double base = p_Log * 0.01, amp = p_Amp * 0.01;
    p_Image.forEach<uint8_t>([&](uint8_t& px, const int pos[]) {
        px = 0;
        if (12 <= pos[0] && pos[0] < 268 && 12 <= pos[1] && pos[1] < 268) {
            int len = log(p_cY[pos[1] - 12].load() + 1) / log(base) * amp;
            if (268 - len <= pos[0] && pos[0] < 268)
                px = 255;
        }
    });
    cv::imshow("Lightness Histogram", p_Image);
}

void LHistogram::p_Tracker(int val, void* userdata) {
    *((int*)userdata) = val;
}

}
}
