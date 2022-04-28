#include <Servo.h>

#include <Wire.h>
#include "RTClib.h"
#include "pitches.h"
#include "Arduino.h"
#include <SoftwareSerial.h>

#define BT_RXD 3
#define BT_TXD 2

RTC_DS3231 rtc; //RTC로 선언
int data[100];  
int h=0;
int m=0;
int s1 =0;
int start=0; 

int i=0;
int n=0; //i=1
int n1=0;
int k=0;//i=2
int k1=0;//i=3
Servo myservo;  // 서보모터 객체를 myservo로 생성 360도
Servo s;  // 서보모터를 제어하는 클래스 180도
unsigned char angle; //각도가 저장되는 변수
SoftwareSerial bluetooth(BT_RXD,BT_TXD);

////////////////////////////////////////////로드셀
class Hx711
{
public:
  Hx711(uint8_t pin_din, uint8_t pin_slk);
  virtual ~Hx711();
  long value();
  long nomalvalue(byte times = 32);
  void setOffset(long offset);
  void setScale(float scale = 742.f);
  float gram();

private:
  const uint8_t DOUT;
  const uint8_t SCK;
  long _offset;
  float _scale;
};



Hx711 scale(A1, A0);
Hx711::Hx711(uint8_t pin_dout, uint8_t pin_slk) :
    DOUT(pin_dout), SCK(pin_slk)
{
  pinMode(SCK, OUTPUT);
  pinMode(DOUT, INPUT);

  digitalWrite(SCK, HIGH);
  delayMicroseconds(100);
  digitalWrite(SCK, LOW);

  nomalvalue();
  this->setOffset(nomalvalue());
  this->setScale();
}

Hx711::~Hx711()
{

}

long Hx711::nomalvalue(byte times)
{
  long sum = 0;
  for (byte i = 0; i < times; i++)
  {
    sum += value();
  }

  return sum / times;
}

long Hx711::value()
{
  byte data[3];

  while (digitalRead(DOUT))
    ;

  for (byte j = 3; j--;)
  {
    for (char i = 8; i--;)
    {
      digitalWrite(SCK, HIGH);
      bitWrite(data[j], i, digitalRead(DOUT));
      digitalWrite(SCK, LOW);
    }
  }

  digitalWrite(SCK, HIGH);
  digitalWrite(SCK, LOW);

  data[2] ^= 0x80;

  return ((uint32_t) data[2] << 16) | ((uint32_t) data[1] << 8)
      | (uint32_t) data[0];
}

void Hx711::setOffset(long offset)
{
  _offset = offset;
}

void Hx711::setScale(float scale)
{
  _scale = scale;
}

float Hx711::gram()
{
  long val = (nomalvalue() - _offset);
  return (float) val / _scale;
}
///////////////////////////////////////////////로드셀

////////////////////////////////////////////피에조
int melody[] = {
  NOTE_C4, NOTE_G3, NOTE_G3, NOTE_A3, NOTE_G3, 0, NOTE_B3, NOTE_C4
};

// note durations: 4 = quarter note, 8 = eighth note, etc.:
int noteDurations[] = {
  4, 8, 8, 4, 4, 4, 4, 4
};
////////////////////////////////////////////////피에조

void setup () 
{
   s.attach(9);  // 서보모터를 제어하는 신호가 발생하는 핀을 우노보드에게 알림//180
  myservo.attach(8);  // 서보모터 9번핀에 연결

  
  Serial.begin(115200);// 시리얼 프린트 시작
   bluetooth.begin(9600);

  delay(1000); // 콘솔을 열기를 기다립니다.

  if (! rtc.begin()) { 
//(!는 논리 부정연산자로 거짓을 참으로 참을 거짓으로 바꾸어 줍니다. 즉 RTC가 시작을 하지 않는다면 
//이라는 조건입니다.)f
    while (1); //1번 반복
  }
  
  if (rtc.lostPower()) { //RTC에 전원이 없다면
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
rtc.adjust(DateTime(2021, 12, 8, 02, 32, 0)); 
  }
}

