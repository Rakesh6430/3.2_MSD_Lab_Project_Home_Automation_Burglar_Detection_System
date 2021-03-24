#include<LiquidCrystal.h>
#include<Keypad.h>
#include<Servo.h>

Servo myservo;

LiquidCrystal lcd(13,12,11,10,9,8);

char HexKeys[4][3]={
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'}
};

byte rowPins[4]={6,5,4,3};
byte colPins[3]={22,23,24};

Keypad customKeypad = Keypad(makeKeymap(HexKeys), rowPins, colPins, 4, 3);

int matrix[][7]={
  {1,1,1,1,1,1,0},
  {0,1,1,0,0,0,0},
  {1,1,0,1,1,0,1},
  {1,1,1,1,0,0,1},
  {0,1,1,0,0,1,1},
  {1,0,1,1,0,1,1},
  {1,0,1,1,1,1,1},
  {1,1,1,0,0,0,0},
  {1,1,1,1,1,1,1},
  {1,1,1,0,0,1,1}
};

int buzzer = 31;
int pir_sensor = 51;
int button_switch = 27;
int _try_=4, f=0, entry=0, n=9, alarm=0;
int led_red=53, led_green=52, valid=0;
int a=34, b=35, c=36, d=38, e=39, fg=40, g=44;

void setup() {
  lcd.begin(16,2);
  pinMode(buzzer,OUTPUT);
  pinMode(led_red,OUTPUT);
  pinMode(led_green,OUTPUT);
  pinMode(a,OUTPUT);
  pinMode(b,OUTPUT);
  pinMode(c,OUTPUT);
  pinMode(d,OUTPUT);
  pinMode(e,OUTPUT);
  pinMode(fg,OUTPUT);
  pinMode(g,OUTPUT);
  myservo.attach(7);
  Serial1.begin(9600);
}

void loop() {
  char arr[4];
  char password[4]="*9##";

  clearLCD(1);
  
  if(!entry){
    digitalWrite(led_red,HIGH);
    digitalWrite(led_green,LOW);
    lcd.setCursor(0,0);
    _try_=1;
    lcd.print("Password");
    n=9;
  }
  else{
    if(n>=0){
      digitalWrite(led_red,LOW);
      digitalWrite(led_green,HIGH);
      lcd.setCursor(0,0);
      lcd.print("    Welcome!!");
    }

    if(!alarm){
       if(digitalRead(pir_sensor)==HIGH){
          activateSensorAlarm();
          counter();
       }
    }
    else{
      n=9;
      Serial1.write('H');
    } 

    if(n<0){
      clearLCD(0);
      lcd.setCursor(0,1);
      lcd.print("Burglar Detected");
      delay(200);
      digitalWrite(led_red,HIGH);
      digitalWrite(led_green,LOW);
      Serial1.write('L');

      if(digitalRead(pir_sensor)==LOW){
        noTone(buzzer);
        entry=0;
        f=1;
      }
    }
  }
  
  while(_try_<=3){
    int i=0;
    f=0;
    char key;
    
    while(i<4){
      key=customKeypad.getKey();
      if(key){
        arr[i]=key;
        lcd.setCursor(0,1);
        lcd.print(arr[i]);
        i++;
      }
    }
    
    for(int j=0; j<4; j++){
      if(arr[j]!=password[j]){ 
        clearLCD(1);
        
        f=1;
        lcd.setCursor(0,1);
        lcd.print("Incorrect!");
        delay(300);
        
        clearLCD(1);
        
        lcd.setCursor(0,1);
        if(_try_==3){
          lcd.print("0 times left!");
          delay(500);
          lcd.clear();
          lcd.setCursor(0,1);
          lcd.print("Burglar Detected!");
          activateAlarm(2);
          clearLCD(1);
          break;
        }
        lcd.print(String("More ")+String(3-_try_)+String(" Times!"));
        delay(300);

        clearLCD(1);

        break;
      }
    }
    if(!f){
      break;
    }
    _try_++;
  }
 
  if(!f && entry==0){
    digitalWrite(led_red,LOW);
    digitalWrite(led_green,HIGH);
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("    Welcome!!");
    delay(200);
    openDoor();
    delay(50);
    closeDoor();
    _try_=4;
    entry=1;
  }
}

void clearLCD(int line){
  for(int i=0; i<16; i++){
    lcd.setCursor(i,line);
    lcd.print(" ");
  }
}

void activateAlarm(int n){
  for(int j=0; j<n; j++){
    tone(buzzer,1000);
    delay(1000);
    noTone(buzzer);
  }
}

void openDoor(){
  for(int i=90; i<180; i++){
      myservo.write(i);
      delay(10);
  }
}

void closeDoor(){
  for(int i=180; i>90; i--){
      myservo.write(i);
      delay(10);
  }
}

void activateSensorAlarm(){
    tone(buzzer,1000);
}

void counter(){
  for(; n>=0; n--){
    digitalWrite(a,matrix[n][0]);
    digitalWrite(b,matrix[n][1]);
    digitalWrite(c,matrix[n][2]);
    digitalWrite(d,matrix[n][3]);
    digitalWrite(e,matrix[n][4]);
    digitalWrite(fg,matrix[n][5]);
    digitalWrite(g,matrix[n][6]);
    delay(200);

    if(digitalRead(button_switch)==HIGH || digitalRead(pir_sensor)==LOW){
      alarm=1;
      noTone(buzzer);
      break;
    }
  }
}
