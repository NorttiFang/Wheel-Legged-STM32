# Changelog

## v2026.04.12

- Added the experimental `application/MPC.c` and `application/MPC.h` module, and included `MPC.c` in the Keil project.
- Refreshed `README.md` to better describe the current controller architecture, project layout, and modeling workflow.
- Added `.gitignore` rules for common Keil and Simulink cache/output files.
- Added new leg-modeling files:
  - `leg/balance_leg_sl.slx`
  - `leg/HerKules_VOCAL_SJ_LQR_v4_with_data.m`
  - `leg/HerKules_legth_K.m`
- Preserved existing local cache files outside this release scope, including `leg/slprj/...` temporary outputs.
