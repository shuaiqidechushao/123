#include <SPI.h>
#include <Mirf.h>
#include <nRF24L01.h>
#include <MirfHardwareSpiDriver.h>

//定义一个变量adata存储最终结果,oldadata存储旧结果，防止相同结果刷屏。
//unsigned int adata = 0, oldadata = 0;
//unsigned int bdata=0,bodata=0;
unsigned int adata[]={0,0},oldadata[]={0,0};
int adata0=0,adata1=0;

int a1,a2;

int Sensor = 7; //接收传感器的信号
int buzzer = 8;//设置控制蜂鸣器的数字IO脚
void setup()
{
  Serial.begin(9600);
  pinMode(3, OUTPUT);  //发蓝色光
  pinMode(Sensor, INPUT); // 设置为人体红外输入
  pinMode(buzzer, OUTPUT); //设置数字IO脚模式，OUTPUT为输出

    //---------初始化部分，不可随时修改---------
    Mirf.cePin = 9;     //设置CE引脚为D9
    Mirf.csnPin = 10;   //设置Csn引脚为D10
    Mirf.spi = &MirfHardwareSpi;
    Mirf.init();  //初始化nRF24L01
 
    //---------配置部分，可以随时修改---------
    //设置接收标识符"Rev01"
    Mirf.setRADDR((byte *)"Rev01");
    //设置一次收发的字节数，这里发一个整数，
    //写sizeof(unsigned int)，实际等于2字节
    //Mirf.payload = sizeof(unsigned int);
    Mirf.payload = sizeof(unsigned int);
    //发送通道，可以填0~128，收发必须一致。
    Mirf.channel = 3;
    Mirf.config();
}
void loop()
{
    int val = digitalRead(Sensor); // 读取引脚电平
//
//    //原码
//    //接收nRF24L01传输的数据
//   
//    //定义一个暂存数组，大小为Mirf.payload。
//    byte data0[Mirf.payload];
//    byte data1[Mirf.payload];
//    if(Mirf.dataReady())    //等待接收数据准备好
//    {
//        Mirf.getData(data0);//接收数据到data数组
//        //data[1]<左移8位与data[0]并，重组数据。
//        Mirf.getData(data1);
//        adata0 = (unsigned int)((data0[1] << 8) | data0[0]);
//        adata1 = (unsigned int)((data1[1] << 8) | data1[0]);
//        adata[0] = (adata0,DEC);
//        adata[1] = (adata1,DEC);
//        Serial.println(adata0);
//        Serial.println(adata1);
//        if(adata != oldadata)
//        {
//            oldadata[0] = adata[0]; //本次结果作为历史结果。adata[0]为湿度
//            oldadata[1] = adata[1];//adata[1]为粉尘浓度
//            if(adata[0]>=80 && val==1)
//            {
//              digitalWrite(3,HIGH);
//            }
//            if(adata[1]>=1500 && val==1)
//            {
//              digitalWrite(buzzer,HIGH);
//             }
//          }
//        }  

if(adata[1]>=1500 && val==1)
            {
              digitalWrite(buzzer,HIGH);
             }

////改码
//if(Mirf.dataReady()){
//  Mirf.getData((byte *) &a1);
//  Mirf.getData((byte *) &a2);
//  Serial.println(a1);
//  Serial.println(a2);
//  if(a1>=80 && val==1)
//            {
//              digitalWrite(3,HIGH);
//            }
//            if(a2>=1500 && val==1)
//            {
//              digitalWrite(buzzer,HIGH);
//             }
//}
}

