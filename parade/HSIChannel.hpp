#ifndef PIC_TOOL_PARADE_HSI_CHANNEL_HPP_INCLUDED
#define PIC_TOOL_PARADE_HSI_CHANNEL_HPP_INCLUDED

#include <opencv2/opencv.hpp>

namespace pt {
namespace parade {

class HSIChannel {
public:
    static void toggle();
    static bool isEnabled();

    static void show(const cv::Mat&);

private:
    HSIChannel() = default;
    HSIChannel(const HSIChannel&) = delete;
    HSIChannel(HSIChannel&&) = delete;
    virtual ~HSIChannel() = default;

    HSIChannel& operator=(const HSIChannel&) = delete;
    HSIChannel& operator=(HSIChannel&&) = delete;

    static bool p_Enabled;
    static cv::Mat p_Image;
    static int p_Mode;

    static void p_Tracker(int, void*);
};

}
}

#endif
