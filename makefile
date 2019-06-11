CC:= g++
exe:= myPolygon
obj:= main.o

all: $(obj)
	$(CC) -o $(exe) $(obj)

$(obj): main.cpp myPolygon.h
	$(CC) -c main.cpp

.PHONY: clean
clean:
	rm -rf $(obj) $(exe)
