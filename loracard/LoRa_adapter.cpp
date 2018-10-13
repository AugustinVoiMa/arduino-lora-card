#include "LoRa_adapter.h"


void LoRa_adapter::init(uint8_t lp_address[]){
  
  Serial_adapter* sa = Serial_adapter::getSerialAdapter();
  this->lp_address=lora_proto::bytes_to_uint32_t(lp_address);  
  if (!LoRa.begin(LORA_FREQ)) {
    sa->info("LoRa init failed. Check your connections.");
    while (true);
  }
  LoRa.enableCrc();
  LoRa.onReceive(LoRa_adapter::onReceive);
  LoRa.receive();  
}



void LoRa_adapter::onReceive(int packetSize) {
  if(LoRa_adapter::new_packet){
    Serial.println("Received a new packet while another is being processed");
    return;
  }
  Serial_adapter* sa = Serial_adapter::getSerialAdapter();
  sa->info("Got a packet");  
  if (packetSize == 0) {
    sa->info("Packet empty");
    return;
  }
  uint8_t i = 0;
  while (LoRa.available()) {
    LoRa_adapter::packet[i++] = (uint8_t) LoRa.read();
  }
  if (i != packetSize) {
    sa->info("Packet length different from size expected");
    return;
  }  
  LoRa_adapter::packet_len = i;
  LoRa_adapter::new_packet = true; 
  sa->info("Packet capture success");
  i = 0;  
}

bool LoRa_adapter::checkReceived(){
  if(! LoRa_adapter::new_packet)
    return false;
  Serial_adapter* sa = Serial_adapter::getSerialAdapter();
  if(SERIAL_DEBUG){
    sa->info("Check received ok! row packet: ");
    int i = 0;    
    while( i < LoRa_adapter::packet_len){
      for(int o = 0; o < 4 && i+o < LoRa_adapter::packet_len; o++){
        sa->info_nnl(String(LoRa_adapter::packet[i+o], HEX)+"\t");      
      }
      sa->info("\n");
      i+= 4;
    }
    sa->info("");
    sa->info("");
  }

  lora_proto::Packet rx = lora_proto::Packet::deserialize(LoRa_adapter::packet_len,LoRa_adapter::packet);  
  if(rx.accept(this->lp_address)){
    sa->info("==========ACCEPTED==========");
    rx.printInfos();
    sa->info(rx.getStringData());
    // TODO: use packet
  }else{
    sa->info("==========REFUSED==========");
    rx.printInfos();
    sa->info(rx.getStringData());
  }

  LoRa_adapter::new_packet = false;
  return true;

}


void LoRa_adapter::sendMessage(uint8_t dst[], uint8_t data_len, uint8_t data[]){
  this->sendMessage(
    lora_proto::bytes_to_uint32_t(dst),
    data_len, data);
}

void LoRa_adapter::sendMessage(uint32_t dst, uint8_t data_len, uint8_t data[]){
  Serial_adapter* sa = Serial_adapter::getSerialAdapter();
  
  sa->info("Sending message from "+String(this->lp_address, HEX)+" to "+String(dst, HEX));
  
  LoRa_adapter::new_packet=true;
  lora_proto::Packet pk = lora_proto::Packet::create(this->lp_address, dst, 0, data_len, data);
  uint8_t packet_len;
  uint8_t* packet = pk.serialize(LoRa_adapter::packet, &packet_len);  
  if(!LoRa.beginPacket()){  
    sa->info("Error beginning LoRa packet");
    LoRa.receive();
    return;
  }  

  if(SERIAL_DEBUG){
    int i = 0;
    while( i < packet_len){
      for(int o = 0; o < 4 && i+o < packet_len; o++){
        sa->info_nnl(String(packet[i+o], HEX)+"\t");
      }
      sa->info("");
      i+= 4;
    }
  }
  
  LoRa.write(packet, packet_len);
  
  LoRa.endPacket();
  sa->info("Sent");
  LoRa_adapter::new_packet=false;
  LoRa.receive();
  
  
}

bool LoRa_adapter::new_packet = false;
uint8_t* LoRa_adapter::packet = (uint8_t*) malloc(sizeof(uint8_t) * 255);
uint8_t  LoRa_adapter::packet_len = 0;
LoRa_adapter* LoRa_adapter::la = nullptr;


