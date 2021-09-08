#define SENSOR A1 //Pressure
#define PUMP 5


int enA = 5;
int in1 = 4;
int in2 = 6;
const int NM = 11;
const int M1 = 10;
const int M2 = 9;
const int M3 = 8;
const int M4 = 7;
int interval = 1;
int PWM2 = 100;
int lowLimit = 69;
int highLimit = 71;

long longInterval = 300000;
long longDuration = 120000;
//long longInterval = 30000;
//long longDuration = 15000;
long millisIntermittentOn, millisIntermittentOff;
boolean intPump = false;
boolean pumpOn = false;
boolean instillOn = false;
boolean intermittentOn = false;
boolean setPres = true;
boolean leakage = false;


#include <Wire.h> 
#include <LiquidCrystal_I2C.h> 
//LiquidCrystal lcd(2, 3, 4, 5, 6, 7); //To lcd (RS,E,D4,D5,D6,D7)
LiquidCrystal_I2C lcd(0x27, 2, 1, 0, 4, 5, 6, 7, 3, POSITIVE);

//Counter to change positions of pages

int mod=1 ;       //To move beetwen pages
int prevMode;

//-------Pins-----//
int up = 9;               //Up button
int down = 10;           //Down button
//---------Storage debounce function-----//
boolean current_up = LOW;          
boolean last_up=LOW;            
boolean last_down = LOW;
boolean current_down = LOW;

int sensorValue = 0;
int pressure = 0;


