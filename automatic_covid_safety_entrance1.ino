#include <LiquidCrystal.h>
#include <Servo.h>
#define trig1 4
#define echo1 3
#define trig2 6
#define echo2 5

Servo s1;
Servo s2;
const int rs = 12, en = 11, d4 = 13, d5 = 8, d6 = 7, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
long duration;
int distance;
int count; // Variable to store the number of people inside
int limit = 3; // The maximum occupancy 
int sensorPin = 0; //This is the Arduino Pin that will read the sensor output
int sensorInput; //The variable we will use to store the sensor input
double temp; //The variable we will use to store temperature in degrees.

double temperature_scale(){
    sensorInput = analogRead(A0); //read the analog sensor and store it
    temp = (double)sensorInput / 1024; //find percentage of input reading
    temp = temp*5;
    temp = temp - 0.5 ;
    temp = temp * 100; //Convert to degrees
    return temp;
  }

double sensor_distance(int trig,int echo){
  digitalWrite(trig, LOW);
  delayMicroseconds(5);
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);
  duration = pulseIn(echo, HIGH); // Calculate time taken (in microseconds) for the pulse emitted by the trigger pin to reach the echo pin.
  distance = (duration/2) * (331.3/10000); // Calculate the distance from the sensor to the obstacle in cm, using the speed of sound in air(m/s) and the time taken (stored in duration variable)
  return distance;
}

void setup()
{
  pinMode(trig1, OUTPUT); // Configure the trig pin of the Ultrasonic sensor module as OUTPUT as it emits ultrasonic waves (pulse)
  pinMode(echo1, INPUT); // Configure the echo pin of the echo pin as INPUT
  pinMode(trig2, OUTPUT); // Configure the trig pin of the Ultrasonic sensor module as OUTPUT as it emits ultrasonic waves (pulse)
  pinMode(echo2, INPUT); // Configure the echo pin of the echo pin as INPUT
  Serial.begin(9600);
  s1.attach(9); // Servo motor is connected to D9
  s1.write(0); // Set the initial position of servo motor as 0 degrees.
  s2.attach(10);
  s2.write(0);
  lcd.begin(16, 2);// set up the LCD's number of columns and rows:
}

void loop(){
  
  if(temperature_scale()<38){
    delay(10);
    if(sensor_distance(trig1,echo1)<50){
      if(count>limit){ // If value stored in count variable is greater than maximum occupancy
        do{
          lcd.setCursor(0, 0);
          lcd.print("People count = ");
          lcd.print(count);
          lcd.setCursor(0, 1);
          lcd.print("Limit reached");
        }while(sensor_distance(trig2,echo2)>50);
      }
      else{
        count++; // Count will increase by one as a person is entering
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("People count = ");
        lcd.println(count);
        s1.write(90);
        do{
          delay(50);
        }while(sensor_distance(trig1,echo1)<50);
        delay(1000);
        s1.write(0);          
      }  
  	}
  }
  else {
    if (sensor_distance(trig1,echo1)<50) {
      lcd.setCursor(0, 0);
      lcd.print("People count = ");
      lcd.print(count);
      lcd.setCursor(0, 1);
      lcd.print("You can't enter");
      do{
      	delay(50);      	
      }while(sensor_distance(trig1,echo1)<50);
      lcd.clear();
      lcd.print("People count = ");
      lcd.print(count);
    }
  }
  
  if(sensor_distance(trig2,echo2)<50){
    s2.write(90);
    do{
      delay(50);
    }while(sensor_distance(trig2,echo2)<50);
    count--;
    lcd.clear();
    lcd.print("People count = ");
    lcd.print(count);
    delay(1000);
    s2.write(0);
  }      
}