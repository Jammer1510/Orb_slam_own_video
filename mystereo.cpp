#include <opencv2/opencv.hpp>
#include <librealsense2/rs.hpp> // Include RealSense SDK
#include "System.h"

#include <string>
#include <chrono>
#include <iostream>
#include <thread>   // For threading
#include <atomic>   // For atomic flag

using namespace std;

// Paths to the vocabulary and settings file
string parameterFile = "/home/jammer/Dev/ORB_SLAM3/Examples/Stereo/myStereo.yaml";
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
    ORB_SLAM3::System SLAM(vocFile, parameterFile, ORB_SLAM3::System::STEREO, true);

    // RealSense pipeline and configuration
    rs2::pipeline pipe;
    rs2::config cfg;

    // Enable streams for left and right IR cameras
    cfg.enable_stream(RS2_STREAM_INFRARED, 1, 640, 480, RS2_FORMAT_Y8, 30); // Left IR
    cfg.enable_stream(RS2_STREAM_INFRARED, 2, 640, 480, RS2_FORMAT_Y8, 30); // Right IR

    // Start the pipeline
    rs2::pipeline_profile profile = pipe.start(cfg);

    // Start a thread to listen for the "stop" command
    thread stopListener(listenForStopCommand);

    auto start = chrono::system_clock::now();

    while (!stopSLAM) {
        // Wait for the next set of frames
        rs2::frameset frames = pipe.wait_for_frames();

        // Get left and right IR frames
        rs2::video_frame left_frame = frames.get_infrared_frame(1);  // Left IR
        rs2::video_frame right_frame = frames.get_infrared_frame(2); // Right IR

        // Convert RealSense frames to OpenCV format
        cv::Mat left_image(cv::Size(640, 480), CV_8UC1, (void*)left_frame.get_data(), cv::Mat::AUTO_STEP);
        cv::Mat right_image(cv::Size(640, 480), CV_8UC1, (void*)right_frame.get_data(), cv::Mat::AUTO_STEP);

        // Get the current timestamp
        auto now = chrono::system_clock::now();
        auto timestamp = chrono::duration_cast<chrono::milliseconds>(now - start);
        double current_timestamp = double(timestamp.count()) / 1000.0;

        // Pass the stereo images to ORB-SLAM3
        SLAM.TrackStereo(left_image, right_image, current_timestamp);

        // Display the images (optional)
        cv::imshow("Left IR", left_image);
        cv::imshow("Right IR", right_image);
        if (cv::waitKey(1) == 27) { // Stop on ESC key
            stopSLAM = true;
        }
    }

    // Shutdown the SLAM system
    SLAM.Shutdown();

    // Save the keyframe trajectory
    SLAM.SaveKeyFrameTrajectoryTUM("KeyFrameTrajectory.txt");
    cout << "Keyframe trajectory saved to KeyFrameTrajectory.txt" << endl;

    // Stop the RealSense pipeline
    pipe.stop();

    // Join the stop listener thread
    stopListener.join();

    return 0;
}
