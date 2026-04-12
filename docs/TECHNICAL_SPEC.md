# AstroPilot Technical Specification

## 1. Purpose

AstroPilot is a modular telescope mount control platform designed to provide:

- Real-time motion control (RA/DEC)
- Sidereal and alternative tracking modes
- Computer-assisted GoTo (target acquisition)
- Alignment workflows
- Browser-based and programmatic control interfaces
- Extensibility toward plate solving, automation, and smart observatory operation

The system is designed to operate across multiple mount types.  
The Sky-Watcher EQ4 retrofit is the first reference implementation.

---

## 2. Scope

### In Scope
- Motion control using stepper-driven mounts
- Tracking and slewing
- Alignment and coordinate-based positioning
- Local web-based UI
- Integration with external tools (future phases)
- Modular architecture supporting multiple hardware configurations

### Out of Scope (Phase 1–2)
- High-precision astrophotography guiding
- Closed-loop encoder systems
- Fully autonomous observatory operation
- Vendor-specific proprietary integrations

---

## 3. Design Principles

- **Separation of concerns**  
  Real-time motion control is isolated from application logic and UI

- **Modularity**  
  Components are replaceable (mount, controller, UI, transport)

- **Configuration-driven**  
  Behaviour is defined by profiles rather than hardcoded logic

- **Hardware abstraction**  
  Platform logic is decoupled from mount-specific details

- **Fail-safe operation**  
  Motion must be predictable and stoppable under all conditions

- **Progressive enhancement**  
  System remains usable at each phase of development

---

## 4. System Architecture

### 4.1 High-Level Overview

AstroPilot is composed of three primary layers:

#### Motion Layer
- Runs on microcontroller (ESP32)
- Responsible for:
  - Stepper control
  - Tracking rates
  - Slewing
  - Mount state

#### Control Layer
- Runs on Raspberry Pi or equivalent
- Responsible for:
  - Command orchestration
  - API exposure
  - State management
  - Logging
  - Integration with external systems

#### Client Layer
- Browser-based UI (iPad or other device)
- Responsible for:
  - User interaction
  - Visualisation
  - Control input

---

### 4.2 Subsystems

#### Motion Control Subsystem
- Step generation (RA/DEC)
- Tracking modes (sidereal, solar, lunar)
- Slew control
- Motor enable/disable
- Safety stop

#### Mount Abstraction Subsystem
- Translates platform commands into mount-specific behaviour
- Handles:
  - Axis inversion
  - Gear ratios
  - Limits
  - Calibration parameters

#### Control/API Subsystem
- Exposes:
  - Movement commands
  - Status queries
  - Configuration endpoints
- Interfaces with motion layer via serial or network

#### UI Subsystem
- Browser-based interface
- Provides:
  - Manual control
  - Target selection
  - Alignment workflow
  - System status

#### Intelligence Subsystem (Future)
- Plate solving
- Automatic correction
- Scheduling and automation
- Session logging and analysis

---

## 5. Configuration Model

AstroPilot is configuration-driven.

### 5.1 Configuration Domains

- **Mount Profile**
  - mount type (e.g. EQ4)
  - axis orientation
  - mechanical constraints

- **Motor Profile**
  - steps per revolution
  - microstepping
  - current limits

- **Driver Profile**
  - interface type (STEP/DIR)
  - voltage/current characteristics

- **Transport Profile**
  - serial / Wi-Fi
  - baud rate / endpoint

- **UI Profile**
  - available features
  - client capabilities

### 5.2 Example Configuration

```yaml
mount: eq4
motion_backend: onstepx
transport: serial
ui_mode: web
camera: disabled


⸻

6. Control Model

6.1 Operating Modes
	•	Idle
	•	Tracking
	•	Slewing
	•	Alignment
	•	Error / Halted

6.2 Commands

The system shall support:
	•	Start/stop tracking
	•	Set tracking rate
	•	Slew to coordinates
	•	Manual jog (RA/DEC)
	•	Emergency stop
	•	Set alignment reference

6.3 State

The system shall maintain:
	•	Current mount position (logical)
	•	Tracking state
	•	Alignment status
	•	Active target
	•	Error conditions

⸻

7. Interfaces

7.1 Motion Interface
	•	Protocol between Control Layer and Motion Layer
	•	Serial or TCP-based
	•	Command/response model

7.2 Control API
	•	HTTP-based API exposed by Raspberry Pi
	•	Used by UI and external tools

7.3 UI Interface
	•	Browser-based
	•	Touch-friendly
	•	Compatible with older devices (e.g. iPad)

7.4 External Integration (Future)
	•	Planetarium software (e.g. Stellarium)
	•	INDI / ASCOM bridges
	•	Camera control and plate solving tools

⸻

8. Power Architecture

8.1 Primary Supply
	•	12V DC nominal

8.2 Rails
	•	12V: motors and drivers
	•	5V: logic (Pi, ESP32, peripherals)

8.3 Requirements
	•	Stable under load
	•	Shared ground
	•	Protected with fuse
	•	Avoid USB-only power for motors

⸻

9. Safety Model

The system must ensure:
	•	No uncontrolled motion
	•	Immediate response to stop commands
	•	No hot-plugging of motors under power
	•	Safe handling of communication loss
	•	Configurable motion limits (future)

⸻

10. Deployment Model

10.1 Minimal Deployment
	•	ESP32 + motion firmware
	•	Direct control via serial or Wi-Fi

10.2 Standard Deployment
	•	ESP32 + Raspberry Pi
	•	Pi-hosted web UI
	•	iPad client

10.3 Extended Deployment (Future)
	•	Camera attached to Pi
	•	Plate solving enabled
	•	External integrations active

⸻

11. Phase Roadmap

Phase 0 — Discovery

Hardware validation and compatibility

Phase 1 — Motion Bring-Up

Single-axis and dual-axis control

Phase 2 — Reference Mount Operation

Tracking, slewing, alignment on EQ4

Phase 3 — Local Control Platform

Pi-based UI and control layer

Phase 4 — Practical GoTo

Target selection and usable pointing

Phase 5 — Assisted Pointing

Plate solving and correction

Phase 6 — Platformisation

Abstract mount support and configuration system

Phase 7 — Ecosystem Integration

External tools, automation, scheduling

⸻

12. Acceptance Criteria (Platform Level)

The platform is considered functional when:
	•	Motion layer reliably controls both axes
	•	Tracking is stable for visual use
	•	Control layer exposes usable API
	•	UI provides full manual control and alignment workflow
	•	System supports at least one mount via configuration
	•	Architecture supports extension without redesign

⸻

13. Reference Implementation

The first implementation of AstroPilot targets:
	•	Mount: Sky-Watcher EQ4
	•	Motion backend: OnStepX (ESP32)
	•	Control host: Raspberry Pi
	•	UI: Browser-based (iPad)

Detailed implementation is defined in:
REFERENCE_IMPLEMENTATION_EQ4.md

⸻

14. Non-Goals
	•	Competing with commercial high-end GoTo systems
	•	Precision astrophotography control (initial phases)
	•	Supporting all mount types immediately
	•	Eliminating need for user alignment

⸻

15. Future Directions
	•	Plate solving integration
	•	Automatic alignment workflows
	•	Multi-mount support
	•	Remote observatory capabilities
	•	Session logging and analytics
	•	Intelligent observation planning

