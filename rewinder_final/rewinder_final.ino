// NewPing library is for the ultrasonic sensor
#include <NewPing.h>

#define TRIGGER_PIN  12  // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN     11  // Arduino pin tied to echo pin on the ultrasonic sensor.
#define MAX_DISTANCE 200 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.

int motor = 0; // Arduino pin tied to motor/MOS SIG pin on the ultrasonic sensor.
int feeder = 10;

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); // NewPing setup of pins and maximum distance.

/*===============================================
 Runs once to initialize ports and begin communication
 115200 baud rate was given by NewPing example
 ===============================================*/
void setup() {
  Serial.begin(115200); // Open serial monitor at 115200 baud to see ping results.

  //if the pin frequency was set successfully, turn pin 10 on
    pinMode(motor, OUTPUT);
    analogWrite(motor, HIGH);    
    
    pinMode(feeder, OUTPUT);
    digitalWrite(feeder, HIGH);
}

void loop() {
  // Wait 5ms between pings (about 200 pings/sec).
  delay(50);
  // Send ping, get ping time in microseconds (uS).
  unsigned int distance = sonar.ping(); 

  // Print output, can be viewed with Serial Monitor (default Ctrl + Shift + M to open it)
  Serial.print("Ping: ");
  // Convert ping time to distance in cm and print result (0 = outside set distance range)
  Serial.print(distance);// / US_ROUNDTRIP_CM); 
  Serial.println("cm");

  // US_ROUNDTRIP_CM = 57 in config
  // Approximation to shut off motor once within about 5 cm
  if(distance < 150){ 
    analogWrite(motor, 0);
  }
  // Values need to be tweaked - dependent on frequency of PWM
  else if(distance >= 150 && distance < 250)
    analogWrite(motor, 150);
  else if(distance >= 250 && distance < 350)
    analogWrite(motor, 175);
  else if(distance >= 350 && distance < 500)
    analogWrite(motor, 200);
  else if(distance >= 500 && distance < 700)
    analogWrite(motor, 240);
  else if(distance >= 700 && distance < 1000)
    analogWrite(motor, 255);
  else 
    analogWrite(motor, 255);
}

