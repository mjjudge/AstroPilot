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

## 2026-04-12

AstroPilot will treat OnStepX as the initial motion backend, not as an optional convenience layer. That matches your architecture and avoids wasted effort duplicating core motion logic. OnStepX is explicitly designed as a computerized telescope controller and supports multiple driver styles, including Step/Dir.


