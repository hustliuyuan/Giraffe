// Huazhong Univserity of Science and Technology.
// project: Netlet Middleware
// author: tuxuping ( borrowed)  
// Author: ArcherSC
// email:  xupingtu@gmail.com, 
// date:   2009-6-23
// all rights reserved.


#include "common/Header.h"
unsigned long in_cksum (unsigned long* data, long len)
{
	long    nleft = len;
	unsigned long sum = 0;
	unsigned short* w = (unsigned short*)data;
	unsigned short answer = 0;

	/*
	9      * Our algorithm is simple, using a 32 bit accumulator (sum), we add
	10      * sequential 16 bit words to it, and at the end, fold back all the
	11      * carry bits from the top 16 bits into the lower 16 bits.
	12      */
	while (nleft > 1) {
		sum += *w++;
		nleft -= 2;
	}
	/* mop up an odd U8, if necessary */
	if (nleft == 1) {
		* (unsigned char *) (&answer) = * (unsigned char *) w;
		sum += answer;
	}

	/* add back carry outs from top 16 bits to low 16 bits */
	sum = (sum >> 16) + (sum & 0xffff); /* add hi 16 to low 16 */
	sum += (sum >> 16);     /* add carry */
	answer = (unsigned short)~sum;     /* truncate to 16 bits */
	return (answer);
}

