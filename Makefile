# Makefile for Smart Parking Management System
# Supports both Linux and Windows (MSYS2/MinGW)

CC = gcc
CFLAGS = -Wall -Wextra -std=c11 -g
GTK_CFLAGS = $(shell pkg-config --cflags gtk+-3.0)
GTK_LIBS = $(shell pkg-config --libs gtk+-3.0)

# MySQL support (optional, enable with: make ENABLE_MYSQL=1)
ifdef ENABLE_MYSQL
CFLAGS += -DENABLE_MYSQL
MYSQL_LIBS = -lmysqlclient
endif

TARGET = smart_parking
SRCDIR = src
SOURCES = $(SRCDIR)/main.c \
          $(SRCDIR)/models.c \
          $(SRCDIR)/file_handling.c \
          $(SRCDIR)/car_entry_exit.c \
          $(SRCDIR)/ui.c

# Add MySQL source if enabled
ifdef ENABLE_MYSQL
SOURCES += $(SRCDIR)/mysql_integration.c
endif

OBJECTS = $(SOURCES:.c=.o)

.PHONY: all clean run install-deps help

all: $(TARGET)

$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -o $(TARGET) $(GTK_LIBS) $(MYSQL_LIBS)

$(SRCDIR)/%.o: $(SRCDIR)/%.c
	$(CC) $(CFLAGS) $(GTK_CFLAGS) -c $< -o $@

clean:
	rm -f $(OBJECTS) $(TARGET) $(TARGET).exe

run: $(TARGET)
	./$(TARGET)

help:
	@echo "Smart Parking Management System - Build System"
	@echo ""
	@echo "Targets:"
	@echo "  make          - Build the application"
	@echo "  make clean    - Remove build files"
	@echo "  make run      - Build and run the application"
	@echo "  make ENABLE_MYSQL=1 - Build with MySQL support"
	@echo ""
	@echo "Requirements:"
	@echo "  - GTK+ 3.0 development libraries"
	@echo "  - pkg-config"
	@echo "  - GCC compiler"
	@echo "  - (Optional) MySQL client library for MySQL support"
