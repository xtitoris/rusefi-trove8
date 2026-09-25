# Board-specific pin definition files

## Custom pin naming (rusEFI PinoutLogic)
`custom_firmware.yaml` drives popular-TS pin naming. It is consumed by
`PinoutLogic.java`, which regenerates `generated_ts_name_by_pin.cpp`,
`generated_outputs.h` and `generated_board_pin_names.h` in this folder.
Name all pins you want to see by their TS name here. Only pins with an `id`
and `class` are picked up; the interactive-pinout fields (`pin`, `type`,
`function`, `color`) are ignored by PinoutLogic.

## Interactive pinout (chuckwagoncomputing/interactive-pinout)
`trove8_j1.yaml` (input connector) and `trove8_j2.yaml` (output connector)
define the full Delphi Sicma 24-pin mapping for the generated interactive
pinout page. They are consumed by the `Generate Interactive Pinouts` workflow
(`.github/workflows/gen-pinouts.yaml`), whose `mapping-path` only matches
`connectors/trove8_*.yaml` so `custom_firmware.yaml` is not parsed as a
connector definition.

Documentation: https://github.com/chuckwagoncomputing/interactive-pinout

### Connector photo + pin coordinates
- `connector.png` is the board photo used as hotspot background (1662x599 px).
- The `info.image.pins` subsection holds an x/y coordinate (pixels, origin
  top-left) for every physical pin. These are currently placeholders (`0,0`);
  measure the real pin positions on `connector.png` in an image editor and
  update both YAML files before running the workflow.

### Regeneration
Run the `Generate Interactive Pinouts` workflow manually (Actions ->
workflow_dispatch). Output lands in the `pinouts` folder and is optionally
uploaded via SSH using the `RUSEFI_SSH_*` secrets.