# Compiler
CXX = g++

# Target executable name
TARGET = reader

# Source files
SRCS = reader.cpp

# OpenCV flags
OPENCV_FLAGS = `pkg-config --cflags --libs opencv4`

# If opencv4 doesn't work, try opencv
ifeq ($(shell pkg-config --exists opencv4 && echo yes),)
    OPENCV_FLAGS = `pkg-config --cflags --libs opencv`
endif

# Compiler flags
CXXFLAGS = -std=c++11 -Wall

# Build target
$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) $(SRCS) -o $(TARGET) $(OPENCV_FLAGS)

# Clean build files
clean:
	rm -f $(TARGET) *.o

# Run the program
run: $(TARGET)
	./$(TARGET)

.PHONY: clean run