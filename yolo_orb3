import cv2
import subprocess
import threading
from ultralytics import YOLO
import time
import os

# Paths
keyframe_file = "MyKeyFrameTrajectoryTUMFormat.txt"  # Replace with your keyframe file path
detection_file_path = "yolo_detection.txt"          # File for YOLO detections
orbslam_executable = "/home/jetson/Dev/ORB_SLAM3/Examples/Monocular/mywebcam"
yolo_model_path = "yolo11n.pt"

# Initialize YOLOv11
model = YOLO(yolo_model_path)

# Open Webcam (0 for default webcam, change if needed)
cap = cv2.VideoCapture(0)

# Ensure webcam is open
if not cap.isOpened():
    print("Error: Could not open webcam.")
    exit()

# Start ORB-SLAM3 in a separate process
orbslam_process = subprocess.Popen(
    [orbslam_executable, "mywebcam.yaml"],  # Ensure it loads the correct config
    stdout=subprocess.PIPE,
    stderr=subprocess.PIPE
)

# Open file for saving YOLO detections
detection_file = open(detection_file_path, "w")

# Flag to control the threads
running = True

# Function to process YOLO detections in real-time
def run_yolo():
    global running
    while running and cap.isOpened():
        ret, frame = cap.read()
        if not ret:
            break

        # Run YOLOv11 on the current frame
        results = model(frame)

        # Get frame timestamp
        timestamp = time.time()

        # Save YOLO detections
        for result in results[0].boxes:
            cls = int(result.cls.cpu().numpy()[0])  # Get class label
            conf = result.conf.cpu().numpy()[0]  # Get confidence score
            class_name = model.names[cls]  # Get class name
            detection_file.write(f"{timestamp}, {class_name}, {conf:.2f}\n")
        detection_file.flush()  # Ensure immediate write to file

        # Annotate and display the frame
        annotated_frame = results[0].plot()
        cv2.imshow("YOLOv11 + ORB-SLAM3", annotated_frame)

        # Exit on 'q' press
        if cv2.waitKey(1) & 0xFF == ord('q'):
            running = False
            break

# Start YOLO thread
yolo_thread = threading.Thread(target=run_yolo)
yolo_thread.start()

# Wait for ORB-SLAM3 process to finish
orbslam_process.communicate()

# Cleanup
running = False
yolo_thread.join()
cap.release()
cv2.destroyAllWindows()
detection_file.close()
orbslam_process.terminate()
