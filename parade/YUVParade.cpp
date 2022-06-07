#include "YUVParade.hpp"

namespace pt {
namespace parade {

YUVParade YUVParade::p_Guard;

bool YUVParade::p_Enabled = false;

std::atomic_int **YUVParade::p_cYUV[3] = {nullptr, nullptr, nullptr};

cv::Mat YUVParade::p_Image;

int YUVParade::p_Amp;

YUVParade::~YUVParade() {
    if (p_Enabled)
        toggle(cv::Size());
}

void YUVParade::toggle(const cv::Size& size) {
    if (p_Enabled) {
        cv::destroyWindow("YUV Parade");
        for (int i = 0; i < 3; ++i) {
            for (int j = 0; j < p_Image.cols; ++j)
                delete[] p_cYUV[i][j];
            delete[] p_cYUV[i];
        }
    }
    else {
        cv::namedWindow("YUV Parade");
        cv::createTrackbar("Amp x1", "YUV Parade", nullptr, 500, p_Tracker, &p_Amp);
        cv::setTrackbarPos("Amp x1", "YUV Parade", 100);
        for (int i = 0; i < 3; ++i) {
            p_cYUV[i] = new std::atomic_int*[size.width];
            for (int j = 0; j < size.width; ++j)
                p_cYUV[i][j] = new std::atomic_int[256];
        }
        p_Image = cv::Mat(256, size.width, CV_8UC3);
    }
    p_Enabled = !p_Enabled;
}

void YUVParade::init() {
    if (!p_Enabled)
        return;
    for (int i = 0; i < 3; ++i)
        for (int j = 0; j < p_Image.size().width; ++j)
            for (int k = 0; k < 256; ++k)
                p_cYUV[i][j][k].store(0);
}

void YUVParade::staticstic(const cv::Vec3b& px, int w) {
    if (!p_Enabled)
        return;
    int Y = 0.299 * px[2] + 0.587 * px[1] + 0.114 * px[0];
    int Cb = 128 - 0.168736 * px[2] - 0.331264 * px[1] + 0.5 * px[0];
    int Cr = 128 + 0.5 * px[2] - 0.418688 * px[1] - 0.081312 * px[0];
    p_cYUV[0][w][Y].fetch_add(1);
    p_cYUV[1][w][Cb].fetch_add(1);
    p_cYUV[2][w][Cr].fetch_add(1);
}

void YUVParade::show() {
    if (!p_Enabled)
        return;
    int w = p_Image.size().width;
    cv::Mat tmp(256, w * 3, CV_8UC3);
    tmp.forEach<cv::Vec3b>([&](cv::Vec3b& px, const int pos[]) {
        int v = std::min(255, p_Amp * p_cYUV[pos[1] / w][pos[1] % w][255 - pos[0]].load());
        switch (pos[1] / w) {
            case 0: 
                px[1] = px[2] = v;
                break;
            case 1:
                px[0] = px[1] = v;
                break;
            default:
                px[0] = px[2] = v;
                break;
        }
    });
    cv::resize(tmp, p_Image, p_Image.size());
    cv::imshow("YUV Parade", p_Image);
}

void YUVParade::p_Tracker(int val, void* userdata) {
    *((int*)userdata) = val;
}

}
}
