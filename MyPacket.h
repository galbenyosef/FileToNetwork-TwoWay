#ifndef _MYPACKET_
#define _MYPACKET_

#include <iostream>

//Pre-defined vars
const unsigned DATA_BYTES = 256;
const unsigned IPv4 = 4;
const unsigned DO_NOTHiNG = 143;
const unsigned TTL_VALUE = 254;
//4.4.1 Datagram Format, Page #333, Figure 4.13, Computer Networking: A Top-Down Approach
const unsigned TYPICAL_HEADER_SIZE = 20;
const unsigned TYPICAL_HEADER_SIZE_AS_32_BIT_WORDS = 5; //Internet Header Length is the number of 32 - bit words in the header
const unsigned _32_BIT_WORDS_TO_BYTES = 32 / 8;

class MyPacket {

	/*
	IPv4DatagramFormat
	13-Key Fields
	_______________________________________________________
	|version|header length|type of service|datagram length|
	|_____________________________________________________|
	|    identifier   |      flags       | fragmentation  |
	|_____________________________________________________|
	|   Time-To-Live  |     protocol     |    checksum    |
	|_____________________________________________________|
	|					ip source						  |
	|_____________________________________________________|
	|					ip destination					  |
	|_____________________________________________________|
	|													  |
	|                      DATA							  |
	|_____________________________________________________|

	*/

	unsigned version; //4bit
	unsigned h_length; //4bit
	unsigned tos; //8bit
	unsigned length; //16bit
	unsigned identifier; //16bit
	unsigned flags; //3bit
	unsigned fragmentation; //13bit
	unsigned ttl; //8bit
	unsigned protocol; //8bit
	unsigned checksum; //16bit
	unsigned ip_src; //32bit
	unsigned ip_dst; //32bit
	unsigned char* data;

public:

	//ctor
	MyPacket();

	//dtor
	~MyPacket();

	enum H_FIELDS {
		//notice: 
		VERSION_and_H_LENGTH, TOS, LENGTH_lsb,
		LENGTH_msb, IDENTIFIER_lsb, IDENTIFIER_msb,
		FLAGS, FRAGMENTATION, TTL, PROTOCOL, CHECKSUM_lsb,
		CHECKSUM_msb, IPSOURCE_lsb1, IPSOURCE_lsb2, IPSOURCE_msb2,
		IPSOURCE_msb1, IPDESTINATION_lsb1, IPDESTINATION_lsb2, IPDESTINATION_msb2, IPDESTINATION_msb1
	};

	// a simple setter
	// all variables are pre-known except of MyPacket's length which is been calculated right away
	// data assigned by pre-allocated raw pointer's address
	void setFields(unsigned vrsn, unsigned h_lngt, unsigned titiel, unsigned prtcl, unsigned ipsrc, unsigned ipdst, unsigned char* dta, unsigned dta_lngt);

	// in order to extract bits from unsigned type, create a mask
	// mask is defined as binary number containing 1's from start to param given, from param and on it gets 0's
	static unsigned createMask(unsigned a);

	//void setChcksm() { checksum = getIP_s() + getIP_d() + getVrsn() + getPrtcl() + getHeaderLength() + getLength(); }


	// Bitwise operations (MyPacket is non-aligned, non-padded but spaced structure, which uses 'empty' memory for no reason, therefore:)
	// function gets all vars, puts them into unsigned char (1Byte = 8Bit), padded if needed, into an array which could be written into a file
	unsigned char* toWrite();

	unsigned getHeaderLength() const;
	unsigned getLength() const;
	unsigned char* getData() const;
	static short Checksum(unsigned char*,size_t len);

	friend std::ostream& operator<<(std::ostream&, const MyPacket&);
};

#endif