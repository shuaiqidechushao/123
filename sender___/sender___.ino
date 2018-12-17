// Example testing sketch for various DHT humidity/temperature sensors
// Written by ladyada, public domain
         
int dustPin=5;

float dustVal=0;
int ledPower=2;
int delayTime=280;
int delayTime2=40;
float offTime=9680;
 

#include<stdlib.h>
#include "DHT.h"
#include <SPI.h>
#include <Mirf.h>
#include <nRF24L01.h>
#include <MirfHardwareSpiDriver.h>
#define DHTPIN A0     // what digital pin we're connected to

#define DHTTYPE DHT11   // DHT 11

DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(9600);
  pinMode(ledPower,OUTPUT);
  pinMode(dustPin, INPUT);
  dht.begin();

    //无线部分
    Mirf.cePin = 9;                //设置CE引脚为D9
    Mirf.csnPin = 10;        //设置CE引脚为D10
    
    Mirf.spi = &MirfHardwareSpi;
    Mirf.init();  //初始化nRF24L01                
        //设置接收标识符"Sen01"
    Mirf.setRADDR((byte *)"Sen01");
    //设置一次收发的字节数，这里发一个整数，写sizeof(unsigned int)，实际等于2字节
    //Mirf.payload = sizeof(unsigned int);
     Mirf.payload = sizeof(unsigned int);
    //发送通道，可以填0~128，收发必须一致。
    Mirf.channel = 3;
    Mirf.config();
}

int convertToPercent(int x)
{
  int percentX = 0;
  percentX = map(x, 0, 1024, 0, 100);
  return percentX;
}

int adata[]={0};
int x,y;

void loop() {
  // Wait a few seconds between measurements.
  delay(2000);

  // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
  float h = dht.readHumidity();
  // Read temperature as Celsius (the default)
  float t = dht.readTemperature();
  // Read temperature as Fahrenheit (isFahrenheit = true)
  float f = dht.readTemperature(true);

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t) || isnan(f)) {
    Serial.println("Failed to read from DHT sensor!");
    return;
  }
  // Compute heat index in Fahrenheit (the default)
  float hif = dht.computeHeatIndex(f, h);
  // Compute heat index in Celsius (isFahreheit = false)
  float hic = dht.computeHeatIndex(t, h, false);
  Serial.println(h);

  digitalWrite(ledPower,LOW); 
  delayMicroseconds(delayTime);
  dustVal=analogRead(dustPin); 
  delayMicroseconds(delayTime2);
  digitalWrite(ledPower,HIGH); 
  delayMicroseconds(offTime);
  delay(1000);
  if (dustVal>36.455)
  Serial.println(int((float(dustVal/1024)-0.0356)*120000*0.035));

  delay(1000);

//原码
    adata[0] = int(h);
    adata[1]=int((float(dustVal/1024)-0.0356)*120000*0.035);
    byte data0[Mirf.payload];
    byte data1[Mirf.payload];
    data0[0] = adata[0] & 0xFF;                //低八位给data[0]，
    data0[1] = adata[0] >> 8;                //高八位给data[1]。
    data1[0] = adata[1] & 0xFF;
    data1[1] = adata[1] >> 8;
    //设置向"Rev01"发送数据
    Mirf.setTADDR((byte *)"Rev01");
    Mirf.send(data0);
    Mirf.send(data1);
    Serial.print("I'm sending the humidity:");
    Serial.println(adata[0]); 
    Serial.print("I'm sending the condition of the air:");
    Serial.println(adata[1]); 
    //while死循环等待发送完毕，才能进行下一步操作。
    while(Mirf.isSending()) {}
    delay(20);
//
////改码
//Mirf.setTADDR((byte *)"Rev01");
//int a1=int(h);
//int a2=int((float(dustVal/1024)-0.0356)*120000*0.035);
//Mirf.send((byte *)&a1);
//Mirf.send((byte *)&a2);
//while(Mirf.isSending()) delay(1);
//delay(1000);
} 
