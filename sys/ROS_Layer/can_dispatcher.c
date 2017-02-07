#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>


#define CANUSE 0
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




#include "global_pointer.h"
#include "Registry_CAN/register_CAN.h"
#include "Interpreter_Registry/interpreteur_CAN.h"

#include "CAN/lowLevel_CAN_definition.h"

#include "CAN/native_can.h"


#include "can_Interface.h"
#define __packed __attribute__((packed))

#include "ROS_Layer/can_dispatcher.h"

//pointeur globale du singleton
// Allocating and initializing GlobalClass's
// static data member.  The pointer is being
// allocated - not the object inself.
static can_dispatcher *global_ptr = 0 ;



//register_can *registerMessage;


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





//void process_one(struct can_frame *frm);

static void /*can_dispatcher::*/_event_cb(can_t *dev, can_event_t event, uint8_t *message , uint16_t lenght)
{

 printf("receive event from can%i\n", *dev);
 can_frame *frameptr=(can_frame *)message;
 switch(event)
 {
	case CAN_EVENT_RX_COMPLETE:
		if ( (frameptr->can_id & 0x1FFFFFFF) == global_ptr->arb_id)
			process_one(/*(struct can_frame *)message*/frameptr);
		break;
 }
}


 void /*can_dispatcher::*/process_one(struct can_frame *frm)
{
    
    #if defined(DEBUG_RECEIVE)
    printf("lenght: %i\n",frm->can_dlc );
    #endif

    // TODO: ici on met le moment ou le data est enregistrer dans le buffer
    // TODO: on doit faire une fontion qui get le id du message
    
    uint32_t id_client_read = get_canFrame_id(frm);
    struct can_message_t formated_message = parse_receive_message(frm);

    /*registerMessage*/global_ptr->add_message(id_client_read,formated_message);

    #if defined(DEBUG_RECEIVE)
    for(uint8_t i=0; i< frm->can_dlc; i++)
    {
        printf("[ %02X ] ",frm->data[i]);
    }
    printf("\nrecus\n" );
    #endif
}

/*can_dispatcher::can_dispatcher(can_event_cb_t event_callback) 
{

}*/
can_dispatcher::can_dispatcher(void) //: can_dispatcher(&_event_cb)
{
 global_ptr = this;
	arb_id=0;
//creation du driver can
    
    m_can_driver = new can_driver(&_event_cb);
    m_can_driver->init(false);
    
    //creation du registre can        
    //registerMessage = new register_can();
    /*registerMessage*/this->init();
}

void can_dispatcher::run (void)
{
	m_can_driver->run();
}

bool can_dispatcher::resend(uint32_t nodeId)
{
    bool result=false;

    can_data_t * ptr_to_client = get_client(nodeId);
    
    if(ptr_to_client != 0 )
    {
        can_message_t* message_found = 0;
        message_found = get_message(nodeId,ptr_to_client->counter_message-1);
        if( message_found != 0)
        {
         //message trouver
         can_frame payload;

         payload = parse_send_message(message_found);

         m_can_driver->send_message(CAN_0,&payload);
        }
        result=true;
    }
    return result;
}
bool can_dispatcher::update (uint32_t nodeId, struct can_message_t message)
{
    bool result=false;

    can_data_t * ptr_to_client = get_client(nodeId);
    
    if(ptr_to_client != 0 )
    {
        result= add_message(nodeId, message);
        if(result)
        {
         resend(nodeId);
        }
    }
    return result;
}
