# Hardware Test — Setup & Upload Guide

## 1. Install PlatformIO in VS Code

1. Open VS Code
2. Go to Extensions (Ctrl+Shift+X)
3. Search for **PlatformIO IDE** (publisher: PlatformIO)
4. Install it and reload VS Code when prompted

## 2. Open the correct folder

Open **this folder** as your VS Code workspace:

```
AstroPilot/firmware/hardware-test/
```

File → Open Folder → navigate to `firmware/hardware-test` and click Open.

PlatformIO requires the folder containing `platformio.ini` to be the workspace root. Opening the repo root instead may work but opening this folder directly is safest.

## 3. Linux serial port permission (one-time)

If you are on Linux and get a permission error when uploading:

```bash
sudo usermod -aG dialout $USER
```

Log out and back in for it to take effect.

## 4. Configure pin assignments before flashing

Open `src/main.cpp` and update the three pin constants at the top to match your actual wiring:

```cpp
#define STEP_PIN    16   // GPIO connected to driver STEP  (AXIS 2 per PINOUTS.md)
#define DIR_PIN     17   // GPIO connected to driver DIR   (AXIS 2 per PINOUTS.md)
#define ENABLE_PIN   4   // GPIO connected to driver ENABLE (AXIS 2 per PINOUTS.md, active LOW)
```

Pin allocations are defined in `docs/PINOUTS.md`. Do not use GPIO18/19 for ENABLE — they are reserved for AXIS 1 STEP/DIR. Safe additional GPIO choices (avoid boot-strapping issues): 21, 22, 23, 25, 26, 27.

## 5. Build and upload

1. Plug the ESP32 into your PC via USB-C
2. The **PlatformIO toolbar** appears at the bottom of VS Code:
   - **✓ Build** — compiles the firmware
   - **→ Upload** — compiles and flashes to the board
   - **🔌 Monitor** — opens the serial monitor at 115200 baud
3. Click **→ Upload** — PlatformIO detects the port automatically

## 6. What the test does

| Phase | Behaviour |
|---|---|
| Boot | Prints banner to serial, pulls ENABLE_PIN LOW to enable the driver |
| Loop | Blinks onboard LED, sweeps 200 steps forward, pauses 500 ms, sweeps 200 steps back |

Open the serial monitor after uploading — you should see:

```
=== AstroPilot hardware test ===
LED blink + stepper STEP/DIR sweep
Driver enabled (ENABLE_PIN LOW)
Forward  200 steps ...
Reverse  200 steps ...
...
```

## 7. Notes before powering the motor

- **Power the driver from 12 V** — the ESP32 GPIO only carries logic signals. Motor power must come from the 12 V rail, not USB.
- **ENABLE_PIN polarity** — most common drivers (A4988, DRV8825, TMC2208/2209) are active-LOW, which the code assumes. If your driver uses active-HIGH enable, change `LOW` to `HIGH` in `setup()`.
- **Step timing** — `STEP_DELAY_US = 2000` (2 ms/step, ~1 rev/sec at 200 steps/rev) is a safe conservative starting point. Increase if the motor stalls; decrease once confirmed working.

## 8. Identifying the motor (for hardware inventory)

The quickest indicator is wire count:

| Wires | Type |
|---|---|
| 4 | Bipolar stepper |
| 6 | Unipolar/bipolar stepper |
| 5 | Unipolar stepper |
| 2 | DC motor (not a stepper) |

Once identified, update `docs/HARDWARE_INVENTORY.md`.
