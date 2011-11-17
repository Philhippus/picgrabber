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


void WebImg::retrieveImg()
{
	SOCKET Socket;
	struct hostent* pHent;
	unsigned long addr = 0;
	sockaddr_in saServer;
	memset((char *) &saServer, 0, sizeof(saServer));
	saServer.sin_family = AF_INET;
	saServer.sin_addr.s_addr = INADDR_ANY ;
	saServer.sin_port = htons(80);

	string htm = WebImg::GetHTTP(Socket, pHent, saServer);
	string htm1;
	string::const_iterator begin;
	string::const_iterator end;

	//create stream and write html
	stringstream htmstream;

	smatch m; //regex match results
	const regex r("https?.*(?:jpg|jpeg|png|bmp|gif|JPEG|JPG|PNG|BMP|GIF|Jpg|Jpeg|Png|Bmp|Gif)");

	try
	{
		
		while(htmstream << htm)
		{
			getline(htmstream, htm1, '\n'); //htm1 is overwritten at each iteration
			begin = htm1.begin();
			end = htm1.end();

			if (regex_search(begin, end, m, r))				
			{
				string imgUrl (m[0].first, m[0].second); 
				cout << imgUrl << endl;

				//if( Address.sin_addr.s_addr == INADDR_ANY)
				string hstr = stringFunc1(imgUrl);
				pHent = gethostbyname(hstr.c_str());

				if(pHent == NULL)
				{
					int lasterr = WSAGetLastError();
					cout << "pHent error code " << lasterr << endl;
					throw SockError("pHent error");
				}
				memcpy((char *) &saServer.sin_addr, pHent->h_addr, pHent->h_length);

				Socket = socket(AF_INET,SOCK_STREAM,0);
				if(Socket == INVALID_SOCKET)
					throw SockError("Socket failed");				

				int wsint = connect(Socket, (LPSOCKADDR)&saServer, sizeof(saServer));
				if (wsint == SOCKET_ERROR)
				{
					closesocket(Socket);
					throw SockError ("Connect failed");
				}

				string szBuffer_s = "GET ";
				szBuffer_s += WebImg::stringFunc2(imgUrl);//tail end of URL
				szBuffer_s += " HTTP/1.1";
				szBuffer_s +=  "\nHost: " ;
				szBuffer_s += WebImg::stringFunc1(imgUrl);  //hostname
				szBuffer_s += " \r\nConnection: keep alive\r\n\r\n";	

				ofstream mtchimage;

				if(!mtchimage)
					throw SockError("Error opening output stream");

				//naming the image files in the loop					
				string str;
				stringstream ss;

				//incrementing stream to generate filenames
				static int i = 1;
				char* PicNum = "Pic number ";
				ss.str("");
				ss << PicNum << (i++);
				str = ss.str();
				PicNum = const_cast<char*>(str.c_str());

				wsint = send(Socket, szBuffer_s.c_str(), szBuffer_s.length(), 0);
				if (wsint == SOCKET_ERROR)
				{
					closesocket(Socket);	
					throw SockError("send socket failed");
				}
				std::cout << "Client message sent: \n\n" << szBuffer_s << std::endl;

				string Data;

				char szBuffer[4096];//watch out for overflow

				while(wsint = recv(Socket, szBuffer, 4096, 0))
				{
					if (wsint == SOCKET_ERROR)
					{
						closesocket(Socket);
						throw SockError("recv socket failed");
					}

					Data.append(szBuffer, 4096);									
				}

				//TODO parse the image header, concatenate file type to Picnum
				//remove HTML header

				//creating filenames out of incrementing int's
				mtchimage.open(PicNum, ios::out|ios::binary);
				mtchimage.write(Data.c_str(), Data.length() );
				mtchimage.close();

			}//end if
		}//end while
		closesocket(Socket);
		WSACleanup();
	}//end try

	catch(SockError &sockerr)
	{
		cerr << sockerr.what() << endl;
	}

}


string WebImg::GetHTTP(SOCKET& Socket, hostent* pHent, sockaddr_in& saServer)
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

		sockaddr_in Address;
		memset(&Address, 0, sizeof (Address));
		Address.sin_family = AF_INET;
		Address.sin_addr.s_addr = INADDR_ANY;
		Address.sin_port = htons(80);

		//if( Address.sin_addr.s_addr == INADDR_ANY)
		string hstr = stringFunc1(WebImg::servername);
		pHent = gethostbyname(hstr.c_str());

		if(pHent == NULL)
		{
			int lasterr = WSAGetLastError();
			cout << "pHent error code " << lasterr << endl;
			throw SockError("pHent error");
		}

		memcpy((char *) &saServer.sin_addr, pHent->h_addr, pHent->h_length);

		Socket = socket(AF_INET,SOCK_STREAM,0);
		if(Socket == INVALID_SOCKET)
			throw SockError("Socket failed");				

		wsint = connect(Socket, (LPSOCKADDR)&saServer, sizeof(saServer));
		if (wsint == SOCKET_ERROR)
		{
			closesocket(Socket);
			throw SockError ("Connect failed");
		}


		string szBuffer_s = "GET ";
		szBuffer_s += stringFunc2(WebImg::servername);//tail end of URL
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

		wsint = 1;
		while(wsint)
		{
			wsint = recv(Socket, szBuffer, 256, 0);
			if (wsint == SOCKET_ERROR)
			{
				closesocket(Socket);
				throw SockError("recv socket failed");
			}

			Data.append(szBuffer);		
		}
		
		return Data;
		
	}

	catch(SockError &sockerr)
	{
		cerr << sockerr.what() << endl;
	}
	
}
