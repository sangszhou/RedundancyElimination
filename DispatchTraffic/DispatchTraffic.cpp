/*
 * DispatchTraffic.cpp
 *
 *  Created on: Apr 28, 2014
 *      Author: sangs
 */


#include "DispatchTraffic.h"
#include "stdio.h"
#include <set>
using namespace std;
using namespace boost::filesystem;

#define STRSIZE 1024
#define DATASIZE 65535
#define CAPWAPOFFSET 42

vector<string> file_list;   // pcap files need to be parsed
map<string, vector<Session> > session_list; // ip address and corresponding <mac address, startTime, endTime>
set<string> createdFile;

void ChoosePcapFile(const char *src_dir_name, const char *dst_dir_name, const char *radius_account_name)  {
	// read disk and store pcap file names to file_list
	// store relation between etherAddr and ip address as well as start/end time
	getFileNames(src_dir_name, file_list);
	getUserLog(radius_account_name, session_list);

	int st = 0, ed = file_list.size() - 1;

	printf("Total Pcap files are from %d to %d\n", st+1, ed+1);

    /*
	printf("Choose the range you want to parse\n");
	printf("If you want to parse files from 1 to 10, type 1 10 and press enter\n");
	printf("Waiting input...\n");

	freopen("CON", "r", stdin);
	scanf("%d%d", &st, &ed);

	if(st <= 0 || ed > file_list.size()-1)  {
		printf("Input out of range\n");
		return;
	}
	*/

	//st --; ed --;

	string cur_parsing_file;
	clock_t startTime, finishTime;
	double execTime = 0.0;
	startTime = clock();

	for(size_t i = st; i <= ed; i ++)  {
		cur_parsing_file = file_list[i];

		HandleOnePcapFile(cur_parsing_file.c_str(), dst_dir_name);
		//if(i%10 == 0)  {
		printf("Parsing %uth file, still have %d files to be deal with\n", i-st+1, ed-i);
		finishTime = clock();
		execTime += (double)(finishTime - startTime)/CLOCKS_PER_SEC;
		printf("Total executing time is %lf seconds\n", execTime);
        printf("\n");
		startTime = clock();  
        //}
	}
}

