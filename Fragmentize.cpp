#include "MyPacketHandler.h"

void MyPacketHandler::fragmentize(std::string input_file_name, std::string output_file_name, unsigned source_ip, unsigned destination_ip, unsigned mtu) {

	MyPacket* packets;
	MyBuffer buffer;

	std::ofstream output;

	// counters
	unsigned packets_count, pack_cur, buff_cur, left;

	// read as binary to MyBuffer
	Read2Buf(input_file_name, buffer);

	// counter initialization
	pack_cur = buff_cur = 0;
	packets_count = (unsigned)((float)buffer.getLength() / mtu + 0.5);

	// dynamically allocation packets
	packets = new MyPacket[packets_count];

	left = buffer.getLength();
	// while buffer hasnt been read completely
	std::cout << "Wrote:\n";
	while (left) {

		unsigned char* data;
		unsigned data_lngth;

		// if packets = [ 1 , 2 , 3 , ... , n ], then:
		// 'nth' packet case (only its data length could be different from the others)
		if (left < mtu) {
			data = new unsigned char[left];
			memcpy(data, buffer.get() + buff_cur, left);
			data_lngth = left;
			left = 0;
		}
		// '1st -> n-1th' packet case
		else {
			data = new unsigned char[mtu];
			memcpy(data, buffer.get() + buff_cur, mtu);
			buff_cur += mtu;
			data_lngth = mtu;
			left -= mtu;
		}

		// Data encapsulation with IPv4 header
		packets[pack_cur].setFields(IPv4, TYPICAL_HEADER_SIZE_AS_32_BIT_WORDS, TTL_VALUE, DO_NOTHiNG, source_ip, destination_ip, data, data_lngth);
		// increase counter
		pack_cur++;
	}

	// sequencially writing the packets to a file
	output.open(output_file_name, std::ios::binary | std::ios::out);

	if (output.is_open()) {
		// for all packets
		for (unsigned i = 0; i < packets_count; i++) {
			// make header writable by toWrite()
			unsigned char* out_header = packets[i].toWrite();
			// get header length
			unsigned out_header_size = packets[i].getHeaderLength();
			// make data writable by do nothing
			unsigned char* out_data = packets[i].getData();
			//get data length
			unsigned out_data_length = packets[i].getLength() - out_header_size;

			output.write((char*)out_header, out_header_size);
			output.write((char*)out_data, out_data_length);

			delete[] out_header;
		}
		output.close();
	}
}