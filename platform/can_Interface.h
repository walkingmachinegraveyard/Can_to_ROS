#ifndef H_CAN_INTERFACE
#define H_CAN_INTERFACE

namespace can {

  class canInterface
  {
  public:
    canInterface();
    ~canInterface();

  void  init(bool recv_own_msgs);
  };

} //namespace can

#endif //H_CAN_INTERFACE
