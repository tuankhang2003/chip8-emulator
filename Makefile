# ---------------------------------------
# CONFIGURATION BEGIN
# ---------------------------------------

#compile: 
#> g++ -Isrc/include -Lsrc/lib -o main chip8.cpp platform.cpp  main.cpp -lmingw32 -lSDL2main -lSDL2

# entry point for the program and target name
MAIN := main.cpp

# assignment task file
PLATFORM := platform.cpp
CHIP:= chip8.cpp

# Additional flags for the compiler
CFLAGS := `sdl2-config --cflags --libs` -Isrc/include -Lsrc/lib  -lmingw32 -lSDL2main -lSDL2 

# ---------------------------------------
# CONFIGURATION END
# ---------------------------------------

# Determine if clang or gcc is available
CC := $(shell command -v g++ || command -v clang)
ifeq ($(strip $(CC)),)
    $(error Neither clang nor gcc is available. Exiting.)
endif

# all source files needed for the program
SOURCES := $(PLATFORM)  $(CHIP)  $(MAIN)
TARGET := $(basename $(MAIN))

# Default to release build for both app and library
all: debug

# Debug build
debug: CFLAGS += -g
debug: $(TARGET)

# Release build
release: CFLAGS += -O2
release: $(TARGET)

# recipe for building the program
$(TARGET): $(SOURCES)
	$(CC) $(CFLAGS) -o $@ $(SOURCES)

# clean up
clean:
	rm -f $(TARGET)

.PHONY: all debug release clean