void loop () 
{
    DateTime now = rtc.now(); //현재시간을 세팅
    h=now.hour();
    if(h>=13) h=h-12;
    m= now.minute();
    s1=  now.second();
    //////////////////////////////시리얼통신
    while(Serial.available()){
            data[i] = Serial.read();
            
            Serial.print("설정시간 : ");
            Serial.print(n1);Serial.print("시"); Serial.println(k1);Serial.println("분");
            Serial.print("현재시간 : ");
            Serial.print(h);Serial.print("시"); Serial.print(m);Serial.print("분");
            if((h==n1)&&(m==k1)){
          start =1;   //시간이 일차하면 start=1
          Serial.println("성공");
        }

if (start==1){    //start가 1이면 배식 시작
      Serial.println("stat=1");
      s.write(angle); // 원하는 각도만큼 서보모터를 움직여요.
  delay(1000); // 서보모터가 움직일 시간
  angle=-90; // 원하는 각도만큼 서보모터를 움직여요.
      if(data[0]==49){
  angle=90;
  s.write(angle); // 원하는 각도만큼 서보모터를 움직여요.
  delay(1000); // 서보모터가 움직일 시간
  angle=-90; // 원하는 각도만큼 서보모터를 움직여요.
   
  }else if(data[0]==50){
 angle=30;
  s.write(angle); // 원하는 각도만큼 서보모터를 움직여요.
  delay(700); // 서보모터가 움직일 시간
   angle=-30;
  s.write(angle); // 원하는 각도만큼 서보모터를 움직여요.
 
  }else if(data[0]==51){
 angle=30;
  s.write(angle); // 원하는 각도만큼 서보모터를 움직여요.
  delay(900); // 서보모터가 움직일 시간
   angle=-30;
  s.write(angle); // 원하는 각도만큼 서보모터를 움직여요.
 
  }else if(data[0]==52){
 angle=30;
  s.write(angle); // 원하는 각도만큼 서보모터를 움직여요.
  delay(1100); // 서보모터가 움직일 시간
   angle=-30;
  s.write(angle); // 원하는 각도만큼 서보모터를 움직여요.

  }else if(data[0]==53){
  angle=30;
  s.write(angle); // 원하는 각도만큼 서보모터를 움직여요.
  delay(1300); // 서보모터가 움직일 시간
  angle=-30;
  s.write(angle); // 원하는 각도만큼 서보모터를 움직여요.
  }
  
myservo.write(160);
  delay(1200);
  myservo.write(90);
  delay(10000);
  for (int thisNote = 0; thisNote < 8; thisNote++) {


    int noteDuration = 1000 / noteDurations[thisNote];
    tone(7, melody[thisNote], noteDuration);

    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    // stop the tone playing:
    noTone(7);
  }
delay(50000);
myservo.write(20);
delay(1200);
myservo.write(90);
delay(500);
int gram = scale.gram()*2;
Serial.print(scale.gram()*2, 1);
Serial.println(" g");
Serial.write("15");
Serial.write(gram);
Serial.write("99");
  
 }
 
  
if(i==1){//시간-앞자리
  if(data[1]==48){
    n=0;
  }else if(data[1]==49){
    n=1;
  }else if(data[1]==50){
    n=2;
  }else if(data[1]==51){
    n=3;
  }else if(data[1]==52){
    n=4;
  }else if(data[1]==53){
    n=5;
  }else if(data[1]==54){
    n=6;
  }else if(data[1]==55){
    n=7;
  }else if(data[1]==56){
    n=8;
  }else if(data[1]==57){
    n=9;
  }
  n=n*10;
}

if(i==2){//분
  if(data[2]==48){
   n1=0;
  }else if(data[2]==49){
    n1=1;
  }else if(data[2]==50){
    n1=2;
  }else if(data[2]==51){
   n1=3;
  }else if(data[2]==52){
    n1=4;
  }else if(data[2]==53){
    n1=5;
  }else if(data[2]==54){
    n1=6;
  }else if(data[2]==55){
    n1=7;
  }else if(data[2]==56){
    n1=8;
  }else if(data[2]==57){
   n1=9;
  }
   
  n1=n+n1;   //분 십의자리계산

}
if(i==3){//분
  if(data[3]==48){
   k=0;
  }else if(data[3]==49){
    k=1;
  }else if(data[3]==50){
    k=2;
  }else if(data[3]==51){
   k=3;
  }else if(data[3]==52){
    k=4;
  }else if(data[3]==53){
    k=5;
  }else if(data[3]==54){
    k=6;
  }else if(data[3]==55){
    k=7;
  }else if(data[3]==56){
    k=8;
  }else if(data[3]==57){
   k=9;
  }
 
  k=k*10;      //분 계산

}

if(i==4){//분
  if(data[4]==48){
   k1=0;
  }else if(data[4]==49){
    k1=1;
  }else if(data[4]==50){
    k1=2;
  }else if(data[4]==51){
   k1=3;
  }else if(data[4]==52){
    k1=4;
  }else if(data[4]==53){
    k1=5;
  }else if(data[4]==54){
    k1=6;
  }else if(data[4]==55){
    k1=7;
  }else if(data[4]==56){
    k1=8;
  }else if(data[4]==57){
   k1=9;
  }
 
  k1=k+k1;      //분 계산

}

i++;
if(i==5) i=0;
 }
    if(bluetooth.available()){
        data[i] = bluetooth.read(); // 0번째 블루투스 입력값
        if((h==n)&&(m==k1)){
          start =1;   //시간이 일차하면 start=1
        }

  if (start==1){    //start가 1이면 배식 시작
      Serial.print("stat=1");
      if(data[0]==49){
  angle=30;
  s.write(angle); // 원하는 각도만큼 서보모터를 움직여요.
  delay(500); // 서보모터가 움직일 시간
  angle=-30;
   s.write(angle); // 원하는 각도만큼 서보모터를 움직여요.
   
  }else if(data[0]==50){
 angle=30;
  s.write(angle); // 원하는 각도만큼 서보모터를 움직여요.
  delay(700); // 서보모터가 움직일 시간
   angle=-30;
  s.write(angle); // 원하는 각도만큼 서보모터를 움직여요.
 
  }else if(data[0]==51){
 angle=30;
  s.write(angle); // 원하는 각도만큼 서보모터를 움직여요.
  delay(900); // 서보모터가 움직일 시간
   angle=-30;
  s.write(angle); // 원하는 각도만큼 서보모터를 움직여요.
 
  }else if(data[0]==52){
 angle=30;
  s.write(angle); // 원하는 각도만큼 서보모터를 움직여요.
  delay(1100); // 서보모터가 움직일 시간
   angle=-30;
  s.write(angle); // 원하는 각도만큼 서보모터를 움직여요.

  }else if(data[0]==53){
  angle=30;
  s.write(angle); // 원하는 각도만큼 서보모터를 움직여요.
  delay(1300); // 서보모터가 움직일 시간
  angle=-30;
  s.write(angle); // 원하는 각도만큼 서보모터를 움직여요.
  }
  
myservo.write(120);
delay(500);
myservo.write(90);
delay(10000);
  for (int thisNote = 0; thisNote < 8; thisNote++) {


    int noteDuration = 1000 / noteDurations[thisNote];
    tone(7, melody[thisNote], noteDuration);

    int pauseBetweenNotes = noteDuration * 1.30;
    delay(pauseBetweenNotes);
    // stop the tone playing:
    noTone(7);
  }
  
myservo.write(60);
delay(500);
myservo.write(90);
Serial.print(scale.gram()*2, 1);
  Serial.println(" g");
 }
 
  
if(i==1){//시간
  if(data[1]==48){
    n=0;
  }else if(data[1]==49){
    n=1;
  }else if(data[1]==50){
    n=2;
  }else if(data[1]==51){
    n=3;
  }else if(data[1]==52){
    n=4;
  }else if(data[1]==53){
    n=5;
  }else if(data[1]==54){
    n=6;
  }else if(data[1]==55){
    n=7;
  }else if(data[1]==56){
    n=8;
  }else if(data[1]==57){
    n=9;
  }
}

if(i==2){//분
  if(data[2]==48){
   k=0;
  }else if(data[2]==49){
    k=1;
  }else if(data[2]==50){
    k=2;
  }else if(data[2]==51){
   k=3;
  }else if(data[2]==52){
    k=4;
  }else if(data[2]==53){
    k=5;
  }else if(data[2]==54){
    k=6;
  }else if(data[2]==55){
    k=7;
  }else if(data[2]==56){
    k=8;
  }else if(data[2]==57){
   k=9;
  }
   
  k=k*10;   //분 십의자리계산

}
if(i==3){//분
  if(data[3]==48){
   k1=0;
  }else if(data[3]==49){
    k1=1;
  }else if(data[3]==50){
    k1=2;
  }else if(data[3]==51){
   k1=3;
  }else if(data[3]==52){
    k1=4;
  }else if(data[3]==53){
    k1=5;
  }else if(data[3]==54){
    k1=6;
  }else if(data[3]==55){
    k1=7;
  }else if(data[3]==56){
    k1=8;
  }else if(data[3]==57){
   k1=9;
  }
 
  k1=k+k1;      //분 계산

}

i++;
 }
}
    


      
