#include "opencv2/objdetect.hpp"
#include "opencv2/highgui.hpp"
#include "opencv2/imgproc.hpp"
 
#include <iostream>
#include "stdio.h"
 
using namespace std;
using namespace cv;
 
CascadeClassifier face_cascade;
string window_name = "Face Detection Demo";
String face_cascade_name = "/usr/share/opencv/haarcascades/haarcascade_frontalface_alt.xml";
const int BORDER = 8;	/* order between GUI elements to the edge of the image */
 
template <typename T> string toString(T t)
{
	ostringstream out;
	out << t;
 
	return out.str();
}
 

int main(int argc, const char** argv)
{
	VideoCapture capture;
  	Mat frame;
    std::vector<Rect> faces;
	Mat frame_gray;
 
 
	if (!face_cascade.load( face_cascade_name ) ) {
		printf("--(!)Error loading training file: haarcascade_frontalface_alt2.xml\n");
		return -1;
	};
 
	try {
		capture.open(0);
		capture.set(CV_CAP_PROP_FRAME_WIDTH, 640);
		capture.set(CV_CAP_PROP_FRAME_HEIGHT, 480);
	}
	catch (cv::Exception &e)
	{
		const char *err_msg = e.what();
		cout << "Exception caught: " << err_msg << endl;
	}
 
	if ( !capture.isOpened() ) {
		cout << "ERROR: Could not access the camera!" << endl;
		exit(1);
	}
 
	while(true) {
		capture >> frame;
 
   		if (!frame.empty()) {
 
			cvtColor(frame, frame_gray, CV_BGR2GRAY);
			equalizeHist(frame_gray, frame_gray);
 
			face_cascade.detectMultiScale(frame_gray, faces, 1.2, 3, CV_HAAR_DO_CANNY_PRUNING, Size(80, 80));
 
			for (size_t i = 0; i < faces.size(); i++) {
				CvPoint pt1 = { faces[i].x, faces[i].y };
				CvPoint pt2 = { faces[i].x + faces[i].width, faces[i].y + faces[i].height };
				rectangle(frame, pt1, pt2, CV_RGB(0, 255, 0), 3, 4, 0);
 
				Mat faceROI = frame_gray(faces[i]);
			}
			//string stringToDisplay = "Number Of Faces: " + toString(faces.size());

			if (!!faces.size()) {
				printf("Face detected!\n");
			}
			imshow(window_name, frame);
		} else {
			printf(" --(!) No captured frame");
		}
 
   		int c = waitKey(1);
		if ((char)c == 27) {
			break;
		}
  	}
 
	return 0;
}
 