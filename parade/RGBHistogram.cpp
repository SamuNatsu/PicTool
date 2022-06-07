#include "RGBHistogram.hpp"

namespace pt {
namespace parade {

RGBHistogram RGBHistogram::p_Guard;

bool RGBHistogram::p_Enabled = false;

std::atomic_int* RGBHistogram::p_cRGB[3] = {nullptr, nullptr, nullptr};

cv::Mat RGBHistogram::p_Image;

int RGBHistogram::p_Log = 110, RGBHistogram::p_Amp = 100;

RGBHistogram::~RGBHistogram() {
    if (isEnabled())
        toggle();
}

void RGBHistogram::toggle() {
    if (p_Enabled) {
        // Destroy window
        cv::destroyWindow("RGB Histogram");
        // Deallocate
        for (int i = 0; i < 3; ++i)
            delete[] p_cRGB[i];
    }
    else {
        // Create window
        cv::namedWindow("RGB Histogram");
        cv::createTrackbar("Log x0.01", "RGB Histogram", nullptr, 200, p_Tracker, &p_Log);
        cv::createTrackbar("Amp x0.01", "RGB Histogram", nullptr, 200, p_Tracker, &p_Amp);
        cv::setTrackbarMin("Log x0.01", "RGB Histogram", 101);
        cv::setTrackbarMin("Amp x0.01", "RGB Histogram", 20);
        cv::setTrackbarPos("Log x0.01", "RGB Histogram", 110);
        cv::setTrackbarPos("Amp x0.01", "RGB Histogram", 100);
        // Allocate
        for (int i = 0; i < 3; ++i)
            p_cRGB[i] = new std::atomic_int[256];
        p_Image = cv::Mat(cv::Size(560, 560), CV_8UC3);
    }
    p_Enabled = !p_Enabled;
}

bool RGBHistogram::isEnabled() {
    return p_Enabled;
}

void RGBHistogram::init() {
    if (!p_Enabled)
        return;
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < 256; ++j)
            p_cRGB[i][j].store(0);
}

void RGBHistogram::staticstic(const cv::Vec3b& px) {
    if (!p_Enabled)
        return;
    for (int i = 0; i < 3; ++i)
        p_cRGB[i][px[i]].fetch_add(1);
}

void RGBHistogram::show() {
    if (!p_Enabled)
        return;
    double base = p_Log * 0.01, amp = p_Amp * 0.01;
    p_Image.forEach<cv::Vec3b>([&](cv::Vec3b& px, const int pos[]) {
        px[0] = px[1] = px[2] = 0;
        // Mix
        if (12 <= pos[0] && pos[0] < 268 && 12 <= pos[1] && pos[1] < 268)
            for (int i = 0; i < 3; ++i) {
                int len = log(p_cRGB[i][pos[1] - 12].load() + 1) / log(base) * amp;
                len = std::min(len, 256);
                if (268 - len <= pos[0] && pos[0] < 268)
                    px[i] = 255;
            }
        // Only red
        else if (12 <= pos[0] && pos[0] < 268 && 292 <= pos[1] && pos[1] < 548) {
            int len = log(p_cRGB[2][pos[1] - 292].load() + 1) / log(base) * amp;
            len = std::min(len, 256);
            if (268 - len <= pos[0] && pos[0] < 268)
                px[2] = 255;
        }
        // Only green
        else if (292 <= pos[0] && pos[0] < 548 && 12 <= pos[1] && pos[1] < 268) {
            int len = log(p_cRGB[1][pos[1] - 12].load() + 1) / log(base) * amp;
            len = std::min(len, 256);
            if (548 - len <= pos[0] && pos[0] < 548)
                px[1] = 255;
        }
        // Only blue
        else if (292 <= pos[0] && pos[0] < 548 && 292 <= pos[1] && pos[1] < 548) {
            int len = log(p_cRGB[0][pos[1] - 292].load() + 1) / log(base) * amp;
            len = std::min(len, 256);
            if (548 - len <= pos[0] && pos[0] < 548)
                px[0] = 255;
        }
    });
    cv::imshow("RGB Histogram", p_Image);
}

void RGBHistogram::p_Tracker(int val, void* userdata) {
    *((int*)userdata) = val;
}

}
}