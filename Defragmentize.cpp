#include "MyPacketHandler.h"

void MyPacketHandler::defragmentize(std::string input_file_name, std::string output_file_name, unsigned source_ip, unsigned destination_ip, unsigned prtcl, unsigned vrsn) {

	MyBuffer buffer;

	std::ofstream output;

	unsigned char* data, *tempdata;

	//counters
	unsigned data_lngth, data_idx, left, total_headers, fixed_size, theHeaderSize;

	Read2Buf(input_file_name, buffer);

	//counters initialization
	left = data_lngth = buffer.getLength();
	data_idx = total_headers = 0;

	tempdata = new unsigned char[data_lngth];;
	std::cout << "Read:\n";
	while (left) {

		//read header size as 1st byte, four msb
		theHeaderSize = (*(buffer.get() + data_lngth - left) & MyPacket::createMask(4)) * _32_BIT_WORDS_TO_BYTES; //header size as 32bit Words
		unsigned char* theHeader = buffer.get() + data_lngth - left;

		left -= theHeaderSize;
		total_headers++;

		// reconstruct as Little Endian in order to read header

		unsigned _vrsn = theHeader[MyPacket::VERSION_and_H_LENGTH] >> 4 & MyPacket::createMask(4);
		unsigned _len_lsb = theHeader[MyPacket::LENGTH_msb];
		unsigned _len_msb = theHeader[MyPacket::LENGTH_lsb];
		unsigned _length = (((_len_msb & MyPacket::createMask(8)) << 8) | _len_lsb & MyPacket::createMask(8));
		unsigned _prtcl = theHeader[MyPacket::PROTOCOL];

		unsigned _chksm_msb = theHeader[MyPacket::CHECKSUM_msb];
		unsigned _chksm_lsb = theHeader[MyPacket::CHECKSUM_lsb];
		unsigned _checksum = (((_chksm_lsb & MyPacket::createMask(8)) << 8) | _chksm_msb & MyPacket::createMask(8));

		unsigned _ips_lsb1 = theHeader[MyPacket::IPSOURCE_msb1];
		unsigned _ips_lsb2 = theHeader[MyPacket::IPSOURCE_msb2];
		unsigned _ips_msb2 = theHeader[MyPacket::IPSOURCE_lsb2];
		unsigned _ips_msb1 = theHeader[MyPacket::IPSOURCE_lsb1];
		unsigned _ipsource = (((((((((_ips_msb1 & MyPacket::createMask(8) ) << 8) | _ips_msb2 & MyPacket::createMask(8))) << 8) |
			_ips_lsb2 & MyPacket::createMask(8))) << 8) | _ips_lsb1 & MyPacket::createMask(8));

		unsigned _ipsourcelittle = (((((((((_ips_lsb1 & MyPacket::createMask(8)) << 8) | _ips_lsb2 & MyPacket::createMask(8))) << 8) |
			_ips_msb2 & MyPacket::createMask(8))) << 8) | _ips_msb1 & MyPacket::createMask(8));

		unsigned _ipd_lsb1 = theHeader[MyPacket::IPDESTINATION_msb1];
		unsigned _ipd_lsb2 = theHeader[MyPacket::IPDESTINATION_msb2];
		unsigned _ipd_msb2 = theHeader[MyPacket::IPDESTINATION_lsb2];
		unsigned _ipd_msb1 = theHeader[MyPacket::IPDESTINATION_lsb1];
		unsigned _ip_dst = (((((((((_ipd_msb1 & MyPacket::createMask(8)) << 8) | _ipd_msb2 & MyPacket::createMask(8))) << 8) |
			_ipd_lsb2 & MyPacket::createMask(8))) << 8) | _ipd_lsb1 & MyPacket::createMask(8));

		unsigned _ipdestinationlittle = (((((((((_ipd_lsb1 & MyPacket::createMask(8)) << 8) | _ipd_lsb2 & MyPacket::createMask(8))) << 8) |
			_ipd_msb2 & MyPacket::createMask(8))) << 8) | _ipd_msb1 & MyPacket::createMask(8));

		// validate fields
		if (_vrsn == vrsn && _prtcl == prtcl && _ipsource == source_ip && _ip_dst == destination_ip) {
			//copy data only if checksum passed also
			unsigned new_checksum = MyPacket::Checksum(theHeader, theHeaderSize);
			if (new_checksum == _checksum) {
				memcpy(tempdata + data_idx, buffer.get() + data_lngth - left, _length - theHeaderSize);
			}
			//increase data index
			data_idx += _length - theHeaderSize;

		}
		//decrease bytes that have been read
		left -= _length - theHeaderSize;
	}
	// filtering headers from tempdata into data (headers shouldnt be copied as defragmented file has no headers)
	fixed_size = data_lngth - total_headers * theHeaderSize;
	data = new unsigned char[fixed_size];
	memcpy(data, tempdata, fixed_size);

	delete[] tempdata;

	output.open(output_file_name, std::ios::binary | std::ios::out);
	if (output.is_open()) {

		output.write((char*)data, fixed_size);
		output.close();
	}
}
