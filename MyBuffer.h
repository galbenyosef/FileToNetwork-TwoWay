#ifndef _MYBUFFER_
#define _MYBUFFER_

#include <exception>
#include <iostream>

class MyBuffer {

	unsigned length;
	unsigned char* buffer;

public:

	MyBuffer();
	~MyBuffer();

	void setBuffer(unsigned len);

	unsigned char* get() const;
	unsigned getLength() const;
};

#endif