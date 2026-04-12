# Phase Plan

## Overview

AstroPilot is developed in phases, progressing from hardware validation on a reference mount (EQ4) to a modular, configuration-driven telescope control platform.

Each phase must result in a usable system before progressing.

---

## Phase 0 — Discovery

### Goal
Confirm hardware compatibility and establish baseline configuration inputs.

### Tasks
- [ ] Photograph RA motor label
- [ ] Photograph DEC motor label
- [ ] Count wires per motor
- [ ] Identify connector types
- [ ] Determine motor type (stepper / DC / other)
- [ ] Identify spare stepper driver model
- [ ] Confirm STEP/DIR compatibility
- [ ] Record power supply specs

### Outputs
- Updated `HARDWARE_INVENTORY.md`
- Initial `PINOUTS.md`
- Draft motor and driver profiles

---

## Phase 1 — Motion Bring-Up

### Goal
Achieve controlled movement of both axes.

### Tasks
- [ ] Wire RA motor to driver
- [ ] Drive RA motor via ESP32
- [ ] Validate smooth rotation and direction
- [ ] Add DEC motor
- [ ] Validate independent control of both axes
- [ ] Set safe current limits

### Success Criteria
- Both axes move reliably
- No overheating or missed steps
- Direction control confirmed

---

## Phase 2 — Motion Control Integration

### Goal
Run full motion firmware (OnStepX) on ESP32.

### Tasks
- [ ] Configure OnStepX for mount geometry
- [ ] Set steps per degree / gear ratios
- [ ] Enable sidereal tracking
- [ ] Validate jog controls
- [ ] Validate controlled slews

### Success Criteria
- Stable tracking on RA
- Smooth slewing on both axes
- No drift or erratic motion

---

## Phase 3 — Reference Mount Operation (EQ4)

### Goal
Achieve usable telescope operation on EQ4.

### Tasks
- [ ] Perform manual polar alignment
- [ ] Implement alignment workflow
- [ ] Test tracking stability
- [ ] Test GoTo on bright objects
- [ ] Validate repeatability

### Success Criteria
- Alignment completes successfully
- Targets land within field of view
- Tracking stable for visual use

---

## Phase 4 — Control Platform (Raspberry Pi)

### Goal
Introduce platform control layer and UI.

### Tasks
- [ ] Establish Pi ↔ ESP32 communication
- [ ] Implement basic API client
- [ ] Create web UI (manual control + status)
- [ ] Add tracking control and jog UI
- [ ] Display system state

### Success Criteria
- Full control via browser (iPad)
- Stable communication with ESP32
- UI usable in real conditions

---

## Phase 5 — Practical GoTo

### Goal
Provide reliable object acquisition.

### Tasks
- [ ] Add target catalogue
- [ ] Implement target selection UI
- [ ] Improve alignment workflow
- [ ] Test common celestial objects
- [ ] Tune movement parameters

### Success Criteria
- Consistent GoTo accuracy for visual observing
- Minimal manual correction required

---

## Phase 6 — Assisted Pointing

### Goal
Improve accuracy via software correction.

### Tasks
- [ ] Integrate camera with Raspberry Pi
- [ ] Implement plate solving pipeline
- [ ] Add correction loop
- [ ] Validate improved pointing accuracy

### Success Criteria
- Targets centred automatically
- Repeatable acquisition across sky

---

## Phase 7 — Platformisation

### Goal
Generalise AstroPilot beyond EQ4.

### Tasks
- [ ] Introduce configuration system (mount profiles)
- [ ] Abstract mount-specific logic
- [ ] Separate motion, control, and UI concerns fully
- [ ] Support multiple mount configurations

### Success Criteria
- New mount can be added via configuration
- No core code changes required for basic support

---

## Phase 8 — Ecosystem Integration

### Goal
Expand AstroPilot into a broader platform.

### Tasks
- [ ] Integrate with planetarium tools (e.g. Stellarium)
- [ ] Add session logging and history
- [ ] Add scheduling capabilities
- [ ] Explore remote access

### Success Criteria
- External tools can control AstroPilot
- System supports extended workflows

---

## Phase 9 — Platform Hardening

### Goal
Improve robustness and usability.

### Tasks
- [ ] Add homing / park positions
- [ ] Implement safety limits
- [ ] Improve error handling
- [ ] Optimise performance

### Success Criteria
- System behaves predictably under all conditions
- Safe operation guaranteed