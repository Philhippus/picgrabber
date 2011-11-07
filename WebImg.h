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
	explicit WebImg (string srvrname):servername(srvrname){}
	~WebImg(){}
	
	void retrieveImg();//uses GetHTTP()

private:
	string servername;
	string GetHTTP();//this might need to return char*?
	
	string stringFunc1(string&);
	string stringFunc2(string&);
	
	
};



	
#endif