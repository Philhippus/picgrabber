#include "WebImg.h"

using std::cout;
using std::endl;
using std::cin;
using std::string;

int main()
{
	char exit;

	do
	{
		string webpic = "http://www.places-to-visit.us/category/Egypt.html"; 
		
		WebImg C(webpic);
	
		C.retrieveImg();

		IplImage* img1 = cvLoadImage("Pic number 4", CV_LOAD_IMAGE_COLOR);
		cvNamedWindow("chosen1", CV_WINDOW_AUTOSIZE);
		cvShowImage("chosen1", img1);
		
		cout << "Enter 'x' to exit: ";
		cin >> exit;

	}
	while(exit != 'x');
		
	return 0;
}


/*GUI commands

Window management

Create and position a window:
  cvNamedWindow("win1", CV_WINDOW_AUTOSIZE); 
  cvMoveWindow("win1", 100, 100); // offset from the UL corner of the screen
Load an image:
  IplImage* img=0; 
  img=cvLoadImage(fileName);
  if(!img) printf("Could not load image file: %s\n",fileName);
Display an image:
  cvShowImage("win1",img);
Can display a color or grayscale byte/float-image. A byte image is assumed to have values in the range . A float image is assumed to have values in the range . A color image is assumed to have data in BGR order.

Close a window:
  cvDestroyWindow("win1");
Resize a window:
  cvResizeWindow("win1",100,100); // new width/heigh in pixels
  */