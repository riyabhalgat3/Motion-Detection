# Motion Detection using OpenCV

This project implements a simple motion detection system using Python and OpenCV. It detects motion by comparing frame differences from a video stream (webcam or file) and highlights the areas with movement using contours.

## How It Works

1. **Video Input**:
   - Captures real-time video from a webcam or a saved video file.

2. **Preprocessing**:
   - Converts each frame to grayscale and applies Gaussian blur to reduce noise and detail.

3. **Motion Detection**:
   - Calculates the absolute difference between the first frame and the current frame.
   - Applies thresholding and dilation to highlight significant changes (i.e., motion).
   - Finds contours around the detected motion and draws bounding boxes.

4. **Display**:
   - Displays the original frame with contours drawn around moving objects.
   - Optionally, logs the motion events or saves the processed video.

## Output Screenshot

<img width="600" alt="Image" src="https://github.com/user-attachments/assets/b0652a0f-4f8d-4388-b200-b96d2b76a7ff" />

### Prerequisites

- Python 3.x
- OpenCV

Install the required packages using:

```bash
pip install -r requirements.txt



