Absolutely — # AstroPilot Configuration Model

## 1. Purpose

AstroPilot is intended to be a configuration-driven telescope control platform.

This document defines how configuration should be structured so that:

- AstroPilot can support multiple mounts over time
- mount-specific behaviour is not hardcoded into platform logic
- hardware-specific unknowns can be captured progressively
- AstroPilot can use OnStepX as a motion backend without coupling the entire system to a single mount

The first reference configuration is the Sky-Watcher EQ4.

---

## 2. Design Principles

### 2.1 Configuration over hardcoding

Mount geometry, motor behaviour, transport choices, and feature enablement should be represented in configuration wherever practical.

### 2.2 Separate platform config from backend config

AstroPilot configuration should not simply mirror OnStepX configuration files.

Instead:

- AstroPilot should define its own platform-level configuration model
- a subset of AstroPilot configuration will map into OnStepX settings
- some OnStepX settings will remain backend-specific and generated or maintained separately

### 2.3 Support incomplete knowledge

Configuration files must tolerate unknown values during early phases.

Examples:
- motor current unknown
- step angle unknown
- gear ratio unknown

Unknowns should be represented explicitly rather than guessed.

### 2.4 Prefer composition

Configuration should be split into domains rather than stored in one large file.

---

## 3. Configuration Domains

AstroPilot configuration is divided into the following domains:

### 3.1 Mount Profile

Defines mount-level behaviour and mechanics.

Examples:
- mount identifier
- mount type
- equatorial vs alt-az
- axis orientation
- inversion flags
- motion limits
- parking behaviour
- reference implementation notes

### 3.2 Motor Profile

Defines the electrical and motion characteristics of each motor.

Examples:
- motor type
- wire count
- step angle
- current rating
- voltage expectations
- gearbox presence
- effective output ratio
- notes on reuse suitability

### 3.3 Driver Profile

Defines the motor driver interface and constraints.

Examples:
- driver model
- interface type
- STEP/DIR support
- microstepping
- current limit method
- cooling requirements
- supply voltage

### 3.4 Transport Profile

Defines how AstroPilot communicates with the motion layer.

Examples:
- serial vs Wi-Fi
- baud rate
- host/port
- retry behaviour
- timeout values

### 3.5 System Profile

Defines whole-system runtime choices.

Examples:
- selected motion backend
- enabled features
- logging level
- UI mode
- camera enabled/disabled
- plate solving enabled/disabled

### 3.6 Feature Flags

Defines optional capabilities that may be unavailable in early phases.

Examples:
- homing switches
- plate solving
- camera integration
- remote access
- scheduling
- external planetarium integration

---

## 4. Proposed Directory Structure

