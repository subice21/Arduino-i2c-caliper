#include <Wire.h> 
#include <LiquidCrystal_I2C.h>

#define CLOCK_PINread1 12
#define DATA_PINread1  11
#define CLOCK_PINread2 4
#define DATA_PINread2  3

LiquidCrystal_I2C lcd(0x27, 16, 2);

void setup()
{
  Serial.begin(9600);  
  // x:
  pinMode(CLOCK_PINread1, INPUT);
  pinMode(DATA_PINread1, INPUT);

  // y:
  pinMode(CLOCK_PINread2, INPUT);
  pinMode(DATA_PINread2, INPUT);
  
  lcd.begin();
  lcd.backlight();
  lcd.print("DRO Cliper");
  lcd.clear();
}
bool mm = true; //define mm to false if you want inces values
unsigned long tmpTime;

//X:
char buf[20];
int sign;
int inches;
long value;
float result;

//Y:
char buf2[20];
int sign2;
int inches2;
long value2;
float result2;





void loop()
{
  while(digitalRead(CLOCK_PINread1)==LOW && digitalRead(CLOCK_PINread2)==LOW) {}
  tmpTime=micros();
  while(digitalRead(CLOCK_PINread1)==HIGH && digitalRead(CLOCK_PINread2)==HIGH) {}
  if((micros()-tmpTime)<500) return;
  readCaliper();
   
  buf[0]=' ';
  buf2[0]=' ';
  
  
  dtostrf(result*2.54,6,3,buf+1); strcat(buf," cm ");  
  dtostrf(result2*2.54,6,3,buf2+1); strcat(buf2," cm "); 

  if(mm)
  {
    Serial.print(result); Serial.println(" mm");
    Serial.print(result2); Serial.println(" mm");
    lcd.clear();
    String stringOne = "X : " + String(result) + " mm";
    String stringTwo = "Y : " + String(result2 / 2) + " mm";
    lcd.setCursor(0, 0); 
    lcd.print(stringOne);
    lcd.setCursor(0, 1);
    lcd.print(stringTwo);
        
    delay(100);
  }
  else
  {
    Serial.print(result); Serial.println(" in");    
    delay(100);
  }
}
void readCaliper()
{
  sign=1;
  value=0;
  inches=0;

  sign2=1;
  value2=0;
  inches2=0;
  
  for(int i=0;i<24;i++) {
    while(digitalRead(CLOCK_PINread1)==LOW) {}   
    while(digitalRead(CLOCK_PINread1)==HIGH) {}
    if(digitalRead(DATA_PINread1)==HIGH ) {
      if(i<20) value|=(1<<i);    
      if(i==20) sign=-1;  
      if(i==23) inches=1;      
    }}
   for(int j=0;j<24;j++) {
      while(digitalRead(CLOCK_PINread2)==LOW) {}
      while(digitalRead(CLOCK_PINread2)==HIGH) {}

      
      if(digitalRead(DATA_PINread2)==HIGH) {   
      if(j<20) value2|=(1<<j);   
      if(j==20) sign2=-1;
      if(j==23) inches2=1; 
    }}
    
  if(mm){
    result=(value*sign)/100.0;
    result2=(value2*sign2)/100.0;
  }else{
  result=(value*sign)/(inches?2000.0:100.0);
  result2=(value2*sign2)/(inches2?2000.0:100.0);  
  }
}
