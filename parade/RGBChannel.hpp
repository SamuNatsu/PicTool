#ifndef PIC_TOOL_PARADE_RGB_CHANNEL_HPP_INCLUDED
#define PIC_TOOL_PARADE_RGB_CHANNEL_HPP_INCLUDED

#include <opencv2/opencv.hpp>

namespace pt {
namespace parade {

class RGBChannel {
public:
    static void toggle();
    static bool isEnabled();

    static void show(const cv::Mat&);

private:
    RGBChannel() = default;
    RGBChannel(const RGBChannel&) = delete;
    RGBChannel(RGBChannel&&) = delete;
    virtual ~RGBChannel() = default;

    RGBChannel& operator=(const RGBChannel&) = delete;
    RGBChannel& operator=(RGBChannel&&) = delete;

    static bool p_Enabled;
    static cv::Mat p_Image;
    static int p_Mode, p_Gray;

    static void p_Tracker(int, void*);
};

}
}

#endif
