#include <gmpxx.h>
#include <stdio.h>

#include "RabinKarp.h"


#define MAGICNUM 10

void RabinKarpAlo(char *payLoad, int payLoadLen, vector<unsigned int> &keys, int blockSize, int samplingRate)  {
	const unsigned int MAXINTEGER = 4294967295;
	mpz_class product(0);

	mpz_class head(1);

	for(int i = 1; i < blockSize; i ++)  {
		head *= MAGICNUM;
	}

	unsigned int localMax = 0;

	// init a key
	for(int i = 0; i < blockSize; i ++)  {
		product =  product * MAGICNUM + (unsigned int)(payLoad[i]-'0'); 
	}

	unsigned long candiMax;
	mpz_class part(1);

	for(int cursor = 1; cursor+samplingRate < payLoadLen; cursor += samplingRate)  {
		localMax = 0; // init

		for(int offset = cursor; offset < cursor + samplingRate; offset ++)  {
			product = product - head * (payLoad[offset-1]-'0');
			product = product * MAGICNUM;
			product = product + (unsigned int)(payLoad[offset+blockSize-1]-'0');
			
			part = product % MAXINTEGER;
			candiMax = part.get_ui();
			
			printf("%ld\n", candiMax);

			if(candiMax > localMax)  {
				localMax = (unsigned int)candiMax;
			}
			
		}

		keys.push_back(localMax);
		printf("Current biggest number is %u\n", localMax);
	}

}

/*
int main()  {
	char str[10] = "123456789";
	int payLoadLen = 9;
	vector<unsigned int> keys;

	RabinKarpAlo(str, payLoadLen, keys, 2, 4);
	return 0;
}
*/