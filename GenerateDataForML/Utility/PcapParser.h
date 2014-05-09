#ifndef _PCAPPARSER_H_
#define _PCAPPARSER_H_

#include <stdio.h>

#include "headers.h"

#define DATASIZE 65535
#define CAPWAPOFFSET 42
#define STRSIZE 65535

class PcapParser  {
public:
	PcapParser(const char *fileName);
	~PcapParser();

	int NextPacket(struct IP_hdr **ip_hdr, struct TCP_hdr **tcp_hdr,  struct UDP_hdr **udp_hdr, 
		int &proto, char **payload, int &payloadLen);

private:
	FILE *fp;
	struct pcap_pkthdr *pkt_hdr;
	char *data;
	int pkt_offset;
	int capwap_len;
	int ip_proto_offset;
	int payloadOffset;
};

#endif