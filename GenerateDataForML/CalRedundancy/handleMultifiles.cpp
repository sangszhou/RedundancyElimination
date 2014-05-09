#include <stdio.h>
#include <string>
#include <cstring>
#include <vector>
#include <boost/filesystem.hpp>

#include "headers.h"
#include "../BloomFilter/bloom.h"
#include "../MyFileSystem/MyFileSystem.h"

using namespace std;
using namespace boost::filesystem;

#define STRSIZE 1024
#define DATASIZE 65535
#define CAPWAPOFFSET 42
#define MAXTHREAD 4


void HandleOnePcapFile(const char *src_file_name, FILE *output, int blockSize, int samplingRate)  {

	struct pcap_pkthdr *pkt_hdr;
	struct Ethernet_hdr *eth_hdr;
	struct IP_hdr *ip_hdr;
	u_char data[DATASIZE];
	u_char payload[DATASIZE];
	FILE *fp;

	string src_ip_addr, dst_ip_addr;
    char src_ip_char[STRSIZE], dst_ip_char[STRSIZE];

    pkt_hdr = (struct pcap_pkthdr*) malloc(sizeof(pcap_pkthdr));

    int ip_proto_offset, payloadOffset;
    int i = 0;  // count packets
    int capwap_len;
    int payloadLen;
    int pkt_offset = 24;
    
    if((fp = fopen(src_file_name, "rb")) == NULL)  {
        printf("Error: cannot open pcap file\n");
        return;
    }

    // init cache
    // set false positive to 0.03 which need 5 hash functions
    // set sampling rate to 
    
    unsigned int fileSize = file_size(src_file_name);
    printf("File size is %d\n", fileSize);
    fileSize /= samplingRate;    // sampling every sampling rate bytes
    bloom cache;
    bloom_init(&cache, fileSize, 0.01);

    // set block size to 32 bytes

    char *localOpt;
    char *candiOpt;

    // hit infomation
    int originalHit = 0;
    int increHit = 0;
    int byteSaving = 0;
    int totalByte = 0;
    
    while(fseek(fp, pkt_offset, SEEK_SET) == 0)  {

        i ++;      
        if(fread(pkt_hdr, sizeof(pcap_pkthdr), 1, fp) != 1)  {
            printf("read end of file\n");
            break;
        }

        pkt_offset += 16 + (pkt_hdr->caplen);  // next packet's offset

        // get packet payload

        fread(data, 1, pkt_hdr->caplen, fp);
        ip_hdr = (struct IP_hdr*)(data + SIZE_ETHERNET);
        ip_proto_offset = SIZE_ETHERNET;
        // tunnel user data payload
        //printf("%x\n", ip_hdr->ip_src[0]);

        if(*(u_char*)(&ip_hdr->ip_src) == '\n' || *(u_char*)(&ip_hdr->ip_dst) == '\n')  {
            capwap_len = int((*(u_char*)(data+CAPWAPOFFSET+1))>>3) * 4; // capwap_len can be 8 or 16 bytes
            ip_proto_offset = (CAPWAPOFFSET+ capwap_len + 32);
            ip_hdr = (struct IP_hdr*)(data+ip_proto_offset);          // read ip header
        }

        // ip  header is 20 bytes
        // udp header is 8 bytes
        // tcp header is 20 bytes
        
        if(ip_hdr->ip_p == IPPROTO_UDP)  {              
            payloadOffset = ip_proto_offset + 28;
        }  else  {
            payloadOffset = ip_proto_offset + 40;
        }

        payloadLen = pkt_hdr->caplen - payloadOffset;

        if(payloadLen <= 40)  {
            continue;
        } 

        totalByte += payloadLen;

        // sampling every samplingRate bytes

     	for(int payloadCursor = payloadOffset; payloadCursor+blockSize < payloadLen; payloadCursor += samplingRate)  { 
     		localOpt = (char*)(data+payloadCursor);  // init localOpt

            // MAXP algorithm, without below code 
            // it is MODP algorithm

            for(int j = payloadCursor+1; j < samplingRate+payloadCursor; j ++)  {
                candiOpt = (char*)(data+j);

                if(strncmp(localOpt, candiOpt, blockSize) < 0)  {
                    localOpt = candiOpt;
                }

            }
            
            // if block has already in there
            // check the entair 

            if(bloom_check(&cache, localOpt, blockSize) == 1)  {
                originalHit ++;
                byteSaving += blockSize;
                /*
                for(int j = payloadCursor; j < samplingRate+payloadOffset; j ++)  {
                    candiOpt = (char*)(data+j);
                    if(bloom_check(&cache, candiOpt, blockSize) == 1)  {
                        increHit ++;
                        byteSaving += 1;
                    }
                }
                */
            }  else  {
                bloom_add(&cache, localOpt, blockSize);
            }
     		
     	}

    }

    fprintf(output, "%s %d %d %d %d %lf\n", src_file_name, originalHit, increHit, byteSaving, totalByte, 1.0 * byteSaving/totalByte);

    /*
    printf("OriginalHit = %d\n", originalHit);
    printf("increHit = %d\n", increHit);
    printf("Traffic saving = %d\n", byteSaving);
    printf("TotalTraffic = %d\n", totalByte);
    printf("%lf\n", 1.0*byteSaving/totalByte);
    */
}

void HandleGroupPcapFiles(const vector<string> &nameList, int index, const char *dstFileName)  {
    FILE *output = fopen(dstFileName, "a");

    for(int i = 0; i < nameList.size(); i ++)  {
        if(i%MAXTHREAD == index)  {
            HandleOnePcapFile(nameList[i].c_str(), output, 64, 128);
        }
    }
    fclose(output);
}

void HandlePcapFiles(const char *srcDirName, const char *dstFileName)  {
    vector<string> fileList;
    getFileNames(srcDirName, fileList);

    FILE *output = fopen(dstFileName, "a");

    for(int i = 0; i < fileList.size(); i ++)  {
        //if(i % 4 == 0)  {
            printf("Processing %s\n", fileList[i].c_str());
            HandleOnePcapFile(fileList[i].c_str(), output,  64, 128);
        //}
    }
    fclose(output);
}

int main()  {
    char srcDirName[100] = "/re/sampleData/input";
    char dstDirName[100] = "/re/sampleData/output/out0.txt";
    
    HandlePcapFiles(srcDirName, dstDirName);

    return 0;
}
