#include "PcapParser.h"

using namespace std;

int main()  {
	char srcFileName[100] = "/re/sampleData/input/00-0b-2f-2e-34-a1";
	PcapParser pparser(srcFileName);

	struct IP_hdr *ip_hdr;
	struct TCP_hdr *tcp_hdr;
	struct UDP_hdr *udp_hdr;
	char *payload;

	int proto;
	int payloadLen;
	
    char src_ip_char[STRSIZE], dst_ip_char[STRSIZE];

    int i = 0;

	while(pparser.NextPacket(&ip_hdr, &tcp_hdr, &udp_hdr, proto, &payload, payloadLen) == 0)  {
		i ++;
		if(20 == i)  {
			break;
		}
		inet_ntop(AF_INET, (void *)&(ip_hdr->ip_src), src_ip_char, 16);
        inet_ntop(AF_INET, (void *)&(ip_hdr->ip_dst), dst_ip_char, 16);

        printf("From ip address %s to ip address %s\n", src_ip_char, dst_ip_char);

        if(proto == 0)  {	// udp protocal
        	printf("UDP Protocal, src port %d, dst port %d\n", 
        		ntohs(udp_hdr->uh_sport), ntohs(udp_hdr->uh_dport));
        }  else  {
        	printf("TCP Protocal, src port %d, dst port %d\n", 
        		ntohs(tcp_hdr->th_sport), ntohs(tcp_hdr->th_dport));
        }

        printf("Payload length is %d\n", payloadLen);
		printf("%02x%02x\n", payload[0], payload[1]);
	}

	return 0;
}