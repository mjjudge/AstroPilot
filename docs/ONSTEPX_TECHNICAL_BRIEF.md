# OnStepX Technical Brief for AstroPilot

**Status:** Pre-hardware. All firmware details confirmed from source. Hardware-dependent values are explicitly unknown.

---

## 1. OnStepX Fit Assessment

### Is OnStepX a good fit for AstroPilot's motion layer?

**Yes. OnStepX is the correct motion backend for AstroPilot.**

Confirmed from source/README:

- Designed explicitly as a computerised telescope controller.
- Supports equatorial mounts (GEM, Fork) and Alt/Az.
- Supports Step/Dir motor drivers (A4988, DRV8825, TMC2130, TMC2209, TMC5160, etc.)
- GoTo, tracking, and alignment are built-in — no need to implement these in AstroPilot.
- ESP32 is a first-class supported platform with built-in WiFi and Bluetooth command channels.
- LX200 command protocol is implemented — this is the interface AstroPilot will use.
- INDI and ASCOM drivers exist for future ecosystem integration.

### What responsibilities should remain in OnStepX?

| Responsibility | Stays in OnStepX |
|---|---|
| Step signal generation (RA/DEC) | ✓ |
| Sidereal/Solar/Lunar tracking rates | ✓ |
| Slew execution and acceleration | ✓ |
| GoTo coordinate computation | ✓ |
| Meridian flip logic | ✓ |
| Pointing model (alignment) | ✓ |
| Periodic error correction (PEC) | ✓ |
| Motor driver fault detection | ✓ |
| Motion safety limits | ✓ |
| Homing/park logic | ✓ |
| Backlash compensation | ✓ |

### What should the Raspberry Pi (AstroPilot control layer) own?

| Responsibility | AstroPilot / Pi |
|---|---|
| HTTP API exposed to browser UI | ✓ |
| Browser UI (iPad-friendly) | ✓ |
| Session management and logging | ✓ |
| Object/target catalogue | ✓ |
| Coordinate preprocessing (future) | ✓ |
| Plate solving (future) | ✓ |
| INDI/ASCOM bridge (future) | ✓ |
| LX200 command translation layer | ✓ |

### What AstroPilot must NOT duplicate from OnStepX

- Step generation and timing
- Tracking rate computation
- GoTo algorithms
- Alignment geometry
- PEC correction
- Meridian flip logic

These are already solved in OnStepX. Duplicating them would introduce divergence and bugs.

---

## 2. Required OnStepX Configuration Inputs

These are the specific settings that must be resolved before OnStepX can be flashed and used for the EQ4.

All settings are in `Config.h` in the root of the OnStepX repo (`hjd1964/OnStepX`).

### 2.1 Controller / Pinmap

| Symbol | Meaning | Hardware-dependent? | Location |
|---|---|---|---|
| `PINMAP` | Selects which ESP32 board layout to use (e.g. `MaxESP4`, `FYSETC_E4`, or custom) | **Yes** — depends on which ESP32 board is chosen | `Config.h` line ~20 |
| `SERIAL_A_BAUD_DEFAULT` | Baud rate for primary serial channel (USB) | No — recommend `9600` or `115200` | `Config.h` |
| `SERIAL_RADIO` | Enable WiFi or Bluetooth on ESP32 (`WIFI_ACCESS_POINT`, `WIFI_STATION`, `BLUETOOTH`) | No | `Config.h` |
| `HOST_NAME` | Network hostname (up to 16 chars) | No — can be set to `AstroPilot` | `Config.h` |

### 2.2 Mount Geometry

| Symbol | Meaning | Hardware-dependent? | Location |
|---|---|---|---|
| `MOUNT_TYPE` | `GEM` for EQ4 (German Equatorial Mount with meridian flips). Other options: `FORK`, `ALTAZM`. | No — EQ4 is a GEM | `Config.h` — MOUNT section |
| `MOUNT_STARTUP_MODE` | `SA_AUTO` (default) allows goto without absolute position source | No | `Config.h` |
| `MOUNT_COORDS` | `TOPOCENTRIC` (default) applies refraction correction | No | `Config.h` |

