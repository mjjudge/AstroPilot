#include <Arduino.h>

#define LED_PIN     2
#define STEP_PIN   16
#define DIR_PIN    17
#define ENABLE_PIN 4

static const int STEPS_PER_SWEEP = 2000;
static const int STEP_HIGH_US    = 200;   // keep ≥100µs for RC-filtered boards
static const int STEP_START_US   = 5000;  // starting delay (slow, safe to torque up from)
static const int STEP_CRUISE_US  = 2000;  // cruise delay; reduce toward 1000 if motor handles it
static const int RAMP_STEPS      = 400;   // steps to reach cruise speed

void pulse_step(int low_us) {
  digitalWrite(STEP_PIN, HIGH);
  delayMicroseconds(STEP_HIGH_US);
  digitalWrite(STEP_PIN, LOW);
  delayMicroseconds(low_us);
}

void run_sweep(bool forward) {
  digitalWrite(DIR_PIN, forward ? HIGH : LOW);
  Serial.println(forward ? "DIR HIGH" : "DIR LOW");
  delay(500);

  for (int i = 0; i < STEPS_PER_SWEEP; i++) {
    int low_us = (i < RAMP_STEPS)
      ? STEP_START_US - ((STEP_START_US - STEP_CRUISE_US) * i / RAMP_STEPS)
      : STEP_CRUISE_US;
    pulse_step(low_us);
  }
}

void setup() {
  Serial.begin(115200);
  delay(500);

  pinMode(LED_PIN, OUTPUT);
  pinMode(STEP_PIN, OUTPUT);
  pinMode(DIR_PIN, OUTPUT);
  pinMode(ENABLE_PIN, OUTPUT);

  digitalWrite(STEP_PIN, LOW);
  digitalWrite(DIR_PIN, LOW);

  // Enable driver - active LOW
  digitalWrite(ENABLE_PIN, LOW);

  Serial.println("Stepper test started");
}

void loop() {
  digitalWrite(LED_PIN, HIGH);
  run_sweep(true);

  delay(1000);

  digitalWrite(LED_PIN, LOW);
  run_sweep(false);

  delay(1000);
}
