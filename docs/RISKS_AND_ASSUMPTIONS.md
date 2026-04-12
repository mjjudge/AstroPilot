# Risks and Assumptions

See `docs/ONSTEPX_TECHNICAL_BRIEF.md` Section 6 for the full, numbered risk table.

## Hardware Unknowns (Block OnStepX Configuration)

| # | Unknown | Why it blocks |
|---|---|---|
| 1 | Motor type (stepper / DC / unipolar) | Determines driver compatibility |
| 2 | Wire count per motor | Identifies motor winding type |
| 3 | Motor step angle | Required for `AXIS1_STEPS_PER_DEGREE` |
| 4 | RA gearbox ratio | Required for `AXIS1_STEPS_PER_DEGREE` |
| 5 | DEC gearbox ratio | Required for `AXIS2_STEPS_PER_DEGREE` |
| 6 | Motor rated current | Required for driver current setting (TMC) |
| 7 | Motor rated voltage | Affects driver and supply choice |
| 8 | Existing driver model | Determines `AXIS1_DRIVER_MODEL` |
| 9 | Driver interface type | Must support STEP/DIR |
| 10 | Connector pinout | Required for safe wiring |
| 11 | ESP32 board variant | Required for `PINMAP` in Config.h |
| 12 | Worm rotation count | Required for PEC configuration |

## Status

All items above are blocked until Phase 0 hardware inspection.
Do **not** power any driver with unknown motor attached.
