#include <Wire.h>
#include<FirebaseArduino.h>
#include<ESP8266WiFi.h>

// MPU6050 Slave Device Address
const uint8_t MPU6050SlaveAddress = 0x68;

//****FIREBASE SHOT**********
#define FIREBASE_AUTH "FIREBASE AUTHORISATION CODE HERE"
#define FIREBASE_HOST "FIREBASE HOST ADDRESS HERE"
#define WIFI_SSID "WIFI SSID HERE"
#define WIFI_PASSWORD "WIFI PASSWORD HERE"


// Select SDA and SCL pins for I2C communication 
const uint8_t scl = D1;
const uint8_t sda = D2;
int carsignal=D6;
int runsignal=D7;
int carmode=0;
int caronoff=0;
// sensitivity scale factor respective to full scale setting provided in datasheet 
const uint16_t AccelScaleFactor = 16384;
const uint16_t GyroScaleFactor = 131;

// MPU6050 few configuration register addresses
const uint8_t MPU6050_REGISTER_SMPLRT_DIV   =  0x19;
const uint8_t MPU6050_REGISTER_USER_CTRL    =  0x6A;
const uint8_t MPU6050_REGISTER_PWR_MGMT_1   =  0x6B;
const uint8_t MPU6050_REGISTER_PWR_MGMT_2   =  0x6C;
const uint8_t MPU6050_REGISTER_CONFIG       =  0x1A;
const uint8_t MPU6050_REGISTER_GYRO_CONFIG  =  0x1B;
const uint8_t MPU6050_REGISTER_ACCEL_CONFIG =  0x1C;
const uint8_t MPU6050_REGISTER_FIFO_EN      =  0x23;
const uint8_t MPU6050_REGISTER_INT_ENABLE   =  0x38;
const uint8_t MPU6050_REGISTER_ACCEL_XOUT_H =  0x3B;
const uint8_t MPU6050_REGISTER_SIGNAL_PATH_RESET  = 0x68;

int16_t AccelX, AccelY, AccelZ, Temperature, GyroX, GyroY, GyroZ;

void setup() {

  pinMode(carsignal, INPUT);
  pinMode(runsignal, INPUT);
  Serial.begin(9600);
  Wire.begin(sda, scl);
  
  WiFi.begin(WIFI_SSID, WIFI_PASSWORD);
  Serial.print("connecting....");
  while(WiFi.status() != WL_CONNECTED)
    {
      Serial.print(".");
      delay(500);
    }
    Serial.println();
    Serial.print("Connected! : ");
    Serial.print(WiFi.localIP());

    Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH);

  MPU6050_Init();  
}

void loop() {

  carmode=digitalRead(carsignal);
  caronoff=digitalRead(runsignal);
  
  double Ax, Ay;
  int accelx;
  int accely;
  
  Read_RawValue(MPU6050SlaveAddress, MPU6050_REGISTER_ACCEL_XOUT_H);  
  //divide each with their sensitivity scale factor
  Ax = (double)AccelX/AccelScaleFactor*100;
  Ay = (double)AccelY/AccelScaleFactor*100;

  accelx=(int)Ax;
  accely=(int)Ay;

  accelx=map(accelx, -100, 100, 0, 180);
  constrain(accelx,0,180);

  accely=map(accely, -100, 100, 0, 180);
  constrain(accely,0,180);

  //check_onoff();
  
  if(carmode==HIGH){
   Firebase.setInt("x3", accelx);
   Firebase.setInt("y3", accely);
  }
   
  delay(1);  
}

void check_onoff(){
  if(caronoff==HIGH)
    Serial.println("ON");
  else
    Serial.println("OFF");
}

void I2C_Write(uint8_t deviceAddress, uint8_t regAddress, uint8_t data){
  Wire.beginTransmission(deviceAddress);
  Wire.write(regAddress);
  Wire.write(data);
  Wire.endTransmission();
}

// read all 14 register
void Read_RawValue(uint8_t deviceAddress, uint8_t regAddress){
  Wire.beginTransmission(deviceAddress);
  Wire.write(regAddress);
  Wire.endTransmission();
  Wire.requestFrom(deviceAddress, (uint8_t)14);
  AccelX = (((int16_t)Wire.read()<<8) | Wire.read());
  AccelY = (((int16_t)Wire.read()<<8) | Wire.read());
  AccelZ = (((int16_t)Wire.read()<<8) | Wire.read());
  Temperature = (((int16_t)Wire.read()<<8) | Wire.read());
  GyroX = (((int16_t)Wire.read()<<8) | Wire.read());
  GyroY = (((int16_t)Wire.read()<<8) | Wire.read());
  GyroZ = (((int16_t)Wire.read()<<8) | Wire.read());
}

//configure MPU6050
void MPU6050_Init(){
  delay(150);
  I2C_Write(MPU6050SlaveAddress, MPU6050_REGISTER_SMPLRT_DIV, 0x07);
  I2C_Write(MPU6050SlaveAddress, MPU6050_REGISTER_PWR_MGMT_1, 0x01);
  I2C_Write(MPU6050SlaveAddress, MPU6050_REGISTER_PWR_MGMT_2, 0x00);
  I2C_Write(MPU6050SlaveAddress, MPU6050_REGISTER_CONFIG, 0x00);
  I2C_Write(MPU6050SlaveAddress, MPU6050_REGISTER_GYRO_CONFIG, 0x00);//set +/-250 degree/second full scale
  I2C_Write(MPU6050SlaveAddress, MPU6050_REGISTER_ACCEL_CONFIG, 0x00);// set +/- 2g full scale
  I2C_Write(MPU6050SlaveAddress, MPU6050_REGISTER_FIFO_EN, 0x00);
  I2C_Write(MPU6050SlaveAddress, MPU6050_REGISTER_INT_ENABLE, 0x01);
  I2C_Write(MPU6050SlaveAddress, MPU6050_REGISTER_SIGNAL_PATH_RESET, 0x00);
  I2C_Write(MPU6050SlaveAddress, MPU6050_REGISTER_USER_CTRL, 0x00);
}
