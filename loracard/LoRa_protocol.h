/**
  Data Link Layer (256 bytes):
      |       |       |       |       |
  0   |     src_node_lp               | # LoRa Protocol addresses of the source
  10  |     dst_node_lp               | # LoRa Protocol addresses of the destination nodes
  20  | nwp   | len   |   data          # nwp: network protocol id (for identification of the protocol to use with data) # len the length of following data (max data len: 240)
  30                 ...


  Network Layer: IP network


*/
#ifndef _LORA_PROTOCOL
#define _LORA_PROTOCOL

#include "Arduino.h"



namespace lora_proto{
  class Packet{
    uint32_t src, dst;
    uint8_t proto, data_len;
    uint8_t* data;    
    uint8_t* serial;
  public:    
    static Packet create(uint32_t src, uint32_t dst, uint8_t proto, uint8_t data_len, uint8_t* data ){ // TX
      Packet pk = Packet();
      pk.src=src;
      pk.dst=dst;
      pk.proto=proto;
      pk.data_len=data_len;
      pk.data=data;
      return pk;
    }

    static Packet deserialize(uint8_t size, uint8_t* data); // deserialize RX

    uint8_t* serialize(uint8_t* buf, uint8_t* packet_len);
    
    bool accept(uint32_t lp_address){ // True if localhost is the destination of this packet or broadcast
      /*Serial.print("lp_address: \t");
      Serial.println(lp_address, HEX);
      Serial.print("dst: \t ");
      Serial.println(this->dst, HEX);*/
      return lp_address==this->dst || this->dst==0xFFFFFFFF;
    }

    void printInfos();
    char* getStringData();
    uint32_t getDst(){ return this->dst; }
  };


  void printRawPacket(uint8_t* pk, uint8_t len);
  uint32_t bytes_to_uint32_t(uint8_t data[]);
  void uint32_t_into_bytes(uint8_t dst[], uint32_t n );
};

#endif
