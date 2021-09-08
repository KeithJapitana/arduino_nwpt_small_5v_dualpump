#define SENSOR A2 //Pressure

int enA = 5;
int in1 = 4;
int in2 = 6;
int sensorValue = 0;
int pressure = 0;

void setup() {
  Serial.begin(9600);
  pinMode(enA, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  digitalWrite(in1, HIGH);
  digitalWrite(in2, LOW);   
  analogWrite(enA, 255);
}

void loop() {

 long x = 0;
 for (int i = 0; i < 100; i++){
    sensorValue = analogRead(SENSOR);
    x = x + abs(sensorValue); 
    delay(2);
 }
 x = x/100;
 pressure = map (x, 451, 368, 70, 120); //(x, Reading of Low Value, Reading of High Value, Low Value, High Value)
 //pressure = map (x, 0, 3100, 0, 300);
 Serial.println(pressure); 
 Serial.println(x);

}
