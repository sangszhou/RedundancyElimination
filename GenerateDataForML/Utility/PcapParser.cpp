#include "PcapParser.h"

using namespace std;

PcapParser::PcapParser(const char *fileName)  {
	if((fp = fopen(fileName, "rb")) == NULL)  {
        printf("Error: cannot open pcap file\n");
        return;
    }

	pkt_hdr = (struct pcap_pkthdr*) malloc(sizeof(pcap_pkthdr));
	data = new char[DATASIZE];	
    pkt_offset = 24;
}

PcapParser::~PcapParser()  {
	delete []data;
	free(pkt_hdr);
	
	if(fp != NULL)  {
		fclose(fp);
		fp = NULL;
	}
}

int PcapParser::NextPacket(struct IP_hdr **ip_hdr,  struct TCP_hdr **tcp_hdr, struct UDP_hdr **udp_hdr, 
	int &proto, char **payload, int &payloadLen)  {

    if(fseek(fp, pkt_offset, SEEK_SET) != 0) return 0;
    
    if(fread(pkt_hdr, sizeof(pcap_pkthdr), 1, fp) != 1)  {
        printf("read end of file\n");
        return 1;
    }
    pkt_offset += 16 + (pkt_hdr->caplen);

    fread(data, 1, pkt_hdr->caplen, fp);
    (*ip_hdr) = (struct IP_hdr*)(data + SIZE_ETHERNET);
    ip_proto_offset = SIZE_ETHERNET;
    
    if(*(u_char*)(&(*ip_hdr)->ip_src) == '\n' || *(u_char*)(&(*ip_hdr)->ip_dst) == '\n')  {
        capwap_len = int((*(u_char*)(data+CAPWAPOFFSET+1))>>3) * 4; // capwap_len can be 8 or 16 bytes
        ip_proto_offset = (CAPWAPOFFSET+ capwap_len + 32);
        (*ip_hdr) = (struct IP_hdr*)(data+ip_proto_offset);          // read ip header
    }

    // ip  header is 20 bytes
    // udp header is 8 bytes
    // tcp header is 20 bytes
      
    if((*ip_hdr)->ip_p == IPPROTO_UDP)  {              
        payloadOffset = ip_proto_offset + 28;
        (*udp_hdr) = (UDP_hdr*)(data + ip_proto_offset+20);
        proto = 0;
    }  else if((*ip_hdr)->ip_p == IPPROTO_TCP) {
        payloadOffset = ip_proto_offset + 40;
    	(*tcp_hdr) = (TCP_hdr*)(data+ip_proto_offset+20);
    	proto = 1;
    }  else  {
        proto = -1;
    }

    payloadLen = pkt_hdr->caplen - payloadOffset;
    (*payload) = (char*)(data+payloadOffset);
    return 0;
}