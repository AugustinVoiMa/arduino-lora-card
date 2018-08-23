#include "LoRa_protocol.h"


#include "Arduino.h"
namespace lora_proto
{
  static Packet Packet::deserialize(uint8_t size, uint8_t data[]){
    Packet pk;
    uint8_t i = 0;
    pk.src = bytes_to_uint32_t(data[i]);
    i+= 4;
    pk.dst = bytes_to_uint32_t(data[i]);
    i+=4;
    pk.proto = data[i++];
    pk.data_len = data[i++];
    pk.data = data[i];
  }
  
  uint8_t* Packet::serialize(uint8_t* packet_len){
    uint8_t serial[ 10 + this->data_len ]; // Header len + data len
    int i = 0;
    uint32_t_into_bytes(serial[i], this->src);
    i += 4;
    uint32_t_into_bytes(serial[i], this->dst);
    i += 4;
    serial[i++] = this->proto;
    serial[i++] = this->data_len;
    for(int o = 0; o < this->data_len; o++)
      serial[i++] = this->data[o];
    *packet_len = i;
    return serial;
  }
  
  uint32_t bytes_to_uint32_t(uint8_t data[]){
    return (uint32_t)  (data[0] << 24 | data[1] << 16 | data[2] << 8 | data[3]);
  }
  
  void uint32_t_into_bytes(uint8_t dst[], uint32_t n ){
    dst[0]=(n >> 24 ) & 0xFF;
    dst[1]=(n >> 16 ) & 0xFF;
    dst[2]=(n >> 8 ) & 0xFF;
    dst[3]=n & 0xFF;
  }
  
  String address_to_string(uint32_t address){
    String res = "";
    res += String((address>> 24 ) & 0xFF)+".";
    res += String((address>> 16 ) & 0xFF)+".";
    res += String((address>> 8 ) & 0xFF)+".";
    res += String(address & 0xFF);
  }
  
  String Packet::getInfos(){
    String str = "";
    str.reserve(128);
    str += "From: "+address_to_string(this->src)+"\n";
    str += "To: "+address_to_string(this->dst)+"\n";
    str += "Protocol: "+String(this->proto)+"\t";
    str += "Data ("+String(this->data_len)+" bytes)\n";
    return str;
  }
  
  String Packet::getStringData(){
    return (char*)this->data;
  }
};
