#include <opencv2/opencv.hpp>
#include "System.h"

#include <string>
#include <chrono>
#include <iostream>

using namespace std;

// Paths to the vocabulary, settings file, and video file
string parameterFile = "/home/jammer/Dev/ORB_SLAM3/Examples/Monocular/mywebcam.yaml";
string vocFile = "/home/jammer/Dev/ORB_SLAM3/Vocabulary/ORBvoc.txt";

int main(int argc, char **argv) {
    // Initialize ORB-SLAM3 system
    ORB_SLAM3::System SLAM(vocFile, parameterFile, ORB_SLAM3::System::MONOCULAR, true);

    // Capture from the default webcam
    cv::VideoCapture cap(0);  // Use 0 for the default webcam
    cap.set(cv::CAP_PROP_FRAME_WIDTH, 640);  // Set the resolution
    cap.set(cv::CAP_PROP_FRAME_HEIGHT, 480);
    cap.set(cv::CAP_PROP_FPS, 15);  // Set the FPS

    if (!cap.isOpened()) {
        cerr << "Error: Could not open webcam." << endl;
        return -1;
    }

    auto start = chrono::system_clock::now();
    double last_timestamp = -1.0;

    while (true) {
        cv::Mat frame;
        cap >> frame;  // Capture a frame from the webcam
        if (frame.empty()) {
            cerr << "Error: Empty frame captured." << endl;
            break;
        }

        // Resize frame (comment out if using the full resolution)
        // cv::Mat frame_resized;
        // cv::resize(frame, frame_resized, cv::Size(640, 480));

        auto now = chrono::system_clock::now();
        auto timestamp = chrono::duration_cast<chrono::milliseconds>(now - start);
        double current_timestamp = double(timestamp.count()) / 1000.0;

        // Skip processing if the timestamp is not newer
        if (current_timestamp <= last_timestamp) {
            continue;
        }
        last_timestamp = current_timestamp;

        // Pass the frame to the SLAM system
        SLAM.TrackMonocular(frame, current_timestamp);
        cv::waitKey(1);  // Adjust wait time for smoother display
    }

    // Shutdown the SLAM system
    SLAM.Shutdown();
    return 0;
}

