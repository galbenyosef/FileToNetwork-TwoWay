#ifndef _MYPCKTHNDLR_
#define _MYPCKTHNDLR_

#include "MyBuffer.h"
#include "MyPacket.h"

#include <fstream>
#include <cstring>

class MyPacketHandler {

public:

	MyPacketHandler();

	~MyPacketHandler();

	static void Read2Buf(std::string input_file_name,MyBuffer& buf);

	//notice: below functions *.cpp file has been seperated for aesthetic considerations
	static void fragmentize(std::string input_file_name, std::string output_file_name, unsigned source_ip, unsigned destination_ip, unsigned mtu);
	static void defragmentize(std::string input_file_name, std::string output_file_name, unsigned source_ip, unsigned destination_ip, unsigned prtcl, unsigned vrsn);
};

#endif