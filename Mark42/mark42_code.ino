#include <Wire.h>
#include <Adafruit_PWMServoDriver.h>

Adafruit_PWMServoDriver pwm = Adafruit_PWMServoDriver();

// Adjust for global change in small sg90 servo movement //
#define SERVOMIN 150
#define SERVOMAX 450
/////////////////////////////////////////////////////
#define SERVO_FREQ 50

uint8_t servonum = 0;
int buttonPin = 2;
int ledPin = 6;
int buttonState = 0;
int globalPos = 1;
unsigned long previousMillis = 0;
unsigned long interval = 5000;
int a = 60;

int animDelay = 1;
int mainServoL = 0;
int mainServoR = 1;
int servoBrowCenter = 4;

// Adjust below numbers for individual servos
int mainServoClosed = 750;
int mainServoOpen = 1950;

void setup() {
  Serial.begin(9600);
  pwm.begin();
  pwm.setOscillatorFrequency(27000000);
  pwm.setPWMFreq(SERVO_FREQ);
  pinMode(buttonPin, INPUT_PULLUP);
  delay(50);
  pwm.sleep();
}

int getAngleToPulse(int angle) {
  return map(angle, 0, 180, SERVOMIN, SERVOMAX);
}

void loop() 
{
  buttonState = digitalRead(buttonPin);
  if (buttonState == 0) {
    Serial.println("Wake up");
    pwm.wakeup();

    if (globalPos > 0) 
    {
      // EYES
      analogWrite(ledPin, 0); 
      // MAIN SERVOS
      for (uint16_t microsec = mainServoClosed; microsec < mainServoOpen; microsec +=5) {
        pwm.writeMicroseconds(mainServoL, microsec);
        pwm.writeMicroseconds(mainServoR, (mainServoOpen+mainServoClosed-microsec));
      }
      globalPos = 0;
      delay(animDelay);
      Serial.println("Open");
    } 
    else 
    {
      //MAIN SERVOS
      for (uint16_t microsec = mainServoOpen; microsec > mainServoClosed; microsec-=5) {
        pwm.writeMicroseconds(mainServoL, microsec);
        pwm.writeMicroseconds(mainServoR, (mainServoOpen+mainServoClosed-microsec));
      }
      delay(animDelay);
      
      //Blinking eyes
      for(int i=0; i<4; i++)
      {
        analogWrite(ledPin, 255) ;
        delay(80) ;
        analogWrite(ledPin, 0) ;
        delay(80) ;
      }
      delay(200) ;
      analogWrite(ledPin, 255) ;
      globalPos = 1;
      delay(100);
      Serial.println("Sleep");
      pwm.sleep();
    }
    delay(500);
  }
  delay(10);
}
