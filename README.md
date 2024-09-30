# ORB-SLAM3 Monocular SLAM with Custom Video

This tutorial demonstrates how to run ORB-SLAM3 on your own video file for monocular SLAM. It guides you through modifying the source code, setting up the configuration files, updating `CMakeLists.txt`, and building and running the system.

## Prerequisites

- Ensure you have installed all dependencies for ORB-SLAM3.
- You should have ORB-SLAM3 cloned and set up on your system.
- A video file (`.mp4` format) to use for SLAM.

## Step-by-Step Guide

### 1. Prepare Your Video and Configuration Files

- Place your video file (e.g., `video.mp4`) in the `Examples/Monocular` directory of the ORB-SLAM3 project:
  ```bash
  cp /path/to/your/video.mp4 ~/Dev/ORB_SLAM3/Examples/Monocular/video.mp4
  ```
### 2. Create a new YAML file for the video configuration in Examples/Monocular:
 - Create YAML
  ```bash
    nano ~/Dev/ORB_SLAM3/Examples/Monocular/myvideo.yaml
  ```
 - Copy the following configuration template into myvideo.yaml and modify the parameters as needed

### 3. Create a Custom Video Example
 - Create a new C++ file in the Examples/Monocular directory:
  ```bash
    nano ~/Dev/ORB_SLAM3/Examples/Monocular/myvideo.cpp
  ```
 - Copy and paste code from myvideo.cpp
   
## 4. Modify CMakeLists.txt
 - Open CMakeLists.txt in the root of the ORB-SLAM3 directory and add the following:
  ```bash
    #add in Monocular section
    add_executable(myvideo Examples/Monocular/myvideo.cpp)
    target_link_libraries(myvideo ${PROJECT_NAME})
  ```
## 5. Build 
 - If you have already compile, need to remove build folder from root and also three build in thirdparty folder
 - Then just build the project.
  ```bash
  chmod +x build.sh
  ./build.sh
  ```
### 6. Run ORB-SLAM3 with the Custom Video
```
cd ~/Dev/ORB_SLAM3/Examples/Monocular
./myvideo
```