### 2.3 RA Axis (Axis 1) Parameters

| Symbol | Meaning | Hardware-dependent? | Location |
|---|---|---|---|
| `AXIS1_DRIVER_MODEL` | Driver chip model (e.g. `A4988`, `DRV8825`, `TMC2209`) | **Yes** — depends on driver hardware | `Config.h` — AXIS1 section |
| `AXIS1_STEPS_PER_DEGREE` | `n = (stepper_steps × microsteps × gear_reduction) / 360.0` | **Yes** — requires motor step angle + gearbox data | `Config.h` |
| `AXIS1_REVERSE` | `ON` if RA moves in wrong direction | **Yes** — determined during axis test | `Config.h` |
| `AXIS1_LIMIT_MIN` | Minimum hour angle (default `-180`) | Adjust after testing | `Config.h` |
| `AXIS1_LIMIT_MAX` | Maximum hour angle (default `180`) | Adjust after testing | `Config.h` |
| `AXIS1_DRIVER_MICROSTEPS` | Microstepping divisor during tracking (e.g. `16`, `32`) | **Yes** — driver capability + desired resolution | `Config.h` |
| `AXIS1_DRIVER_MICROSTEPS_GOTO` | Microstepping during slews (typically coarser for speed) | Optional | `Config.h` |
| `AXIS1_DRIVER_IRUN` | Run current in mA (TMC drivers only) | **Yes** — motor rated current | `Config.h` |
| `AXIS1_DRIVER_IHOLD` | Standstill current in mA | **Yes** — derived from motor rating | `Config.h` |
| `AXIS1_DRIVER_IGOTO` | Current during slews | Optional | `Config.h` |
| `AXIS1_SENSE_HOME` | Home switch sense logic level | Optional | `Config.h` |
| `AXIS1_SENSE_LIMIT_MIN/MAX` | Limit switch sense | Optional | `Config.h` |

### 2.4 DEC Axis (Axis 2) Parameters

Same set as Axis 1 but prefixed `AXIS2_`. The EQ4 DEC axis may have different gearing than RA — must be measured separately.

| Symbol | Note |
|---|---|
| `AXIS2_DRIVER_MODEL` | Same driver model as RA (usually) |
| `AXIS2_STEPS_PER_DEGREE` | May differ from AXIS1 if gearing differs |
| `AXIS2_LIMIT_MIN` | Default `-90` |
| `AXIS2_LIMIT_MAX` | Default `90` |
| `AXIS2_REVERSE` | Determined during axis test |

### 2.5 Tracking

| Symbol | Meaning | Hardware-dependent? | Location |
|---|---|---|---|
| `TRACK_AUTOSTART` | `ON` to begin sidereal tracking on power-up | No — operational preference | `Config.h` |
| `TRACK_COMPENSATION_DEFAULT` | Refraction compensation (`OFF`, `REFRACTION`, `MODEL`) | No | `Config.h` |
| `TRACK_BACKLASH_RATE` | Rate during backlash takeup (default `20` × sidereal) | Adjust after testing | `Config.h` |

### 2.6 PEC (Periodic Error Correction)

| Symbol | Meaning | Hardware-dependent? | Location |
|---|---|---|---|
| `PEC_STEPS_PER_WORM_ROTATION` | `n = (AXIS1_STEPS_PER_DEGREE × 360) / final_worm_ratio`. Set `0` to disable. | **Yes** — worm gear ratio required | `Config.h` |

### 2.7 Slewing

| Symbol | Meaning | Hardware-dependent? | Location |
|---|---|---|---|
| `SLEW_RATE_BASE_DESIRED` | Target slew speed in deg/sec (default `1.0`) | Tune after motor testing | `Config.h` |
| `SLEW_ACCELERATION_DIST` | Distance in degrees for accel/decel (default `5.0`) | Tune after testing | `Config.h` |
| `SLEW_RAPID_STOP_DIST` | Stopping distance when abort triggered (default `2.0`) | Tune after testing | `Config.h` |

