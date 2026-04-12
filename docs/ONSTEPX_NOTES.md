# OnStepX Notes

See `docs/ONSTEPX_TECHNICAL_BRIEF.md` for the full repository-grounded technical brief.

## Quick Reference

- OnStepX repo: https://github.com/hjd1964/OnStepX
- Primary config file: `Config.h` (compile-time `#define` settings)
- Command protocol: LX200 (ASCII, serial or TCP)
- ESP32 WiFi: enabled via `SERIAL_RADIO = WIFI_ACCESS_POINT` or `WIFI_STATION`
- Built-in web UI: Smart Web Server (SWS), requires `OPERATIONAL_MODE` ≥ Ethernet/WiFi
- Mount type for EQ4: `MOUNT_TYPE = GEM`
- Steps per degree formula: `(stepper_steps × microsteps × gear_reduction) / 360.0`
- RA axis: `AXIS1_*`, DEC axis: `AXIS2_*`
- Motor current (TMC drivers): `AXIS1_DRIVER_IRUN`, `AXIS1_DRIVER_IHOLD`

## Status

All configuration values that depend on hardware (motor type, step angle, gear ratio, driver model, current)
are blocked until Phase 0 hardware inspection. See `docs/RISKS_AND_ASSUMPTIONS.md`.