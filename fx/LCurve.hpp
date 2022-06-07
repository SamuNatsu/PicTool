#ifndef PIC_TOOL_FX_L_CURVE_HPP_INCLUDED
#define PIC_TOOL_FX_L_CURVE_HPP_INCLUDED

#include <bits/stdc++.h>
#include <opencv2/opencv.hpp>

namespace pt {
namespace fx {

class LCurve {
public:
    static void toggle();
    static bool isEnabled();

    static void effect(cv::Mat&);

private:
    LCurve() = default;
    LCurve(const LCurve&) = delete;
    LCurve(LCurve&&) = delete;
    ~LCurve() = default;

    LCurve& operator=(const LCurve&) = delete;
    LCurve& operator=(LCurve&&) = delete;

    static bool p_Enable;
    static int p_L[256];
    static std::atomic_int p_cY[256];
    static int p_Log, p_Amp, p_Tune;
    static cv::Mat p_Image;

    static void p_Tracker(int, void*);
    static void p_Mouse(int, int, int, int, void*);
};

}
}

#endif
