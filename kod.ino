#include <TinyGPS++.h>
#include <Wire.h>
#include <SoftwareSerial.h>
#include <ESP8266WiFi.h> 
#include "ThingSpeak.h"
#include <FirebaseArduino.h>

#define IFTTT_HOST "maker.ifttt.com"
#define EVENT_MAIL "kaza_ani"
#define IFTTTKEY "vtZvUgUVRK1HLQt88-tqq"

#define FIREBASE_HOST "arabadurum-default-rtdb.firebaseio.com" 
#define FIREBASE_AUTH "n3N13cVS7p2zua3Xd6H1n0NC8UE129mxDbezWgfI" 

TinyGPSPlus gps;
SoftwareSerial ss(12, 13);
WiFiClient client;

const int atesSensoru = D0;
const int sicaklikSensoru = A0; 

unsigned long channelNumber = 1257763;
const char * writeAPIKey = "D84925DLG27GJ4YI";

const char * ssid = "Furkan(2.4GHz)";
const char * password = "12091999furkan";

float latitude, longitude;
int year, month, date, hour, minute, second;
String date_str, time_str, lat_str, lng_str;
int pm;

float sicaklik, sicaklikC;

const int MPU_addr = 0x68; // I2C MPU-6050 adresi
int16_t AcX, AcY, AcZ, Tmp, GyX, GyY, GyZ;
float ax = 0, ay = 0, az = 0, gx = 0, gy = 0, gz = 0;

boolean fall = false; 
boolean trigger1 = false; 
boolean trigger2 = false;
boolean trigger3 = false;

byte trigger1count = 0;
byte trigger2count = 0; 
byte trigger3count = 0;

const long eventTime_0_GPS = 5000; //in ms
const long eventTime_1_THINKSPEAK = 10000; //in ms
const long eventTime_2_FIREBASE = 20000; //in ms

/* When did they start the race? */
unsigned long previousTime_1 = 0;
unsigned long previousTime_2 = 0;
unsigned long previousTime_3 = 0;

int angleChange = 0;
//WiFiServer server(80);
void setup() {
  pinMode(atesSensoru,INPUT);
  
  Serial.begin(115200);
  ss.begin(9600);

  Wire.begin();
  Wire.beginTransmission(MPU_addr);
  Wire.write(0x6B); 
  Wire.write(0);
  Wire.endTransmission(true);

  Serial.print("Baglaniliyor: ");
  Serial.println(ssid);

  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("");
  Serial.println("WiFi baglandi");
  
  ThingSpeak.begin(client);
  Firebase.begin(FIREBASE_HOST, FIREBASE_AUTH); 
}

