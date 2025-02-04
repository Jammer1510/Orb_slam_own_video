#include <opencv2/opencv.hpp>
#include "System.h"

#include <string>
#include <chrono>
#include <iostream>
#include <thread>   // For threading
#include <atomic>   // For atomic flag

using namespace std;

// Paths to the vocabulary and settings file
string parameterFile = "/home/jammer/Dev/ORB_SLAM3/Examples/Monocular/mywebcam.yaml";
string vocFile = "/home/jammer/Dev/ORB_SLAM3/Vocabulary/ORBvoc.txt";

// Atomic flag to signal when to stop
atomic<bool> stopSLAM(false);

// Function to listen for "stop" command
void listenForStopCommand() {
    string input;
    while (!stopSLAM) {
        cin >> input;
        if (input == "stop") {
            stopSLAM = true;
            cout << "Stopping ORB-SLAM3..." << endl;
        }
    }
}

int main(int argc, char **argv) {
    // Initialize ORB-SLAM3 system
    ORB_SLAM3::System SLAM(vocFile, parameterFile, ORB_SLAM3::System::MONOCULAR, true);

    // Open the webcam
    cv::VideoCapture cap(0);  // Use 0 for the default webcam
    cap.set(cv::CAP_PROP_FRAME_WIDTH, 640);  // Set resolution
    cap.set(cv::CAP_PROP_FRAME_HEIGHT, 480);
    cap.set(cv::CAP_PROP_FPS, 15);  // Set FPS

    if (!cap.isOpened()) {
        cerr << "Error: Could not open webcam." << endl;
        return -1;
    }

    // Start a thread to listen for the "stop" command
    thread stopListener(listenForStopCommand);

    auto start = chrono::system_clock::now();
    double last_timestamp = -1.0;

    while (!stopSLAM) {
        cv::Mat frame;
        cap >> frame;  // Capture a frame
        if (frame.empty()) {
            cerr << "Error: Empty frame captured." << endl;
            break;
        }

        // Get the current timestamp
        auto now = chrono::system_clock::now();
        auto timestamp = chrono::duration_cast<chrono::milliseconds>(now - start);
        double current_timestamp = double(timestamp.count()) / 1000.0;

        // Skip processing if the timestamp is not newer
        if (current_timestamp <= last_timestamp) {
            continue;
        }
        last_timestamp = current_timestamp;

        // Pass the frame to ORB-SLAM3
        SLAM.TrackMonocular(frame, current_timestamp);
        cv::waitKey(1);  // Adjust delay for smoother display
    }

    // Shutdown the SLAM system
    SLAM.Shutdown();

    // Save the keyframe trajectory
    SLAM.SaveKeyFrameTrajectoryTUM("KeyFrameTrajectory.txt");
    cout << "Keyframe trajectory saved to KeyFrameTrajectory.txt" << endl;

    // Join the stop listener thread
    stopListener.join();

    return 0;
}

