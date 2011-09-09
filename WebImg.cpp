#include "WebImg.h"
#include <cstdlib>

//this function takes the full url and shortens it to just the hostname.
string WebImg::stringFunc1(string &hostnm)
{
   string url1 = hostnm;
   string ht = "http://";
   string hts = "https://";
   string slash = "/";

   unsigned pos = url1.find(ht,0);

   if (pos != string::npos)
   url1.erase(0,7);
   else if (pos == string::npos)
   {
      pos = url1.find(hts,0);

      if (pos != string::npos)
      url1.erase(0,8);
   }

  
   pos = url1.find(slash,0);
   if (pos != string::npos)
   url1.erase(pos, url1.size());

   return url1;
}

//takes full url and returns the tail end
string WebImg::stringFunc2(string &hostnm)
{
	string url1;
	string slash = "/";

	unsigned pos = hostnm.find(slash, 9);

	if (pos != string::npos)
		url1.assign(hostnm, pos, hostnm.size());

	return url1;
}


void WebImg::retrieveImg(const char* longstr, int wsint, SOCKET Socket)
{
	//parse HTML, get image locations, retrieve image, opencv
	
	string htm(longstr);
	delete[] longstr;

	smatch m; //regex match results
	const regex r("https?.*(?:jpg|jpeg|png|bmp|JPEG|JPG|PNG|BMP|Jpg|Jpeg|Png|Bmp)");//check which are supported by IplImage
	
	try
		{
				
			if(regex_search(htm,m,r))
				{
		
					ofstream mtchimage[sizeof m];
	
					if(!mtchimage)
					throw SockError("Error opening output file");

					//arguments for cvCreateImageHeader
					CvSize size;
					size.height = 330;
					size.width = 520;
					int depth = 1;
					int channels = 1;

					//naming the image files in the loop					
					string str;
				    stringstream ss;
		
					
					for(int i = 0; i < sizeof m; i++)
						{
							//incrementing stream to generate filenames
							char* PicNum = "Pic number ";
							ss.str("");
							ss << PicNum << (i+1);
							str = ss.str();
							PicNum = const_cast<char*>(str.c_str());

									//collect the image data over network
									string imgUrl = m[i];
								   string szBuffer_s = "GET ";
								   szBuffer_s += stringFunc2(imgUrl);//tail end of URL
								   szBuffer_s += " HTTP/1.1";
								   szBuffer_s +=  "\nHost: " ;
								   szBuffer_s +=  stringFunc1(imgUrl);  //hostname
								   szBuffer_s += " \r\nConnection: keep alive\r\n\r\n";
   
    
							wsint = send(Socket, szBuffer_s.c_str(), szBuffer_s.length(), 0);
							if (wsint == SOCKET_ERROR)
							{
								closesocket(Socket);	
								throw SockError("send socket failed");
							}
							 std::cout << "Client message sent: \n\n" << szBuffer_s << std::endl;
	
							string Data;
	
							char szBuffer[256];//watch out for overflow
							int array_int_copy = 0;
	
							while(1)
							{
								wsint = recv(Socket, szBuffer, 256, 0);
								if (wsint == SOCKET_ERROR)
								{
									closesocket(Socket);
									throw SockError("recv socket failed");
								}

								if (wsint == 0)
									break;

								//while(array_int_copy != wsint)
								//{
									Data.append(szBuffer);
									//array_int_copy++;
								//}		
							}

							 char* thedata = new char[Data.size()+1];
							 strcpy(thedata, Data.c_str());

							//assigning returned image data
							IplImage* s = cvCreateImageHeader(size, depth, channels);//creating just the header because data is added next
							s->imageData = thedata;

							//creating filenames out of incrementing int's
							mtchimage[i].open(PicNum, ios::binary);
							mtchimage[i] << s << endl;
					}
			}
			else throw SockError("no image matched by regex");
		 }
	catch(SockError &sockerr)
			{
				cerr << sockerr.what() << endl;
			}
					
}


