
# SDL2 Library
LIBS = -lSDL2 -lSDL2_ttf

# Output binary name
OUTPUT = test

CFLAGS = -Wall

# Automatically find all .c files in the current directory
SRC = $(wildcard *.c)

# Object files corresponding to source files
OBJ = $(SRC:.c=.o)

# Default target
all: $(OUTPUT)

# Link the object files to create the executable
$(OUTPUT): $(OBJ)
	$(CC) $(OBJ) -o $(OUTPUT) $(LIBS)

# Compile each .c file into a .o file
%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

# Clean up build files
clean:
	rm -f $(OBJ) $(OUTPUT)

# Phony targets
.PHONY: all clean