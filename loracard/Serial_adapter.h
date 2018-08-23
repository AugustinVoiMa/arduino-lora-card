#ifndef _SERIAL_ADAPTER
#define _SERIAL_ADAPTER

#define SERIAL_RATE 4800

#include "Arduino.h"

class Serial_adapter{
  String input;
  void (*callbackOnLocalMessage)(String str);
  static Serial_adapter* sa;  
  static String message;  
  static bool new_message;
public:
  static Serial_adapter* getSerialAdapter(){
    if(nullptr == Serial_adapter::sa){
      Serial_adapter::sa = new Serial_adapter();
    }
    return Serial_adapter::sa;
  }
  
  void setCallBackOnLocalMessage(void (*callbackOnLocalMessage)(String str)){
    this->callbackOnLocalMessage = callbackOnLocalMessage;
  }
  void info(String str);
  void serialEvent();
  void checkNewMessage();

private:
  Serial_adapter();
};


#endif
