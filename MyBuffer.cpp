#include "MyBuffer.h"

MyBuffer::MyBuffer() : length(0), buffer(nullptr) { }
MyBuffer::~MyBuffer() { delete[] buffer; }

void MyBuffer::setBuffer(unsigned len) {
	if (!buffer) {
		try {
			buffer = new unsigned char[len]();
			length = len;
		}
		catch (std::bad_alloc& ba) {
			std::cerr << "bad_alloc caught: " << ba.what() << '\n';
		}
	}
}

unsigned char* MyBuffer::get() const { return buffer; }
unsigned MyBuffer::getLength() const { return length; }