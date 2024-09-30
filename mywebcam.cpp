#include <opencv2/opencv.hpp>
#include "System.h"

#include <string>
#include <chrono>
#include <iostream>

using namespace std;

string parameterFile = "/home/jammer/Dev/ORB_SLAM3/Examples/Monocular/mywebcam.yaml";
string vocFile = "/home/jammer/Dev/ORB_SLAM3/Vocabulary/ORBvoc.txt";

int main(int argc, char **argv) {
    // Initialize ORB-SLAM3 system
    ORB_SLAM3::System SLAM(vocFile, parameterFile, ORB_SLAM3::System::MONOCULAR, true);

    // Open the default camera (webcam)
    cv::VideoCapture cap(0);  // 0 is the default ID for a built-in webcam
    if (!cap.isOpened()) {
        cerr << "Error: Cannot open the webcam." << endl;
        return -1;
    }

    auto start = chrono::system_clock::now();

    while (true) {
        cv::Mat frame;
        cap >> frame;  // Capture a frame from the webcam
        if (frame.empty()) {
            cerr << "Error: Empty frame captured." << endl;
            break;
        }

        // Resize the frame if needed
        cv::Mat frame_resized;
        cv::resize(frame, frame_resized, cv::Size(640, 480));

        auto now = chrono::system_clock::now();
        auto timestamp = chrono::duration_cast<chrono::milliseconds>(now - start);
        SLAM.TrackMonocular(frame_resized, double(timestamp.count()) / 1000.0);
        cv::waitKey(30);
    }

    SLAM.Shutdown();
    return 0;
}

