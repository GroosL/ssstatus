# ssstatus

`ssstatus` is a small C utility that prints a compact system status line to standard output. It is designed to be used as a status bar on tiling window manager setups or terminal environments where a single line of live information is enough. It is heavily inspired by `dwmblocks`, taking the same minimal “one-line status output” philosophy and the delayed update with manual updating via signals while staying portable and easy to customize.

The program updates:

- CPU usage
- memory/RAM usage
- battery percentage
- custom user provided shell command outputs
- current time

It refreshes on a timer and can also be nudged via signals to update immediately.

## Features

- lightweight, single-process stdout status reporter
- configurable update intervals
- custom command output slots
- generated status line suitable for status bars or shell scripts

## Build

From the project root:

```bash
make
```

This compiles the binary as `ssstatus`.

To rebuild with debugging symbols:

```bash
make debug
```

To remove generated build artifacts:

```bash
make clean
```

## Run

```bash
./ssstatus
```

### Example on dwl
```bash
ssstatus | dwl
```

The program continuously writes a line like this to stdout:

```text
<custom1> | <custom2> | <custom3> | BAT: 82.00% | RAM: 48.12% | CPU: 12.34% | Sat, Aug 22 | 15:42:18
```

## Configuration

The build system generates `config.h` from `config.def.h` before compiling:

```make
config.h: config.def.h
	cp config.def.h config.h
```

This means you usually edit `config.def.h` to change:

- which modules are enabled
- their update intervals
- which signal each module reacts to
- the output format string
- custom command entries

The default configuration includes:

- CPU module
- RAM module
- battery module
- three custom command slots

## Signals

The program registers handlers for the configured module/custom-command signals. When a signal is received, that item is marked for refresh immediately rather than waiting for its interval.

Common signals used by the project include:

- `SIGUSR1` for CPU
- `SIGUSR2` for RAM
- `SIGPWR` for battery
- custom signal `34` for command refreshes (please note that on OpenBSD the signals range from 1-31 instead of the usual 1-64 because the system chooses not to implement RT signals)

## Project layout

- `main.c` — main event loop and signal handling
- `config.def.h` — configurable module list and formatting
- `config.h` — generated config used at build time
- `assets/` — module implementations and helper code
- `Makefile` — build rules

## Notes

This project is intentionally minimal and focused on outputting a status line to stdout. It draws heavy inspiration from `dwmblocks`, but keeps a simpler, self-contained C implementation. It is useful as a base for custom status bars, desktop widgets, or scripts that want lightweight live system information without a larger framework.
