#include "FirebaseESP8266.h"
#include<ESP8266WiFi.h>

FirebaseData firebaseData;

//****FIREBASE SHOT**********
#define FIREBASE_AUTH "FIREBASE AUTH HERE"
#define FIREBASE_HOST "FIREBASE HOST ADDRESS HERE"
#define WIFI_SSID "WIFI SSID HERE"
#define WIFI_PASSWORD "WIFI PASSWORD HERE"

const int enbA = D1;
const int enbB = D2;

const int IN1 = D8;    //Right Motor (-)
const int IN2 = D7;    //Right Motor (+)
const int IN3 = D6;    //Left Motor (+)
const int IN4 = D5;    //Right Motor (-)

void setup() {
  
  Serial.begin(115200);
    WiFi.mode(WIFI_STA);
    WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
    Serial.print("connecting....");
    while(WiFi.status() != WL_CONNECTED)
      {
        Serial.print(".");
        delay(500);
        yield();
      }
      Serial.println();
      Serial.print("Connected! : ");
      Serial.print(WiFi.localIP());
  
    Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);
     
    pinMode(enbA, OUTPUT);
    pinMode(enbB, OUTPUT);
    pinMode(IN1, OUTPUT);
    pinMode(IN2, OUTPUT);
    pinMode(IN3, OUTPUT);
    pinMode(IN4, OUTPUT);
    

       yield();
}
int acx;
int acy;
int pwm;
String cs;

void loop() {

  if (Firebase.getInt(firebaseData, "/x3")) {
    if  (firebaseData.dataType() == "int") {
      acx = firebaseData.intData();
      }
    }

   if (Firebase.getInt(firebaseData, "/y3")) {
    if  (firebaseData.dataType() == "int") {
      acy = firebaseData.intData();
      }
    }
  
  if(acy > 110){ 
    forward();
      delay(1);
      yield();
  } 
  else if (acy < 70){ 
    back();
      delay(0);
      yield();
  }    

  else if (acx > 70 && acx < 110 && acy > 70 && acy < 110){
      stopcar();
      delay(0);
      yield();
  }
  
  else if(acx > 110){
      left();
      delay(0);
      yield();
  }
  else if(acx < 70){
      right();
      delay(0);
      yield();
  }
  
  delay(0);
  yield();
  ESP.wdtFeed();
}

//****LOOP ENDS*******//

// ********DEFINITIONS*************
void forward(){
      pwm = map(acy, 110, 180, 300, 700);           
      analogWrite(enbA, pwm);
      analogWrite(enbB, pwm);
      digitalWrite(IN1, HIGH);
      digitalWrite(IN2, LOW);
      digitalWrite(IN3, HIGH);
      digitalWrite(IN4, LOW);
      delay(0);
      yield();
}

void back(){
      pwm = map(acy, 70, 0, 300, 700);              
      analogWrite(enbA, pwm);
      analogWrite(enbB, pwm);
      digitalWrite(IN1, LOW);
      digitalWrite(IN2, HIGH );
      digitalWrite(IN3, LOW);
      digitalWrite(IN4, HIGH);
      delay(0);
      yield();
}

void left(){
      pwm = map(acx, 110, 180, 300, 700);
      analogWrite(enbA, pwm);
      analogWrite(enbB, pwm);
      digitalWrite(IN1, HIGH);
      digitalWrite(IN2, LOW);
      digitalWrite(IN3, LOW);
      digitalWrite(IN4, HIGH);
      delay(0);
      yield();
}

void right(){
      pwm = map(acx, 70, 0, 300,700);
      analogWrite(enbA, pwm);
      analogWrite(enbB, pwm);
      digitalWrite(IN1, LOW);
      digitalWrite(IN2, HIGH);
      digitalWrite(IN3, HIGH);
      digitalWrite(IN4, LOW);
      delay(0);
      yield();
}

void stopcar(){
      analogWrite(enbA, 0);
      analogWrite(enbB, 0);
      digitalWrite(IN1, LOW);
      digitalWrite(IN2, LOW);
      digitalWrite(IN3, LOW);
      digitalWrite(IN4, LOW);
      delay(0);
      yield();
}
