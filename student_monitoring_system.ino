#include <Wire.h>
#include <U8g2lib.h>

// === OLED SETUP ===
U8G2_SH1106_128X64_NONAME_F_HW_I2C u8g2(U8G2_R0, /* reset=*/ U8X8_PIN_NONE);

// === Ultrasonic Sensor Pins ===
#define TRIG_ENTRY 2
#define ECHO_ENTRY 3
#define TRIG_EXIT 4
#define ECHO_EXIT 5

int count = 0;
long duration;
int distance;
const int threshold = 10;  // Distance in cm to detect person
bool entryDetected = false;
bool exitDetected = false;

void setup() {
  pinMode(TRIG_ENTRY, OUTPUT);
  pinMode(ECHO_ENTRY, INPUT);
  pinMode(TRIG_EXIT, OUTPUT);
  pinMode(ECHO_EXIT, INPUT);

  u8g2.begin();
  u8g2.setFont(u8g2_font_ncenB14_tr);

  displayCount();
}

// === Function to measure distance ===
int getDistance(int trigPin, int echoPin) {
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);

  long duration = pulseIn(echoPin, HIGH);
  int distance = duration * 0.034 / 2;  // Convert to cm
  return distance;
}

// === Function to display on OLED ===
void displayCount() {
  u8g2.clearBuffer();
  u8g2.setCursor(10, 30);
  u8g2.print("Students:");
  u8g2.setCursor(40, 60);
  u8g2.print(count);
  u8g2.sendBuffer();
}

void loop() {
  int entryDist = getDistance(TRIG_ENTRY, ECHO_ENTRY);
  int exitDist = getDistance(TRIG_EXIT, ECHO_EXIT);

  // === Entry Detection ===
  if (entryDist < threshold && !entryDetected) {
    count++;
    entryDetected = true;
    displayCount();
  }
  if (entryDist > threshold && entryDetected) {
    entryDetected = false;
  }

  // === Exit Detection ===
  if (exitDist < threshold && !exitDetected) {
    if (count > 0) count--;
    exitDetected = true;
    displayCount();
  }
  if (exitDist > threshold && exitDetected) {
    exitDetected = false;
  }

  delay(200); // Small delay for stability
}
