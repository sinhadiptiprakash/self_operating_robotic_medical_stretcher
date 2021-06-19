#include <RF24Network.h>
#include <RF24.h>
#include <SPI.h>
const int red=6;
const int green=5;
RF24 radio(7, 8);               // nRF24L01 (CE,CSN)
RF24Network network(radio);      // Include the radio in the network
const uint16_t this_node = 02; 
const uint16_t node0 = 01;
// Address of our node in Octal format ( 04,031, etc)
void setup() {
  SPI.begin();
  radio.begin();
  network.begin(90, this_node);
  radio.setChannel(115); 
  radio.setPALevel(RF24_PA_MAX);
  radio.setDataRate(RF24_2MBPS);
  pinMode(red, OUTPUT);
  pinMode(green, OUTPUT);
  Serial.begin(115200);
}
void loop() {
  network.update();
 if(network.available()){ //===== Receiving =====//
  while ( network.available() ) {     // Is there any incoming data?
    RF24NetworkHeader header(node0);
    int data; 
    network.read(header, &data, sizeof(data));
    Serial.println(data);
    if(data==5){
    digitalWrite(green,LOW);
    digitalWrite(red,HIGH);
    }
     
  }
 }
  else{
    // Turn on or off the LED
     digitalWrite(red,LOW);
    digitalWrite(green,HIGH);
      }
  }
  
  