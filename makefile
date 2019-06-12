CC:= g++
CFLAGS:= -Wall -g 
exe:= myPolygon
obj:= main.o myPolygon.o

all: $(obj)
	$(CC) -o $(exe) $(obj)

$(obj): main.cpp myPolygon.cpp myPolygon.h
	$(CC) $(CFLAGS) -c main.cpp myPolygon.cpp

.PHONY: clean
clean:
	rm -rf $(obj) $(exe)