### 2.8 Time and Location

| Symbol | Meaning | Hardware-dependent? | Location |
|---|---|---|---|
| `TIME_LOCATION_SOURCE` | `GPS`, `NTP`, `DS3231` RTC, or `OFF` | Optional for initial bring-up | `Config.h` |

For initial bring-up, date/time can be set via LX200 command from the Pi.

### 2.9 Alignment

| Symbol | Meaning | Location |
|---|---|---|
| `ALIGN_MAX_STARS` | Max alignment stars (default `AUTO` → 6 or 9) | `Config.h` |
| `ALIGN_AUTO_HOME` | Use home switches to find home before align | `Config.h` |

---

## 3. Integration Surfaces

### 3.1 LX200 Serial Protocol (Primary Interface)

**Confirmed from source:** OnStepX implements the LX200 command protocol.

- All mount commands (tracking, slewing, GoTo, status, alignment) are available via LX200.
- Supported on any configured serial channel.
- The Pi connects via USB serial (`/dev/ttyUSB0` or similar) to ESP32.
- Commands are ASCII text over UART. Request/response model.
- Baud rate set via `SERIAL_A_BAUD_DEFAULT` in `Config.h` (default `9600`; recommend `115200`).

**Recommended for initial bring-up:** Serial (USB) to Pi. Simple, reliable, no networking required.

### 3.2 WiFi / TCP (ESP32 Built-In)

**Confirmed from source:** OnStepX supports ESP32 WiFi natively.

- `SERIAL_RADIO = WIFI_ACCESS_POINT` — ESP32 creates its own network.
- `SERIAL_RADIO = WIFI_STATION` — ESP32 joins existing LAN.
- When enabled, LX200 commands are available on a TCP socket (port `9999` by convention in OnStep ecosystem).
- The Pi can connect to ESP32 over WiFi instead of USB.

**Recommended for later phases:** WiFi/TCP removes the USB cable and allows greater flexibility.

### 3.3 Built-In Web Server (SWS — Smart Web Server)

**Confirmed from source:** OnStepX includes a web server (see `src/lib/ethernet/webServer/WebServer.h`).

- Activated when `OPERATIONAL_MODE` is set to Ethernet or WiFi and `WEB_SERVER != OFF`.
- Provides a browser-accessible status and control interface (OnStep Smart Web Server, "SWS").
- Available at the ESP32's IP address when WiFi is enabled.
- Supports up to 24 registered handlers (`WEB_HANDLER_COUNT_MAX`).

SWS provides: mount status, basic goto/slew controls, configuration review.

### 3.4 INDI Driver (Future)

- Official INDI driver exists for OnStep: `indi-onstep` in the INDI library.
- When AstroPilot runs a full INDI server on the Pi, the Pi connects to OnStepX via serial or TCP and exposes a standard INDI telescope device.
- Enables control from KStars, Stellarium, and any INDI-compatible client.
- Relevant from Phase 7+ (Ecosystem Integration).

### 3.5 ASCOM Driver (Windows / Future)

- ASCOM driver for OnStep exists with IP and Serial support.
- Not relevant to the current Pi/iPad architecture but useful for future Windows PC integration.

### 3.6 Summary: Recommended Interface by Phase

| Phase | Interface | Rationale |
|---|---|---|
| Phase 1–2 (Bring-Up) | USB Serial (`115200`) | Simple, no networking, easy debugging |
| Phase 3–4 (Mount Op) | USB Serial or WiFi TCP | Test both; WiFi preferred for field use |
| Phase 5+ (Platform) | WiFi TCP + Pi API | Pi becomes the sole command surface |
| Phase 7+ (Ecosystem) | INDI on Pi | Full planetarium software integration |

---

## 4. UI / Web Capability Review

### 4.1 What OnStepX's Built-In Web UI Provides

**Confirmed from source:** The Smart Web Server (SWS) provides:

