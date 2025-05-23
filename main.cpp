#include <opencv2/opencv.hpp>
#include <thread>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <atomic>

using namespace cv;
using namespace std;

queue<Mat> frameQueue;
mutex mtx;
condition_variable cv_frame;
atomic<bool> isRunning(true);

void captureThread(VideoCapture& cap) {
    while (isRunning) {
        Mat frame;
        cap >> frame;
        if (frame.empty()) {
            isRunning = false;
            break;
        }

        {
            lock_guard<mutex> lock(mtx);
            frameQueue.push(frame);
        }
        cv_frame.notify_one();
        this_thread::sleep_for(chrono::milliseconds(30));
    }
}

int main(int argc, char** argv) {
    string inputPath;
    if (argc > 1) inputPath = argv[1];

    VideoCapture cap;
    if (!inputPath.empty()) {
        cap.open(inputPath);
    } else {
        cap.open(0);
    }

    if (!cap.isOpened()) {
        cerr << "Error opening video source\n";
        return -1;
    }

    thread t1(captureThread, ref(cap));

    namedWindow("Motion Detection", WINDOW_AUTOSIZE);

    Mat prevGray;
    while (isRunning) {
        Mat frame;
        {
            unique_lock<mutex> lock(mtx);
            cv_frame.wait(lock, [] { return !frameQueue.empty() || !isRunning; });

            if (!frameQueue.empty()) {
                frame = frameQueue.front();
                frameQueue.pop();
            }
        }

        if (!frame.empty()) {
            Mat gray, diff, thresh;
            cvtColor(frame, gray, COLOR_BGR2GRAY);
            GaussianBlur(gray, gray, Size(21, 21), 0);

            if (!prevGray.empty()) {
                absdiff(prevGray, gray, diff);
                threshold(diff, thresh, 25, 255, THRESH_BINARY);
                dilate(thresh, thresh, Mat(), Point(-1, -1), 2);

                vector<vector<Point>> contours;
                findContours(thresh, contours, RETR_EXTERNAL, CHAIN_APPROX_SIMPLE);

                for (const auto& contour : contours) {
                    if (contourArea(contour) < 500) continue; // Skip small movements
                    Rect rect = boundingRect(contour);
                    rectangle(frame, rect, Scalar(0, 255, 0), 2);
                    putText(frame, "Motion Detected", Point(10, 20), FONT_HERSHEY_SIMPLEX, 0.6, Scalar(0, 0, 255), 2);
                }
            }

            prevGray = gray.clone();

            imshow("Motion Detection", frame);
            if (waitKey(10) == 27) {
                isRunning = false;
                break;
            }
        }
    }

    t1.join();
    cap.release();
    destroyAllWindows();
    return 0;
}