void loop() {

  unsigned long currentTime = millis();
  
  int atesDurumu = digitalRead(atesSensoru);
  sicaklik = analogRead(sicaklikSensoru);
  sicaklik = (sicaklik/1024) * 3300;
  sicaklikC = sicaklik/10;

  //JIROSKOP OKUNUYOR
  mpu_read();

  //GPS KODU
  while (ss.available() > 0)
    if (gps.encode(ss.read())) {
      
      if (gps.location.isValid()) {
        latitude = gps.location.lat();
        lat_str = String(latitude, 6);
        longitude = gps.location.lng();
        lng_str = String(longitude, 6);
        Serial.println(lng_str);
        Serial.println(lat_str);
      }
    }

  float hiz = gps.speed.mps(); 

  //JIROSKOP KAZA DEGERLERI ICIN 
  ax = (AcX-2050)/16384.00;
  ay = (AcY-77)/16384.00;
  az = (AcZ-1947)/16384.00;
  gx = (GyX+270)/131.07;
  gy = (GyY-351)/131.07;
  gz = (GyZ+136)/131.07;

  float Raw_Amp = pow(pow(ax,2)+pow(ay,2)+pow(az,2),0.5);
  int Amp = Raw_Amp * 10; 

  Serial.println(Amp);
  if (Amp<=2 && trigger2==false){ 
   trigger1=true;

   }
 if (trigger1==true){
   trigger1count++;
   if (Amp>=12){ 
     trigger2=true;
     trigger1=false; trigger1count=0;
     }
 }
 if (trigger2==true){
   trigger2count++;
   angleChange = pow(pow(gx,2)+pow(gy,2)+pow(gz,2),0.5); Serial.println(angleChange);
   if (angleChange>=30 && angleChange<=400){ 
     trigger3=true; trigger2=false; trigger2count=0;
     Serial.println(angleChange);
       }
   }
 if (trigger3==true){
    trigger3count++;
    if (trigger3count>=10){ 
       angleChange = pow(pow(gx,2)+pow(gy,2)+pow(gz,2),0.5);
       //delay(10);
       Serial.println(angleChange); 
       if ((angleChange>=0) && (angleChange<=10)){ 
           fall=true; trigger3=false; trigger3count=0;
           Serial.println(angleChange);

             }
       else{ 
          trigger3=false; trigger3count=0;
       }
     }
  }
 if (fall==true){
   Serial.println("KAZA YAPILDI");
    mailGonder();
    Firebase.setBool("kaza", true);
    fall=false;
   }
 if (trigger2count>=6){ 
   trigger2=false; trigger2count=0;
   }
 if (trigger1count>=6){
   trigger1=false; trigger1count=0;
   }

  //THINGSPEAK ICIN ZAMANLAYICI
  if ( currentTime - previousTime_1 >= eventTime_1_THINKSPEAK) {
    ThingSpeak.setField(1, sicaklikC);
    ThingSpeak.setField(2, hiz);
    ThingSpeak.setField(3, lat_str);
    ThingSpeak.setField(4, lng_str);
    ThingSpeak.writeFields(channelNumber, writeAPIKey);
    previousTime_1 = currentTime;
  }

  //FIREBASE ICIN ZAMANLAYICI
  if ( currentTime - previousTime_2 >= eventTime_2_FIREBASE) {
    Firebase.setFloat("sicaklik", sicaklikC);
    Firebase.setString("latitude", lat_str);
    Firebase.setString("longitude", lng_str);
    Firebase.setBool("atesVarMi", atesDurumu);
    Firebase.setFloat("hiz", hiz);
    
    previousTime_2 = currentTime;
  }
  delay(100);
}

 void mpu_read(){
   Wire.beginTransmission(MPU_addr);
   Wire.write(0x3B);  // starting with register 0x3B (ACCEL_XOUT_H)
   Wire.endTransmission(false);
   Wire.requestFrom(MPU_addr,14,true);  // request a total of 14 registers
   AcX=Wire.read()<<8|Wire.read();  // 0x3B (ACCEL_XOUT_H) & 0x3C (ACCEL_XOUT_L)    
   AcY=Wire.read()<<8|Wire.read();  // 0x3D (ACCEL_YOUT_H) & 0x3E (ACCEL_YOUT_L)
   AcZ=Wire.read()<<8|Wire.read();  // 0x3F (ACCEL_ZOUT_H) & 0x40 (ACCEL_ZOUT_L)
   Tmp=Wire.read()<<8|Wire.read();  // 0x41 (TEMP_OUT_H) & 0x42 (TEMP_OUT_L)
   GyX=Wire.read()<<8|Wire.read();  // 0x43 (GYRO_XOUT_H) & 0x44 (GYRO_XOUT_L)
   GyY=Wire.read()<<8|Wire.read();  // 0x45 (GYRO_YOUT_H) & 0x46 (GYRO_YOUT_L)
   GyZ=Wire.read()<<8|Wire.read();  // 0x47 (GYRO_ZOUT_H) & 0x48 (GYRO_ZOUT_L)
 }

void mailGonder()
{
  if (client.connected())
  {
    client.stop();
  }

  client.flush();
  if (client.connect(IFTTT_HOST,80)) {
    // build the HTTP request
    String toSend = "GET /trigger/";
    toSend += EVENT_MAIL;
    toSend += "/with/key/";
    toSend += IFTTTKEY;
    toSend += "?value1=";
    toSend += lat_str;
    toSend += "&value2=";
    toSend += lng_str;
    toSend += " HTTP/1.1\r\n";
    toSend += "Host: ";
    toSend += IFTTT_HOST;
    toSend += "\r\n";
    toSend += "Connection: close\r\n\r\n";
    client.print(toSend);
    delay(250);
    Serial.println("Email Gönderildi");
  delay(1000);
  }
  client.flush();
  client.stop();  
}
 
