#include <RF24Network.h>
#include <RF24.h>
#include <SPI.h>
#include <SoftwareSerial.h>
#include<Servo.h>
//Radio Communication Definitions
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
const int pingPin = 4;
const int echo=6;
SoftwareSerial mySerial(3,2);


Servo sup;
Servo sdown;

void forward(){
     digitalWrite(lmotp, LOW); 
     digitalWrite(lmotn, HIGH); 
     digitalWrite(rmotp, LOW); 
     digitalWrite(rmotn, HIGH);
     digitalWrite(bmotp,LOW);
     digitalWrite(bmotn,HIGH);    
}

void Stop(){
     digitalWrite(lmotp, LOW); 
     digitalWrite(lmotn, LOW); 
     digitalWrite(rmotp, LOW); 
     digitalWrite(rmotn, LOW);
     digitalWrite(bmotp,LOW);
     digitalWrite(bmotn,LOW);      
}
void backward(){
     digitalWrite(lmotp, HIGH); 
     digitalWrite(lmotn, LOW); 
     digitalWrite(rmotp, HIGH); 
     digitalWrite(rmotn, LOW);
     digitalWrite(bmotp,HIGH);
     digitalWrite(bmotn,LOW); 
}
void up(){
     digitalWrite(lmotp, LOW); 
     digitalWrite(lmotn, LOW); 
     digitalWrite(rmotp, LOW); 
     digitalWrite(rmotn, LOW);
     digitalWrite(bmotp,LOW);
     digitalWrite(bmotn,LOW);
     sup.write(95);//30//27//initial position
     sdown.write(27);//100
     Serial.println();
 }

  void down(){
     digitalWrite(lmotp, LOW); 
     digitalWrite(lmotn, LOW); 
     digitalWrite(rmotp, LOW); 
     digitalWrite(rmotn, LOW);
     digitalWrite(bmotp,LOW);
     digitalWrite(bmotn,LOW);
     sup.write(27);//95/
     sdown.write(95);      
  }

  void left(){
  digitalWrite(lmotp,LOW);
  digitalWrite(lmotn,HIGH);
  digitalWrite(rmotp,LOW);
  digitalWrite(rmotn,LOW);
  digitalWrite(bmotp,LOW);
  digitalWrite(bmotn,LOW);
  }

  void right(){
  digitalWrite(lmotp,LOW);
  digitalWrite(lmotn,LOW);
  digitalWrite(rmotp,LOW);
  digitalWrite(rmotn,HIGH);
  digitalWrite(bmotp,LOW);
  digitalWrite(bmotn,LOW);
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
  pinMode(pingPin, OUTPUT);
  pinMode(echo, OUTPUT);
  sup.attach(9);
  sdown.attach(10);
  mySerial.begin(9600);
  pinMode(pingPin, OUTPUT);
  // initialize serial communication:
  Serial.begin(115200);
}
  void networkFun(){
  network.update();
  if (network.available()){
  while ( network.available() ) {     // Is there any incoming data?
    RF24NetworkHeader header(node01);
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
  
  /*RF24NetworkHeader header2(node02);  // (Address where the data is going)
    int switch1=1;
    int switch0=0;
    if(digitalRead(button)==1){
    network.write(header2, &switch1, sizeof(switch1));
    }
    else {
    network.write(header2,&switch0,sizeof(switch0));
      }
  // Send the data
  }*/
}
  }

void voiceControl(){
  String voice;
  if(mySerial.available()){
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
  else if(voice == "up"||voice=="ap"||voice=="apps"||voice=="aap") {
    up();
    }
    else if(voice == "stop") {
    Stop();
    }
    
//-----------------------------------------------------------------------//  
voice="";}}
/*else if(!mySerial.available()){
  delay(5000);
  Stop();
  }*/
} 

long microsecondsToInches(long microseconds) {
  return microseconds / 74 / 2;
}

long microsecondsToCentimeters(long microseconds) {
  return microseconds / 29 / 2;
}
 void loop(){
  long duration, inches, cm;

  // The PING))) is triggered by a HIGH pulse of 2 or more microseconds.
  // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
  digitalWrite(pingPin, LOW);
  delayMicroseconds(2);
  digitalWrite(pingPin, HIGH);
  delayMicroseconds(5);
  digitalWrite(pingPin, LOW);

  // The same pin is used to read the signal from the PING))): a HIGH pulse
  // whose duration is the time (in microseconds) from the sending of the ping
  // to the reception of its echo off of an object.
  pinMode(echo, INPUT);
  duration = pulseIn(echo, HIGH);

  // convert the time into a distance
  inches = microsecondsToInches(duration);
  cm = microsecondsToCentimeters(duration);

  Serial.print(inches);
  Serial.print("in, ");
  Serial.print(cm);
  Serial.print("cm");
  Serial.println();
if(cm<=30||inches<=5){
  Stop();
  }
  delay(200);
  
  voiceControl();
  networkFun();
  
  }