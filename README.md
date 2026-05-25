# RadonMCU
Experimental MCU (Microcontroller Unit)

RadonMCU is an FPGA-based soft-core microcontroller platform built around Intel Nios II.
The repository includes both hardware design (Quartus + Platform Designer) and software
examples (Nios II SBT/Eclipse) used to validate the system on real hardware.

## Project overview

Current platform profile (based on generated BSP):
- Device family: Intel MAX 10
- CPU: Nios II Gen2 (tiny implementation)
- CPU clock: 50 MHz
- On-chip RAM: 32 KB
- Console I/O: JTAG UART

This project is intended for iterative hardware/software co-design:
- modify the SoC in Platform Designer,
- regenerate HDL and compile in Quartus,
- regenerate BSP and rebuild software examples.

## Repository structure

- RadonMCU.qpf, RadonMCU.qsf: top-level Quartus project files
- RadonMCUSystemDesign/: Platform Designer system source and generated HDL wrapper files
- SoftwareExamplesForRadonMCU/: Nios II software workspace
- SoftwareExamplesForRadonMCU/RadonHelloWorld/: example application
- SoftwareExamplesForRadonMCU/RadonHelloWorld_bsp/: generated BSP for the application

## Toolchain requirements

- Intel Quartus Prime Lite 18.1 (or compatible with this project)
- Nios II EDS / Nios II SBT for Eclipse
- Device support for MAX 10

Important:
- Build software from the Nios II Command Shell or Nios II Eclipse environment.
- Running plain system shells without Nios II environment may fail due to missing tools
	such as make, nios2-elf-gcc, and nios2-download in PATH.

## Build workflow

### 1. Hardware build

1. Open project in Quartus using RadonMCU.qpf.
2. Open Platform Designer and update the system if needed.
3. Generate HDL from Platform Designer.
4. Compile the Quartus project.

### 2. Software build

1. Open SoftwareExamplesForRadonMCU in Nios II SBT for Eclipse.
2. Regenerate BSP for RadonHelloWorld_bsp after hardware changes.
3. Clean and build RadonHelloWorld_bsp.
4. Clean and build RadonHelloWorld.
5. Run download-elf target or debug configuration to program and start the app.

Expected output from sample app:
- Hello from Nios II!

## Common issue: ELF not generated

If RadonHelloWorld.elf is not created and linker reports:
- section .bss is not within region RAM

then software image does not fit into available RAM.

Typical mitigations:
- reduce BSP footprint (for example lower file descriptor count, enable small C library),
- reduce application memory usage,
- increase on-chip memory in hardware design.

After changing BSP settings, regenerate BSP and rebuild both BSP and app.

## Notes about generated files

The repository tracks key design sources and selected generated artifacts needed for
reproducible builds. Temporary editor/build outputs are ignored through .gitignore
(for example local .qsys_edit cache and software build outputs).

## License

BSD 3-Clause License
