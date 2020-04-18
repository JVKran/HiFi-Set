#ifndef __VS1053_HPP
#define __VS1053_HPP

#include <Arduino.h>
#include <SPI.h>

class VS1053 {
private:
    uint8_t csPin;
    uint8_t dcsPin;
    uint8_t dreqPin;
    uint8_t currentVolume;
    int8_t  currentBalance = 0;

    const uint8_t chunkSize = 32;
    
    SPISettings SlaveSettings;
    uint8_t endFillByte;

    enum class registers : uint8_t {
        MODE            = 0x0,
        STATUS          = 0x1,
        BASS            = 0x2,
        CLOCKF          = 0x3,
        DECODE_TIME     = 0x4,
        AUDATA          = 0x5,
        WRAM            = 0x6,
        WRAMADDR        = 0x7,
        AIADDR          = 0xA,
        VOL             = 0xB,
        AICTRL0         = 0xC,
        AICTRL1         = 0xD,
        NUM_REG         = 0xF,
    };
    
    enum class bits : uint8_t {
        SDINEW          = 11,
        RESET           = 2,
        CANCEL          = 3,
        TESTS           = 5,
        LINE1           = 14,
    };


    inline void awaitDataRequest() const {
        while (!digitalRead(dreqPin)) {
            yield();                        // Very short delay
        }
    }

    inline void controlModeOn() const {
        SPI.beginTransaction(SlaveSettings);   // Prevent other SPI users
        digitalWrite(dcsPin, HIGH);        // Bring slave in control mode
        digitalWrite(csPin, LOW);
    }

    inline void controlModeOff() const {
        digitalWrite(csPin, HIGH);         // End control mode
        SPI.endTransaction();               // Allow other SPI users
    }

    inline void dataModeOn() const {
        SPI.beginTransaction(SlaveSettings);   // Prevent other SPI users
        digitalWrite(csPin, HIGH);         // Bring slave in data mode
        digitalWrite(dcsPin, LOW);
    }

    inline void dataModeOff() const {
        digitalWrite(dcsPin, HIGH);        // End data mode
        SPI.endTransaction();               // Allow other SPI users
    }

    uint16_t readRegister(uint8_t registerToRead) const;
    void writeRegister(uint8_t registerToWrite, uint16_t value) const;

    void sdiSendBuffer(uint8_t *data, size_t len);
    void sdiSendFiller(size_t length);

    void wramWrite(uint16_t address, uint16_t data);
    uint16_t wramRead(uint16_t address);

public:
    VS1053(uint8_t csPin, uint8_t dcsPin, uint8_t dreqPin);

    void begin();
    void switchToMp3Mode();

    bool isChipConnected();
    void softReset();

    void startSong();
    void stopSong();
    void playChunk(uint8_t *data, size_t len);

    void setVolume(uint8_t volume);
    uint8_t getVolume();

    void setBalance(int8_t balance);
    int8_t getBalance();
    void setTone(uint8_t *rtone);

    void setBass(uint8_t bassEnhancement){
        if(bassEnhancement > 15){
            bassEnhancement = 15;
        }
        writeRegister((uint8_t)registers::BASS, bassEnhancement << 4);
    }

    void setTreble(uint8_t trebleEnhancement){
        if(trebleEnhancement > 8){
            trebleEnhancement = 8;
        }
        writeRegister((uint8_t)registers::BASS, trebleEnhancement << 12);
    }

    uint16_t getSeconds();
    void clearSeconds();

    inline bool dataRequest() const {
        return (digitalRead(dreqPin) == HIGH);
    }

};

#endif //__VS1053_HPP