void HandleOnePcapFile(const char *src_file_name, const char *dst_dir_name)  {
	struct pcap_pkthdr *pkt_hdr;
	struct Ethernet_hdr *eth_hdr;
	struct IP_hdr *ip_hdr;

    char my_time[STRSIZE];
    u_char data[DATASIZE];
    FILE *fp, *output;

    int pkt_offset, ip_proto_offset;
    int i = 0, j = 0, filter_pro = 0, filter_nolist = 0;
    int capwap_len;

    string src_ip_addr, dst_ip_addr;
    int curTime;
    pkt_hdr = (struct pcap_pkthdr*) malloc(sizeof(pcap_pkthdr));

    char src_ip_char[STRSIZE], dst_ip_char[STRSIZE];

    if((fp = fopen(src_file_name, "rb")) == NULL)  {
        printf("Error: cannot open pcap file\n");
        return;
    }
    pkt_offset = 24;

    vector<Session>::iterator it_session;
    string etherAddr;
    bool etherOnlist = false;
    string party_path = dst_dir_name;
    string full_path;

    while(fseek(fp, pkt_offset, SEEK_SET) == 0)  {

        i ++;      
        if(fread(pkt_hdr, sizeof(pcap_pkthdr), 1, fp) != 1)  {
            printf("read end of file\n");
            break;
        }

        pkt_offset += 16 + (pkt_hdr->caplen);  // next packet's offset

        if(pkt_hdr->caplen != pkt_hdr->len)  {
            printf("pkt_hdr->len != pkt_hdr->caplen, offset error happened\n");
            break;
        }

        // get packet payload
        if(fread(data, 1, pkt_hdr->caplen, fp) != pkt_hdr->caplen)  {
            printf("Read %dth packet data error \n", i);
            printf("Current Packet Size is %d\n", pkt_hdr->caplen);
            break;
        }
        // get eth_hdr
        eth_hdr = (struct Ethernet_hdr*) data; 
        if(eth_hdr->ether_type != (0x0008))  {
            filter_pro ++;
            continue;
        }       
        ip_hdr = (struct IP_hdr*)(data + SIZE_ETHERNET);

        // tunnel user data payload
        //printf("%x\n", ip_hdr->ip_src[0]);
        curTime = pkt_hdr->ts.tv_sec;
        if(*(u_char*)(&ip_hdr->ip_src) == '\n' || *(u_char*)(&ip_hdr->ip_dst) == '\n')  {
            capwap_len = int((*(u_char*)(data+CAPWAPOFFSET+1))>>3) * 4;
            ip_proto_offset = (CAPWAPOFFSET+ capwap_len + 30);

            if(*(u_char*)(data+ip_proto_offset) != (0x0008)) {
                filter_pro ++;
                //printf("%dth packet is filted because its ip protocal is not 0800\n", i);
                continue;
            }

            ip_hdr = (struct IP_hdr*)(data+ip_proto_offset+2);
            
            if(ip_hdr->ip_p != IPPROTO_UDP && ip_hdr->ip_p != IPPROTO_TCP) {
                filter_pro ++;
                //printf("%dth packet is filted because it is not UDP or TCP packet\n", i);
                continue;
            }
        }

        inet_ntop(AF_INET, (void *)&(ip_hdr->ip_src), src_ip_char, 16);
        inet_ntop(AF_INET, (void *)&(ip_hdr->ip_dst), dst_ip_char, 16);

        //printf("From ip address %s to ip address %s\n", src_ip_char, dst_ip_char);


        src_ip_addr = src_ip_char;
        dst_ip_addr = dst_ip_char;

        if(session_list.count(src_ip_addr) != 0)  {
            for(it_session = session_list[src_ip_addr].begin();
                    it_session != session_list[src_ip_addr].end(); it_session ++)  {
                if(curTime <= it_session->endTime && curTime >= it_session->startTime)  {
                    etherAddr = it_session->etherAddr;
                    etherOnlist = true;
                    break;
                }
            }
        }  else  if(session_list.count(dst_ip_addr) != 0)  {
            for(it_session = session_list[dst_ip_addr].begin();
                    it_session != session_list[dst_ip_addr].end(); it_session ++)  {
                if(curTime <= it_session->endTime && curTime >= it_session->startTime)  {
                    etherAddr = it_session->etherAddr;
                    etherOnlist = true;
                    break;
                }
            }
        }

        if(!etherOnlist) {
            filter_nolist ++;
            continue;

        }        
        j ++;
        etherOnlist = false;
        full_path = party_path + "/" + etherAddr;
        output = fopen(full_path.c_str(), "ab");
        if(createdFile.count(full_path) == 0)  {    // write pcap file description
            createdFile.insert(full_path);
            create_pcap_file(full_path.c_str(), output);
        }

        WritePcapPacket(output, pkt_hdr, data, pkt_hdr->caplen);
        fclose(output);        
    }

    printf("%d-%d packets is remained\n", j, i);
    printf("%d packets is filted by protocal and %d packets is filted because ip is not on list \n", filter_pro, filter_nolist);
    fclose(fp);
    free(pkt_hdr);
}

int WritePcapPacket(FILE* file,pcap_pkthdr *header, const unsigned char *data, size_t length)  {

	if(fwrite(header, sizeof(pcap_pkthdr), 1, file) != 1) {
        return 1;
    }

    if(fwrite(data, 1, length, file) != length) {
        return 2;
    }

    return 0;
}

void create_pcap_file(const char* filename, FILE* file)  {
    struct pcap_file_header fh;
    fh.magic = 0xa1b2c3d4;
    fh.sigfigs = 0;
    fh.version_major = 2;
    fh.version_minor = 4;
    fh.snaplen = (1<<16)-1; 
    fh.thiszone = 0;
    fh.linktype = 1;

    //FILE *file = fopen(filename, "ab");
    if(file != NULL) {
        fwrite(&fh, sizeof(fh), 1, file);
    }
}