#ifndef _LORA_ADAPTER
#define _LORA_ADAPTER

#define LORA_FREQ 433E6

#include <SPI.h>
#include <LoRa.h>
#include "config.h"


#include "LoRa_protocol.h"
#include "Serial_adapter.h"

class LoRa_adapter{
  uint32_t lp_address;    
  static uint8_t* packet;
  static uint8_t packet_len;
  static bool new_packet;
  static LoRa_adapter* la;
public:
  static LoRa_adapter* getLoRaAdapter(){
    if(nullptr == LoRa_adapter::la){
      LoRa_adapter::la = new LoRa_adapter();
    }
    return LoRa_adapter::la;
  }  
  void init(uint8_t lp_address[]);
  bool checkReceived(); 
  
  static void onReceive(int packetSize);
  
  void sendMessage(uint8_t dst[], uint8_t data_len, uint8_t data[]);
  void sendMessage(uint32_t dst, uint8_t data_len, uint8_t data[]);
};

#endif
