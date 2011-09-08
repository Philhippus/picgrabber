#include "WebImg.h"

using std::cout;
using std::endl;
using std::cin;
using std::string;
//TODO string manipulation to format retrieved files to fit into GET command.

int main()
{
	string webpic = "http://www.places-to-visit.us/category/Egypt.html"; 
	string path = "IMG6.jpg";
	IplImage* img;//, *img2;
	
	cout << "Select your image: ";
	//getline(cin, path);

	img = cvLoadImage(path.c_str(), CV_LOAD_IMAGE_COLOR);
	cvNamedWindow("chosen", CV_WINDOW_AUTOSIZE);
	cvShowImage("chosen", img);

	double thresh = 0.0;
	/*cout << "Set threshold: ";
	cin >> thresh;*/

//set up the comparator
	WebImg C(webpic, path);
	WebImg* Cp = &C;

	
	
	//char* image[4] = {"image1","image2","image3","image4"};
	
	//save images that match
	
		/*img2 = */Cp->compare(thresh, img);
		//if(img2 == nullptr)
		
			//cout << "Compare call failed"<<  endl;
		


	

	return 0;
}





/*
	Img a(imgPath); //single arg object has to be created before variables
	int BufSize = a.getSize(imgPath);
	Img b(BufSize,imgPath);

	//storage for comparator image
	imgPath = "path_to_comparator";
	Img One(imgPath);
	BufSize = One.imgStore(imgPath);//One.getSize(imgPath);
	Img Two(BufSize, imgPath);
	*/
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