
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

#define USE_DEBUG



#include "CAN/native_can.h"

//unshure//#include <endian.h>

void can_driver::run(void)
{
  //TODO:change how we process each can
  for(uint8_t i= 0; i!=CAN_NUMOF; i++)
  {
    struct can_frame frm;
    struct sockaddr_can addr;
    int ret;
    socklen_t len;

    ret = recvfrom(m_socket[i], &frm, sizeof(struct can_frame), 0,
        (struct sockaddr *)&addr, &len);
    if (ret < 0) {
      perror("can raw socket read");
      exit(1);
    }

    frame_cnt[i]++;
    can_t cantindex=(can_t)i;
    m_event_callback(&cantindex,can_event_t::CAN_EVENT_RX_COMPLETE,(uint8_t*)&frm,len);
  }
}

can_driver::can_driver(can_event_cb_t event_callback) : canInterface(event_callback)
{
  for(uint8_t i= 0; i!=CAN_NUMOF; i++)
  {
    m_socket[i]=0;
    #if defined( ERROR_SALCO_01_PAS_SUR_UTILITER )
    m_current_mode[i]=mode_t::receive;
    #endif
    frame_cnt[i]=0;

  }
}/*
can_driver::can_driver(void* event_callback) : canInterface((can_event_cb_t) event_callback)
{
  for(uint8_t i= 0; i!=CAN_NUMOF; i++)
  {
    m_socket[i]=0;
    #if defined( ERROR_SALCO_01_PAS_SUR_UTILITER )
    m_current_mode[i]=mode_t::receive;
    #endif
    frame_cnt[i]=0;

  }
}*/



uint8_t  can_driver::init(bool recv_own_msgs, can_t canID)
{
  struct sockaddr_can addr;
	struct ifreq ifr;

  if(m_socket[canID] == 0)
  {
    m_socket[canID] = socket(PF_CAN, SOCK_RAW, CAN_RAW);
    if (m_socket[canID] < 0) {
      perror("socket");
      exit(1);
    }

    //clean the variable
    memset(&ifr.ifr_name, 0, sizeof(ifr.ifr_name));
    //we set it
    strncpy(ifr.ifr_name, CAN_INTERFACE_NAME, IFNAMSIZ);
    if (ioctl(m_socket[canID], SIOCGIFINDEX, &ifr) < 0) {
      perror("SIOCGIFINDEX");
      exit(1);
    }

    //clean the variable
    memset(&addr, 0, sizeof(addr));
    addr.can_family = AF_CAN;
    addr.can_ifindex = ifr.ifr_ifindex;
    if (bind(m_socket[canID], (struct sockaddr *)&addr, sizeof(addr)) < 0) {
      perror("bind");
      return 1;
    }
  }
  int i_recv_own_msgs = (recv_own_msgs? 1:0); // 0 = disabled (default), 1 = enabled //
  setsockopt(m_socket[canID], SOL_CAN_RAW, CAN_RAW_RECV_OWN_MSGS,
      &i_recv_own_msgs, sizeof(i_recv_own_msgs));

  return 0;
}
uint8_t  can_driver::init(bool recv_own_msgs)
{
  //int recv_own_msgs;


  for(uint8_t nbr_of_can = 0; nbr_of_can != CAN_NUMOF ; nbr_of_can++ )
  {
    this->init(recv_own_msgs, (can_t) nbr_of_can);
  }
  return 0;
}

#if defined( ERROR_SALCO_01_PAS_SUR_UTILITER )
uint8_t can_driver::set_in_receive_mode(can_t canID)
{
  uint8_t result =0;
     if(m_current_mode[canID] != mode_t::receive)
     {
       //TODO: check befor if nobody using it
       //TODO: put a way to customise if yes or no we want calback
       init(false, canID);
     }
 return result;
}

uint8_t can_driver::set_in_transmit_mode(can_t canID)
{
  uint8_t result =0;
     if(m_current_mode[canID] != mode_t::transmit)
     {
       //TODO: check befor if nobody using it

       init(false, canID);
     }
 return result;
}

void can_driver::setMode(can_t canID, mode_t mode)
{
  if(canID < CAN_NUMOF)
  {
    switch (mode)
    {
      case mode_t::receive:
        set_in_receive_mode(canID);
        break;
      case mode_t::transmit:
        set_in_receive_mode(canID);
        break;
    }
  }
}
#endif
uint8_t can_driver::send_message(can_t canID, can_frame *message)
{
  uint8_t result= 0;
  uint8_t nbyte=0;
  if (!(message == 0) && !(CAN_NUMOF == 0))
  {
    //setMode(canID, modde_t::transmit);
    #if defined( ERROR_SALCO_01_PAS_SUR_UTILITER )
    setMode(canID,mode_t::transmit);
    #endif

    #if defined(USE_DEBUG)
    printf("send_message:\n" );
    printf("can_id: %04x\n",message->can_id );
    printf("can_dlc: %02x\n",message->can_dlc );
    printf("data:\n" );
    for(uint8_t i=0; i< message->can_dlc; i++)
    {
      printf("[ %02X ] ",message->data[i]);
    }

    #endif
    nbyte= write(m_socket[canID], message, sizeof(struct can_frame));
    if (nbyte == 0)
    {
  		perror("write");
  		return 1;
    }
    printf("envoyer: %i\n", nbyte );

    #if defined( ERROR_SALCO_01_PAS_SUR_UTILITER )
    setMode(canID,mode_t::receive);
    #endif
  }
  return result;
}
