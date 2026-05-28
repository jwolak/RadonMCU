# NiosLoggerDemo

A minimal application showing how to use the EquiniosLogger library on Nios II.

## What This Demo Shows

- log level configuration,
- timestamp provider setup,
- `log_process` interval configuration,
- periodic logging with `LOGD/LOGW/LOGE` and buffer flushing.

## Requirements

- Nios II EDS in PATH (`nios2-app-generate-makefile`, `nios2-elf-gcc`, `make`),
- `SOPC_KIT_NIOS2` environment variable set,
- generated BSP (default: `SoftwareExamplesForRadonMCU/RadonHelloWorld_bsp`),
- built FPGA hardware image (`.sof`) for the target system.

## Quick Start

From this example directory:

```bash
./create-this-app
```

The script:

1. builds `../../libEquiniosLogger.a`,
2. generates the application Makefile,
3. builds the ELF file.

## Running On Hardware

```bash
nios2-configure-sof <path-to-sof>
nios2-terminal
make download-elf
```

After starting, you should see periodic logger output in the JTAG terminal.

## Useful Variables

You can override default paths before running the script:

- `BSP_DIR` - BSP directory,
- `QUARTUS_PROJECT_DIR` - Quartus project directory,
- `ELF_NAME` - output ELF file name.
