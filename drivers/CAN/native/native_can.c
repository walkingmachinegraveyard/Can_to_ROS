
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

//use by can
#include <sys/types.h>
#include <sys/socket.h>
#include <linux/can.h>
#include <linux/can/raw.h>
#include <net/if.h>
#include <sys/ioctl.h>

#include "native_can.h"

//unshure//#include <endian.h>

can_driver::can_driver() : canInterface()
{
  for(uint8_t i= 0; i!=CAN_NUMOF; i++)
  {
    m_socket[i]=0;
    m_current_mode[i]=mode_t::receive;
  }
}

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

uint8_t can_driver::send_message(can_t canID, can_frame *message)
{
  uint8_t result= 0;

  if (!(message == 0) && !(CAN_NUMOF == 0))
  {
    //setMode(canID, modde_t::transmit);
    setMode(canID,mode_t::transmit);

  }
  return result;
}
