#include <AFMotor.h>

AF_DCMotor motorL(2);
AF_DCMotor motorR(4);

int g_nInitSpeed = 150;
int g_nTRval = 0;
int g_nTLval = 0;
int g_nThrs = 100;
int g_nSensitive = 1000; // Less is more
int g_nowStat = 0; // 0=stop, 1=foward, 2=backward, 3=left, 4=right
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

#define GoFWD \
    motorR.run(FORWARD); \
    motorL.run(FORWARD); \
    g_nowStat = 1;

#define GoBWD \
    motorR.run(BACKWARD); \
    motorL.run(BACKWARD);
    //g_nowStat = 2;
    
#define GoLT \
    motorR.run(RELEASE); \
    motorL.run(BACKWARD); \
    g_nowStat = 3;
    
#define GoRT \
    motorR.run(BACKWARD); \
    motorL.run(RELEASE); \
    g_nowStat = 4;

boolean ChkStat() {
  g_nTRval = analogRead(pin_TR);
  g_nTLval = analogRead(pin_TL);
  //Serial.println(g_nTLval);
  //delay(1000);
  
  g_TLastStat = g_TStat;
  g_TStat = 0;
  g_TStat |= ((g_nTLval > g_nThrs) << 1);
  g_TStat |= (g_nTRval > g_nThrs);
  //Serial.print(g_TLastStat);
  //Serial.print("->");
  //Serial.println(g_TStat);
  //delay(1000);
  
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
  if(!ChkStat()) {
    return;
  }
  
  //Serial.print("In loop: ");
  //Serial.println(g_TStat);
  //delay(1000);
  switch(g_TStat) {
  case 0:
    GoFWD;
    break;
  case 1:
    GoBWD;
    delay(500);
    GoLT;
    delay(500);
    break;
  case 2:
    GoBWD;
    delay(500);
    GoRT;
    delay(500);
    break;
  case 3:
    GoBWD;
    delay(500);
    break;
  }
}
