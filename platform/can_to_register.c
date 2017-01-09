



/*
                /register_CAN //used to keep the data
can_to_regiser-<
                \interpreteur_CAN//used to procces the data from the can
*/

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

//#define USE_CURSE
#define CANUSE 0

#include <string.h>
//#if defined( USE_OLD_CODE )
	#include <unistd.h>

	#include <sys/types.h>
	#include <sys/socket.h>
	//#include <linux/can.h>
	#include <linux/can/raw.h>
	#include <net/if.h>
	#include <sys/ioctl.h>

	#include <endian.h>
//#endif
#if defined( USE_CURSE )
	#include <curses.h>
#endif
#include <linux/can.h>
#include "CAN/native_can.h"
#include "can_Interface.h"
#define __packed __attribute__((packed))

#include "global_pointer.h"
#include "Registry_CAN/register_CAN.h"
#include "Interpreter_Registry/interpreteur_CAN.h"
register_can *registerMessage;
can_driver * m_can_driver;

#if !defined( CAN_MTU )
	#define CAN_MAX_DLC 8
	#define CAN_MAX_DLEN 8

	#define CANFD_MAX_DLC 15
	#define CANFD_MAX_DLEN 64
	#define USE_CANFD_FRAME //define used for knowing we use parse_frame
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

	#define CANID_DELIM '#'
	#define DATA_SEPERATOR '.'

	unsigned char asc2nibble(char c)
	{
		if ((c >= '0') && (c <= '9'))
			return c - '0';

		if ((c >= 'A') && (c <= 'F'))
			return c - 'A' + 10;

		if ((c >= 'a') && (c <= 'f'))
			return c - 'a' + 10;

		return 16; /* error */
	}

	int parse_canframe(char *cs, struct canfd_frame *cf)
	{
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
#endif  /* !defined( CAN_MTU ) */



uint32_t arb_id=0;
void process_one(struct can_frame *frm);

static void _event_cb(can_t *dev, can_event_t event, uint8_t *message, uint16_t lenght)
{

 printf("receive event from can%i\n", *dev);
 can_frame *frameptr=(can_frame *)message;
 switch(event)
 {
	case CAN_EVENT_RX_COMPLETE:
		if ( (frameptr->can_id & 0x1FFFFFFF) == arb_id)
			process_one((can_frame *)message);
		break;
 }
}


void process_one(struct can_frame *frm)
{
    
    #if defined(DEBUG_RECEIVE)
    printf("lenght: %i\n",frm->can_dlc );
    #endif

    // TODO: ici on met le moment ou le data est enregistrer dans le buffer
    // TODO: on doit faire une fontion qui get le id du message
    
    uint32_t id_client_read = get_canFrame_id(frm);
    struct can_message_t formated_message = parse_receive_message(frm);

    registerMessage->add_message(id_client_read,formated_message);

    #if defined(DEBUG_RECEIVE)
    for(uint8_t i=0; i< frm->can_dlc; i++)
    {
        printf("[ %02X ] ",frm->data[i]);
    }
    printf("\nrecus\n" );
    #endif
}



int main(int argc, char **argv)
{
	if (argc != 2)
	{
		printf("Syntax: %s <Arbitration ID>\n", argv[0]);
		exit(1);
	}

	//creation du driver can
    #if 0
	m_can_driver = new can_driver(&_event_cb);
	m_can_driver->init(false);
    #endif
    //creation du registre can        
    registerMessage = new register_can();

    //only as a test
    registerMessage->add_client(1);
    registerMessage->add_client(2);
    registerMessage->add_client(3);
    registerMessage->add_client(4);
    registerMessage->add_client(5);
    registerMessage->add_client(6);
    registerMessage->init();
    registerMessage->add_client(7);
    registerMessage->add_client(8);
    registerMessage->add_client(9);
    registerMessage->add_client(10);
    registerMessage->add_client(11);
    registerMessage->add_client(12);

    registerMessage->print_list_clientId();
    #if 0
		#if defined(USE_CANFD_FRAME)
			canfd_frame frame;

			if( parse_canframe(argv[1], &frame))
			{
				//TODO: ici on met le moment ou on doit lire si on doit envoyer un message

					//To change Because it's transforming canfd into canframe
					// Because I'v not found other way
					can_frame localframe;
					//transfer all frame
					localframe.can_id		= frame.can_id;
					localframe.can_dlc	= (frame.len < 8 ? frame.len:7);
					//localframe.__pad		= frame.flags;
					//localframe.__res0		= frame.__res0;
					//localframe.__res1		= frame.__res1;
					for(uint8_t i=0;i<8;i++)
					{
						localframe.data[i]	=	frame.data[i];
					}
				m_can_driver->send_message(CAN_0,(can_frame*)&frame);
			}
			else
			{
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
			}
		#endif /* USE_CANFD_FRAME */
    #endif
    while(1)//TODO le reste du code.
    {
    }

	return 0;
}
