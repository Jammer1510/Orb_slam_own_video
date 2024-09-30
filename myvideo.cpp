#include <opencv2/opencv.hpp>
#include "System.h"

#include <string>
#include <chrono>   // for time stamp
#include <iostream>

using namespace std;

//need to change to your own path
string parameterFile = "/home/jammer/Dev/ORB_SLAM3/Examples/Monocular/myvideo.yaml";
string vocFile = "/home/jammer/Dev/ORB_SLAM3/Vocabulary/ORBvoc.txt";
string videoFile = "/home/jammer/Dev/ORB_SLAM3/Examples/Monocular/video.mp4";

int main(int argc, char **argv) {

    
    ORB_SLAM3::System SLAM(vocFile, parameterFile, ORB_SLAM3::System::MONOCULAR, true);

    
    cv::VideoCapture cap(videoFile);    // change to 1 if you want to use USB camera.
    
    auto start = chrono::system_clock::now();

    while (1) {
        cv::Mat frame;
        cap >> frame;   
        if ( frame.data == nullptr )
            break;

        // rescale because image is too large
        cv::Mat frame_resized;
        cv::resize(frame, frame_resized, cv::Size(640,480));

        auto now = chrono::system_clock::now();
        auto timestamp = chrono::duration_cast<chrono::milliseconds>(now - start);
        SLAM.TrackMonocular(frame_resized, double(timestamp.count())/1000.0);
        cv::waitKey(30);
    }

    SLAM.Shutdown();
    return 0;
}
