#ifndef PIC_TOOL_PARADE_RGB_PARADE_HPP_INCLUDED
#define PIC_TOOL_PARADE_RGB_PARADE_HPP_INCLUDED

#include <bits/stdc++.h>
#include <opencv2/opencv.hpp>

namespace pt {
namespace parade {

class RGBParade {
public:
    static void toggle(const cv::Size&);
    static bool isEnabled();

    static void init();
    static void staticstic(const cv::Vec3b&, int);
    static void show();

private:
    RGBParade() = default;
    RGBParade(const RGBParade&) = delete;
    RGBParade(RGBParade&&) = delete;
    virtual ~RGBParade();

    static RGBParade p_Guard;

    static bool p_Enabled;
    static std::atomic_int** p_cRGB[3];
    static cv::Mat p_Image;
    static int p_Amp;

    static void p_Tracker(int, void*);
};

}
}

#endif
