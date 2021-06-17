// Include the library:
#include <NewPing.h>

// Define Trig and Echo pin:
#define trigPin D6
#define echoPin D7

// Define maximum distance we want to ping for (in centimeters). Maximum sensor distance is rated at 400-500 cm:
#define MAX_DISTANCE 300

// NewPing setup of pins and maximum distance.
NewPing sonar = NewPing(trigPin, echoPin, MAX_DISTANCE);

void JSNSR04Tstart(){
  // Wait 50ms between pings (about 20 pings/sec). 29ms should be the shortest delay between pings:
  delay(50);
}

// Calculate the volume using the formula V=h*pi*r2
int calculateVolume(){
  int distance = sonar.ping_cm();
  Serial.print("distance: "); Serial.print(String(distance).c_str());Serial.println("cm");
  int measured_height = collector_inner_height - distance;
  int powered_radius = pow(collector_inner_radius,2);
  int volume = (PI * powered_radius * measured_height) * 0.001;
  return volume;
}
