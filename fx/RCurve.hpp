#ifndef PIC_TOOL_FX_R_CURVE_HPP_INCLUDED
#define PIC_TOOL_FX_R_CURVE_HPP_INCLUDED

#include <bits/stdc++.h>
#include <opencv2/opencv.hpp>

namespace pt {
namespace fx {

class RCurve {
public:
    static void toggle();
    static bool isEnabled();

    static void effect(cv::Mat&);

private:
    RCurve() = default;
    RCurve(const RCurve&) = delete;
    RCurve(RCurve&&) = delete;
    ~RCurve() = default;

    RCurve& operator=(const RCurve&) = delete;
    RCurve& operator=(RCurve&&) = delete;

    static bool p_Enable;
    static int p_R[256];
    static std::atomic_int p_cR[256];
    static int p_Log, p_Amp, p_Tune;
    static cv::Mat p_Image;

    static void p_Tracker(int, void*);
    static void p_Mouse(int, int, int, int, void*);
};

}
}

#endif
