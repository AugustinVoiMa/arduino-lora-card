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
      this->info("1 CallBack on "+this->input);
      this->callbackOnLocalMessage(input);
      this->input = "";
      this->input.reserve(200);
    }
  }
}
Serial_adapter* Serial_adapter::sa = nullptr;


