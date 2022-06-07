#include "ColorKey.hpp"

namespace pt {
namespace fx {

bool ColorKey::p_Enable = false;

int ColorKey::p_R = 0, ColorKey::p_G = 0, ColorKey::p_B = 0, ColorKey::p_Dlt = 0;

cv::Mat ColorKey::p_Image, ColorKey::p_External;

void ColorKey::toggle(const cv::Size& size) {
    if (p_Enable)
        cv::destroyWindow("Color Key");
    else {
        cv::namedWindow("Color Key");
        cv::createTrackbar("Red", "Color Key", nullptr, 255, p_Tracker, &p_R);
        cv::createTrackbar("Green", "Color Key", nullptr, 255, p_Tracker, &p_G);
        cv::createTrackbar("Blue", "Color Key", nullptr, 255, p_Tracker, &p_B);
        cv::createTrackbar("Delta", "Color Key", nullptr, 500, p_Tracker, &p_Dlt);
        cv::setTrackbarPos("Red", "Color Key", 0);
        cv::setTrackbarPos("Green", "Color Key", 0);
        cv::setTrackbarPos("Blue", "Color Key", 0);
        cv::setTrackbarPos("Delta", "Color Key", 0);
        p_Image = cv::Mat::zeros(cv::Size(100, 100), CV_8UC3);
        if (!p_External.data) {
            p_External = cv::imread("background", cv::IMREAD_COLOR);
            if (p_External.data) {
                double scale = std::max((double)size.width / p_External.size().width, (double)size.height / p_External.size().height);
                cv::resize(p_External, p_External, cv::Size(), scale, scale);
            }
        }
    }
    p_Enable = !p_Enable;
}

bool ColorKey::isEnabled() {
    return p_Enable;
}

void ColorKey::effect(cv::Mat& io) {
    if (!p_Enable)
        return;
    p_Image.forEach<cv::Vec3b>([&](cv::Vec3b& px, const int pos[]) {
        px[2] = p_R;
        px[1] = p_G;
        px[0] = p_B;
    });
    cv::imshow("Color Key", p_Image);
    io.forEach<cv::Vec3b>([&](cv::Vec3b& px, const int pos[]) {
        double dis = sqrt(pow(p_R - px[2], 2) + pow(p_G - px[1], 2) + pow(p_B - px[0], 2));
        if (dis < p_Dlt) {
            if (p_External.data)
                px = p_External.at<cv::Vec3b>(pos[0], pos[1]);
            else
                px[0] = px[1] = px[2] = 0;
        }
    });
}

void ColorKey::p_Tracker(int val, void* userdata) {
    *((int*)userdata) = val;
}

}
}