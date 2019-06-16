CC:= g++
CFLAGS:= -Wall -O3 
exe:= myPolygon
obj:= main.o myPolygon.o merge.o clip.o split.o

all: $(obj)
	$(CC) -o $(exe) $(obj)

main.o: main.cpp myPolygon.h
	$(CC) $(CFLAGS) -c main.cpp myPolygon.h

myPoligon.o: myPolygon.cpp myPoligon.h
	$(CC) $(CFLAGS) -c myPolygon.cpp myPolygon.h

merge.o: merge.cpp myPolygon.h
	$(CC) $(CFLAGS) -c merge.cpp myPolygon.h

clip.o: clip.cpp myPolygon.h
	$(CC) $(CFLAGS) -c clip.cpp myPolygon.h

split.o: split.cpp myPolygon.h
	$(CC) $(CFLAGS) -c split.cpp myPolygon.h

.PHONY: clean
clean:
	rm -rf $(obj) $(exe)
