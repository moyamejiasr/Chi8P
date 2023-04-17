# Define compiler and flags
CXX = g++
CXXFLAGS = -std=c++11 -Wall -g `sdl2-config --cflags`
LDFLAGS = `sdl2-config --libs`

# Define source and object file paths
SRCDIR = src
OBJDIR = obj
BINDIR = .
TEST_SRCDIR = test
TEST_OBJDIR = $(OBJDIR)/test

# Create obj directory if it doesn't exist
$(shell mkdir -p $(OBJDIR))
$(shell mkdir -p $(TEST_OBJDIR))

# Define source and object files
SOURCES := $(wildcard $(SRCDIR)/*.cpp)
OBJECTS := $(patsubst $(SRCDIR)/%.cpp,$(OBJDIR)/%.o,$(SOURCES))

# Define test source and object files
TEST_SOURCES := $(wildcard $(TEST_SRCDIR)/*.cpp)
TEST_OBJECTS := $(patsubst $(TEST_SRCDIR)/%.cpp,$(TEST_OBJDIR)/%.o,$(TEST_SOURCES))

# Define output executable names
EXECUTABLE = chip8
TEST_EXECUTABLE = chip8_test

# Define main object file
MAIN_OBJECT = $(OBJDIR)/main.o

# Define non-main object files
NON_MAIN_OBJECTS = $(filter-out $(MAIN_OBJECT),$(OBJECTS))

# Build executable
all: $(BINDIR)/$(EXECUTABLE)

test: $(BINDIR)/$(TEST_EXECUTABLE)
	$(BINDIR)/$(TEST_EXECUTABLE)

$(BINDIR)/$(EXECUTABLE): $(OBJECTS)
	$(CXX) $(CXXFLAGS) $^ -o $@ $(LDFLAGS)

$(BINDIR)/$(TEST_EXECUTABLE): $(NON_MAIN_OBJECTS) $(TEST_OBJECTS)
	$(CXX) $(CXXFLAGS) $^ -o $@ -L./lib -lgtest -lpthread $(LDFLAGS)

# Build object files from source files
$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Build object files from test source files
$(TEST_OBJDIR)/%.o: $(TEST_SRCDIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Dependency rules
$(OBJDIR)/%.o: $(SRCDIR)/%.cpp $(SRCDIR)/%.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean up object files and executables
clean:
	rm -f $(OBJDIR)/*.o $(TEST_OBJDIR)/*.o $(BINDIR)/$(EXECUTABLE) $(BINDIR)/$(TEST_EXECUTABLE)
