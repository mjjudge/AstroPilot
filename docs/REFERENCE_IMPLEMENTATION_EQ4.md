# Reference Implementation: EQ4

## 1. Purpose

This document defines the first reference implementation of the AstroPilot platform using a Sky-Watcher EQ4 equatorial mount.

It captures all mount-specific assumptions, constraints, and implementation details required to bring AstroPilot to a functional state on this hardware.

---

## 2. Overview

### 2.1 Target Hardware

- Mount: Sky-Watcher EQ4 equatorial mount
- Axes:
  - Right Ascension (RA)
  - Declination (DEC)
- Drive system:
  - Worm gear on both axes
- Existing upgrade:
  - Dual-axis motor kit (installed)

### 2.2 Implementation Role

The EQ4 serves as:

- The **first validation target** for AstroPilot
- A **baseline for motion control and GoTo capability**
- A **testbed for future platform features** (plate solving, automation)

---

## 3. Existing Components

### 3.1 Mount Mechanics

- Equatorial mount aligned to celestial pole
- RA axis provides continuous tracking
- DEC axis provides positional offset
- Manual clutches present on both axes

### 3.2 Motors

Assumed:

- One motor on RA
- One motor on DEC
- Connected via RJ-style connectors to legacy controller
- Likely stepper or stepper-like motors

**Status:** To be confirmed during Phase 0

### 3.3 Legacy Controller

Capabilities:

- Manual directional control (N/S/E/W)
- Variable speed selection
- Dual-axis control

Limitations:

- No position awareness
- No GoTo capability
- Unknown internal motor driving method

**Decision:**
- Legacy controller will be **fully replaced or bypassed**

---

## 4. Assumptions and Unknowns

### 4.1 To Be Confirmed

- Motor type (stepper / unipolar / bipolar / DC geared)
- Number of wires per motor
- Motor voltage and current ratings
- Step angle or effective gearing
- Connector pinout
- Whether existing spare stepper controller supports STEP/DIR

### 4.2 Risks

- Motors may not be compatible with modern drivers
- Gear backlash may affect pointing accuracy
- Motor torque may be insufficient for fast slewing
- Electrical noise from motors may affect logic systems

---

## 5. Power Implementation

### 5.1 Primary Supply

- 12V DC supply (bench PSU or battery)

### 5.2 Rails

- 12V:
  - Stepper drivers
  - Motors
- 5V (via buck converter):
  - Raspberry Pi
  - ESP32
  - Logic components

### 5.3 Requirements

- Common ground across all components
- Adequate current capacity (≥ 3A recommended)
- Fused input
- Power switch

---

## 6. Motion Control Implementation

### 6.1 Motion Controller

- Platform: ESP32
- Firmware: OnStepX

Responsibilities:

- Generate step signals for RA and DEC
- Maintain tracking rates
- Execute slews
- Maintain mount state

### 6.2 Driver Interface

Preferred:

- STEP / DIR interface

Options:

- Reuse existing stepper controller (if compatible)
- Use dedicated stepper drivers (e.g. A4988, TMC series)

### 6.3 Motor Reuse Strategy

Preferred:

- Reuse existing motors if:
  - Electrically compatible
  - Mechanically reliable
  - Provide sufficient torque

Fallback:

- Replace motors with known compatible steppers

---

## 7. Wiring Strategy

### 7.1 Approach

- Disconnect motors from legacy controller
- Identify motor coil pairs
- Connect motors to stepper driver outputs
- Connect drivers to ESP32 (STEP/DIR)

### 7.2 Safety Rules

- Never connect/disconnect motors under power
- Verify wiring before applying power
- Start with low current limits
- Test one axis at a time

---

## 8. Communication Setup

### 8.1 ESP32 ↔ Raspberry Pi

Initial approach:

- Serial (USB)

Future option:

- Wi-Fi (TCP)

### 8.2 Raspberry Pi ↔ Client

- HTTP over local network
- Browser-based UI

---

## 9. Bring-Up Plan

### Phase 0 — Discovery

- Photograph motor labels
- Count motor wires
- Identify connectors and pinout
- Identify stepper driver hardware
- Record power specifications

**Output:**
- Updated hardware inventory
- Confirmed motor/driver compatibility

---

### Phase 1 — Single Axis Test

- Wire RA motor to driver
- Connect driver to ESP32
- Run simple step test
- Verify:
  - Smooth rotation
  - Direction control
  - No overheating

---

### Phase 2 — Dual Axis Control

- Add DEC motor
- Validate independent axis control
- Confirm no interference between axes

---

### Phase 3 — OnStepX Integration

- Configure axis parameters
- Enable tracking
- Test jog controls
- Test controlled slews

---

### Phase 4 — Mount Operation

- Perform manual polar alignment
- Execute basic alignment workflow
- Test tracking stability
- Test GoTo on bright objects

---

## 10. Acceptance Criteria (EQ4)

The EQ4 implementation is considered successful when:

- Both axes move under software control
- RA tracking is stable for visual observation
- Manual control (jogging) works from UI
- Alignment workflow can be completed
- Mount can slew to selected targets
- Targets appear within field of view at low-medium magnification

---

## 11. Known Limitations

- Backlash may affect GoTo precision
- No encoder feedback in initial phases
- Alignment accuracy dependent on user input
- Not suitable for precision astrophotography (initially)

---

## 12. Future Enhancements

- Add homing switches
- Add plate solving via Raspberry Pi
- Improve pointing accuracy via correction loops
- Introduce mount calibration profiles
- Replace motors if required for performance

---

## 13. Related Documents

- `TECHNICAL_SPEC.md`
- `ARCHITECTURE.md`
- `HARDWARE_INVENTORY.md`
- `PINOUTS.md`
- `TEST_PLAN.md`
- `PHASE_PLAN.md`