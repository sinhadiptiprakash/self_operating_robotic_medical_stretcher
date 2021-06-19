#include <RF24.h>
#include <RF24Network.h>
#include <SPI.h>
#include<Servo.h>
#include <SoftwareSerial.h>
//Radio Definitions
 RF24 radio(7, 8);               // nRF24L01 (CE,CSN)
RF24Network network(radio);      // Include the radio in the network
const uint16_t node00 = 00;   // Address of this node in Octal format ( 04,031, etc)
const uint16_t node01 = 01;      // Address of the other node in Octal format
const uint16_t node02 = 02;

const int button=5;
const int lmotp = A0;
const int lmotn =A1;
const int rmotp = A2;
const int rmotn = A3;
const int bmotp = A4;
const int bmotn = A5;
String voice;
Servo sup;
Servo sdown;
SoftwareSerial mySerial(3,2);
void forward(){
     digitalWrite(lmotp, LOW); 
     digitalWrite(lmotn, HIGH); 
     digitalWrite(rmotp, LOW); 
     digitalWrite(rmotn, HIGH);
     digitalWrite(bmotp,LOW);
     digitalWrite(bmotn,HIGH);
     Serial.println("forward");    
}

void Stop(){
     digitalWrite(lmotp, LOW); 
     digitalWrite(lmotn, LOW); 
     digitalWrite(rmotp, LOW); 
     digitalWrite(rmotn, LOW);
     digitalWrite(bmotp,LOW);
     digitalWrite(bmotn,LOW);
     Serial.println("stop");      
}
void backward(){
     digitalWrite(lmotp, HIGH); 
     digitalWrite(lmotn, LOW); 
     digitalWrite(rmotp, HIGH); 
     digitalWrite(rmotn, LOW);
     digitalWrite(bmotp,HIGH);
     digitalWrite(bmotn,LOW);
     Serial.println("backward"); 
}
void down(){
     digitalWrite(lmotp, LOW); 
     digitalWrite(lmotn, LOW); 
     digitalWrite(rmotp, LOW); 
     digitalWrite(rmotn, LOW);
     digitalWrite(bmotp,LOW);
     digitalWrite(bmotn,LOW);
     sup.write(30);//27//initial position
     sdown.write(100);
     Serial.println();
     Serial.println("up");
 }

  void up(){
     digitalWrite(lmotp, LOW); 
     digitalWrite(lmotn, LOW); 
     digitalWrite(rmotp, LOW); 
     digitalWrite(rmotn, LOW);
     digitalWrite(bmotp,LOW);
     digitalWrite(bmotn,LOW);
     sup.write(95);//95/
     sdown.write(27);
     Serial.println("down");      
  }

  void left(){
  digitalWrite(lmotp,HIGH);
  digitalWrite(lmotn,LOW);
  digitalWrite(rmotp,LOW);
  digitalWrite(rmotn,LOW);
  digitalWrite(bmotp,LOW);
  digitalWrite(bmotn,LOW);
  Serial.println("left");
  }

  void right(){
  digitalWrite(lmotp,LOW);
  digitalWrite(lmotn,LOW);
  digitalWrite(rmotp,HIGH);
  digitalWrite(rmotn,LOW);
  digitalWrite(bmotp,LOW);
  digitalWrite(bmotn,LOW);
  Serial.println("right");
  }

void setup() {
  SPI.begin();
  radio.begin();
  network.begin(90,node00);  //(channel, node address)
  radio.setDataRate(RF24_2MBPS);
  radio.setChannel(115); 
  radio.setPALevel(RF24_PA_MAX);
  pinMode(button, INPUT_PULLUP);
  pinMode(lmotp, OUTPUT); 
  pinMode(lmotn, OUTPUT); 
  pinMode(rmotp, OUTPUT); 
  pinMode(rmotn, OUTPUT);
  pinMode(bmotp, OUTPUT); 
  pinMode(bmotn, OUTPUT);
  sup.attach(9);
  sdown.attach(10);
  mySerial.begin(9600);
  }
  void loop(){
    voiceControl();
    networkFun();
    }
    
  void networkFun(){
    network.update();
    if (network.available()){
    while ( network.available() ) {     // Is there any incoming data?
    RF24NetworkHeader header;
    int data;
    network.read(header, &data, sizeof(data));
    Serial.println(data);
    if(data==1){
      left();
      Serial.println("left");
      }
    else if(data==2){
      right();
      Serial.println("right");
      }
      else if(data==3){
      up();
      Serial.println("up");
      }
      else if(data==4){
      down();
      Serial.println("down");
      }
    // Read the incoming data
  }
  // LED Control at Node 02
  int buttonState = digitalRead(button);
  RF24NetworkHeader header2(node02);  // (Address where the data is going)
    int switch1=1;
    int switch0=0;
    if(buttonState==1){
    network.write(header2, &switch1, sizeof(switch1));
    }
    else {
    network.write(header2,&switch0,sizeof(switch0));
      }
  // Send the data
  }
}

  void voiceControl(){
    while (mySerial.available()){
  delay(10);
  char c = mySerial.read();
  if (c == '#') {break;} //Exit the loop when the # is detected after the word
  voice += c; //
  }  
  if (voice.length() > 0) {
    Serial.println(voice); 
       if(voice == "forward") {
        forward();
        }
  else if(voice == "backward"){
    backward();
    } 
  else if(voice == "left"){
   left();
  } 
  else if(voice == "right") {
    right();
  }
  else if(voice == "down") {
    down();
    } 
  else if(voice == "up"||voice=="ap") {
    up();
    }
    else if(voice == "stop") {
    Stop();
    }
    
//-----------------------------------------------------------------------//  
voice="";}}
