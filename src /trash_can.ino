#include <ESP32Servo.h>

// Servo motor parameters
Servo servo;
const int servoPin = 27;
const int openAngle = 90;
const int closeAngle = 0;

// Minimum and maximum pulse widths for the servo
const int minPulseWidth = 500; // 0.5 ms
const int maxPulseWidth = 2500; // 2.5 ms


// Ultrasonic sensor parameters
const int trigPin = 26;
const int echoPin = 25;
long distance, averageDistance;
long averDist[3];

// Distance threshold in centimeters
const int distanceThreshold = 20;

void setup() {
  Serial.begin(9600);
  pinMode(trigPin, OUTPUT);
  pinMode(echoPin, INPUT);
  servo.attach(servoPin);
  servo.write(closeAngle);
  delay(100);
}

void loop() {
    // Measure distance three times
    for (int i = 0; i <= 2; i++) {
        distance = readDistance();
        averDist[i] = distance;
        delay(10);
    }

    // Calculate the average distance
    averageDistance = (averDist[0] + averDist[1] + averDist[2]) / 3;
    Serial.println(averageDistance);

    // Control the servo based on the averaged distance
    if (averageDistance <= distanceThreshold) {
        servo.attach(servoPin);  // Reattach the servo before sending a command
        delay(1);
        servo.write(openAngle);  // Rotate the servo to the open position
        delay(3500);
    } else {
        servo.write(closeAngle);  // Rotate the servo back to the closed position
        delay(1000);
        servo.detach();  // Detach the servo to save power when not in use
    }
}

float readDistance() {
    // Send a pulse on the trigger pin of the ultrasonic sensor
    digitalWrite(trigPin, LOW);
    delayMicroseconds(2);
    digitalWrite(trigPin, HIGH);
    delayMicroseconds(10);
    digitalWrite(trigPin, LOW);

    // Measure the pulse width of the echo pin and calculate the distance value
    float distance = pulseIn(echoPin, HIGH) / 58.00;  // Formula: (340m/s * 1us) / 2
    return distance;
}

// Function to make the servo work
void servoWrite(int angle){
    int pulseWidth = map(angle, 0, 180, minPulseWidth, maxPulseWidth);
    servo.writeMicroseconds(pulseWidth);
}
