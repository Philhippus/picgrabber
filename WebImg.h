#ifndef WEBIMG_H
#define WEBIMG_H

#include "cv.h"
#include "cxcore.h"
#include "highgui.h"
#include <fstream>
#include <regex>
#include <string>
#include <sstream>
//#include <WS2tcpip.h> //has to be included for getaddrinfo()
//#include <winsock2.h>
#include <windows.h>
#include <stdexcept>
#include <iostream>

using namespace std;

struct SockError: public runtime_error
{
	explicit SockError(char* mess): runtime_error(mess)
	{}
};


class WebImg
{
public:
	WebImg(){}
	WebImg (string srvrname, string flstr)
		:servername(srvrname),filestr(flstr){}
	~WebImg(){}
	
	IplImage* compare(double, IplImage*);//uses retrieveImg

private:
	string servername;
	string filestr;  //path to user's image
	
	const char* GetHTTP(string&);//this might need to return char*?
	void retrieveImg(const char*);//uses GetHTTP()

	string stringFunc1(string&);
	string stringFunc2(string&);
	
	
};



	
#endif