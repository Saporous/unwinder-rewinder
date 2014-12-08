//#include <ATimerDefs.h>
//#include <BTimerDefs.h>
#include <PWM.h>

// NewPing library is for the ultrasonic sensor
#include <NewPing.h>
// PWM library gives functions to enable the internal timers


#define TRIGGER_PIN  12  // Arduino pin tied to trigger pin on the ultrasonic sensor.
#define ECHO_PIN     11  // Arduino pin tied to echo pin on the ultrasonic sensor.
#define MAX_DISTANCE 200 // Maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500cm.

int motor = 10; // Arduino pin tied to motor/MOS SIG pin on the ultrasonic sensor.
int frequency = 20000; // A frequency for the PWM that works with the motor - needs tuning

NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); // NewPing setup of pins and maximum distance.

/*===============================================
 Runs once to initialize ports and begin communication
 115200 baud rate was given by NewPing example
 ===============================================*/
void setup() {
  Serial.begin(115200); // Open serial monitor at 115200 baud to see ping results.

  //initialize all timers except for 0, to save time keeping functions
  InitTimersSafe(); 

  //sets the frequency for the specified pin
  bool success = SetPinFrequencySafe(motor, frequency);

  //if the pin frequency was set successfully, turn pin 10 on
  if(success) {
    pinMode(motor, OUTPUT);
    digitalWrite(motor, HIGH);    
  }
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
  if(distance < 300){ 
    pwmWrite(motor, 0);
  }
  // Values need to be tweaked - dependent on frequency of PWM
  else if(distance >= 300 && distance < 400)
    pwmWrite(motor, 235);
  else if(distance >= 400 && distance < 500)
    pwmWrite(motor, 240);
  else if(distance >= 500 && distance < 700)
    pwmWrite(motor, 245);
  else if(distance >= 700 && distance < 1000)
    pwmWrite(motor, 255);
  else 
    pwmWrite(motor, 0);
}

