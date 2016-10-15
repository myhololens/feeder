#include <Time.h>

#define HG7881_B_IA 5 // D5 --> Motor B Input A --> MOTOR B +
#define HG7881_B_IB 6 // D6 --> Motor B Input B --> MOTOR B -
 
// functional connections
#define MOTOR_B_PWM HG7881_B_IA // Motor B PWM Speed
#define MOTOR_B_DIR HG7881_B_IB // Motor B Direction
 
// the actual values for "fast" and "slow" depend on the motor
#define PWM_SLOW 100  // arbitrary slow speed PWM duty cycle
#define PWM_FAST 255 // arbitrary fast speed PWM duty cycle
#define DIR_DELAY 1000 // brief delay for abrupt motor changes

#define btnPin 2

#define tim 6000

boolean stpFlag = false;
int vlt[12];
int last_time = now();
boolean first_zeros;

void setup() {
  pinMode( MOTOR_B_DIR, OUTPUT );
  pinMode( MOTOR_B_PWM, OUTPUT );
  digitalWrite( MOTOR_B_DIR, LOW );
  digitalWrite( MOTOR_B_PWM, LOW );
  pinMode(btnPin, INPUT);
  moov();
  first_zeros = true;
}

void loop() {
  if (stpFlag) {
    moov();
    stpFlag = !stpFlag;
  }
  else {
    while (!stpFlag) {
      delay(100);
      check();
    }
    first_zeros = false;
    delay(tim);
  }
}

void moov() {
  digitalWrite( MOTOR_B_DIR, HIGH ); // direction = forward
  analogWrite( MOTOR_B_PWM, 255-PWM_FAST );
}

void check() {
   int count;
   int sum = 0;
   int M;
   int now_time;
   for (int j = 0; j < 11; j++) {
    vlt[j] = vlt[j+1];
    sum += vlt[j];
   }
   vlt[11] = analogRead(btnPin);
   sum += vlt[11];
    now_time = now();
    if (now_time <= 10 && last_time >= 50) {
      last_time -= 60;
    }
   if (sum == 0 && !stpFlag && ((now_time - last_time - tim / 1000) % 9 >= 0 && (now_time - last_time - tim / 1000) % 9 <= 1 || first_zeros)) {
    memset(vlt,1,sizeof(vlt));
    stp();
    last_time = now_time;
   }
}

void stp() {
  delay(500);
  digitalWrite( MOTOR_B_DIR, LOW );
  digitalWrite( MOTOR_B_PWM, LOW );
  stpFlag = !stpFlag;
}
