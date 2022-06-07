#ifndef PIC_TOOL_FX_G_CURVE_HPP_INCLUDED
#define PIC_TOOL_FX_G_CURVE_HPP_INCLUDED

#include <bits/stdc++.h>
#include <opencv2/opencv.hpp>

namespace pt {
namespace fx {

class GCurve {
public:
    static void toggle();
    static bool isEnabled();

    static void effect(cv::Mat&);

private:
    GCurve() = default;
    GCurve(const GCurve&) = delete;
    GCurve(GCurve&&) = delete;
    ~GCurve() = default;

    GCurve& operator=(const GCurve&) = delete;
    GCurve& operator=(GCurve&&) = delete;

    static bool p_Enable;
    static int p_G[256];
    static std::atomic_int p_cG[256];
    static int p_Log, p_Amp, p_Tune;
    static cv::Mat p_Image;

    static void p_Tracker(int, void*);
    static void p_Mouse(int, int, int, int, void*);
};

}
}

#endif
