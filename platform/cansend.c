/*
 * Original work copyright 2013 Fabio Baltieri <fabio.baltieri@gmail.com>
 * Modified work copyright 2015 Dan Kouba
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 */

#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <linux/can.h>
//#include "lib.h"
#include <linux/can/raw.h>
#include <net/if.h>
#include <sys/ioctl.h>
#if defined( USE_CURSE )
	#include <curses.h>
#endif
#include <endian.h>

//#include "lib.h"
//patch for the moment//
#define CAN_MAX_DLC 8
#define CAN_MAX_DLEN 8

#define CANFD_MAX_DLC 15
#define CANFD_MAX_DLEN 64


#if !defined( CAN_MTU )
	struct canfd_frame {
         canid_t can_id;  /* 32 bit CAN_ID + EFF/RTR/ERR flags */
         __u8    len;     /* frame payload length in byte */
         __u8    flags;   /* additional flags for CAN FD */
         __u8    __res0;  /* reserved / padding */
         __u8    __res1;  /* reserved / padding */
         __u8    data[CANFD_MAX_DLEN] __attribute__((aligned(8)));
	 };
	#define CAN_MTU         (sizeof(struct can_frame))
	#define CANFD_MTU       (sizeof(struct canfd_frame))
#endif

#define CANID_DELIM '#'
#define DATA_SEPERATOR '.'


unsigned char asc2nibble(char c) {

	if ((c >= '0') && (c <= '9'))
		return c - '0';

	if ((c >= 'A') && (c <= 'F'))
		return c - 'A' + 10;

	if ((c >= 'a') && (c <= 'f'))
		return c - 'a' + 10;

	return 16; /* error */
}
///
#define __packed __attribute__((packed))

// SocketCAN flags
// Upper 3 bits of ID integer in CAN frame are used for these flags
#define CAN_EFF_FLAG 0x80000000U
#define CAN_RTR_FLAG 0x40000000U
#define CAN_ERR_FLAG 0x20000000U

static int sk;
//static int frame_cnt = 0;
//static int valid_frame_cnt = 0;
static struct canfd_frame/*can_frame*/ frame;

union dataframe {

	struct __packed {
        uint8_t b7;
        uint8_t b6;
        uint8_t b5;
        uint8_t b4;
        uint8_t b3;
        uint8_t b2;
        uint8_t b1;
        uint8_t b0;
    } generic;

};


int parse_canframe(char *cs, struct canfd_frame *cf) {
	/* documentation see lib.h */

	int i, idx, dlen, len;
	int maxdlen = CAN_MAX_DLEN;
	int ret = CAN_MTU;
	unsigned char tmp;

	len = strlen(cs);
	//printf("'%s' len %d\n", cs, len);

	memset(cf, 0, sizeof(*cf)); /* init CAN FD frame, e.g. LEN = 0 */

	if (len < 4)
		return 0;

	if (cs[3] == CANID_DELIM) { /* 3 digits */

		idx = 4;
		for (i=0; i<3; i++){
			if ((tmp = asc2nibble(cs[i])) > 0x0F)
				return 0;
			cf->can_id |= (tmp << (2-i)*4);
		}

	} else if (cs[8] == CANID_DELIM) { /* 8 digits */

		idx = 9;
		for (i=0; i<8; i++){
			if ((tmp = asc2nibble(cs[i])) > 0x0F)
				return 0;
			cf->can_id |= (tmp << (7-i)*4);
		}
		if (!(cf->can_id & CAN_ERR_FLAG)) /* 8 digits but no errorframe?  */
			cf->can_id |= CAN_EFF_FLAG;   /* then it is an extended frame */

	} else
		return 0;

	if((cs[idx] == 'R') || (cs[idx] == 'r')){ /* RTR frame */
		cf->can_id |= CAN_RTR_FLAG;

		/* check for optional DLC value for CAN 2.0B frames */
		if(cs[++idx] && (tmp = asc2nibble(cs[idx])) <= CAN_MAX_DLC)
			cf->len = tmp;

		return ret;
	}

	if (cs[idx] == CANID_DELIM) { /* CAN FD frame escape char '##' */

		maxdlen = CANFD_MAX_DLEN;
		ret = CANFD_MTU;

		/* CAN FD frame <canid>##<flags><data>* */
		if ((tmp = asc2nibble(cs[idx+1])) > 0x0F)
			return 0;

		cf->flags = tmp;
		idx += 2;
	}

	for (i=0, dlen=0; i < maxdlen; i++){

		if(cs[idx] == DATA_SEPERATOR) /* skip (optional) separator */
			idx++;

		if(idx >= len) /* end of string => end of data */
			break;

		if ((tmp = asc2nibble(cs[idx++])) > 0x0F)
			return 0;
		cf->data[i] = (tmp << 4);
		if ((tmp = asc2nibble(cs[idx++])) > 0x0F)
			return 0;
		cf->data[i] |= tmp;
		dlen++;
	}
	cf->len = dlen;

	return ret;
}

