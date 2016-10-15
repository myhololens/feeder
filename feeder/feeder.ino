#include <Ethernet.h>
#define MOTOR_B_PWM 5 // Motor B PWM Speed
#define MOTOR_B_DIR 6 // Motor B Direction
#define CNTR_CLCKWISE  255
#define CLCKWISE 0
#define FULLCIRCLE_TIME 9000

#define FEED_TIMEOUT 6000
#define DRIVE_SENSOR_PIN 2
#define DRIVE_SENSOR_RANGE 20

boolean reading = false;

// Setup the static network info for you arduino
byte ip[] = { 192, 168, 1, 20 };
byte subnet[] = { 255, 255, 255, 0 };
byte gateway[] = { 192, 168, 1, 1 };
byte mac[] = { 0xDE, 0xAD, 0xBE, 0xEF, 0xFE, 0xED };

EthernetServer server = EthernetServer(80); // Port 80
String HTTPget = "";

boolean stop_flag = false;
boolean main_flag = true;

int vlt[DRIVE_SENSOR_RANGE];

void setup() {
  pinMode( MOTOR_B_DIR, OUTPUT );
  pinMode( MOTOR_B_PWM, OUTPUT );
  
  digitalWrite( MOTOR_B_DIR, LOW );
  digitalWrite( MOTOR_B_PWM, LOW );
  
  pinMode(DRIVE_SENSOR_PIN, INPUT);
  
  Ethernet.begin(mac, ip, gateway, subnet);

  //check_start();
}

void loop() {
  // listen for incoming clients
  EthernetClient client = server.available();
  if (client)
  {
    client.println(F("HTTP/1.1 200 OK\nContent-Type: text/html\n"));
    processClient(client);
  }
}


void move_drive(byte direction) {
  //while (main_flag) {
    digitalWrite( MOTOR_B_DIR, direction );      // directionection = forward
    analogWrite( MOTOR_B_PWM, 255 - direction ); // drive's speed
    delay(FULLCIRCLE_TIME);
    stop_drive();
    //delay(FEED_TIMEOUT);
  //}
}

// find the drive unit's sensor placement
void check_start() {
   digitalWrite( MOTOR_B_DIR, HIGH ); // direction = forward
   analogWrite( MOTOR_B_PWM, 0 );
   
   // find zero data when drive's sensor isn't active
   while (!stop_flag) {
     delay(30);
     int sum = 0;
     int M;
     for (int j = 0; j < DRIVE_SENSOR_RANGE-1; j++) {
       vlt[j] = vlt[j+1];
       sum += vlt[j];
     }
     vlt[DRIVE_SENSOR_RANGE-1] = analogRead(DRIVE_SENSOR_PIN);
     sum += vlt[DRIVE_SENSOR_RANGE-1];
     if (sum == 0 && !stop_flag) {
       stop_drive();
     }
   }
}

// stop drive unit
void stop_drive() {
  delay(500);
  digitalWrite( MOTOR_B_DIR, LOW );
  digitalWrite( MOTOR_B_PWM, LOW );
  //stop_flag = !stop_flag;
}

void processClient(EthernetClient client)
{
  // http request will end with a blank line
  boolean lineIsBlank = true;

  while (client.connected())
  {
    if (client.available())
    {
      char c = client.read();

      if(reading && c == ' ') reading = false;
      if(c == '?') reading = true;
      if(reading && c != '?') HTTPget += c;
      if (c == '\n' && lineIsBlank)  break;
      if (c == '\n') lineIsBlank = true;
      else if (c != '\r') lineIsBlank = false;
    }
  }
  if ( HTTPget == "left" ){
     // drive unit should move the feeder left
     move_drive(CNTR_CLCKWISE);
     client.print(F("moved left"));
  }
  else if ( HTTPget == "right"){
     // drive unit should move the feeder left
     move_drive(CLCKWISE);
     client.print(F("moved right"));
  }
  else client.print(HTTPget);

  delay(1);      // give the web browser a moment to recieve
  client.stop(); // close connection
  HTTPget = "";  // clear out the get param we saved
}
