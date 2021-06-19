#include <SPI.h>  
#include "RF24.h" 
#include<Servo.h>

Servo sup;
Servo sdown;
RF24 myRadio (7, 8); 
struct package
{
  int text;
};

byte addresses[][6] = {"0"}; 

typedef struct package Package;
Package data;

const int lmotp = A0;
const int lmotn =A1;
const int rmotp = A2;
const int rmotn = A3;
const int bmotp = A4;
const int bmotn = A5;

//RemoteXY DEfinitions
#define REMOTEXY_MODE__SOFTSERIAL

#include <SoftwareSerial.h>

#include <RemoteXY.h>

#define REMOTEXY_SERIAL_RX 3
#define REMOTEXY_SERIAL_TX 2
#define REMOTEXY_SERIAL_SPEED 9600


// RemoteXY configurate  
#pragma pack(push, 1)
uint8_t RemoteXY_CONF[] =
  { 255,4,0,0,0,48,0,8,13,0,
  5,40,40,20,30,30,177,172,37,2,
  1,7,4,22,11,2,26,31,31,85,
  80,0,68,111,119,110,0,2,1,74,
  3,22,11,2,26,31,31,68,111,119,
  110,0,85,112,0 };
  
// this structure defines all the variables of your control interface 
struct {

    // input variable
  int8_t joystick_1_x; // =-100..100 x-coordinate joystick position 
  int8_t joystick_1_y; // =-100..100 y-coordinate joystick position 
  uint8_t switch_1; // =1 if switch ON and =0 if OFF 
  uint8_t sdown; // =1 if switch ON and =0 if OFF 

    // other variable
  uint8_t connect_flag;  // =1 if wire connected, else =0 

} RemoteXY;
#pragma pack(pop) 
  
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
     sup.write(90);
     sdown.write(0);
  }

  void down(){
     digitalWrite(lmotp, LOW); 
     digitalWrite(lmotn, LOW); 
     digitalWrite(rmotp, LOW); 
     digitalWrite(rmotn, LOW);
     digitalWrite(bmotp,LOW);
     digitalWrite(bmotn,LOW);
     sup.write(0);
     sdown.write(90);
  }

  void left(){
  digitalWrite(lmotp,HIGH);
  digitalWrite(lmotn,LOW);
  digitalWrite(rmotp,LOW);
  digitalWrite(rmotn,LOW);
  digitalWrite(bmotp,LOW);
  digitalWrite(bmotn,LOW);
  }

  void right(){
  digitalWrite(lmotp,LOW);
  digitalWrite(lmotn,LOW);
  digitalWrite(rmotp,HIGH);
  digitalWrite(rmotn,LOW);
  digitalWrite(bmotp,LOW);
  digitalWrite(bmotn,LOW);
  }


void setup() 
{
  Serial.begin(115200);
  delay(1000);
  pinMode(lmotp, OUTPUT); 
  pinMode(lmotn, OUTPUT); 
  pinMode(rmotp, OUTPUT); 
  pinMode(rmotn, OUTPUT);
  pinMode(bmotp, OUTPUT); 
  pinMode(bmotn, OUTPUT);
  sup.attach(9);
  sdown.attach(6);
  myRadio.begin(); 
  myRadio.setChannel(115); 
  myRadio.setPALevel(RF24_PA_MAX);
  myRadio.setDataRate( RF24_250KBPS ) ; 
  myRadio.openReadingPipe(1, addresses[0]);
  myRadio.startListening();
  RemoteXY_Init ();
}


void remoteXYLoop(){
RemoteXY_Handler ();
  
  //digitalWrite(PIN_SWITCH_1, (RemoteXY.switch_1==0)?LOW:HIGH);
  //digitalWrite(PIN_SDOWN, (RemoteXY.sdown==0)?LOW:HIGH);
  
  // TODO you loop code
  // use the RemoteXY structure for data transfer

  if(RemoteXY.joystick_1_x<-20){
    left();
    }
   else if(RemoteXY.joystick_1_x>20){
    right();
    }
   else if(RemoteXY.joystick_1_y<-20){
    backward();
    }
    else if(RemoteXY.joystick_1_y>20){
    forward();
    }
    else if(RemoteXY.joystick_1_x==0||RemoteXY.joystick_1_y==0){
    Stop();
    }
}
  
  void RTLoop(){
    if ( myRadio.available()) {
    while (myRadio.available()){
      myRadio.read( &data, sizeof(data) );
    }
   /* Serial.println(data.text);*/
    /*Serial.println(data.text1);
    Serial.println(data.text2);
    Serial.println(data.text3);
    Serial.println();*/
    Serial.println(data.text);
    if(data.text==1){
      left();
      Serial.println("left");
      }
    else if(data.text==2){
      right();
      Serial.println("right");
      }
      else if(data.text==3){
      up();
      Serial.println("up");
      }
      else if(data.text==4){
      down();
      Serial.println("down");
      }
    delay (500);
  }
    }

void loop()  {  
   remoteXYLoop(); 
   RTLoop(); 
}

