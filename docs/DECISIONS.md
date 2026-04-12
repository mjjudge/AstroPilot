# Decisions

## 2026-04-11
- Use OnStepX on ESP32 for motion control
- Use Raspberry Pi for UI + orchestration
- Use browser-based UI (iPad target)
- Use 12V primary power rail
- Original hand controller will not be reused

## 2026-04-12 — Configuration-Driven Architecture

AstroPilot will be a configuration-driven platform.

All mount-specific behaviour must be defined via configuration where possible, including:

- mount geometry
- motor characteristics
- driver interfaces
- transport mechanisms

The EQ4 implementation is treated as a reference configuration, not a hardcoded system.

No core platform logic should depend directly on EQ4-specific assumptions.