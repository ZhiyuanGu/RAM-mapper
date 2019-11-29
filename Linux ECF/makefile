# Makefile for ECE1756 assignment3 RAM mapper - Zhiyuan Gu(1004920400)

CC := g++
RM := rm -f

CFLAGS := -Wall
LDFLAGS :=
LDLIBS := 

OBJFILES := main.o RAM_mapper.o Architecture.o Circuit.o logicRAM.o Solution.o 
TARGET := RM

all: $(TARGET)

$(TARGET): $(OBJFILES)
	$(CC) $(CFLAGS) $(LDLIBS) -o $(TARGET) $(OBJFILES) $(LDFLAGS)

clean:
	$(RM) $(OBJFILES) $(TARGET)
