# Compiler and flags
CXX = g++
CXXFLAGS = -pthread -g

# Directories
BIN_DIR = bin
ASSETS_DIR = assets

# Executables
TARGETS = $(BIN_DIR)/dataGenBlock.exe $(BIN_DIR)/filterAndThresholdBlock.exe $(ASSETS_DIR)/csvGen.exe lineCam.exe

# Source files
DATAGEN_SRC = dataGenBlock.cpp
FILTER_SRC = filterAndThresholdBlock.cpp
CSVGEN_SRC = assets/csvGen.cpp
LINECAM_SRC = lineCamera.cpp

# Default rule
all: $(BIN_DIR) $(TARGETS)

# Create bin directory if it doesn't exist
$(BIN_DIR):
	mkdir $(BIN_DIR)

# Build rules
$(BIN_DIR)/dataGenBlock.exe: $(DATAGEN_SRC)
	$(CXX) $(CXXFLAGS) -o $@ $(DATAGEN_SRC)

$(BIN_DIR)/filterAndThresholdBlock.exe: $(FILTER_SRC)
	$(CXX) $(CXXFLAGS) -o $@ $(FILTER_SRC)

$(ASSETS_DIR)/csvGen.exe: $(CSVGEN_SRC)
	$(CXX) $(CXXFLAGS) -o $@ $(CSVGEN_SRC)

lineCam.exe: $(LINECAM_SRC)
	$(CXX) $(CXXFLAGS) -o $@ $(LINECAM_SRC)

# Clean rule
clean:
	-del /Q $(BIN_DIR)\*.exe $(ASSETS_DIR)\*.exe lineCam.exe 2>nul

.PHONY: all clean