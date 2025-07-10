# Compiler and flags
CXX = g++
CXXFLAGS = -pthread -g

# Directories
BIN_DIR = bin

# Executables
TARGETS = $(BIN_DIR)/dataGenBlock.exe $(BIN_DIR)/filterAndThresholdBlock.exe

# Source files
DATAGEN_SRC = dataGenBlock.cpp
FILTER_SRC = filterAndThresholdBlock.cpp

# Default rule
all: $(BIN_DIR) $(TARGETS)

# Create bin directory if it doesn't exist
$(BIN_DIR):
	mkdir $(BIN_DIR)


$(BIN_DIR)/dataGenBlock.exe: $(DATAGEN_SRC)
	$(CXX) $(CXXFLAGS) -o $@ $(DATAGEN_SRC)

$(BIN_DIR)/filterAndThresholdBlock.exe: $(FILTER_SRC)
	$(CXX) $(CXXFLAGS) -o $@ $(FILTER_SRC)

# Clean up build files
clean:
	del /Q $(BIN_DIR)\*.exe 2>nul || true

.PHONY: all clean
