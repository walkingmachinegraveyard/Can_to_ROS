#ifndef H_REGISTER_CAN
#define H_REGISTER_CAN

#include <stdint.h>
#include <iostream>
#include <algorithm>
#include <list>
#include <vector>

#define MAX_CLIEN_CAN 10
#define MAX_MESSAGE_CAN 20

/* Id for CAN message data type. */
/* Service code, see CAN WM specification. */
/* Message code, see CAN WM specification. */
/* the lenght of the data */
/* the message gived */
struct can_message_t {
  uint8_t 	dataType;
  uint8_t 	service_Code;
  uint8_t 	message_Code;
  uint8_t* 	lenght;
  uint8_t* 	data;
};

struct can_data_t {
  uint8_t 	nodeId; /*    	Id of transmitting/receiving node. */

  std::list<can_message_t> m_list_of_message;

};


class register_can {
private:
	std::list<can_data_t> m_list_of_clien;

public:
	//add
	//remove
	//find
	//get_list_id
};

#ifdef __cplusplus
extern "C" {
#endif


#ifdef __cplusplus
}
#endif

#endif //H_REGISTER_CAN
