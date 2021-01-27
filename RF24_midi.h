#ifndef RF24_MIDI_H
#define RF24_MIDI_H

#ifdef ARDUINO
    #include <Arduino.h>
#else
    #include <cstdint>
#endif
#include <RF24.h>

class RF24Midi {
public:
    RF24Midi(uint16_t ce_pin, uint16_t csn_pin, uint64_t midiOutPipe, uint64_t midiInPipe);
    bool begin();

    void noteOn(uint8_t channel, uint8_t note, uint8_t velocity);
    void noteOff(uint8_t channel, uint8_t note, uint8_t velocity);
    void cc(uint8_t channel, uint8_t controller, uint8_t value);
    void pc(uint8_t channel, uint8_t program);

    void setCCCallback(void (*ccCallback)(uint8_t, uint8_t, uint8_t));

    /**
     * Handles radio events
     * */
    void loop();
    /**
     * Makes a delay while continuing to handle radio events
     * */
    void delay(unsigned long ms);


private:
    RF24 radio;
    uint64_t midiOutPipe, midiInPipe;

    void (*noteOnCallback)(uint8_t channel, uint8_t note, uint8_t velocity) = nullptr;
    void (*noteOffCallback)(uint8_t channel, uint8_t note, uint8_t velocity) = nullptr;
    void (*ccCallback)(uint8_t channel, uint8_t controller, uint8_t value) = nullptr;
    void (*pcCallback)(uint8_t channel, uint8_t program) = nullptr;

    
};

#endif