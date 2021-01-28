all: RF24Midi.o

RF24Midi.o: RF24Midi.cpp RF24Midi.h
	g++ RF24Midi.cpp -c -o RF24Midi.o

.PHONY: clean

clean:
	rm -f *.o