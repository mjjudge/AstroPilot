# AstroPilot Architecture

## 1. Overview

AstroPilot is a modular, layered telescope control system composed of:

- A real-time motion controller (ESP32)
- A control/orchestration node (Raspberry Pi)
- A browser-based client interface (e.g. iPad)

The system is designed to separate real-time motion from higher-level logic and user interaction.

---

## 2. High-Level Architecture

+–––––––––––+
|      Client UI       |
|   (iPad / Browser)   |
+–––––+———–+
|
| HTTP
|
+–––––v———–+
|   Control Layer      |

Raspberry Pi
- Web UI
- API
- State mgmt
- Logging
- Integrations
+–––––+———–+

       |
       | Serial / TCP
       |

+–––––v———–+
|   Motion Layer       |

ESP32
- OnStepX
- Tracking
- Slewing
- Axis control
+–––––+———–+

       |
       | STEP / DIR
       |

+–––––v———–+
|   Motor Drivers      |
+–––––+———–+
|
|
+–––––v———–+
|   RA / DEC Motors    |
|   (EQ4 Mount)        |
+–––––––––––+

---

## 3. Layer Responsibilities

### 3.1 Motion Layer (ESP32)

Runs OnStepX or equivalent firmware.

Responsibilities:
- Generate precise step signals for RA and DEC
- Maintain tracking rates (sidereal, etc.)
- Execute slews
- Maintain mount state (position, motion mode)
- Enforce motion safety (stop conditions)

Constraints:
- Must be deterministic
- Must not depend on network availability
- Must continue tracking independently of higher layers

---

### 3.2 Control Layer (Raspberry Pi)

Acts as the system “brain”.

Responsibilities:
- Provide HTTP API
- Host browser-based UI
- Manage system state (session-level)
- Translate user commands into motion commands
- Log activity and diagnostics
- Integrate with external tools (future)

Optional Responsibilities (later phases):
- Plate solving
- Camera control
- Scheduling and automation
- External system integration (e.g. planetarium tools)

---

### 3.3 Client Layer (Browser / iPad)

User-facing interface.

Responsibilities:
- Provide controls:
  - manual movement
  - tracking toggle
  - target selection
- Display system state:
  - connection status
  - tracking status
  - alignment state
- Guide user through alignment workflow

Constraints:
- Must be touch-friendly
- Must function on older browsers
- Should degrade gracefully

---

## 4. Data Flow

### 4.1 Command Flow

User Action (UI)
↓
HTTP Request (Client → Pi)
↓
Control Layer processes command
↓
Command sent to ESP32 (Serial/TCP)
↓
Motion Layer executes (OnStepX)
↓
Motors move

---

### 4.2 Status Flow

Motion Layer (ESP32)
↓
Status response
↓
Control Layer (Pi)
↓
API / WebSocket / Polling
↓
Client UI updates

---

## 5. Communication Interfaces

### 5.1 ESP32 ↔ Raspberry Pi

Supported modes:
- Serial (USB)
- Wi-Fi (TCP/IP)

Initial preference:
- Serial for simplicity and reliability

Future:
- Network-based communication for flexibility

---

### 5.2 Raspberry Pi ↔ Client

- HTTP (REST-style API)
- Optional WebSocket (future for real-time updates)

---

### 5.3 External Integrations (Future)

- Planetarium software (e.g. Stellarium)
- INDI / ASCOM bridges
- Camera systems
- Plate solving tools

---

## 6. Component Boundaries

### 6.1 Motion Boundary

The ESP32:
- Does not handle UI
- Does not manage sessions
- Does not perform high-level planning

It only:
- Moves the mount
- Maintains timing-critical behaviour

---

### 6.2 Control Boundary

The Raspberry Pi:
- Does not generate step signals
- Does not require real-time guarantees

It:
- Orchestrates
- Translates commands
- Hosts logic and UI

---

### 6.3 UI Boundary

The client:
- Does not control hardware directly
- Interacts only through the Pi API

---

## 7. Deployment Modes

### 7.1 Minimal

- ESP32 only
- Direct control via serial or Wi-Fi
- No Pi required

Use case:
- Early testing
- Bench development

---

### 7.2 Standard

- ESP32 + Raspberry Pi
- Pi hosts UI and API
- iPad used as control interface

Use case:
- Field operation
- Normal observing

---

### 7.3 Extended

- ESP32 + Raspberry Pi + camera
- Plate solving enabled
- External integrations active

Use case:
- Assisted pointing
- Advanced workflows

---

## 8. Power Architecture (Logical View)

    12V Supply
        |
 +------+------+
 |             |

Motor Drivers   Buck Converter (5V)
|             |
Motors        +––+—–+
|          |
ESP32     Raspberry Pi

Key points:
- Motors powered from 12V
- Logic powered from regulated 5V
- Shared ground across system

---

## 9. Failure Modes

### 9.1 Control Layer Failure (Pi crash)

- ESP32 continues tracking
- UI becomes unavailable
- System recoverable without power cycling

### 9.2 Network Failure

- UI disconnects
- Motion continues safely
- Reconnection restores control

### 9.3 Motion Layer Failure

- Loss of motor control
- Requires manual intervention or restart

### 9.4 Power Instability

- Risk of missed steps or resets
- Mitigated by stable 12V supply and regulation

---

## 10. Extension Points

AstroPilot is designed to be extended at defined boundaries:

### Motion Layer
- Alternative firmware
- Different microcontroller platforms

### Control Layer
- Alternative APIs
- Different orchestration logic
- Remote access capabilities

### UI Layer
- Mobile-first UI
- Desktop control panel
- Integration into other apps

### Intelligence Layer (Future)
- Plate solving
- Target optimisation
- Observation planning

---

## 11. Summary

AstroPilot separates:

- **Real-time motion (ESP32)**
- **System logic (Raspberry Pi)**
- **User interaction (browser UI)**

This architecture ensures:

- Stability under real-world conditions
- Flexibility for future expansion
- Reusability across different mounts 
