#include <RF24Network.h>
#include <RF24.h>
#include <SPI.h>
#define button 2
const int x=A0;
const int y=A1;
RF24 radio(7, 8);               // nRF24L01 (CE,CSN)
RF24Network network(radio);      // Include the radio in the network
const uint16_t node = 01;
const uint16_t node0 = 00;
const uint16_t node0 = 02;

// Address of the other node in Octal format
void setup() {
  Serial.begin(115200);
  pinMode(A0,INPUT);
  pinMode(A1,INPUT);
  buttonState=digitalRead(button);
  delay(100);
  SPI.begin();
  radio.begin();
  network.begin(90, node); //(channel, node address)
  radio.setDataRate(RF24_2MBPS);
  radio.setChannel(115); 
  radio.setPALevel(RF24_PA_MAX);
  pinMode(button, INPUT_PULLUP);
}
void loop() {
  int xval=analogRead(x);
  int yval=analogRead(y);
  Serial.print("X=");Serial.println(xval);
  Serial.print("Y=");Serial.println(yval);
  RF24NetworkHeader header(node0);
  if(xval<320){
    int data=1;
    network.write(header, &data, sizeof(data));
    }
   else if(xval>370){
    int data1=2;
    network.write(header, &data1, sizeof(data1));
    }
   else if(yval<350){
    int data2=3;
    network.write(header, &data2, sizeof(data2));
    }
    else if(yval>370){
    int data3=4;
    network.write(header, &data3, sizeof(data3));
    }
else if(buttonState==1){
      unsigned long buttonState = digitalRead(button);
     //RF24NetworkHeader header2(node02);
     int data4=5;
     network.write(header2, &data4, sizeof(data4));
     }
  
   else if(buttonState==0){
    int data5=6;
     network.write(header2, &data5, sizeof(data5));
     }
}