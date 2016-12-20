#ifndef H_REGISTER_CAN
#define H_REGISTER_CAN

#ifdef __cplusplus
extern "C" {
#endif

#include <list>

#define MAX_CLIEN_CAN 10
#define MAX_MESSAGE_CAN 20

struct can_data_t {
  uint8_t 	nodeId, /*    	Id of transmitting/receiving node. */

  std::list<can_message_t> m_list_of_message,

};

class register_can {
private:
  std::list<can_message_t> m_list_of_clien;

public:

};


#ifdef __cplusplus
}
#endif

#endif //H_REGISTER_CAN
