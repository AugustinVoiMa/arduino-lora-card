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
    this->serial = (uint8_t*) malloc(sizeof(uint8_t) * (10 + this->data_len)) ; // Header len + data len
    int i = 0;
    uint32_t_into_bytes(&(this->serial[i]), this->src);    
    i += 4;
    uint32_t_into_bytes(&(this->serial[i]), this->dst);
    i += 4;
    this->serial[i++] = this->proto;
    this->serial[i++] = this->data_len;
    for(int o = 0; o < this->data_len; o++)
      this->serial[i++] = this->data[o];
    *packet_len = i;
    return this->serial;
  }
  
  uint32_t bytes_to_uint32_t(uint8_t data[]){
    return (uint32_t) data[0] << 24 
          |(uint32_t) data[1] << 16 
          |(uint32_t) data[2] << 8 
          |(uint32_t) data[3];
  }
  
  void uint32_t_into_bytes(uint8_t dst[], uint32_t n ){    
    dst[0] = (uint8_t)(n>>24);
    dst[1] = (uint8_t)(n>>16);
    dst[2] = (uint8_t)(n>>8);
    dst[3] = (uint8_t)n;    
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