```text
configs/
├── mounts/
│   └── eq4.yaml
├── motors/
│   ├── ra_unknown.yaml
│   └── dec_unknown.yaml
├── drivers/
│   └── unknown_driver.yaml
├── transport/
│   ├── serial.yaml
│   └── wifi.yaml
└── system/
    └── default.yaml

This structure is intentionally modular.

It allows AstroPilot to:
	•	swap mounts without changing platform logic
	•	refine motor and driver details incrementally
	•	test different transport modes
	•	evolve from reference implementation to platform

⸻

5. Configuration Ownership

AstroPilot must distinguish between:

5.1 Platform-Owned Configuration

This belongs to AstroPilot and should live in the AstroPilot repo.

Examples:
	•	mount profile identity
	•	transport choice
	•	UI mode
	•	feature flags
	•	system deployment mode
	•	high-level capability toggles

5.2 Backend-Mapped Configuration

This is defined in AstroPilot terms but ultimately passed into the motion backend.

Examples:
	•	axis inversion
	•	tracking mode defaults
	•	driver interface type
	•	steps-per-degree derived values
	•	motion limits

5.3 Backend-Native Configuration

This belongs to OnStepX or another motion backend and should not be redefined blindly in AstroPilot.

Examples:
	•	firmware-specific compile-time settings
	•	low-level timing details
	•	backend-internal features not surfaced by AstroPilot

AstroPilot should reference these where necessary, but not duplicate them unnecessarily.

⸻

6. Initial Configuration Strategy

6.1 Early Phase Strategy

Before hardware inspection is complete:
	•	create placeholder configuration files
	•	mark unknown values explicitly
	•	avoid inventing electrical or mechanical parameters
	•	document assumptions separately from confirmed values

6.2 Reference Mount Strategy

The EQ4 should be represented as a mount profile, not as special-case logic in code.

Example:
	•	configs/mounts/eq4.yaml

This file should contain:
	•	mount metadata
	•	axis conventions
	•	known constraints
	•	references to motor and driver profiles

6.3 Future Platform Strategy

Additional mounts should be introduced by adding new profiles, for example:
	•	configs/mounts/heq5.yaml
	•	configs/mounts/dobsonian_generic.yaml

The goal is that AstroPilot core code does not need to change to support a basic new mount configuration.

⸻

7. Example Configuration Shapes

7.1 Mount Profile Example

id: eq4
display_name: Sky-Watcher EQ4
mount_type: equatorial
axes:
  primary: ra
  secondary: dec
reference_implementation: true
motor_profiles:
  ra: ra_unknown
  dec: dec_unknown
driver_profile: unknown_driver
transport_profile: serial
notes:
  - First AstroPilot reference implementation
  - Existing dual-axis motor kit fitted

7.2 Motor Profile Example

id: ra_unknown
axis: ra
motor_type: unknown
wire_count: unknown
step_angle_deg: unknown
rated_voltage: unknown
rated_current_a: unknown
gearbox: unknown
reuse_candidate: true
status: awaiting_inspection

7.3 Driver Profile Example

id: unknown_driver
interface: unknown
step_dir_supported: unknown
microstepping: unknown
supply_voltage: 12v_nominal
status: awaiting_identification

7.4 Transport Profile Example

id: serial
type: serial
device: /dev/ttyUSB0
baud_rate: 115200
status: initial_preference

7.5 System Profile Example

motion_backend: onstepx
control_host: raspberry_pi
ui_mode: web
client_target: ipad
features:
  plate_solving: false
  camera: false
  homing_switches: false
  remote_access: false


⸻

8. Unknown Value Conventions

Unknown values should be represented explicitly.

Preferred forms:
	•	unknown
	•	awaiting_inspection
	•	not_applicable

Avoid:
	•	guessed numeric defaults
	•	placeholder values that look real
	•	silent omissions where the field is important

This is especially important for:
	•	motor current
	•	step angle
	•	gear ratios
	•	driver interface type

⸻

9. Relationship to OnStepX

AstroPilot uses OnStepX as the initial motion backend. OnStepX is designed as a computerized telescope controller and supports multiple motor-driver styles, including Step/Dir, making it a strong fit for AstroPilot’s motion layer.

OnStepX also centers configuration around firmware-level settings such as those in Config.h, and the project guidance points users to a configuration calculator for mount-specific values.

Therefore:
	•	AstroPilot should maintain platform-level configuration in its own repo
	•	AstroPilot should map relevant values into OnStepX configuration
	•	AstroPilot should not treat OnStepX configuration files as the sole source of truth for the whole system

⸻

10. Phase Alignment

Phase 0–2
	•	Create placeholder config structure
	•	Populate mount, transport, and system profiles
	•	Leave motor and driver values explicitly unknown until inspection

Phase 3–5
	•	Refine EQ4 configuration with confirmed hardware values
	•	Add mappings from AstroPilot config to OnStepX config
	•	Add feature flags for UI and GoTo workflows

Phase 6+
	•	Generalise config model for additional mounts
	•	Separate reference implementation from platform defaults
	•	Add validation rules for config completeness

⸻

11. Validation Rules

Over time, AstroPilot should validate configuration at startup.

Examples:
	•	mount profile must exist
	•	transport profile must exist
	•	motor profile must be assigned per axis
	•	required fields must be confirmed before motion is enabled
	•	unknown motor current must block live driver enablement

This helps ensure:
	•	safe bring-up
	•	fewer hidden assumptions
	•	cleaner multi-mount support

⸻

12. Decisions Captured by This Model

This configuration model formalises the following decisions:
	•	AstroPilot is a platform, not a one-off EQ4 hack
	•	EQ4 is the first reference implementation
	•	configuration is a first-class architectural concern
	•	mount-specific logic should be represented in profiles where practical
	•	OnStepX is the initial motion backend
	•	AstroPilot owns platform configuration even when using OnStepX underneath

⸻
