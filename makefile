# Define compiler and flags
CXX = g++
CXXFLAGS = -std=c++11 -Wall -g
# g for debug

# Define source and object file paths
SRCDIR = src
OBJDIR = obj
BINDIR = .

# Create obj directory if it doesn't exist
$(shell mkdir -p $(OBJDIR))

# Define source and object files
SOURCES := $(wildcard $(SRCDIR)/*.cpp)
OBJECTS := $(patsubst $(SRCDIR)/%.cpp,$(OBJDIR)/%.o,$(SOURCES))

# Define output executable name
EXECUTABLE = chip8

# Build executable
all: $(BINDIR)/$(EXECUTABLE)

$(BINDIR)/$(EXECUTABLE): $(OBJECTS)
	$(CXX) $(CXXFLAGS) $^ -o $@

# Build object files from source files
$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Dependency rules
$(OBJDIR)/%.o: $(SRCDIR)/%.cpp $(SRCDIR)/%.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

# Clean up object files and executable
clean:
	rm -f $(OBJDIR)/*.o $(BINDIR)/$(EXECUTABLE)