- Mount status display
- Basic tracking controls
- GoTo controls
- Configuration parameter view
- Focuser controls (if configured)

Accessible at the ESP32's IP address when WiFi is active.

### 4.2 Should AstroPilot Use the OnStepX Web UI?

**Short-term (Phase 1–3):** Yes, use SWS directly for early validation.

- No Pi software needed during bring-up.
- Allows confirming motor movement, tracking, and slewing before any AstroPilot code exists.
- Accessible from the iPad browser over WiFi.

**Medium-term (Phase 4):** Begin replacing SWS with AstroPilot's own Pi-hosted UI.

- AstroPilot's UI will be touch-optimised for iPad.
- Will expose AstroPilot-specific workflows (alignment, object selection, plate solving later).
- Pi API abstracts OnStepX commands — not all LX200 complexity exposed to the user.

**Long-term:** AstroPilot's Pi-hosted UI becomes the primary interface. SWS remains accessible as a fallback/diagnostic tool.

### 4.3 Phased UI Recommendation

| Phase | UI Approach |
|---|---|
| Phase 1–2 | SWS only (direct ESP32 WiFi). No Pi UI. |
| Phase 3 | SWS for validation + start Pi API skeleton |
| Phase 4 | Pi UI takes over for all user interaction |
| Phase 5+ | Pi UI + object catalogue + alignment workflow |
| Phase 7+ | Pi UI + INDI / Stellarium integration |

---

## 5. Configuration-Driven Platform Implications

### 5.1 AstroPilot Config vs OnStepX Config

OnStepX is configured via compile-time `#define` values in `Config.h`. There is no runtime YAML or JSON system — configuration is baked into the firmware binary at flash time.

This means:

- AstroPilot's YAML profiles (mount, motor, driver) must eventually map to values written into OnStepX `Config.h` before flashing.
- A `config-generator` tool or script will be needed to translate AstroPilot config → OnStepX `Config.h`.
- Some values (axis reversal, backlash) can be adjusted at runtime via LX200 commands and stored in NV (EEPROM/FRAM).

### 5.2 Recommended Config Domains

Based on `Config.h` analysis, the following AstroPilot config domains map cleanly to OnStepX:

| AstroPilot Domain | File | Maps to OnStepX Symbols |
|---|---|---|
| Mount Profile | `config/mounts/eq4.yaml` | `MOUNT_TYPE`, `AXIS1/2_LIMIT_MIN/MAX`, `AXIS1/2_REVERSE` |
| Motor Profile | `config/motors/ra_motor.yaml` | `AXIS1_STEPS_PER_DEGREE`, `AXIS1_DRIVER_IRUN/IHOLD` |
| Driver Profile | `config/drivers/ra_driver.yaml` | `AXIS1_DRIVER_MODEL`, `AXIS1_DRIVER_MICROSTEPS` |
| Transport Profile | `config/transport/serial.yaml` | `SERIAL_A_BAUD_DEFAULT`, `SERIAL_RADIO` |
| System Profile | `config/system/default.yaml` | `TRACK_AUTOSTART`, `GOTO_FEATURE`, `TIME_LOCATION_SOURCE` |

### 5.3 Config Ownership Rules

| Config Type | Owner | Notes |
|---|---|---|
| Platform identity, UI mode, feature flags | AstroPilot | Lives in `config/` in this repo |
| Mount geometry, axis limits, tracking defaults | AstroPilot → OnStepX | AstroPilot is source of truth; generates OnStepX values |
| Driver model, step counts, currents | AstroPilot → OnStepX | Must be confirmed from hardware before populating |
| Low-level timing, firmware-internal features | OnStepX | Should not be reimplemented in AstroPilot |

### 5.4 Recommended New Config Files

Based on the existing `config/mounts/` directory:

