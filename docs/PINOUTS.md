# Pinouts

# ESP32 DevKit (ESP-WROOM-32) Pinout

**Orientation (TOP VIEW):**

* USB connector: **Top**
* Antenna: **Bottom**

---

## Pin Layout (Top View)

| Left Side (Top → Bottom) | Right Side (Top → Bottom) |
| ------------------------ | ------------------------- |
| 3V3                      | GND                       |
| EN                       | GPIO23 (MOSI)             |
| GPIO36 (VP, input only)  | GPIO22 (SCL)              |
| GPIO39 (VN, input only)  | GPIO1 (TX)                |
| GPIO34 (input only)      | GPIO3 (RX)                |
| GPIO35 (input only)      | GPIO21 (SDA)              |
| GPIO32                   | GND                       |
| GPIO33                   | GPIO19 (MISO)             |
| GPIO25 (DAC1)            | GPIO18 (SCK)              |
| GPIO26 (DAC2)            | GPIO5                     |
| GPIO27                   | GPIO17                    |
| GPIO14                   | GPIO16                    |
| GPIO12 ⚠️                | GPIO4                     |
| GND                      | GPIO0 ⚠️                  |
| GPIO13                   | GPIO2 ⚠️                  |
| GPIO9 ⚠️ (SD2)           | GPIO15 ⚠️                 |
| GPIO10 ⚠️ (SD3)          | GPIO8 ⚠️ (SD1)            |
| GPIO11 ⚠️ (CMD)          | GPIO7 ⚠️ (SD0)            |
| 5V                       | GPIO6 ⚠️ (CLK)            |

---

## Notes & Constraints

### Input-only pins

* `GPIO34`, `GPIO35`, `GPIO36`, `GPIO39`
* No output, no pull-ups

---

### Flash-connected pins (DO NOT USE)

* `GPIO6`, `GPIO7`, `GPIO8`, `GPIO9`, `GPIO10`, `GPIO11`

These are internally connected to the SPI flash.

---

### Boot strapping pins (use with caution)

* `GPIO0` → boot mode (LOW = flash mode)
* `GPIO2` → must be LOW/FLOAT during boot
* `GPIO12` → affects flash voltage
* `GPIO15` → boot config

Avoid pulling these to the wrong level at startup.

---

### Default interfaces

**UART (programming / logs)**

* TX → `GPIO1`
* RX → `GPIO3`

**I²C (default)**

* SDA → `GPIO21`
* SCL → `GPIO22`

**SPI (default VSPI)**

* MOSI → `GPIO23`
* MISO → `GPIO19`
* SCK  → `GPIO18`
* CS   → `GPIO5`

---

## Recommended GPIOs (General Use)

Safe, flexible pins for most applications:

```
GPIO4, GPIO5, GPIO16, GPIO17, GPIO18, GPIO19,
GPIO21, GPIO22, GPIO23, GPIO25, GPIO26, GPIO27,
GPIO32, GPIO33
```

---

## AstroPilot Notes (Stepper / Control Use)

Suggested allocation strategy:

```
STEPPER CONTROL:
  STEP → GPIO18 / GPIO19
  DIR  → GPIO16 / GPIO17
  ENABLE → GPIO5

I²C (encoders, sensors):
  SDA → GPIO21
  SCL → GPIO22

UART (debug / GPS):
  GPIO1 / GPIO3 (or remap)
```

---

# TMC2209 Stepper Driver (StepStick Format)

**Orientation (TOP VIEW):**

* Potentiometer (current adjust) = **Top Right**
* “EN” marking near **top edge**
* “DIR / GND” marking near **bottom edge**

---

## Pin Layout (Top View)

| Left Side (Top → Bottom) | Right Side (Top → Bottom)  |
| ------------------------ | -------------------------- |
| EN (Enable, LOW = ON)    | VM (Motor Power, e.g. 12V) |
| MS1 (PDN / UART)         | GND (Motor Ground)         |
| MS2                      | A1 (Motor Coil A)          |
| MS3                      | A2 (Motor Coil A)          |
| RST (Reset)              | B1 (Motor Coil B)          |
| SLP (Sleep)              | B2 (Motor Coil B)          |
| STEP (Step Pulse)        | GND (Logic Ground)         |
| DIR (Direction)          | VIO (Logic Voltage)        |

---

## Notes & Constraints

### Power

* `VM` → Motor supply (typically 9–24V, e.g. 12V)
* `VIO` → Logic level (3.3V for ESP32 ✔)
* **All grounds must be common**

---

### Control Pins

* `STEP` → Pulse input (each pulse = one microstep)
* `DIR` → Direction control
* `EN` → Enable (LOW = enabled, HIGH = disabled)

---

### Microstepping / UART

* `MS1` → Often used as **PDN_UART** (single-wire UART)
* `MS2`, `MS3` → Microstepping configuration
* Many boards default to **stealthChop + UART-capable mode**

---

### Motor Connections

* `A1 / A2` → Coil A
* `B1 / B2` → Coil B

(If motor vibrates but doesn’t turn → swap one coil pair)

---

## Recommended Minimal Wiring (ESP32)

```id="1kz6yu"
VIO  → 3.3V
GND  → GND (shared with ESP32)
VM   → 12V motor supply

STEP → GPIO (e.g. GPIO18)
DIR  → GPIO (e.g. GPIO19)
EN   → optional (tie LOW or control via GPIO)
```

---

## Recommended GPIO Mapping (AstroPilot)

```id="rm9s0z"
AXIS 1:
  STEP → GPIO18
  DIR  → GPIO19
  EN   → GPIO5 (optional)

AXIS 2:
  STEP → GPIO16
  DIR  → GPIO17
  EN   → GPIO4 (optional)
```

---

## Safety Notes

```id="7njtqk"
- NEVER connect VM without GND
- Do NOT hot-plug the motor
- Set current limit via potentiometer BEFORE use
- Ensure common ground between ESP32 and driver
```

---

## Quick Test Procedure

```id="d3o2pu"
1. Power VIO (3.3V) and GND
2. Power VM (e.g. 12V)
3. Connect STEP to a GPIO pulse source
4. Toggle DIR and confirm direction change
```

---