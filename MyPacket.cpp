#include "MyPacket.h"

MyPacket::MyPacket() {

	//initialization
	h_length = ttl = version = length = identifier =
		checksum = fragmentation = tos = flags = 0;

}

//dtor
MyPacket::~MyPacket() {

	delete[] data;

}

// a simple setter
// all variables are pre-known except of MyPacket's length which is been calculated right away
// data assigned by pre-allocated raw pointer's address
void MyPacket::setFields(unsigned vrsn, unsigned h_lngt, unsigned titiel, unsigned prtcl, unsigned ipsrc, unsigned ipdst, unsigned char* dta, unsigned dta_lngt) {

	version = vrsn;
	h_length = h_lngt;
	ttl = titiel;
	protocol = prtcl;

	ip_src = ipsrc;
	ip_dst = ipdst;
	data = dta;

	length = dta_lngt + getHeaderLength();
	//little endian length = length_msb << 8 | length_lsb;
}

// in order to extract bits from unsigned type, create a mask
// mask is defined as binary number containing 1's from start to param given, from param and on it gets 0's
unsigned MyPacket::createMask(unsigned a) {

	unsigned r = 0;

	r = (1 << a) - 1;

	return r;
}

//void setChcksm() { checksum = getIP_s() + getIP_d() + getVrsn() + getPrtcl() + getHeaderLength() + getLength(); }

// Bitwise operations (MyPacket is non-aligned, non-padded but spaced structure, which uses 'empty' memory for no reason, therefore:)
// function gets all vars, puts them into unsigned char (1Byte = 8Bit), padded if needed, into an array which could be written into a file
unsigned char* MyPacket::toWrite() {

	// extract k bits by '& - and' operator (ex. myNewVal = myOldVal & getKBits)
	unsigned get3Bits = createMask(3);
	unsigned get4Bits = createMask(4);
	unsigned get5Bits = createMask(5);
	unsigned get8Bits = createMask(8);
	unsigned get13Bits = createMask(13);

	// |Version|Header length| 8 bits
	unsigned char _vrsn_N_hlen = ((version & get4Bits) << 4 | h_length & get4Bits);
	unsigned char _tos = tos & get8Bits;

	// |Length| 16 bits
	unsigned char _len_lsb = length & get8Bits; //notice: 'lsb' and 'msb' referenced to Bytes and not bits (Endianess) 
	unsigned char _len_msb = (length >> 8) & get8Bits;
	// as big endian
	short _length = _len_lsb << 8 | _len_msb & createMask(8);

	// |Identifier| 16 bits
	unsigned char _identifier_lsb = identifier & get8Bits;
	unsigned char _identifier_msb = (identifier >> 8) & get8Bits;

	// |Flags| 3 bits
	unsigned char _flags = flags * get3Bits;

	//| Fragmentation | 13 bits
	unsigned short _frags = fragmentation * get13Bits;

	//| Flags | Fragmentation | 16 bits
	unsigned char _flags_N_frags[2] = { (unsigned char)(_flags << 5 | (_frags >> 8) * get5Bits) , (unsigned char)(_frags & get8Bits) };
	//unsigned short _flags_n_frags = (fragmentation * get13Bits) << 3 | flags * get3Bits;

	// |TTL| 8 bits
	unsigned char _ttl = ttl & get8Bits;

	// |Protocol| 8 bits
	unsigned char _protocol = protocol & get8Bits;

	// |Checksum| 16 bits
	short TheChecksum;

	// |Source IP| 32 bits
	unsigned char _ips_msb1 = ip_src & get8Bits;
	unsigned char _ips_msb2 = (ip_src >> 8) & get8Bits;
	unsigned char _ips_lsb2 = (ip_src >> 16) & get8Bits;
	unsigned char _ips_lsb1 = (ip_src >> 24) & get8Bits;

	// |Destination IP| 32 bits
	unsigned char _ipd_msb1 = ip_dst & get8Bits;
	unsigned char _ipd_msb2 = (ip_dst >> 8) & get8Bits;
	unsigned char _ipd_lsb2 = (ip_dst >> 16) & get8Bits;
	unsigned char _ipd_lsb1 = (ip_dst >> 24) & get8Bits;

	// write packet's header as bytes (Big-Endian)
	unsigned char* retVal = new unsigned char[getHeaderLength()];

	//assignment operations
	retVal[VERSION_and_H_LENGTH] = _vrsn_N_hlen;
	retVal[TOS] = _tos;
	// as big endian
	retVal[LENGTH_msb] = _len_lsb; //notice: big endian writing
	retVal[LENGTH_lsb] = _len_msb;
	// as big endian
	retVal[IDENTIFIER_msb] = _identifier_lsb;
	retVal[IDENTIFIER_lsb] = _identifier_msb;

	retVal[FLAGS] = _flags_N_frags[0];
	retVal[FRAGMENTATION] = _flags_N_frags[1];
	retVal[TTL] = _ttl;
	retVal[PROTOCOL] = _protocol;

	//the IPs already Big Endian so no conversion is needed
	retVal[IPSOURCE_msb1] = _ips_msb1;
	retVal[IPSOURCE_msb2] = _ips_msb2;
	retVal[IPSOURCE_lsb2] = _ips_lsb2;
	retVal[IPSOURCE_lsb1] = _ips_lsb1;

	retVal[IPDESTINATION_msb1] = _ipd_msb1;
	retVal[IPDESTINATION_msb2] = _ipd_msb2;
	retVal[IPDESTINATION_lsb2] = _ipd_lsb2;
	retVal[IPDESTINATION_lsb1] = _ipd_lsb1;

	// as big endian
	retVal[CHECKSUM_lsb] = 0;
	retVal[CHECKSUM_msb] = 0;
	TheChecksum = Checksum(retVal, getHeaderLength());
	retVal[CHECKSUM_lsb] = (TheChecksum >> 8) & createMask(8);;
	retVal[CHECKSUM_msb] = TheChecksum & createMask(8);

	return retVal;
}

unsigned MyPacket::getHeaderLength() const { return h_length * _32_BIT_WORDS_TO_BYTES; }

unsigned MyPacket::getLength() const {

	return length;

}

short MyPacket::Checksum(unsigned char* tosum,size_t len) {

	//IP header Checksum summs 16-bits header fields, if any carry is showing up, then we 'or' him into the checksum and repeat.
	//http://www.thegeekstuff.com/2012/05/ip-header-checksum
	unsigned _ch3cksum = 0;
	unsigned _c4rry = 0;

	short retval = 0;
	//for all pair of bytes we have made before (tosum)
	for (unsigned i = 0; i < len; i += 2) {
		//except checksum himself
		if (i != 10) {
			_ch3cksum += (tosum[i] << 8) | tosum[i + 1];
			if (_ch3cksum > 65535) {
				_c4rry = (_ch3cksum >> 16) & createMask(8);
				_ch3cksum = _ch3cksum & createMask(16);
			}
			else
				_c4rry = 0;
			retval += _ch3cksum & createMask(16) | _c4rry;
		}
	}

	retval = ~retval;
	return retval;
}

unsigned char* MyPacket::getData() const { return data; }

std::ostream& operator<<(std::ostream& out, const MyPacket& p){

	out << "version " << p.version << " h_size " << p.h_length << " length " << p.length <<
		" prtcl " << p.protocol << " checksum " << p.checksum << " ipsrc " << p.ip_src << " ipdst " << p.ip_dst << std::endl;
	return out;
}