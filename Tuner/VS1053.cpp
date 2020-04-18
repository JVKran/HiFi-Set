#include "VS1053.hpp"

VS1053::VS1053(uint8_t csPin, uint8_t dcsPin, uint8_t dreqPin):
    csPin(csPin),
    dcsPin(dcsPin),
    dreqPin(dreqPin)
{
    pinMode(dreqPin, INPUT);
    pinMode(csPin, OUTPUT);
    pinMode(dcsPin, OUTPUT);
}

void VS1053::begin() {
    digitalWrite(dcsPin, HIGH);
    digitalWrite(csPin, HIGH);
    delay(100);
    digitalWrite(dcsPin, LOW);
    digitalWrite(csPin, LOW);
    delay(500);
    digitalWrite(dcsPin, HIGH);
    digitalWrite(csPin, HIGH);
    delay(500);
    SlaveSettings = SPISettings(200000, MSBFIRST, SPI_MODE0);                       // Initialize SPI in slow mode for initialization of peripherals.
    delay(20);
    writeRegister((uint8_t)registers::AUDATA, 44101);                               // Initialize DAC at 44.1kHz in stereo format.
    writeRegister((uint8_t)registers::CLOCKF, 6 << 12);
    SlaveSettings = SPISettings(4000000, MSBFIRST, SPI_MODE0);                      // Set SPI frequency to 4MHz
    writeRegister((uint8_t)registers::MODE, _BV((uint8_t)bits::SDINEW) | _BV((uint8_t)bits::LINE1));
    delay(10);
    awaitDataRequest();
    endFillByte = wramRead(0x1E06) & 0xFF;
    delay(100);
}

uint16_t VS1053::readRegister(uint8_t registerToRead) const {
    controlModeOn();
    SPI.write(3);
    SPI.write(registerToRead);
    uint16_t result = (SPI.transfer(0xFF) << 8) | (SPI.transfer(0xFF));
    awaitDataRequest();
    controlModeOff();
    return result;
}

void VS1053::writeRegister(uint8_t registerToWrite, uint16_t value) const {
    controlModeOn();
    SPI.write(2);
    SPI.write(registerToWrite);
    SPI.write16(value);
    awaitDataRequest();
    controlModeOff();
}

void VS1053::sdiSendBuffer(uint8_t *data, size_t length) {
    size_t chunkLength;

    dataModeOn();
    while (length){
        awaitDataRequest(); // Wait for space available
        chunkLength = length;
        if (length > chunkSize) {
            chunkLength = chunkSize;
        }
        length -= chunkLength;
        SPI.writeBytes(data, chunkLength);
        data += chunkLength;
    }
    dataModeOff();
}

void VS1053::sdiSendFiller(size_t length) {
    size_t chunkLength;

    dataModeOn();
    while (length){
        awaitDataRequest(); // Wait for space available
        chunkLength = length;
        if (length > chunkSize) {
            chunkLength = chunkSize;
        }
        length -= chunkLength;
        while (chunkLength--) {
            SPI.write(endFillByte);
        }
    }
    dataModeOff();
}

void VS1053::wramWrite(uint16_t address, uint16_t data) {
    writeRegister((uint8_t)registers::WRAMADDR, address);
    writeRegister((uint8_t)registers::WRAM, data);
}

uint16_t VS1053::wramRead(uint16_t address) {
    writeRegister((uint8_t)registers::WRAMADDR, address);
    return readRegister((uint8_t)registers::WRAM);
}


void VS1053::setVolume(uint8_t volume) {
    currentVolume = volume;
    uint8_t volumeLeft = volume;
    uint8_t volumeRight = volume;

    if (currentBalance < 0) {
        volumeRight = max(0, volume + currentBalance);
    } else {
        volumeLeft = max(0, volume - currentBalance);
    }

    volumeLeft = map(volumeLeft, 0, 100, 0xFE, 0x00);
    volumeRight = map(volumeRight, 0, 100, 0xFE, 0x00);

    writeRegister((uint8_t)registers::VOL, (volumeLeft << 8) | volumeRight);
}

void VS1053::setBalance(int8_t balance) {
    if (balance > 100) {
        currentBalance = 100;
    } else if (balance < -100) {
        currentBalance = -100;
    } else {
        currentBalance = balance;
    }
}

void VS1053::setTone(uint8_t *rtone) {
    uint16_t value = 0; 

    for (uint_fast8_t i = 0; i < 4; i++) {
        value = (value << 4) | rtone[i]; // Shift next nibble in
    }
    writeRegister((uint8_t)registers::BASS, value); // Volume left and right
}

uint8_t VS1053::getVolume() { // Get the currenet volume setting.
    return currentVolume;
}

int8_t VS1053::getBalance() { // Get the currenet balance setting.
    return currentBalance;
}

void VS1053::startSong() {
    sdiSendFiller(10);
}

void VS1053::playChunk(uint8_t *data, size_t len) {
    sdiSendBuffer(data, len);
}

void VS1053::stopSong() {
    uint16_t modeRegister; // Read from mode register

    sdiSendFiller(2052);
    delay(10);
    writeRegister((uint8_t)registers::MODE, _BV((uint8_t)bits::SDINEW) | _BV((uint8_t)bits::CANCEL));
    for (uint_fast8_t i = 0; i < 200; i++) {
        sdiSendFiller(32);
        modeRegister = readRegister((uint8_t)registers::MODE); // Read status
        if ((modeRegister & _BV((uint8_t)bits::CANCEL)) == 0) {
            sdiSendFiller(2052);
            return;
        }
        delay(10);
    }
}

void VS1053::softReset() {
    writeRegister((uint8_t)registers::MODE, _BV((uint8_t)bits::SDINEW) | _BV((uint8_t)bits::RESET));
    delay(10);
    awaitDataRequest();
}

void VS1053::switchToMp3Mode() {
    wramWrite(0xC017, 3);
    wramWrite(0xC019, 0);
    delay(100);
    softReset();
}

bool VS1053::isChipConnected() {
    uint16_t status = readRegister((uint8_t)registers::STATUS);
    return !(status == 0 || status == 0xFFFF);
}

uint16_t VS1053::getSeconds() {
    return readRegister((uint8_t)registers::DECODE_TIME);
}

void VS1053::clearSeconds() {
    writeRegister((uint8_t)registers::DECODE_TIME, 0x00);
    writeRegister((uint8_t)registers::DECODE_TIME, 0x00);
}