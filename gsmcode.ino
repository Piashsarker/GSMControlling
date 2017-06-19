#include <SoftwareSerial.h>
//#include<Arduino.h>

#define resetPin 2
#define ledPin 7

void readSMS(void);
void reset(void);
void sendCmd(char str[] );
SoftwareSerial gsm(10, 11); // RX, TX

char msg;
char text[200];
char text1[50];
int w=0;
void setup() {
   Serial.begin(9600); 
   while(!Serial){
     if(w==2000) {
     Serial.end();
     break;
     }
     w++;
     delay(1);
    }
  
  gsm.begin(9600);
  digitalWrite(resetPin,LOW);
  pinMode(ledPin,OUTPUT);
  pinMode(resetPin,OUTPUT);
  digitalWrite(ledPin,LOW);
  digitalWrite(resetPin,LOW);
  delay(100);
  
  
  Serial.println("Initializing...");
  reset();
  delay(1000);
  sendCmd("AT+CMGF=1\r\n");
  delay(1000);
  sendCmd("AT+CSCS=\"GSM\"\r\n");
  delay(1000);
  sendCmd("AT+CMGDA=\"DEL ALL\"\r\n");
  delay(1000);
  Serial.println();
  Serial.println("Ready to work...");
  Serial.println("Press '>' to enter command mode:");
  Serial.println("And press '<' to exit");

}


void loop() { 

  byte x=0;
  char c=0;
  char cr=0;
  char ca=0;
  unsigned int l=0;
  unsigned char d=0;
  if(gsm.available()){
    main:
    x=0;
    c=0;
    cr=0;
    ca=0;
    l=0;
    d=0;
    msg=0;
    text[200]={};
    delay(1000);
    Serial.println();
    while(1){
       if(gsm.available()){
          x=1;
          msg=gsm.read();
          Serial.print(msg);
           if(c == 1) {
            if(msg=='1'){ 
              d=msg;
              goto out;
              }
            
           }          
           if(msg==',') c=1;
          
       }
       out:
       if(d=='1'){
        c=0;
        text[200]={};
        readSMS();
        Serial.println(text);
        if((text[1]+text[2])==157){
            digitalWrite(ledPin,HIGH);
            sendSMS("LED ON");  
            delay(1000);
            Serial.println();   
            sendCmd("AT+CMGDA=\"DEL ALL\"\r\n"); 
            delay(1000); 
            goto main;     
          }else if((text[1]+text[2]+text[3])==219){
            digitalWrite(ledPin,LOW);
            sendSMS("LED OFF");
            delay(1000);
            Serial.println();
            sendCmd("AT+CMGDA=\"DEL ALL\"\r\n");
            delay(1000);
            goto main;
           }else {
            delay(1000);
            Serial.println();
            sendCmd("AT+CMGDA=\"DEL ALL\"\r\n");
            delay(1000);
            goto main;
            }
        goto main;
       }else if (d!=0 && d!=1){
          delay(1000);
          Serial.println();
          sendCmd("AT+CMGDA=\"DEL ALL\"\r\n");
          delay(1000);
          goto main;
        }

       if(Serial.available()){
         ca = Serial.read();
        if(ca == '>') goto cmd;
       }

        if(l>30000) {
            c=0;
            cr=0;
            ca=0;
            l=0;
            d=0;
            msg=0;
            text[200]={};
            Serial.println();
            sendCmd("AT+CMGDA=\"DEL ALL\"\r\n");
            Serial.println();
            delay(2000);
            goto main;
          }else delay(10);
          
       l++;
      }
    }
    if(Serial.available()){
        cr = Serial.read();
        if(cr == '>'){
          cmd:
          Serial.println("Command Mode is Started...");
          while(1){
            if (gsm.available()) {
                Serial.write(gsm.read());
             }
            if (Serial.available()) {
              cr = Serial.read();
              gsm.write(cr);
            }
            if(cr=='<'){ 
              Serial.println(cr);
              delay(100);
              goto main;
              }
              
            }
          }
      }

}

void reset(void){
  char a=0;
  char c=0;
  digitalWrite(resetPin,HIGH);
  delay(100);
  digitalWrite(resetPin,LOW);
  delay(1000);
  sendCmd("AT\r\n");

 }


void sendCmd(char str[] ){
   char b=0;
   char c=0;
   int i=0;
   gsm.write(str);
  while(i<50000){
    
    if(gsm.available()){
       
      b = gsm.read();
       if(b=='K') c=b;
    } 
    if(c=='K') break; 
    i++; 
  }
}


void readSMS(void){
    char b=0;
   char c=0;
   byte i=0;
   text[200]={};
    gsm.write("AT+CMGR=1\r\n");
  while(1){
   
    if(gsm.available()){

      b = gsm.read();
      if(b==':')c=1;
      if(b=='\n' && c==1)c=2;
      if(c==2){
        if(b=='\n' && i!=0) break;
        text[i] = b;
        i++;
      }
     }
    }
    delay(500);
    sendCmd("AT+CMGDA=\"DEL ALL\"\r\n");
    delay(500);
}


void sendSMS(char str[]){
  char a=0;
    gsm.write("AT+CMGS=\"+8801738586528\"\r");//+8801738586528
    delay(500);
    gsm.write(str);
    gsm.write("\r");
    delay(100);
    gsm.write((char)26);
    while(1){
      if(gsm.available()){
        a=gsm.read();       
        }else break;
        
      }
  }


  
