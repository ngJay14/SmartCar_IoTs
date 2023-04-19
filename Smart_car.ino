
#define BLYNK_PRINT Serial
#include <WiFi.h>
#include <BlynkSimpleEsp32.h>
uint8_t flag = 1;
#define ENA 5
#define IN1 4
#define IN2 12
#define IN3 0 
#define IN4 2
#define ENB 32
#include <Servo.h>
Servo myservo; 
const int trig = 23; 
const int echo = 22;
int gioihan = 15;
unsigned long thoigian; // biến đo thời gian
int khoangcach;           // biến lưu khoảng cách
int khoangcachtrai,khoangcachphai;
#define motorA1 4  
#define motorA2 12 
#define motorAspeed 5 
#define motorB1 0
#define motorB2 2
#define motorBspeed 32      
const int L_S1 = 33; // cb dò line phải 1
const int L_S2 = 35; // cb dò line phải 2
const int S_S = 39; // cb dò line giữa
const int R_S1 = 34; //cb dò line trái 1
const int R_S2 = 14; //cb dò line trái 2
// Variables for the Blynk widget values
int x = 50;
int y = 50;
int Speed;
int mode = 0;

char auth[] = ""; //Enter your Blynk auth token
char ssid[] = ""; //Enter your WIFI name
char pass[] = ""; //Enter your WIFI passowrd


void setup() {
  pinMode(L_S1, INPUT);
  pinMode(L_S2, INPUT); 
  pinMode(R_S1, INPUT);
  pinMode(R_S2, INPUT);
  pinMode(S_S, INPUT);
  pinMode(motorA1, OUTPUT);
  pinMode(motorA2, OUTPUT);
  pinMode(motorB1, OUTPUT);
  pinMode(motorB2, OUTPUT);
  pinMode(motorAspeed, OUTPUT);
  pinMode(motorBspeed, OUTPUT);
  myservo.attach(18);  
  pinMode(trig,OUTPUT);   // chân trig sẽ phát tín hiệu
  pinMode(echo,INPUT);    // chân echo sẽ nhận tín hiệu
  Serial.begin(9600);
  myservo.write(90);    
  delay(500);
  delay(2000);
  
  Serial.begin(9600);
  pinMode(ENA, OUTPUT);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(ENB, OUTPUT);
  WiFi.begin(ssid, pass);
  unsigned int cur_connect,startconnect = millis();
  while (WiFi.status() != WL_CONNECTED) {
        cur_connect = millis();
        delay(200);
        if(cur_connect - startconnect > 10000 ) {
          flag = 0;
           break;
        }
  }
  if (flag==1)
  {
    Blynk.begin(auth, ssid, pass, "blynk.cloud", 80);
  }
}

// Get the joystick values
BLYNK_WRITE(V0) {
  x = param[0].asInt();
}
// Get the joystick values
BLYNK_WRITE(V1) {
  y = param[0].asInt();
}
//Get the slider values
BLYNK_WRITE(V2) {
  Speed = param.asInt();
}
BLYNK_WRITE(V3) {
  mode = param.asInt();
}
// Check these values using the IF condition
void smartcar() {
  if (y > 70) {
    carForward();
    Serial.println("carForward");
  } else if (y < 30) {
    carBackward();
    Serial.println("carBackward");
  } else if (x < 30) {
    carLeft();
    Serial.println("carLeft");
  } else if (x > 70) {
    carRight();
    Serial.println("carRight");
  } else if (x < 70 && x > 30 && y < 70 && y > 30) {
    carStop();
    Serial.println("carstop");
  }
}


/**************Motor movement functions*****************/
void carForward() {
  analogWrite(ENA, Speed);
  analogWrite(ENB, Speed);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}
void carBackward() {
  analogWrite(ENA, Speed);
  analogWrite(ENB, Speed);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}
