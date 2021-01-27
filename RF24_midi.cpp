#include "RF24_midi.h"

RF24Midi::RF24Midi(uint16_t ce_pin, uint16_t csn_pin, uint64_t midiOutPipe, uint64_t midiInPipe) 
: radio(ce_pin, csn_pin), midiOutPipe(midiOutPipe), midiInPipe(midiInPipe) {}

bool RF24Midi::begin()
{
    if(!radio.begin())
        return false;
    
    radio.enableDynamicPayloads();
    radio.setRetries(5, 15);
    radio.openWritingPipe(midiOutPipe);
    radio.openReadingPipe(1, midiInPipe);
    radio.startListening();

    return true;
}

void RF24Midi::noteOn(uint8_t channel, uint8_t note, uint8_t velocity)
{
    if(channel > 15)
        return;
    uint8_t message[] = {(uint8_t)(0b10010000 | channel), note, velocity};
    radio.stopListening();
    radio.write(message, sizeof(message));
    radio.startListening();
}

void RF24Midi::noteOff(uint8_t channel, uint8_t note, uint8_t velocity)
{
    if(channel > 15)
        return;
    uint8_t message[] = {(uint8_t)(0b10000000 | channel), note, velocity};
    radio.stopListening();
    radio.write(message, sizeof(message));
    radio.startListening();
}

void RF24Midi::cc(uint8_t channel, uint8_t controller, uint8_t value)
{
    if(channel > 15)
        return;
    uint8_t message[] = {(uint8_t)(0b10110000 | channel), controller, value};
    radio.stopListening();
    radio.write(message, sizeof(message));
    radio.startListening();
}

void RF24Midi::pc(uint8_t channel, uint8_t program)
{
    if(channel > 15)
        return;
    uint8_t message[] = {(uint8_t)(0b11000000 | channel), program};
    radio.stopListening();
    radio.write(message, sizeof(message));
    radio.startListening();
}

void RF24Midi::setCCCallback(void (*ccCallback)(uint8_t, uint8_t, uint8_t))
{
    this->ccCallback = ccCallback;
}

void RF24Midi::loop()
{
    if(radio.available()) {
        char buffer[32];
        auto len = radio.getDynamicPayloadSize();
        if(!len)
            return;
        radio.read(buffer, len);

        uint8_t message = buffer[0] >> 4;
        uint8_t channel = buffer[0] & 0b1111;
        switch(message) {
            case 0b1011:
                if(ccCallback != nullptr)
                    ccCallback(channel, buffer[1], buffer[2]);
                break;
        }
    }
}

void RF24Midi::delay(unsigned long ms)
{
    auto tStart = millis();
    while(millis() - tStart < ms)
        loop();
}