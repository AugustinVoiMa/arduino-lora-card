#ifndef _LORA_ADAPTER
#define _LORA_ADAPTER

#define LORA_FREQ 433E6

#include <SPI.h>
#include <LoRa.h>

#include "LoRa_protocol.h"
#include "Serial_adapter.h"

class LoRa_adapter{
  uint32_t lp_address;  
  static uint8_t* packet;
  static uint8_t packet_len;
  static bool new_packet;
public:
  LoRa_adapter(){}
  void init(uint8_t lp_address[]);
  void checkReceived(); 
  
  static void onReceive(int packetSize);
  
  void sendMessage(uint8_t dst[], uint8_t data_len, uint8_t data[]);
  void sendMessage(uint32_t dst, uint8_t data_len, uint8_t data[]);
};

#endif
