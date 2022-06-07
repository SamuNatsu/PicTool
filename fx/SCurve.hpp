#ifndef PIC_TOOL_FX_S_CURVE_HPP_INCLUDED
#define PIC_TOOL_FX_S_CURVE_HPP_INCLUDED

#include <bits/stdc++.h>
#include <opencv2/opencv.hpp>

namespace pt {
namespace fx {

class SCurve {
public:
    static void toggle();
    static bool isEnabled();

    static void effect(cv::Mat&);

private:
    SCurve() = default;
    SCurve(const SCurve&) = delete;
    SCurve(SCurve&&) = delete;
    ~SCurve() = default;

    SCurve& operator=(const SCurve&) = delete;
    SCurve& operator=(SCurve&&) = delete;

    static bool p_Enable;
    static int p_S[256];
    static std::atomic_int p_cS[256];
    static int p_Log, p_Amp, p_Tune;
    static cv::Mat p_Image;

    static void p_Tracker(int, void*);
    static void p_Mouse(int, int, int, int, void*);
};

}
}

#endif
