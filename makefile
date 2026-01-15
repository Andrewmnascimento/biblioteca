CC = clang
CFLAGS = -Wall -Wextra -std=c99 -MMD -MP
TARGET = biblioteca
SOURCES = biblioteca.c
OBJECTS = $(SOURCES:.c=.o)
DEPENDENCIES = $(OBJECTS:.o=.d)

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(CFLAGS) -o $(TARGET) $(OBJECTS)

%.o: %.c 
	$(CC) $(CFLAGS) -c $< -o $@

-include $(DEPENDENCIES)
clean:
	rm -f $(OBJECTS) $(TARGET) $(DEPENDENCIES)
 
run: $(TARGET)
	./$(TARGET)

.PHONY: all clean run