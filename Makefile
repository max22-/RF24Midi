all: RF24Midi.o

RF24Midi.o: RF24Midi.cpp RF24Midi.h
	g++ RF24Midi.cpp -o RF24Midi.o