```
config/
├── mounts/
│   └── eq4.yaml            # Mount profile (partial — pending hardware)
├── motors/
│   ├── ra_motor.yaml       # RA motor (values unknown until inspection)
│   └── dec_motor.yaml      # DEC motor (values unknown until inspection)
├── drivers/
│   └── driver_unknown.yaml # Driver profile (pending identification)
├── transport/
│   ├── serial.yaml         # USB serial (initial preference)
│   └── wifi.yaml           # WiFi TCP (future)
└── system/
    └── default.yaml        # System runtime profile
```

These files should exist as placeholders now, with unknown values marked explicitly as `unknown` or `awaiting_inspection`.

### 5.5 OnStepX Config.h Placement

The actual OnStepX `Config.h` should live at:

```
firmware/onstepx/Config.h
```

It should be generated or maintained alongside AstroPilot config, with unknown values left at OnStepX defaults (`OFF`) until hardware is confirmed.

---

## 6. Risks / Unknowns

These items **block finalising OnStepX configuration** and must be resolved during Phase 0 hardware inspection.

| # | Unknown | Why it blocks | How to resolve |
|---|---|---|---|
| 1 | **Motor type** (stepper vs DC geared vs unipolar) | Determines driver compatibility entirely | Inspect motor labels, count wires |
| 2 | **Wire count per motor** | 4-wire = bipolar stepper; 5/6-wire = unipolar; 2-wire = DC | Physical wire count |
| 3 | **Motor step angle** | Required to compute `AXIS1_STEPS_PER_DEGREE` | Read motor label (typically 1.8° or 0.9°) |
| 4 | **Gearbox ratio (RA)** | Required to compute `AXIS1_STEPS_PER_DEGREE` | Count worm gear teeth or find model spec sheet |
| 5 | **Gearbox ratio (DEC)** | Required to compute `AXIS2_STEPS_PER_DEGREE` | Same as above |
| 6 | **Motor rated current** | Required for `AXIS1_DRIVER_IRUN` (TMC drivers); excess current burns motors | Read motor label |
| 7 | **Motor rated voltage** | Affects driver choice and supply voltage | Read motor label |
| 8 | **Existing driver model** | Determines `AXIS1_DRIVER_MODEL` and microstepping options | Inspect driver PCB markings |
| 9 | **Driver interface** (STEP/DIR vs native protocol) | Determines if existing driver can be reused at all | Check driver documentation |
| 10 | **Connector pinout** | Required for safe wiring | Physical measurement + continuity test |
| 11 | **ESP32 board variant** | Required for `PINMAP` in Config.h | Identify board model |
| 12 | **Worm rotation count for PEC** | Required for `PEC_STEPS_PER_WORM_ROTATION` | Mount specification or measurement |

Until items 1–11 are resolved, do not power up any driver with unknown motor connected.

---

## 7. Concrete Repo Recommendations

### 7.1 Immediate Documentation Actions

| Action | File | Notes |
|---|---|---|
| Update with confirmed facts from this brief | `docs/ONSTEPX_NOTES.md` | Mark each fact as source-confirmed or assumption |
| Update risks with numbered hardware unknowns | `docs/RISKS_AND_ASSUMPTIONS.md` | Use the table in Section 6 above |
| Add Phase 0 checklist aligned to this brief | `docs/PHASE_PLAN.md` | Hardware inspection tasks are blocking |

### 7.2 Placeholder Config Files to Create Now

Create these files now with explicit `unknown`/`awaiting_inspection` values:

```
config/motors/ra_motor.yaml
config/motors/dec_motor.yaml
config/drivers/driver_unknown.yaml
config/transport/serial.yaml
config/system/default.yaml
```

The `config/mounts/` directory already exists; an `eq4.yaml` should be created there.

### 7.3 Firmware Scaffold

Create the firmware directory structure now:

```
firmware/
└── onstepx/
    ├── Config.h         # Copy of template from OnStepX repo with EQ4 values to-be-filled
    └── README.md        # Notes on how to build and flash
```

### 7.4 Decisions to Record

Add these to `docs/DECISIONS.md`:

