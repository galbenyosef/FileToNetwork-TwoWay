#ifndef _ipfilehandler_
#define _ipfilehandler_

#include <iostream>
#include <fstream>

#include "MyPacketHandler.h"

class ip_file_handler {

public:

	static bool file_to_ip_packets(std::string, std::string, unsigned, unsigned);
	static bool ip_packets_to_file(std::string, std::string, unsigned, unsigned);

};

bool ip_file_handler::file_to_ip_packets(std::string input_file_name, std::string output_file_name, unsigned source_ip, unsigned destination_ip) {

	MyPacketHandler handler;
	handler.fragmentize(input_file_name, output_file_name, source_ip, destination_ip,DATA_BYTES);

	return true;
}

bool ip_file_handler::ip_packets_to_file(std::string input_file_name, std::string output_file_name, unsigned source_ip, unsigned destination_ip) {

	MyPacketHandler handler;
	handler.defragmentize(input_file_name, output_file_name, source_ip, destination_ip, 143, 4);


	return true;
}

#endif