/*
 * DispatchTraffic.h
 *
 *  Created on: Apr 28, 2014
 *      Author: sangs
 */

#ifndef DISPATCHTRAFFIC_H_
#define DISPATCHTRAFFIC_H_

#include "MyFileSystem.h"
#include "Headers.h"

void ChoosePcapFile(const char *src_dir_name, const char *dst_dir_name, const char *radius_account_name);

void HandleOnePcapFile(const char *src_file_name, const char *dst_dir_name);

int  WritePcapPacket(FILE* file,pcap_pkthdr *header, const unsigned char *data, size_t length);

void create_pcap_file(const char* filename, FILE* file);

#endif /* DISPATCHTRAFFIC_H_ */
