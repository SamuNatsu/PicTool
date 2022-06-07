#ifndef PIC_TOOL_PARADE_RGB_HISTOGRAM_HPP_INCLUDED
#define PIC_TOOL_PARADE_RGB_HISTOGRAM_HPP_INCLUDED

#include <bits/stdc++.h>
#include <opencv2/opencv.hpp>

namespace pt {
namespace parade {

class RGBHistogram {
public:
    static void toggle();
    static bool isEnabled();

    static void init();
    static void staticstic(const cv::Vec3b&);
    static void show();

private:
    RGBHistogram() = default;
    RGBHistogram(const RGBHistogram&) = delete;
    RGBHistogram(RGBHistogram&&) = delete;
    virtual ~RGBHistogram();

    RGBHistogram& operator=(const RGBHistogram&) = delete;
    RGBHistogram& operator=(RGBHistogram&&) = delete;

    static RGBHistogram p_Guard;

    static bool p_Enabled;
    static std::atomic_int* p_cRGB[3];
    static cv::Mat p_Image;
    static int p_Log, p_Amp;

    static void p_Tracker(int, void*);
};

}
}

#endif
