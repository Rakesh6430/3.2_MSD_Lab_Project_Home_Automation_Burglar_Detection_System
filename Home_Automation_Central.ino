#include<LiquidCrystal.h>
#include<Servo.h>
#include "DHT.h"
#define DHTPin 17

LiquidCrystal lcd1(13,12,11,10,9,8);
LiquidCrystal lcd2(22,23,24,25,26,27);
Servo servo;
DHT dht(DHTPin, DHT22);

int ldr_sensor=A1, led_green=6, AC_Lamp=7, button1=53, button2=52, regulator=A15, rotate=0;
int gas_sensor=41, buzzer=38, pos=90;

int fan1_forward=4;
int fan1_reverse=2;
int fan2_forward=6;
int fan2_reverse=5;

void setup() {
  lcd1.begin(16,2);
  lcd2.begin(16,2);
  pinMode(AC_Lamp,OUTPUT);
  pinMode(ldr_sensor,INPUT);
  pinMode(gas_sensor,INPUT);
  pinMode(buzzer,OUTPUT);
  servo.attach(3);
  Serial1.begin(9600);
  Serial3.begin(9600);
}

void loop() {
  while(Serial1.available()){
    char c=Serial1.read();
    if(c=='H'){
      float temp = dht.readTemperature();
      float humidity = dht.readHumidity();

      lcd1.setCursor(0,0);
      lcd1.print(String("Temp = ")+String(temp)+String(" ")+String(char(223))+String("C"));
      lcd1.setCursor(0,1);
      lcd1.print(String("RH = ")+String(humidity)+String(" %"));

      int value=analogRead(ldr_sensor);
      Serial3.print("Ldr sensor = ");
      Serial3.println(value);

      value=map(value,10,975,0,255);
      analogWrite(AC_Lamp,255-value);

      for(; pos<180; pos++){
        servo.write(pos);
        delay(10);
      }

      if(digitalRead(button1)==HIGH || rotate==1){
        int fan_speed=analogRead(regulator);
        fan_speed=map(fan_speed,0,1023,0,255);

        Serial3.print("Regu sensor = ");
        Serial3.println(fan_speed);
  
        //fan rotate
        rotate=1;
        analogWrite(fan1_forward, fan_speed);
        analogWrite(fan1_reverse, 0); 
      }

      if(digitalRead(button2)==HIGH){
        rotate=0;
        analogWrite(fan1_forward, 0);
        analogWrite(fan1_reverse, 0); 
      }

      if(digitalRead(gas_sensor)==HIGH){
        lcd2.setCursor(0,1);
        lcd2.print("Gas Detected!!");
        tone(buzzer,1000);
        digitalWrite(fan2_forward, LOW);
        digitalWrite(fan2_reverse, HIGH); 
        
        //activateAlarm();
      }
      else{
        lcd2.clear();
        noTone(buzzer);
        analogWrite(fan2_forward, 0);
        analogWrite(fan2_reverse, 0);
      }
    }
    else{
      lcd1.setCursor(0,0);
      lcd1.print("Burglar Detect!");
    }
  }
}

void activateAlarm(){
  for(int k=0; k<5; k++){
    tone(buzzer,1000);
    delay(500);
    noTone(buzzer);
  }
}