- OnStepX `Config.h` is generated from AstroPilot mount/motor/driver config profiles (future tooling)
- Initial communication is USB serial at `115200` baud
- OnStepX SWS (built-in web UI) is used for Phase 1–2 validation before the Pi UI exists
- All motor and driver values remain `unknown` until Phase 0 hardware inspection
- `MOUNT_TYPE = GEM` is confirmed for EQ4 — no further hardware inspection needed

### 7.5 Checklist for Hardware Inspection Session

Add to `docs/HARDWARE_INVENTORY.md` or a new `docs/PHASE_0_CHECKLIST.md`:

```
- [ ] Photograph RA motor label (model, step angle, voltage, current)
- [ ] Photograph DEC motor label
- [ ] Count wires: RA motor
- [ ] Count wires: DEC motor
- [ ] Identify connector type and pinout (continuity test)
- [ ] Identify/photograph stepper driver PCB markings
- [ ] Confirm driver STEP/DIR interface
- [ ] Identify ESP32 board model
- [ ] Record PSU output voltage and current rating
- [ ] Count RA worm gear teeth (if visible without disassembly)
```

---

## Confirmed from Source

The following facts are confirmed directly from the OnStepX repository (`hjd1964/OnStepX`, `main` branch):

- OnStepX supports ESP32 natively with built-in WiFi and Bluetooth (`SERIAL_RADIO`)
- `Config.h` contains all firmware configuration as compile-time `#define` values
- `MOUNT_TYPE = GEM` is the correct setting for a German Equatorial Mount (EQ4)
- `AXIS1_STEPS_PER_DEGREE` formula: `(stepper_steps × microsteps × gear_reduction) / 360.0`
- `PEC_STEPS_PER_WORM_ROTATION` formula: `(AXIS1_STEPS_PER_DEGREE × 360) / reduction_final_stage`
- LX200 protocol is implemented as the primary command interface
- A built-in web server exists (`src/lib/ethernet/webServer/WebServer.h`) activated via `OPERATIONAL_MODE` and `WEB_SERVER != OFF`
- TMC-series drivers support `IRUN`, `IHOLD`, `IGOTO` in milliamps in `Config.h`
- `SERIAL_A_BAUD_DEFAULT` controls primary serial baud rate (default `9600`)
- `ALIGN_MAX_STARS` defaults to `AUTO` (6 or 9 stars depending on HAL)
- INDI and ASCOM drivers exist for OnStep in their respective ecosystems
- Backlash values (`AXIS1/2_BACKLASH`) are stored in NV and adjustable at runtime
- Mount position memory (`MOUNT_COORDS_MEMORY`) requires FRAM NV and is `OFF` by default

---

## Assumptions

The following are reasonable assumptions not yet confirmed by hardware inspection:

- EQ4 motors are bipolar stepper motors (common in this class of mount)
- Existing dual-axis motor kit uses standard worm gear reduction
- Existing driver uses or can be replaced by a STEP/DIR-compatible driver
- 1.8° step angle (standard for most NEMA-class steppers)
- A 4-wire bipolar connection will be used
- 12V motor supply voltage is appropriate

---

## Next Actions Before Hardware Access

1. **Create placeholder config files** in `config/motors/`, `config/drivers/`, `config/transport/`, `config/system/`
2. **Create `firmware/onstepx/Config.h`** as a template with all EQ4-relevant symbols and `unknown`/`OFF` for hardware-dependent values
3. **Update `docs/RISKS_AND_ASSUMPTIONS.md`** with the numbered unknowns from Section 6
4. **Add hardware inspection checklist** to `docs/PHASE_PLAN.md` or `docs/HARDWARE_INVENTORY.md`
5. **Record decisions** in `docs/DECISIONS.md`:
   - `MOUNT_TYPE = GEM` confirmed
   - USB serial at `115200` as initial transport
   - SWS used for Phase 1–2 validation
6. **Do not flash OnStepX** until motor type, wiring, and driver are confirmed

---

*Document generated: 2026-04-12. Based on OnStepX repo `hjd1964/OnStepX` main branch and AstroPilot repo state at same date.*
