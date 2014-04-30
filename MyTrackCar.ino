#include <AFMotor.h>

AF_DCMotor motorL(2);
AF_DCMotor motorR(4);

int g_nInitSpeed = 150;
int g_nTRval = 0;
int g_nTLval = 0;
int g_nThrs = 100;
int g_nSensitive = 1000; // Less is more
volatile int g_TStat = 1;
volatile int g_TLastStat = 0;

int pin_TR = 10;
int pin_TL = 9;

#define SETSPEED(n) \
  motorL.setSpeed(n); \
  motorR.setSpeed(n);

void setup() {
  Serial.begin(9600);           // set up Serial library at 9600 bps
  Serial.println("Motor test!");

  // turn on motor
  SETSPEED(g_nInitSpeed);
  motorL.run(RELEASE);
  motorR.run(RELEASE);
}

#define GoFWD(t) \
    motorR.run(FORWARD); \
    motorL.run(FORWARD); \
    delay(t);

#define GoBWD(t) \
    motorR.run(BACKWARD); \
    motorL.run(BACKWARD); \
    delay(t);
    
#define GoLT(t) \
    motorR.run(RELEASE); \
    motorL.run(BACKWARD); \
    delay(t);
    
#define GoRT(t) \
    motorR.run(BACKWARD); \
    motorL.run(RELEASE); \
    delay(t);

boolean ChkStat() {
  g_nTRval = analogRead(pin_TR);
  g_nTLval = analogRead(pin_TL);
  
  g_TLastStat = g_TStat;

  // Set the flag if black
  g_TStat = 0;
  g_TStat |= ((g_nTLval > g_nThrs) << 1);
  g_TStat |= (g_nTRval > g_nThrs);
  
  if(g_TLastStat == g_TStat) {
    return false;
  }
  return true;
}

/*
void cdelay(int nTime) {
  int i = nTime;
  while(i > 0) {
    delay(g_nSensitive);
    i -= g_nSensitive;
    if(ChkStat()) {
      return;
    }
  }
}
*/

void loop() {
  ChkStat();
  
  //Serial.println(g_TStat);
  //delay(1000);

L_ACT:
  switch(g_TStat) {
  case 0: // Error, all white
    g_TStat = g_TLastStat;
    goto L_ACT;
    break;
  case 1: // If left side out of black 
    GoBWD(500);
    GoRT(500);
    break;
  case 2: // If right side out of black 
    GoBWD(500);
    GoLT(500);
    break;
  case 3: // If all black
    GoFWD(500);
    break;
  }
}
