#ifndef PIC_TOOL_FX_B_CURVE_HPP_INCLUDED
#define PIC_TOOL_FX_B_CURVE_HPP_INCLUDED

#include <bits/stdc++.h>
#include <opencv2/opencv.hpp>

namespace pt {
namespace fx {

class BCurve {
public:
    static void toggle();
    static bool isEnabled();

    static void effect(cv::Mat&);

private:
    BCurve() = default;
    BCurve(const BCurve&) = delete;
    BCurve(BCurve&&) = delete;
    ~BCurve() = default;

    BCurve& operator=(const BCurve&) = delete;
    BCurve& operator=(BCurve&&) = delete;

    static bool p_Enable;
    static int p_B[256];
    static std::atomic_int p_cB[256];
    static int p_Log, p_Amp, p_Tune;
    static cv::Mat p_Image;

    static void p_Tracker(int, void*);
    static void p_Mouse(int, int, int, int, void*);
};

}
}

#endif
