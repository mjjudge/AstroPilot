#include <Arduino.h>

// ── Onboard LED ──────────────────────────────────────────────────────────────
#define LED_PIN 2

// ── Stepper driver pins (STEP/DIR interface) ──────────────────────────────────
// Mapped to AXIS 2 per docs/PINOUTS.md:
//   STEP → GPIO16, DIR → GPIO17, EN → GPIO4
// AXIS 1 uses GPIO18 (STEP), GPIO19 (DIR), GPIO5 (EN) — do not use those here.
#define STEP_PIN    16
#define DIR_PIN     27
#define ENABLE_PIN   4   // Active LOW on most drivers (A4988, DRV8825, TMC22xx)

// ── Test parameters ───────────────────────────────────────────────────────────
// Increase STEP_DELAY_US if the motor stalls or jitters (too fast).
// Decrease for smoother motion once you know the motor can handle it.
static const int STEPS_PER_SWEEP = 1000;   // 200 steps = 1 full rev at full-step
static const int STEP_DELAY_US   = 4000;  // 2 ms per step → ~1 rev/sec at 200 spr

static void pulse_step() {
    digitalWrite(STEP_PIN, HIGH);
    delayMicroseconds(1000);
    digitalWrite(STEP_PIN, LOW);
    delayMicroseconds(STEP_DELAY_US);
}

static void run_sweep(bool forward) {
    digitalWrite(DIR_PIN, forward ? HIGH : LOW);
    delayMicroseconds(5);  // direction setup time
    for (int i = 0; i < STEPS_PER_SWEEP; i++) {
        pulse_step();
    }
}

void setup() {
    Serial.begin(115200);
    Serial.println("\n=== AstroPilot hardware test ===");
    Serial.println("LED blink + stepper STEP/DIR sweep");

    pinMode(LED_PIN, OUTPUT);

    pinMode(STEP_PIN,   OUTPUT);
    pinMode(DIR_PIN,    OUTPUT);
    pinMode(ENABLE_PIN, OUTPUT);

    // Enable the driver (active LOW)
    digitalWrite(ENABLE_PIN, LOW);
    Serial.println("Driver enabled (ENABLE_PIN LOW)");
    delay(100);
}

void loop() {
    // ── LED alive indicator ───────────────────────────────────────────────────
    digitalWrite(LED_PIN, HIGH);

    // ── Forward sweep ─────────────────────────────────────────────────────────
    Serial.printf("Forward  %d steps ...\n", STEPS_PER_SWEEP);
    run_sweep(true);

    delay(500);

    // ── Reverse sweep ─────────────────────────────────────────────────────────
    Serial.printf("Reverse  %d steps ...\n", STEPS_PER_SWEEP);
    run_sweep(false);

    digitalWrite(LED_PIN, LOW);
    delay(500);
}