void setup() {
  lcd.begin(16,2); 
  lcd.backlight();
  Serial.begin(9600);
  pinMode(PUMP, OUTPUT);
  pinMode(enA, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(NM, INPUT);
  pinMode(M1, INPUT);
  pinMode(M2, INPUT);
  pinMode(M3, INPUT);
  pinMode(M4, INPUT);
  
}


   //---- De-bouncing function for all buttons----//
boolean debounce(boolean last, int pin)
{
boolean current = digitalRead(pin);
if (last != current)
{
delay(5);
current = digitalRead(pin);
}
return current;
}


void loop() {
   readPressure();
   if (mod == 2 || mod == 4){
    lowLimit = 69;
    highLimit = 71;
    }else{
      lowLimit = 119;
      highLimit = 121;
   }



//  current_up = debounce(last_up, up);         //Debounce for Up button
//  current_down = debounce(last_down, down);   //Debounce for Down button
//
//  //----Page counter function to move pages----//
//
//  //Page Up
//  if (last_up== LOW && current_up == HIGH){  //When up button is pressed
//      lcd.clear();                     //When page is changed, lcd clear to print new page  
//      if(mod <4){              //Page counter never higher than 3(total of pages)
//      mod= mod +1;   //Page up
//      
//      }
//      else{
//      mod = 1;  
//      }
//  }
//  
//    last_up = current_up;
//
//  //Page Down
//    if (last_down== LOW && current_down == HIGH){ //When down button is pressed
//      lcd.clear();                     //When page is changed, lcd clear to print new page    
//      if(mod >1){              //Page counter never lower than 1 (total of pages)
//      mod= mod -1;   //Page down
//      
//      }
//      else{
//      mod = 4;  
//      }
//  }
//    
//    last_down = current_down;

 if(digitalRead(NM)==1){
       lcd.clear(); 
       if(mod <5);
       mod=1;
       Serial.println("NOMODE");
     } 

    else if(digitalRead(M1)==1){
      lcd.clear(); 
      if(mod <5);
       mod=2;
       Serial.println("MOD1");
       
     }

    else if(digitalRead(M2)==1){
      lcd.clear(); 
      if(mod <5);
       mod=3;
       Serial.println("MOD2");
       
     }
    else if(digitalRead(M3)==1){
      lcd.clear(); 
      if(mod <5);
       mod=4;
       Serial.println("MOD3");
       
     }
    else if(digitalRead(M4)==1){
      lcd.clear(); 
      if(mod <5);
       mod=5;
       Serial.println("MOD4");
       
     }
     

    
     //------- Switch function to write and show what you want---// 
  if (leakage == false){   
  lcd.clear();
  switch (mod) {

    case 1:{     
      lcd.setCursor(0,0);
      lcd.print(pressure);
      lcd.setCursor(4,0);                      
      lcd.print("mmHg");
      lcd.setCursor(0,1);
      lcd.print("Pump is OFF");
      }
    break;
   
    case 2:{     //Design of home page 1
      lcd.setCursor(0,0);
      lcd.print(pressure);
      lcd.setCursor(4,0);
      lcd.print("mmHg");
      lcd.setCursor(0,1);
      lcd.print("Continuous LOW"); 
      
    }
    break;
    
    case 3:{     
      lcd.setCursor(0,0);
      lcd.print(pressure);
      lcd.setCursor(4,0);
      lcd.print("mmHg");
      lcd.setCursor(0,1);
      lcd.print("Continuous HIGH");
      }
    break;  
      
    case 4:{     
      lcd.setCursor(0,0);
      lcd.print(pressure);
      lcd.setCursor(4,0);
      lcd.print("mmHg");
      lcd.setCursor(0,1);
      lcd.print("Int LOW");
      }
    break;

    case 5:{     
      lcd.setCursor(0,0);
      lcd.print(pressure);
      lcd.setCursor(4,0);
      lcd.print("mmHg");
      lcd.setCursor(0,1);
      lcd.print("Int HIGH");
      }
    break;
  }
  }
     processMode();
}//loop end
void writeLCD(){
   

}

void readPressure(){
 long x = 0;
 for (int i = 0; i < 100; i++){
    sensorValue = analogRead(SENSOR);
    x = x + abs(sensorValue); 
    delay(2);
 }
 x = x/100;
 //pressure = map (x, 451, 368, 70, 120);//3.3v //(x, Reading of Low Value, Reading of High Value, Low Value, High Value)
 pressure = map (x, 630, 140, 0, 180);//5V sensor
//pressure = map (x, 0, 3100, 0, 300); //3.3v Sensor
// Serial.println(pressure); 
// Serial.println(x);
 if (pressure < 0){
  pressure = 0;
  }
}

void setPressure(){
     if(pressure > highLimit){
    PWM2 = PWM2 - interval;
    analogWrite(enA,PWM2);
    readPressure();
    leakage = false;
    writeLCD();
    }
   else if(pressure < lowLimit){
    if (PWM2 > 250){
      PWM2 = 255;
      readPressure();
      lcd.noBacklight();
      delay (500);
      lcd.backlight();
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print(pressure);
      lcd.setCursor(4,0);
      lcd.print("mmHg");
      lcd.setCursor(0,1);
      lcd.print("TOO MUCH LEAKAGE");
      leakage = true;
      }
      else{
      PWM2 = PWM2 + interval;
      analogWrite(enA,PWM2);
      readPressure();
      writeLCD();
      leakage = false;
      }

    }
   else{
    
    }
    
    }

void pressure_control(){
  
  digitalWrite(in1, LOW);
  digitalWrite(in2, HIGH);

  //Accelarate
  for (int i = 0; i < 255; i++) {
    analogWrite(enA, i);
    delay(2);
  } 
}


void processMode(){

   if (mod == 1){
    //setPressure();
    turnOff(); 
  }else if (mod == 3){
    setPressure();
    turnOn();
    intermittentOn = false;
  }else if (mod == 2){
    setPressure();
    turnOn(); 
    intermittentOn = false;
  }else if(mod==5){
    if(!intermittentOn){
      turnOn();
      millisIntermittentOn = millis();
      intermittentOn = true;
      delay(1000);
      setPressure();
      setPres = true;
      intPump = true;
    }else{
      if(((millis() - millisIntermittentOn) > longInterval) && intermittentOn == true && intPump == true){
        turnOff();
        millisIntermittentOff = millis();
        setPres = false;
        intPump = false;
      }else if(((millis() - millisIntermittentOff) > longDuration) && intPump == false){
        turnOn();
        delay(1000);
        intermittentOn = false;
        millisIntermittentOn = millis();
        setPres = true;
        intPump = true;
      }
    }
    if(setPres == true){
        setPressure();
      }else{
        readPressure();
      }  
  }else if(mod==4){    
    if(!intermittentOn){
      turnOn();
      millisIntermittentOn = millis();
      intermittentOn = true;
      setPressure ();
      setPres = true;
      intPump = true;
    }else{
      if(((millis() - millisIntermittentOn) > longInterval) && intermittentOn == true && intPump == true){
      turnOff();
        millisIntermittentOff = millis();
        setPres = false;
        intPump = false;
      }else if(((millis() - millisIntermittentOff) > longDuration) && intPump == false){
      turnOn();
        delay(1000);
        intermittentOn = false;
        millisIntermittentOn = millis();
        setPres = true;
        intPump = true;
      }
    }
    if(setPres == true){
        setPressure ();
      }else{
        readPressure();
      }
  }
}

void turnOn(){
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);
  }

void turnOff(){
  digitalWrite(in1, LOW);
  digitalWrite(in2, LOW);
  }
