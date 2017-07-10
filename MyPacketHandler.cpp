#include "MyPacketHandler.h"

MyPacketHandler::MyPacketHandler() {

}

MyPacketHandler::~MyPacketHandler() { }

void MyPacketHandler::Read2Buf(std::string input_file_name, MyBuffer& buffer) {

	std::ifstream input(input_file_name, std::ios::binary | std::ios::in);

	if (input.is_open()) {

		input.seekg(0, input.end);
		unsigned buf_s = (unsigned)input.tellg();
		input.seekg(0, input.beg);

		buffer.setBuffer(buf_s);

		input.read((char*)buffer.get(), buf_s);
		input.close();
	}
}


