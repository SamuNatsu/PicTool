#ifndef PIC_TOOL_FX_COLOR_KEY_HPP_INCLUDED
#define PIC_TOOL_FX_COLOR_KEY_HPP_INCLUDED

#include <bits/stdc++.h>
#include <opencv2/opencv.hpp>

namespace pt {
namespace fx {

class ColorKey {
public:
    static void toggle(const cv::Size&);
    static bool isEnabled();

    static void effect(cv::Mat&);

private:
    ColorKey() = default;
    ColorKey(const ColorKey&) = delete;
    ColorKey(ColorKey&&) = delete;
    ~ColorKey() = default;

    ColorKey& operator=(const ColorKey&) = delete;
    ColorKey& operator=(ColorKey&&) = delete;

    static bool p_Enable;
    static int p_R, p_G, p_B, p_Dlt;
    static cv::Mat p_Image, p_External;

    static void p_Tracker(int, void*);
};

}
}

#endif