static int net_init(char *ifname)
{
	//int recv_own_msgs;
	struct sockaddr_can addr;
	struct ifreq ifr;

	sk = socket(PF_CAN, SOCK_RAW, CAN_RAW);
	if (sk < 0) {
		perror("socket");
		exit(1);
	}

	memset(&ifr.ifr_name, 0, sizeof(ifr.ifr_name));
	strncpy(ifr.ifr_name, ifname, IFNAMSIZ);
	if (ioctl(sk, SIOCGIFINDEX, &ifr) < 0) {
		perror("SIOCGIFINDEX");
		exit(1);
	}

	memset(&addr, 0, sizeof(addr));
	addr.can_family = AF_CAN;
	addr.can_ifindex = ifr.ifr_ifindex;
	if (bind(sk, (struct sockaddr *)&addr, sizeof(addr)) < 0) {
		perror("bind");
		return 1;
	}


	setsockopt(sk, SOL_CAN_RAW, CAN_RAW_FILTER,
			NULL, 0);

	return 0;
}

static int send_one(int req_mtu)
{
if (!req_mtu){
		fprintf(stderr, "\nWrong CAN-frame format! Try:\n\n");
		fprintf(stderr, "    <can_id>#{R|data}          for CAN 2.0 frames\n");
		fprintf(stderr, "    <can_id>##<flags>{data}    for CAN FD frames\n\n");
		fprintf(stderr, "<can_id> can have 3 (SFF) or 8 (EFF) hex chars\n");
		fprintf(stderr, "{data} has 0..8 (0..64 CAN FD) ASCII hex-values (optionally");
		fprintf(stderr, " separated by '.')\n");
		fprintf(stderr, "<flags> a single ASCII Hex value (0 .. F) which defines");
		fprintf(stderr, " canfd_frame.flags\n\n");
		fprintf(stderr, "e.g. 5A1#11.2233.44556677.88 / 123#DEADBEEF / 5AA# / ");
		fprintf(stderr, "123##1 / 213##311\n     1F334455#1122334455667788 / 123#R ");
		fprintf(stderr, "for remote transmission request.\n\n");
		return 1;
}

/* send frame */
	if (write(sk, &frame, req_mtu) != req_mtu) {
		perror("write");
		return 1;
}
close(sk);
return 0;
}

int main(int argc, char **argv)
{
	if (argc != 3) {
		printf("Syntax: %s <Interface Name> <Arbitration ID>\n", argv[0]);
		exit(1);
	}

//    uint32_t id = (uint32_t)strtol(argv[2], NULL, 16) & 0x7FFFFFFF;
#if defined( USE_CURSE )
    initscr();
#endif
	net_init(argv[1]);

	/* parse CAN frame */
	int required_mtu;
	required_mtu = parse_canframe(argv[2], &frame);
	send_one(required_mtu );

	/*for (;;)
		receive_one(id);*/

#if defined( USE_CURSE )
	endwin();
#endif
	return 0;
}
