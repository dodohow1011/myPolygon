CC:= g++
CFLAGS:= -Wall -g 
exe:= myPolygon
obj:= main.o myPolygon.o merge.o clip.o split.o

all: $(obj)
	$(CC) -o $(exe) $(obj)

$(obj): main.cpp myPolygon.cpp merge.cpp clip.cpp split.cpp myPolygon.h
	$(CC) $(CFLAGS) -c main.cpp myPolygon.cpp merge.cpp clip.cpp split.cpp

.PHONY: clean
clean:
	rm -rf $(obj) $(exe)
