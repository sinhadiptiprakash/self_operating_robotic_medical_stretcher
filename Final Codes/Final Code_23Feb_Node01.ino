#include <RF24Network.h>
#include <RF24.h>
#include <SPI.h>
const int x=A0;
const int y=A1;
RF24 radio(7, 8);               // nRF24L01 (CE,CSN)
RF24Network network(radio);      // Include the radio in the network
const uint16_t node01 = 01;
const uint16_t node00 = 00;

// Address of the other node in Octal format
void setup() {
  Serial.begin(115200);
  pinMode(A0,INPUT);
  pinMode(A1,INPUT);
  delay(100);
  SPI.begin();
  radio.begin();
  network.begin(90, node01); //(channel, node address)
  radio.setDataRate(RF24_2MBPS);
  radio.setChannel(115); 
  radio.setPALevel(RF24_PA_MAX);
 
}
void loop() {
  int xval=analogRead(x);
  int yval=analogRead(y);
  int buttonState=0;
  Serial.print("X=");Serial.println(xval);
  Serial.print("Y=");Serial.println(yval);
  RF24NetworkHeader header(node00);
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
    /*else if(buttonState==HIGH){
      //Serial.println(buttonState);
     //RF24NetworkHeader header2(node02);
     int data4=5;
     network.write(header2, &data4, sizeof(data4));
     Serial.println(buttonState);
     }
  
      else if(buttonState==LOW){
      int data5=6;
      network.write(header2, &data5, sizeof(data5));
     }*/
     delay(100);
}