#ifndef PIC_TOOL_PARADE_L_HISTOGRAM_HPP_INCLUDED
#define PIC_TOOL_PARADE_L_HISTOGRAM_HPP_INCLUDED

#include <bits/stdc++.h>
#include <opencv2/opencv.hpp>

namespace pt {
namespace parade {

class LHistogram {
public:
    static void toggle();
    static bool isEnabled();

    static void init();
    static void staticstic(const cv::Vec3b&);
    static void show();

private:
    LHistogram() = default;
    LHistogram(const LHistogram&) = delete;
    LHistogram(LHistogram&&) = delete;
    virtual ~LHistogram() = default;

    LHistogram& operator=(const LHistogram&) = delete;
    LHistogram& operator=(LHistogram&&) = delete;

    static bool p_Enabled;
    static std::atomic_int p_cY[256];
    static cv::Mat p_Image;
    static int p_Log, p_Amp;

    static void p_Tracker(int, void*);
};

}
}

#endif
