#include <iostream>
#include <Windows.h>
#include <stdio.h>
#include <math.h>
#include "opencv2/objdetect/objdetect.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui_c.h"
#include "SharedMemoryManager.h"


#include <opencv2/opencv.hpp>
#include "aruco.h"
#include "cvdrawingutils.h"
 using namespace aruco;
 using namespace std;
 using namespace cv;

// the function that detect face (and display the picture)
 void detectAndDisplay( Mat frame );

 // dataset used for face detection
 //String face_cascade_name = "lbpcascade_frontalface.xml";
 String face_cascade_name = "haarcascade_frontalface_alt.xml";
 CascadeClassifier face_cascade;

 // initialize random generator (why? I don't know, it seems that face detection needs it)
 RNG rng(12345);

 // title of window
 const char* window_title = "FaceDetection group 9";

 // the object that give access to the memorymapped file
 SharedMemoryManager *mem;

 // an array storing the position of the middle of the head
 double head_center[2]={0.0,0.0};

 // threshold (in pixel) beyond which we take into account a move of the head
 double min_step=10.0;


 // Vectors of markers detected in an image
vector<Marker> TheMarkers;
// Parameters camera
CameraParameters TheCameraParameters;
// Marker detector
MarkerDetector MDetector;
double averagedOrientation[4];

int main (int argc, char* argv[])
{
	mem=new SharedMemoryManager();
	for (int i=0;i<4;i++)
		averagedOrientation[i]=0.0;
	Mat frame;
	char key='a';
	CvCapture *capture;
	capture = cvCreateCameraCapture( CV_CAP_ANY );
	TheCameraParameters.readFromXMLFile("calib0.yml");
	TheCameraParameters.resize(cv::Size(600,480));
	
	

   if( !face_cascade.load( face_cascade_name ) ){ printf("--(!)Error loading\n"); return -1; };
	cvNamedWindow (window_title, CV_WINDOW_AUTOSIZE);


	while(key != 'q' && key != 'Q') {
	   frame = cvQueryFrame(capture);


	   if( !frame.empty() ){ 
		   detectAndDisplay( frame ); 
	   }
       else{
			printf(" --(!) No captured frame -- Break!"); 
			break; 
	   }
	   key = cvWaitKey(1);
 
	}

	cvReleaseCapture(&capture);
	cvDestroyAllWindows();
	delete mem;
	return EXIT_SUCCESS;
}

void detectAndDisplay( Mat frame )
{
	
  // Rect that will contain the faces
  std::vector<Rect> faces;

  // detection of faces is done in black and white : convert to BW
  Mat frame_gray;
  cvtColor( frame, frame_gray, CV_BGR2GRAY );
  equalizeHist( frame_gray, frame_gray );
  
  //detect faces
  face_cascade.detectMultiScale( frame_gray, faces, 1.3, 3, 0|CV_HAAR_SCALE_IMAGE, Size(30, 30) );

  double nearest_center[2]={head_center[0],head_center[1]};
  double shortest_distance=100000000;
  for( int i = 0; i < faces.size(); i++ )
  {
		// the algorithm can detect several faces
		// this part extract the center that is the nearest to the last obtained
		double center_face[2]={ faces[i].x + faces[i].width*0.5, faces[i].y + faces[i].height*0.5};
		double distance_to_previous_center=sqrt(pow(center_face[0]-head_center[0],2)+pow(center_face[1]-head_center[1],2));
		if (distance_to_previous_center<shortest_distance){
			nearest_center[0]=center_face[0];
			nearest_center[1]=center_face[1];
			shortest_distance=distance_to_previous_center;
		}
		Point center( faces[i].x + faces[i].width*0.5, faces[i].y + faces[i].height*0.5 );
		ellipse( frame, center, Size( faces[i].width*0.5, faces[i].height*0.5), 0, 0, 360, Scalar( 255, 0, 255 ), 4, 8, 0 );
		Mat faceROI = frame_gray( faces[i] );
  }
  // if the distance between the detected position of the head is less than PRECISION_STEP
  if (shortest_distance>min_step){
	  head_center[0]=nearest_center[0];
	  head_center[1]=nearest_center[1];

	  
	  // we store a number between -1 and 1 in the shared file
	  // which will be better interpreted by following program 
	  double width=(double)frame.cols;
	  double height=(double) frame.rows;
	  mem->writeHeadPosition(-2.0*head_center[0]/width+1,-2*(head_center[1]-height/2)/width);
  }
  circle(frame,Point(head_center[0],head_center[1]),5,Scalar(0,255,0),5);
  
  //_---------------------------- Detect Markers ----------------------------------------------// Actually in a thread
  
	MDetector.setDesiredSpeed(1);
	MDetector.detect(frame,TheMarkers,TheCameraParameters,0.08);
	MDetector.enableErosion(false);
	


	double weight=5;
	for (int i=0;i<4;i++)
				averagedOrientation[i]*=weight;
	for (unsigned int i=0;i<TheMarkers.size();i++) {
		double position[3];
		double orientation[4];
		switch(TheMarkers[i].id){
		case 987:
		case 444:
		case 594:
		case 1006:
			TheMarkers[i].draw(frame,Scalar(0,0,255),1);
			TheMarkers[i].OgreGetPoseParameters(position, orientation);
			cout<<orientation[0]<<orientation[1]<<orientation[2]<<orientation[3]<<"\n";

			for (int i=0;i<4;i++)
				averagedOrientation[i]+=orientation[i];
			weight++;
			break;
		default : break;
		}

	}
	if (weight>0){
			cout<<"bloc\n\n";
		for (int i=0;i<4;i++)
			averagedOrientation[i]/=weight;
		mem->writeCrossBowOrientation(averagedOrientation);
	}
	
	

  //-- Show what you got
  imshow( window_title, frame );
 }