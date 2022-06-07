#include "RGBParade.hpp"

namespace pt {
namespace parade {

RGBParade RGBParade::p_Guard;

bool RGBParade::p_Enabled = false;

std::atomic_int **RGBParade::p_cRGB[3] = {nullptr, nullptr, nullptr};

cv::Mat RGBParade::p_Image;

int RGBParade::p_Amp;

RGBParade::~RGBParade() {
    if (p_Enabled)
        toggle(cv::Size());
}

void RGBParade::toggle(const cv::Size& size) {
    if (p_Enabled) {
        cv::destroyWindow("RGB Parade");
        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < p_Image.cols; ++j)
                delete[] p_cRGB[i][j];
            delete[] p_cRGB[i];
        }
    }
    else {
        cv::namedWindow("RGB Parade");
        cv::createTrackbar("Amp x1", "RGB Parade", nullptr, 500, p_Tracker, &p_Amp);
        cv::setTrackbarPos("Amp x1", "RGB Parade", 100);
        for (int i = 0; i < 3; ++i) {
            p_cRGB[i] = new std::atomic_int*[size.width];
            for (int j = 0; j < size.width; ++j)
                p_cRGB[i][j] = new std::atomic_int[256];
        }
        p_Image = cv::Mat(256, size.width, CV_8UC3);
    }
    p_Enabled = !p_Enabled;
}

void RGBParade::init() {
    if (!p_Enabled)
        return;
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < p_Image.size().width; ++j)
            for (int k = 0; k < 256; ++k)
                p_cRGB[i][j][k].store(0);
}

void RGBParade::staticstic(const cv::Vec3b& px, int w) {
    if (!p_Enabled)
        return;
    for (int i = 0; i < 3; ++i)
        p_cRGB[i][w][px[i]].fetch_add(1);
}

void RGBParade::show() {
    if (!p_Enabled)
        return;
    int w = p_Image.size().width;
    cv::Mat tmp(256, w * 3, CV_8UC3);
    tmp.forEach<cv::Vec3b>([&](cv::Vec3b& px, const int pos[]) {
        px[2 - pos[1] / w] = std::min(255, p_Amp * p_cRGB[2 - pos[1] / w][pos[1] % w][255 - pos[0]].load());
    });
    cv::resize(tmp, p_Image, p_Image.size());
    cv::imshow("RGB Parade", p_Image);
}

void RGBParade::p_Tracker(int val, void* userdata) {
    *((int*)userdata) = val;
}

}
}
