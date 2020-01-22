// Author: Nicholas Graca
// Date: 10/14/17
// This program takes in a file that has packet data in binary and reads the
// data and prints it out.

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <arpa/inet.h>
// Inserts ARPA stuff so I can swap to handle endians
uint32_t hton(uint32_t hostlong);
uint16_t htons(uint16_t hostshort);
uint32_t ntohl(uint32_t netlong);
uint16_t ntohs(uint16_t netshort);

int length = 0;
int count = 0;
unsigned int version = 0;
unsigned int IHL = 0;
unsigned int TOS = 0;
unsigned int totLength = 0;
unsigned int id = 0;
unsigned int flags = 0;
unsigned int offset = 0;
unsigned int ttl = 0;
unsigned int protocol = 0;
unsigned int checksum = 0;
unsigned int source1 = 0;
unsigned int source2 = 0;
unsigned int source3 = 0;
unsigned int source4 = 0;
unsigned int dest1 = 0;
unsigned int dest2 = 0;
unsigned int dest3 = 0;
unsigned int dest4 = 0;
unsigned long leftover = 0;

int main(int argc, char** argv){
	// checks to see if there is a file passed in
    if(argc != 2){
        fprintf(stderr, "usage: %s inputFile", argv[0]);
        exit( EXIT_FAILURE);
    }
	// creates a pointer to the file and checks it
	FILE* fp = fopen(argv[1], "r");
	if(!fp){
	fprintf(stderr, "Failed to open input file: No such file or directory");
	exit(EXIT_FAILURE);
     }
	// Gets the first 4 bytes in the file which should be the amount of 
	// packets in that file
	int nread = fread(&count, 4, 1, fp);		
	// If fread doesnt work like it should, handle it
	if(nread != 1){
		fprintf(stderr, "Failed to read count of packets.");
		exit(EXIT_FAILURE);
	}
	// Prints the header of the output as specified in the instructions
	printf("==== File %s contains %d Packets.\n", argv[1], count);
	// Loops for how many packets are in the file
	for(int i = 0; i < count; i++){
		// Prints the number of the packet that it is printing info for
		printf("==>Packet %d\n", i + 1);
		// Get how many bytes are in the packet
		fread(&length, 4,1,fp);
		// Get the version and bit shift to get the correct number
		fread(&version, 1, 1, fp);
		unsigned int v  = version >> 4;
		// The IHL is the second bit in version
		IHL = version & 0x0f;
		printf("Version:\t\t0x%X (%d)\n", v, v);
		printf("IHL (Header Length):\t\t0x%X (%d)\n", IHL, IHL);
		// Get TOS
		fread(&TOS, 1, 1, fp);
		printf("Type of Service (TOS):\t\t0x%X (%d)\n", TOS, TOS);
		// Get the total length and use the ARPA func to get the bits right
		// Then shift them to get rid of the extra zero's
		fread(&totLength, 2, 1, fp);
		totLength = htonl(totLength);
		totLength = totLength >> 16;
		printf("Total Length:\t\t0x%X (%d)\n", totLength, totLength);
		//Get the ID and do the same thing as we did to totLength
		fread(&id,2, 1, fp);
		id = htonl(id);
		id = id >> 16;
		printf("Identification:\t\t0x%X (%d)\n", id, id);
		// Get the IP flags
		fread(&flags, 1, 1, fp);
		printf("IP Flags:\t\t0x%X (%d)\n", flags, flags);
		// Get the offset
		fread(&offset, 1, 1, fp);
		printf("Fragment Offset:\t\t0x%X (%d)\n", offset, offset);
		//Get the TTL
		fread(&ttl, 1, 1, fp);
		printf("Time To Live (TTL):\t\t0x%X (%d)\n", ttl, ttl);
		// Get the protocol
		fread(&protocol, 1, 1, fp);
		char* type;
		// Check the protocol number to know what protocol it is so that you
		// can print it
		if(protocol == 6){
			type = "TCP";
		}
		if(protocol == 1){
			type = "ICMP";
		}
		if(protocol == 17){
			type = "UDP";
		}
		printf("Protocol:\t\t%s 0x%X (%d)\n",type, protocol, protocol);
		// Get the checksum and do the same thing we did to totLength
		fread(&checksum, 2, 1, fp);
		checksum = htonl(checksum);
		checksum = checksum >> 16;
		printf("Header Checksum:\t\t0x%X (%d)\n", checksum, checksum);
		// Get each part of the IP address seperately. There is probably
		// a better way but this works so I wont touch it.
		fread(&source1, 1, 1, fp);
		fread(&source2, 1, 1, fp);
		fread(&source3, 1, 1, fp);
		fread(&source4, 1, 1, fp);
		printf("Source Address:\t\t%d.%d.%d.%d\n", source1, source2, source3, source4);
		// I did the same thing for destination address as I did for Source address
		fread(&dest1, 1, 1, fp);
		fread(&dest2, 1, 1, fp);
		fread(&dest3, 1, 1, fp);
		fread(&dest4, 1, 1, fp);
		printf("Destination Address:\t\t%d.%d.%d.%d\n", dest1, dest2, dest3, dest4);
		// Calculate the data portion of the packet so you know where the next
		// packet begins
		int over = length - 20;
		fread(&leftover, over, 1, fp);
	}
}
