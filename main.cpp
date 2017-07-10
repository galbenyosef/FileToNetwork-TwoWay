#include "ip_file_handler.h"

int main() {

	bool fragmentation_success = ip_file_handler::file_to_ip_packets("file1.jpg", "file1_ip.bin", 0x3234FF21, 0x3F34DF29);
	bool defragmentation_success = ip_file_handler::ip_packets_to_file("file1_ip.bin", "file2.jpg", 0x3234FF21, 0x3F34DF29);

}