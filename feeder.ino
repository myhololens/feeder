#include <Time.h>

#define RTCC_R  0xa3
#define RTCC_W  0xa2

#define MOTOR_B_PWM 5 // Motor B PWM Speed
#define MOTOR_B_DIR 6 // Motor B Direction

#define tim 6000
#define btnPin 2
#define k 20

boolean stpFlag = false;
boolean main_flag = true;
int vlt[k];
int direct = 1;
boolean here_is_your_statement = 1;

void setup() {
  Serial.begin(9600);
  
  pinMode( MOTOR_B_DIR, OUTPUT );
  pinMode( MOTOR_B_PWM, OUTPUT );
  
  digitalWrite( MOTOR_B_DIR, LOW );
  digitalWrite( MOTOR_B_PWM, LOW );
  
  pinMode(btnPin, INPUT);
  
  check_start();
  delay(50);
}

void loop() {
  if (here_is_your_statement) {
    switch(direct) {
      case 1:
      direct = 255;
      break;
      case -1:
      direct = 0;
      break;
    }
    moov(direct);
  }
}

void moov(int dir) {
  while (main_flag) {
    digitalWrite( MOTOR_B_DIR, dir ); // direction = forward
    analogWrite( MOTOR_B_PWM, 255-dir );
    delay(9000);
    stp();
    delay(tim);
  }
}

void check_start() {
   digitalWrite( MOTOR_B_DIR, HIGH ); // direction = forward
   analogWrite( MOTOR_B_PWM, 0 );
   while (!stpFlag) {
     delay(30);
     int sum = 0;
     int M;
     for (int j = 0; j < k-1; j++) {
       vlt[j] = vlt[j+1];
       sum += vlt[j];
     }
     vlt[k-1] = analogRead(btnPin);
     sum += vlt[k-1];
     if (sum == 0 && !stpFlag) {
       stp();
     }
   }
}

void stp() {
  delay(500);
  digitalWrite( MOTOR_B_DIR, LOW );
  digitalWrite( MOTOR_B_PWM, LOW );
  stpFlag = !stpFlag;
}
