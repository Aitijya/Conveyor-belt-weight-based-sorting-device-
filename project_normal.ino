#include <Wire.h> 
#include <LiquidCrystal_I2C.h>
#include <Servo.h>
#include<HX711_ADC.h>
HX711_ADC Loadcell(3,2);

Servo myservo;
// Set the LCD address to 0x27 for a 16 chars and 2 line display
LiquidCrystal_I2C lcd(0x27, 16, 2);

int sensor, pass=0,fail=0,state=0;
 #define DT A0
 #define SCK A1
 #define sw 9 
long sample=0;
 float val=0;
 long count=0;
 unsigned long readCount(void)
 { unsigned long Count;
 unsigned char i;
 pinMode(DT, OUTPUT); 
digitalWrite(DT,HIGH);
 digitalWrite(SCK,LOW); 
Count=0;
 pinMode(DT, INPUT);
 while(digitalRead(DT));
 for (i=0;i<24;i++) 
{
 digitalWrite(SCK,HIGH);
 Count=Count<<1;
 digitalWrite(SCK,LOW);
 if(digitalRead(DT)) 
Count++;
 }
 digitalWrite(SCK,HIGH);
 Count=Count^0x800000;
 digitalWrite(SCK,LOW);
 return(Count);
 }
void setup() {
  myservo.attach(6);
  // put your setup code here, to run once:
  lcd.begin(16,2);

  // Turn on the blacklight and print a message.
  lcd.backlight();
  lcd.setCursor(0,0);
  lcd.print("Conveyor System");
  pinMode(2,OUTPUT);
  pinMode(3,OUTPUT);
  pinMode(4,OUTPUT);
  pinMode(5,OUTPUT);
  pinMode(6,OUTPUT);
  pinMode(7,OUTPUT);
  pinMode(12,INPUT);
  Serial.begin(9600);
  myservo.write(90);
  delay(500);
  myservo.write(02);
  delay(500);
  myservo.write(180);

pinMode(SCK, OUTPUT); 
pinMode(sw, INPUT_PULLUP);
 lcd.begin(16, 2);
 lcd.print(" Weight ");
 lcd.setCursor(0,1);
 lcd.print(" Measurement ");
 delay(1000);
 lcd.clear();
}

void loop() {
  if(digitalRead(12))
  state = !state;
  delay(100);
  Serial.println(state);
  
  // put your main code here, to run repeatedly:
//  Serial.println(analogRead(A0));
//  delay(100);
  sensor = analogRead(A0)>900;
  lcd.setCursor(0,1);
  lcd.print("Rejected: ");
  lcd.print(fail);
  if(!sensor)
  {
    fail++;
    sensor = analogRead(A0)>900;
    while(!sensor)
    {
      sensor = analogRead(A0)>900;
//      belt(1);
    
    }
    delay(10);
    myservo.write(0);
    delay(500);
    myservo.write(180);
  }
//  Serial.println(fail);
//  delay(200);
count= readCount();
 int w=(((count-sample)/val)-2*((count-sample)/val)); 
lcd.setCursor(0,0);
 lcd.print("Measured Weight");
 lcd.setCursor(0,1);
 lcd.print(w);
 lcd.print("g ");
 if(digitalRead(sw)==0)
 {
 val=0;
 sample=0;
 w=0;
 count=0;
 calibrate();
 }
 }


void belt(bool value)
{
  if(value)
  {
  digitalWrite(2,1);
  digitalWrite(4,0);
  analogWrite(11,120);
  }
  else
  {
  digitalWrite(2,0);
  digitalWrite(4,0);
  analogWrite(11,180);
    }
} void calibrate()
 { lcd.clear();
 lcd.print("Calibrating...");
 lcd.setCursor(0,1);
 lcd.print("Please Wait...");
 for(int i=0;i<100;i++)
 { count=readCount();
 sample+=count;
 }
 sample/=100;
 lcd.clear();
 lcd.print("Put 100g & wait");
 count=0;
 while(count<1000)
 {
   count=readCount();
 count=sample-count;
 }
 lcd.clear();
 lcd.print("Please Wait...."); 
delay(2000);
 for(int i=0;i<100;i++) 
{ count=readCount();
 val+=sample-count;
 }
 val=val/100.0;
 val=val/100.0; 
// put here your calibrating weight
 lcd.clear();
 }
