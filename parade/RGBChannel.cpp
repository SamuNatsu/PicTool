#include "RGBChannel.hpp"

namespace pt {
namespace parade {

bool RGBChannel::p_Enabled = false;

cv::Mat RGBChannel::p_Image;

int RGBChannel::p_Mode, RGBChannel::p_Gray;

void RGBChannel::toggle() {
    if (p_Enabled)
        cv::destroyWindow("RGB Channel");
    else {
        cv::namedWindow("RGB Channel");
        cv::createTrackbar("Mode [R, G, B]", "RGB Channel", nullptr, 2, p_Tracker, &p_Mode);
        cv::createTrackbar("Gray scale", "RGB Channel", nullptr, 1, p_Tracker, &p_Gray);
        cv::setTrackbarPos("Gray scale", "RGB Channel", 1);
        p_Image = cv::Mat(cv::Size(1, 1), CV_8UC3);
    }
    p_Enabled = !p_Enabled;
}

bool RGBChannel::isEnabled() {
    return p_Enabled;
}

void RGBChannel::show(const cv::Mat& in) {
    if (!p_Enabled)
        return;
    if (p_Image.rows != in.rows || p_Image.cols != in.cols)
        cv::resize(p_Image, p_Image, in.size());
    p_Image.forEach<cv::Vec3b>([&](cv::Vec3b& px, const int pos []) {
        px[0] = px[1] = px[2] = 0;
        cv::Vec3b tmp = in.at<cv::Vec3b>(pos[0], pos[1]);
        switch (p_Mode) {
            case 0:
                if (p_Gray)
                    px[0] = px[1] = px[2] = tmp[2];
                else 
                    px[2] = tmp[2];
                break;
            case 1:
                if (p_Gray)
                    px[0] = px[1] = px[2] = tmp[1];
                else 
                    px[1] = tmp[1];
                break;
            default:
                if (p_Gray)
                    px[0] = px[1] = px[2] = tmp[0];
                else 
                    px[0] = tmp[0];
                break;
        }
    });
    cv::imshow("RGB Channel", p_Image);
}

void RGBChannel::p_Tracker(int val, void* userdata) {
    *((int*)userdata) = val;
}

}
}