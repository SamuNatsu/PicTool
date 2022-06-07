#include "HSIChannel.hpp"

namespace pt {
namespace parade {

bool HSIChannel::p_Enabled = false;

cv::Mat HSIChannel::p_Image;

int HSIChannel::p_Mode;

void HSIChannel::toggle() {
    if (p_Enabled)
        cv::destroyWindow("HSI Channel");
    else {
        cv::namedWindow("HSI Channel");
        cv::createTrackbar("Mode [H, S, I]", "HSI Channel", nullptr, 2, p_Tracker, &p_Mode);
        p_Image = cv::Mat(cv::Size(1, 1), CV_32FC3);
    }
    p_Enabled = !p_Enabled;
}

bool HSIChannel::isEnabled() {
    return p_Enabled;
}

void HSIChannel::show(const cv::Mat& in) {
    if (!p_Enabled)
        return;
    if (p_Image.rows != in.rows || p_Image.cols != in.cols)
        cv::resize(p_Image, p_Image, in.size());
    const double pi3 = CV_PI / 3;
    p_Image.forEach<cv::Vec3f>([&](cv::Vec3f& px, const int pos []) {
        // Clear
        px[0] = px[1] = px[2] = 0;
        // Source
        cv::Vec3b tmp = in.at<cv::Vec3b>(pos[0], pos[1]);
        // Normalize
        float r = tmp[2] / 255.0, g = tmp[1] / 255.0, b = tmp[0] / 255.0;
        // Intensity
        float i = (r + g + b) / 3;
        // Saturation
        float s = 1.0 - (fabs(i) < 1e-8 ? 1.0 : std::min({r, g, b}) / i);
        // Hue
        float h = 0;
        if (fabs(r - g) > 1e-8 || fabs(r - b) > 1e-8 || fabs(g - b) > 1e-8)
            h = acos(0.5 * (r * 2 - g - b) / sqrt(pow(r - g, 2) + (r - b) * (g - b)));
        switch (p_Mode) {
            case 0:
                if (h < pi3 * 2) {
                    px[2] = 0.5 * (1 + cos(h) / cos(pi3 - h));
                    px[1] = 1.5 - px[2];
                }
                else if (h < pi3 * 4) {
                    h -= pi3 * 2;
                    px[1] = 0.5 * (1 + cos(h) / cos(pi3 - h));
                    px[0] = 1.5 - px[1];
                }
                else {
                    h -= pi3 * 4;
                    px[0] = 0.5 * (1 + cos(h) / cos(pi3 - h));
                    px[2] = 1.5 - px[0];
                }
                break;
            case 1:
                px[0] = px[1] = px[2] = s;
                break;
            default:
                px[0] = px[1] = px[2] = i;
                break;
        }
    });
    cv::imshow("HSI Channel", p_Image);
}

void HSIChannel::p_Tracker(int val, void* userdata) {
    *((int*)userdata) = val;
}

}
}