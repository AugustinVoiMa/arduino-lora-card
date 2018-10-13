#include "LoRa_adapter.h"
#include "Serial_adapter.h"

#define DEF_LP_ADDRESS
#include "config.h"

uint8_t bcast_lp[] = {255, 255, 255, 255}; // broadcast

Serial_adapter* sa;
LoRa_adapter* la;


void setup(){
  sa = Serial_adapter::getSerialAdapter();
  la = LoRa_adapter::getLoRaAdapter();
  la->init(lp_address);  
  sa->setCallBackOnLocalMessage(&sendMessage);
  sa->info("Ready");
}

void loop(){
  la->checkReceived();
  sa->checkNewMessage();
}


void sendMessage(String str){
  sa->info("sendMessage("+str+");");
  uint8_t message[str.length()];  
  str.toCharArray((char*) message, str.length());
  la->sendMessage( bcast_lp, str.length(), message );
  return ;
}

String input = "";

void serialEvent(){
  sa->serialEvent();
}

