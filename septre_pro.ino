/**
 @file read9axis.ino
 @brief This is an Example for the FaBo 9Axis I2C Brick.

   http://fabo.io/202.html

   Released under APACHE LICENSE, VERSION 2.0

   http://www.apache.org/licenses/

 @author FaBo<info@fabo.io>
*/

#include <Wire.h>
#include <FaBo9Axis_MPU9250.h>
#include <boarddefs.h>
#include <IRremote.h>
#include <IRremoteInt.h>
#include <ir_Lego_PF_BitStreamEncoder.h>
void LED (void)
  {       digitalWrite(8,LOW);//Blue LED
          digitalWrite(6,LOW);//Red LED
          digitalWrite(5,HIGH);//Green LED
          delay(3000);
          Serial.println("ready");
          
      }
IRsend irsend;

#define ARRAY 10
int RECV_PIN = 11;
String sceptre[ARRAY] = {"a","A","90","65","s"};
int gesture[ARRAY] = {90,56,40,5,6,20};

IRrecv irrecv(RECV_PIN);

decode_results results;


FaBo9Axis fabo_9axis;

void setup() {
  Serial.begin(9600);
  Serial.println("RESET");
  pinMode(6,OUTPUT);
  pinMode(5,OUTPUT);
  pinMode(7,OUTPUT);
  Serial.println();
  irrecv.enableIRIn(); // Start the receiver
  Serial.println("configuring device.");

  if (fabo_9axis.begin()) {
    Serial.println("configured FaBo 9Axis I2C Brick");
  } else {
    Serial.println("device error");
    while(1);
  }
}

void dump(decode_results *results) {
  // Dumps out the decode_results structure.
  // Call this after IRrecv::decode()
  int count = results->rawlen;
  if (results->decode_type == UNKNOWN) {
    Serial.print("Unknown encoding: ");
  }
  else if (results->decode_type == NEC) {
    Serial.print("Decoded NEC: ");

  }
  else if (results->decode_type == SONY) {
    Serial.print("Decoded SONY: ");
  }
  else if (results->decode_type == RC5) {
    Serial.print("Decoded RC5: ");
  }
  else if (results->decode_type == RC6) {
    Serial.print("Decoded RC6: ");
  }
  else if (results->decode_type == PANASONIC) {
    Serial.print("Decoded PANASONIC - Address: ");
    Serial.print(results->address, HEX);
    Serial.print(" Value: ");
  }
  else if (results->decode_type == LG) {
    Serial.print("Decoded LG: ");
  }
  else if (results->decode_type == JVC) {
    Serial.print("Decoded JVC: ");
  }
  else if (results->decode_type == AIWA_RC_T501) {
    Serial.print("Decoded AIWA RC T501: ");
  }
  else if (results->decode_type == WHYNTER) {
    Serial.print("Decoded Whynter: ");
  }
  Serial.print(results->value, HEX);
  Serial.print(" (");
  Serial.print(results->bits, DEC);
  Serial.println(" bits)");
  Serial.print("Raw (");
  Serial.print(count, DEC);
  Serial.print("): ");

  for (int i = 1; i < count; i++) {
    if (i & 1) {
      Serial.print(results->rawbuf[i]*USECPERTICK, DEC);
    }
    else {
      Serial.write('-');
      Serial.print((unsigned long) results->rawbuf[i]*USECPERTICK, DEC);
    }
    Serial.print(" ");
  }
  Serial.println();
}


#define X_NEG_TO_POS_NEG 
void loop() {
  float ax,ay,az;
  float gx,gy,gz;
  float mx,my,mz;
  float temp;

  fabo_9axis.readAccelXYZ(&ax,&ay,&az);
  fabo_9axis.readGyroXYZ(&gx,&gy,&gz);
  fabo_9axis.readMagnetXYZ(&mx,&my,&mz);
  fabo_9axis.readTemperature(&temp);
  
  unsigned long now = millis();
  
  if(ax<-0.75){
    Serial.println("Move now in x-"); 
    digitalWrite(6,HIGH);//Red LED
    while(millis() - now < 2000){
      //Serial.println(ax);
      fabo_9axis.readAccelXYZ(&ax,&ay,&az);
      if(ax>0.25)
      {for(int i=0;i<3;i++)
      {Serial.println("X -");//fan speed low//TV PREVISOUS CHANNEL
      irsend.sendSony(0xc90, 12);
      delay(200); 
      }
        LED();
        
        break;     
        }
    }
  }

   if(ax>0.75){
    Serial.println("Move now in x+");
    digitalWrite(6,HIGH);//Red LED
    while(millis() - now < 2000){
     // Serial.println(ax);
      fabo_9axis.readAccelXYZ(&ax,&ay,&az); 
      if(ax<-0.5)
      {
       
      for(int i=0;i<3;i++)
      {  Serial.println("X +");
        irsend.sendSony(0x490, 12);
        delay(200);//fan speed high;//TV NEXT CHANNEL
        } 
        LED();
        
        break;
      }
      else if(az<-1.19)
      {
       
      for(int i=0;i<3;i++)
      {  Serial.println("Hand Flip");
        irsend.sendSony(0xa90, 12);//FAN RELAY ON
        delay(200);
        } 
        LED();
        
        break; 
      }
      }
    }


if(ay>0.6){
    Serial.println("Move now in y-90"); 
    digitalWrite(6,HIGH);//Red LED
    while(millis() - now < 2000){
      //Serial.println(ax);
      fabo_9axis.readAccelXYZ(&ax,&ay,&az);
      if(ay>0.9)
      {  while(millis() - now < 2000){
      //Serial.println(ax);
      fabo_9axis.readAccelXYZ(&ax,&ay,&az);
       if(ax>0.3)
      {for(int i=0;i<3;i++)
      {Serial.println("unique gesture 2");
      irsend.sendSony(0xc90, 12);
      delay(200); 
      }
        LED();
        
        break; 
      }
      else if (ay<0.2)
      {
        for(int i=0;i<3;i++)
      {Serial.println("y -90 degree");
      irsend.sendSony(0X555, 12);
      delay(200); 
      }
      
        LED();
        
        break;
        } 
      }
      }
      else if(ay<0.3)
      {for(int i=0;i<3;i++)
      {Serial.println("y -60 degree");
      irsend.sendSony(0x333, 12);//FAN RELAY OFF
      delay(200); 
      }
      
        LED();
        
        break; 
      }
      
    }
  }

if(ay<-0.15)
{
    Serial.println("Move now in y+90"); 
    digitalWrite(6,HIGH);//Red LED
    while(millis() - now < 2000){
      //Serial.println(ax);
      fabo_9axis.readAccelXYZ(&ax,&ay,&az);
      if(ay<-0.9)
      {for(int i=0;i<3;i++)
      {Serial.println("y +90 degree");
      irsend.sendSony(0x777, 12); // LED RELAY ON
      delay(200); 
      }
       LED();
        
       break; 
      }
      else if(ax>0.3)
      {for(int i=0;i<3;i++)
      {Serial.println("unique gesture 1");
      irsend.sendSony(0xc90, 12);
      delay(200); 
      }
        LED();
        
        break; 
      }
    else if(ay>0.3)
      {for(int i=0;i<3;i++)
      {Serial.println("y +60 degree");//LED RELAY OFF
      irsend.sendSony(0x888, 12);
      delay(200); 
      }
        LED();
        
        break; 
      }
    }
  }

}
