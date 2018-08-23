#include "LoRa_adapter.h"

void LoRa_adapter::init(uint8_t lp_address[]){
  Serial_adapter sa = *Serial_adapter::getSerialAdapter();
  this->lp_address=lora_proto::bytes_to_uint32_t(lp_address);

  if (!LoRa.begin(LORA_FREQ)) {
    sa.info("LoRa init failed. Check your connections.");
    while (true);
  }
  LoRa.onReceive(LoRa_adapter::onReceive);
  LoRa.receive();  
}



void LoRa_adapter::onReceive(int packetSize) {
  Serial_adapter* sa = Serial_adapter::getSerialAdapter();
  sa->info("Got a packet");
  if (packetSize == 0) return;

  uint8_t serial_packet[packetSize];
  uint8_t i = 0;
  while (LoRa.available()) {
    serial_packet[i++] = LoRa.read();
  }

  if (i != packetSize) {
    // TODO: error length does not match expected
    return;
  }
  LoRa_adapter::packet = serial_packet;
  LoRa_adapter::packet_len = i;
  LoRa_adapter::new_packet = true; 
  sa->info("Packet capture ok");

}
void LoRa_adapter::checkReceived(){
  if(! LoRa_adapter::new_packet)
    return ;
  
  LoRa_adapter::new_packet = false;
  
  Serial_adapter sa = *Serial_adapter::getSerialAdapter();
  sa.info("Check received ok! row packet: ");
  int i = 0;
  while( i < LoRa_adapter::packet_len){
    for(int o = 0; o < 4 && i+o < LoRa_adapter::packet_len; o++){
      Serial.print(LoRa_adapter::packet[i], HEX);
      Serial.print(" ");
    }
    Serial.println();
    i+= 4;
  }

  Serial.println();
  Serial.println();
  lora_proto::Packet rx = lora_proto::Packet::deserialize(LoRa_adapter::packet_len,LoRa_adapter::packet);  
  if(rx.accept(this->lp_address)){
    sa.info("==========ACCEPTED==========");
    sa.info(rx.getInfos());
    sa.info(rx.getStringData());
  }else{
    sa.info("==========REFUSED==========");
    sa.info(rx.getInfos());
    sa.info(rx.getStringData());
  }
}


void LoRa_adapter::sendMessage(uint8_t dst[], uint8_t data_len, uint8_t data[]){
  this->sendMessage(
    lora_proto::bytes_to_uint32_t(dst),
    data_len, data);
}

void LoRa_adapter::sendMessage(uint32_t dst, uint8_t data_len, uint8_t data[]){
  Serial_adapter sa = *Serial_adapter::getSerialAdapter();
  sa.info("Sending message");
  lora_proto::Packet pk = lora_proto::Packet::create(this->lp_address, dst, 0, data_len, data);
  uint8_t packet_len;
  uint8_t* packet = pk.serialize(&packet_len);
  if(!LoRa.beginPacket()){
    sa.info("Error beginning LoRa packet");
    LoRa.receive();
    return;
  }  
  LoRa.write(packet, packet_len);
  sa.info("Sending...");
  LoRa.endPacket();
  sa.info("Sent");
  LoRa.receive();
}

bool LoRa_adapter::new_packet = false;
uint8_t* LoRa_adapter::packet = nullptr;
uint8_t  LoRa_adapter::packet_len = 0;


