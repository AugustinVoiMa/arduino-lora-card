#include "Serial_adapter.h"



Serial_adapter::Serial_adapter(){  
  Serial.begin(SERIAL_RATE);  
  this->input = "";
  this->input.reserve(200);
}

void setCallBackOnLocalMessage(void (&callbackOnLocalMessage)(String str));

void Serial_adapter::info(String str){
  Serial.println(str);
}

void Serial_adapter::serialEvent(){
  while(Serial.available()){
    char inChar = (char)Serial.read();  
    this->input += inChar;
    if (inChar == '\n'){
      /*this->info("1 CallBack on "+this->input);
      this->callbackOnLocalMessage(input);*/
      if(Serial_adapter::new_message){
        this->info("message queue is not empty");
        return;
      }
      Serial_adapter::message = this->input;
      Serial_adapter::new_message = true;
      this->input = "";
      this->input.reserve(200);
    }
  }
}

void Serial_adapter::checkNewMessage(){
  if(! Serial_adapter::new_message)
    return ;
  Serial_adapter::new_message = false;
  this->info("New Message detected: "+this->message);
  this->callbackOnLocalMessage(input);
}

Serial_adapter* Serial_adapter::sa = nullptr;

bool Serial_adapter::new_message = false;
String Serial_adapter::message = "";