IplImage* WebImg::compare(double threshold, IplImage* imge )
{	
	bool match = true;

	const char* webpage = GetHTTP(servername);//returns heap data.
	WSACleanup();
	cout << webpage << endl;
	
	
	IplImage* img1 = cvLoadImage("mtchimage", CV_LOAD_IMAGE_COLOR);
	if(img1 == nullptr)
		match = false;
	/*double l2_norm = cvNorm(imge,img1);
	if (l2_norm <= threshold)
		match = false;*/
	
	if(match == true)
		return img1;
	else return nullptr;

	
}

const char* WebImg::GetHTTP(string& svername)
{
	
	try
	{
	
	//Initialise Winsock with call to WSAStartup
	WSADATA wsaData;
	
	int wsint = WSAStartup(MAKEWORD(2,0), &wsaData);

	if(wsint != 0)
	{
		WSACleanup();
		throw SockError("WSAStartup failed");
	}
	
	struct sockaddr_in Address;
	memset(&Address, 0, sizeof (Address));
	Address.sin_family = AF_INET;
    Address.sin_addr.s_addr = INADDR_ANY;
	Address.sin_port = htons(80);
    

	struct hostent* pHent = NULL;

	//if( Address.sin_addr.s_addr == INADDR_ANY)
		string hstr = stringFunc1(svername);
		 pHent = gethostbyname(hstr.c_str());
	
	if(pHent == NULL)
		{
			int lasterr = WSAGetLastError();
			cout << "pHent error code " << lasterr << endl;
			throw SockError("pHent error");
		}

	SOCKET Socket;

	Socket = socket(AF_INET,SOCK_STREAM,0);
	if(Socket == INVALID_SOCKET)
		throw SockError("Socket failed");

	struct servent* pSent = NULL;
	sockaddr_in saServer;

	pSent = getservbyname("http","tcp");

	if (pSent == NULL)
		saServer.sin_port = htons(80);
	else
		saServer.sin_port = pSent->s_port;

	saServer.sin_family = AF_INET;
	saServer.sin_addr = *((LPIN_ADDR) *pHent -> h_addr_list);

	wsint = connect(Socket, (LPSOCKADDR)&saServer, sizeof(SOCKADDR_IN));
	if (wsint == SOCKET_ERROR)
	{
		closesocket(Socket);
		throw SockError ("Connect failed");
	}

	
		   string szBuffer_s = "GET ";
	       szBuffer_s += stringFunc2(svername);//tail end of URL
		   szBuffer_s += " HTTP/1.1";
           szBuffer_s +=  "\nHost: " ;
           szBuffer_s +=  hstr;  //hostname
		   szBuffer_s += " \r\nConnection: keep alive\r\n\r\n";
   
    
	wsint = send(Socket, szBuffer_s.c_str(), szBuffer_s.length(), 0);
	if (wsint == SOCKET_ERROR)
	{
		closesocket(Socket);	
		throw SockError("send socket failed");
	}
	 std::cout << "Client message sent: \n\n" << szBuffer_s << std::endl;
	
	string Data;
	
	char szBuffer[256];
	int array_int_copy = 0;
	
	while(1)
	{
		wsint = recv(Socket, szBuffer, 256, 0);
		if (wsint == SOCKET_ERROR)
		{
            closesocket(Socket);
			throw SockError("recv socket failed");
		}

		if (wsint == 0)
			break;

		//while(array_int_copy != wsint)
		//{
			Data.append(szBuffer);
			//array_int_copy++;
		//}		
	}

	 char* thedata = new char[Data.size()+1];
	 strcpy(thedata, Data.c_str());

	 retrieveImg(thedata, wsint, Socket);

	 closesocket(Socket);
	 return thedata;
	
	}
	
	catch(SockError &sockerr)
	{
		cerr << sockerr.what() << endl;
	}
	
}
