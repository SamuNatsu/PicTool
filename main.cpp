#include <bits/stdc++.h>
#include <opencv2/opencv.hpp>

#include "fx/Fx.hpp"
#include "parade/Parade.hpp"

// Entrance
int main(int argc, char* argv[]) {
    // Camera select
    int dev = 0, camW = 0, camH = 0;
    if (argc > 1) {
        if (strcmp(argv[1], "count") == 0) {
            int tot = 0;
            while (1) {
                cv::VideoCapture cam;
                if (!cam.open(tot))
                    break;
                ++tot;
            }
            printf("Devices: %d\n", tot);
            return 0;
        }
        else {
            dev = atoi(argv[1]);
            if (argc > 2)
                camW = atoi(argv[2]);
            if (argc > 3)
                camH = atoi(argv[3]);
        }
    }

    // Initialize camera
    cv::VideoCapture cam;
    if (!cam.open(dev)) {
        std::cerr << "Error" << std::endl;
        return 0;
    }
    if (camW && camH) {
        cam.set(cv::CAP_PROP_FRAME_WIDTH, camW);
        cam.set(cv::CAP_PROP_FRAME_HEIGHT, camH);
    }
    int w = cam.get(cv::CAP_PROP_FRAME_WIDTH), h = cam.get(cv::CAP_PROP_FRAME_HEIGHT);
    std::cout << "Camera frame size: " << w << 'x' << h << '\n';

    // Allocation
    cv::Mat frame;

    // Loop
    while (1) {
        // Read camera
        cam >> frame;
        // Read key
        int key = cv::waitKey(1);
        if (key == 13)
            break;
        switch (key) {
            // F1
            case 112:
                pt::parade::RGBHistogram::toggle();
                break;
            // F2
            case 113:
                pt::parade::LHistogram::toggle();
                break;
            // F3
            case 114:
                pt::parade::RGBChannel::toggle();
                break;
            // F4
            case 115:
                pt::parade::HSIChannel::toggle();
                break;
            // F5
            case 116:
                pt::parade::RGBParade::toggle(frame.size());
                break;
            // F6
            case 117:
                pt::parade::YUVParade::toggle(frame.size());
                break;
            // F7
            case 118:
                pt::fx::ColorKey::toggle(frame.size());
                break;
            // F8
            case 119:
                pt::fx::LCurve::toggle();
                break;
            // F9
            case 120:
                pt::fx::SCurve::toggle();
                break;
            // F10
            case 121:
                pt::fx::RCurve::toggle();
                break;
            // F11
            case 122:
                pt::fx::GCurve::toggle();
                break;
            // F12
            case 123:
                pt::fx::BCurve::toggle();
            // Else
            default:
                break;
        }
        // Initialize
        pt::parade::RGBHistogram::init();
        pt::parade::LHistogram::init();
        pt::parade::RGBParade::init();
        pt::parade::YUVParade::init();
        // Fx
        pt::fx::ColorKey::effect(frame);
        pt::fx::LCurve::effect(frame);
        pt::fx::SCurve::effect(frame);
        pt::fx::RCurve::effect(frame);
        pt::fx::GCurve::effect(frame);
        pt::fx::BCurve::effect(frame);
        // Scan
        frame.forEach<cv::Vec3b>([&](const cv::Vec3b& px, const int pos[]) {
            pt::parade::RGBHistogram::staticstic(px);
            pt::parade::LHistogram::staticstic(px);
            pt::parade::RGBParade::staticstic(px, pos[1]);
            pt::parade::YUVParade::staticstic(px, pos[1]);
        });
        // Show camera
        cv::imshow("Camera", frame);
        // Show RGB Histogram
        pt::parade::RGBHistogram::show();
        // Show Lightness Histogram
        pt::parade::LHistogram::show();
        // Show RGB channel
        pt::parade::RGBChannel::show(frame);
        // Show HSI channel
        pt::parade::HSIChannel::show(frame);
        // Show RGB parade
        pt::parade::RGBParade::show();
        // Show YUV parade
        pt::parade::YUVParade::show();
    }

    return 0;
}