void carLeft() {
  analogWrite(ENA, Speed);
  analogWrite(ENB, Speed);
  digitalWrite(IN1, HIGH);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, HIGH);
  digitalWrite(IN4, LOW);
}
void carRight() {
  analogWrite(ENA, Speed);
  analogWrite(ENB, Speed);
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, HIGH);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, HIGH);
}
void carStop() {
   analogWrite(ENA, 0);
   analogWrite(ENB, 0);
}
void doline() {
    dokhoangcach();

  if ((digitalRead(L_S1) == 1) && (digitalRead(S_S) == 0) && (digitalRead(R_S1) == 1)) {
    forword(); // đi tiến
  }
  if ((digitalRead(L_S2) == 1) && (digitalRead(S_S) == 0) && (digitalRead(R_S2) == 1)) {
    forword(); // đi tiến
  }

  if ((digitalRead(L_S1) == 0) && (digitalRead(S_S) == 0) && (digitalRead(R_S1) == 1)) {
    turnLeft(); 
  }
  if ((digitalRead(L_S2) == 0) && (digitalRead(S_S) == 0) && (digitalRead(R_S2) == 1)) {
    turnLeft(); 
  }
  if ((digitalRead(L_S1) == 0) && (digitalRead(S_S) == 1) && (digitalRead(R_S1) == 1)) {
    turnLeft(); 
  }
  if ((digitalRead(L_S2) == 0) && (digitalRead(S_S) == 1) && (digitalRead(R_S2) == 1)) {
    turnLeft(); 
  }


  if ((digitalRead(L_S1) == 1) && (digitalRead(S_S) == 0) && (digitalRead(R_S1) == 0)) {
    turnRight(); // rẻ phải
  }
  if ((digitalRead(L_S2) == 1) && (digitalRead(S_S) == 0) && (digitalRead(R_S2) == 0)) {
    turnRight(); // rẻ phải
  }
  if ((digitalRead(L_S1) == 1) && (digitalRead(S_S) == 1) && (digitalRead(R_S1) == 0)) {
    turnRight(); // rẻ phải
  }
   if ((digitalRead(L_S2) == 1) && (digitalRead(S_S) == 1) && (digitalRead(R_S2) == 0)) {
    turnRight(); // rẻ phải
  }

  if ((digitalRead(L_S1) == 0) && (digitalRead(S_S) == 0) && (digitalRead(R_S1) == 0)&&(digitalRead(L_S2) == 0) && (digitalRead(S_S) == 0) && (digitalRead(R_S2) == 0)) {
    xoayphai(); 
  }
   if ((digitalRead(L_S1) == 1) && (digitalRead(S_S) == 1) && (digitalRead(R_S1) == 1)&&(digitalRead(L_S2) == 1) && (digitalRead(S_S) == 1) && (digitalRead(R_S2) == 1)) {
    lui(); // đi lùi
  }
  if ((digitalRead(L_S1) == 1) && (digitalRead(S_S) == 1) && (digitalRead(R_S1) == 1)&&(digitalRead(L_S2) == 1) && (digitalRead(S_S) == 1) && (digitalRead(R_S2) == 0)) {
    turnRight(); // rẻ phải
  }
   if ((digitalRead(L_S1) == 0) && (digitalRead(S_S) == 1) && (digitalRead(R_S1) == 1)&&(digitalRead(L_S2) == 1) && (digitalRead(S_S) == 1) && (digitalRead(R_S2) == 1)) {
   turnLeft(); // rẻ trái
  }




  if (khoangcach < gioihan) // nếu khoảng cách nhỏ hơn giới hạn
  {
     Stop();
     delay(500);
     lui();
     delay(500);
     Stop();
     quaycbsangtrai();
     khoangcachtrai=khoangcach;
     quaycbsangphai();
     khoangcachphai=khoangcach;
    if(khoangcachphai>khoangcachtrai) 
    {
       xoayphai();
       delay(700);
    }
    else 
    {
      xoaytrai();
     delay(700);
    }
  }


  
}

void forword() { // chương trình con xe robot đi tiến

  digitalWrite (motorA1, LOW);
  digitalWrite(motorA2, HIGH);
  digitalWrite (motorB1, HIGH);
  digitalWrite(motorB2, LOW);
  analogWrite(motorAspeed, 180); 
  analogWrite(motorBspeed, 200);
}


void turnLeft() {

  digitalWrite (motorA1, LOW);
  digitalWrite(motorA2, LOW);
  digitalWrite (motorB1, HIGH);
  digitalWrite(motorB2, LOW);
  analogWrite(motorAspeed, 180); // tốc độ động cơ a ban đầu 120 ( 0 - 255)
  analogWrite(motorBspeed, 200);
}

void turnRight() {

  digitalWrite (motorA1, LOW);
  digitalWrite(motorA2, HIGH);
  digitalWrite (motorB1, LOW);
  digitalWrite(motorB2, LOW);
  analogWrite(motorAspeed, 180); // tốc độ động cơ a ban đầu 120 ( 0 - 255)
  analogWrite(motorBspeed, 200);
}

void Stop() {

  digitalWrite (motorA1, LOW);
  digitalWrite(motorA2, LOW);
  digitalWrite (motorB1, LOW);
  digitalWrite(motorB2, LOW);
}
void lui() { // chương trình con xe robot đi tiến

  digitalWrite (motorA1, HIGH);
  digitalWrite(motorA2, LOW);
  digitalWrite (motorB1, LOW);
  digitalWrite(motorB2, HIGH);
   analogWrite(motorAspeed, 100); // tốc độ động cơ a ban đầu 120 ( 0 - 255)
   analogWrite(motorBspeed, 110);
}
void xoaytrai() { // chương trình con xe robot đi tiến

  digitalWrite (motorA1, HIGH);
  digitalWrite(motorA2, LOW);
  digitalWrite (motorB1, HIGH);
  digitalWrite(motorB2, LOW);
  analogWrite(motorAspeed, 160); // tốc độ động cơ a ban đầu 120 ( 0 - 255)
  analogWrite(motorBspeed, 180);
}
void xoayphai() { 

  digitalWrite (motorA1, LOW);
  digitalWrite(motorA2, HIGH);
  digitalWrite (motorB1, LOW);
  digitalWrite(motorB2, HIGH);
  analogWrite(motorAspeed, 160); // tốc độ động cơ a ban đầu 120 ( 0 - 255)
  analogWrite(motorBspeed, 180);
}
void dokhoangcach()
{
/* Phát xung từ chân trig */
    digitalWrite(trig,0);   // tắt chân trig
    delayMicroseconds(2);
    digitalWrite(trig,1);   // phát xung từ chân trig
    delayMicroseconds(5);   // xung có độ dài 5 microSeconds
    digitalWrite(trig,0);   // tắt chân trig
    
    /* Tính toán thời gian */
    // Đo độ rộng xung HIGH ở chân echo. 
    thoigian = pulseIn(echo,HIGH);  
    // Tính khoảng cách đến vật.
    khoangcach = int(thoigian/2/29.412);
}
void quaycbsangtrai()
{
    myservo.write(180); 
    delay(1000);             
    dokhoangcach();
    myservo.write(90);             
}
void quaycbsangphai()
{
    myservo.write(0);             
    delay(1000);
    dokhoangcach();
    myservo.write(90);             
}
void resetservo()
{
   myservo.write(90);
}
void loop() {
  
  if(flag==1)
   Blynk.run();
  if(mode==1 && flag==1 )
  {
    smartcar();
  }
  else
  {
    doline();
  }
 
}