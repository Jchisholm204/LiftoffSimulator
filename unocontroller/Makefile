# .PHONY tells 'make' that these targets are commands, not files
.PHONY: all build clean flash upload monitor compiledb init

# Default target: runs when you simply type 'make'
all: build

# 1. BUILD: Compile the project
build:
	pio run

# 2. FLASH: Compile and upload to the board
# (PlatformIO uses 'upload', but we alias it to 'flash' for you)
flash:
	pio run --target upload

# 3. CLEAN: Remove build files
clean:
	pio run --target clean

# --- BONUS TARGETS ---

# MONITOR: Open the serial monitor
monitor:
	pio device monitor

# COMPILEDB: Generate compile_commands.json for Neovim LSP
# (Run this whenever you add a new library)
compiledb:
	pio run --target compiledb

# UPDATE: Update project libraries and platform
update:
	pio pkg update

# INIT: Initialize a new project (interactive)
init:
	pio project init
