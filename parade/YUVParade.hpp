#ifndef PIC_TOOL_PARADE_YUV_PARADE_HPP_INCLUDED
#define PIC_TOOL_PARADE_YUV_PARADE_HPP_INCLUDED

#include <bits/stdc++.h>
#include <opencv2/opencv.hpp>

namespace pt {
namespace parade {

class YUVParade {
public:
    static void toggle(const cv::Size&);
    static bool isEnabled();

    static void init();
    static void staticstic(const cv::Vec3b&, int);
    static void show();

private:
    YUVParade() = default;
    YUVParade(const YUVParade&) = delete;
    YUVParade(YUVParade&&) = delete;
    virtual ~YUVParade();

    static YUVParade p_Guard;

    static bool p_Enabled;
    static std::atomic_int** p_cYUV[3];
    static cv::Mat p_Image;
    static int p_Amp;

    static void p_Tracker(int, void*);
};

}
}

#endif
