# Define the compiler
compiler = gcc

# Define compiler flags
CFLAGS = -Wall -g

# Define the target executable
TARGET = main

# List of source files
source = main.c

# Default rule to build the target
all: $(TARGET)

# Rule to build the target executable
$(TARGET): $(sourse)
	$(compiler) $(CFLAGS) -o $(TARGET) $(source)

# Rule to clean up generated files
clean:
	rm -f $(TARGET)

.PHONY: all clean
