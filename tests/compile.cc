#include<iostream>
#include"../include/fast_io.h"
#include<exception>
#include<cstddef>
#include<iomanip>

int main()
{
	try
	{
		fast_io::outbuf<<"hello world\n";	//Hello World
		
		fast_io::outbuf<<fast_io::fixed(2424235235322.1454325325235235325423,15)<<"\n";
		fast_io::outbuf.flush();
		std::cout<<"ok\n";
/*		fast_io::obuf o("a.txt");	// Create and output to a new file a.txt
		o<<"2\n";
		o<<fast_io::setw(10,"52",'0')<<"\n"; //Test setw
		fast_io::ibuf ibuf("b.txt");	// Try to open a not exist file;
		std::size_t n;
		ibuf>>n;*/
	}
	catch(const std::exception &ex)
	{
		fprint(fast_io::log,"%\n",ex.what());
		return 1;
	}
}