/// @file

#ifndef __RDA5807_HPP
#define __RDA5807_HPP

#include <Arduino.h>
#include <Wire.h>

/// \brief
/// RDA5807 Interface
/// \details
/// This is an interface that simplifies the use of RDA5807 chips. Tuning, Muting, Auto-search
/// and Setting of Frequencies are some of its posibilites. It is compatible with all RDA58XX chips,
/// though some RDA58XX chips have no registers for RDS-Data thus not having RDS-abilities.
/// 
/// Some supported operations are:
///     - Set/Get Frequency
///     - Set/Get Stereo
///     - Set/Get Mute
///     - Set/Get Volume
///     - Automatic Search
///     - Set/Get Stand By
///     - Enable/Receive Radio Data
///     - More to be seen in test
///
/// Some possibilities are shown below. For other functions and examples you can take a look
/// at the tests.
/// 
/// ~~~~~~~~~~~~~~~{.cpp}
/// auto scl = target::pin_oc( target::pins::d8 );
/// auto sda = target::pin_oc( target::pins::d9 );
/// auto i2c_bus = hwlib::i2c_bus_bit_banged_scl_sda(scl, sda);
/// 
/// auto radio = RDA5807(i2c_bus);
/// radio.begin();
/// hwlib::cout << "Tuning to 100.7FM: ";
/// radio.setFrequency(100.7);
/// for(unsigned int i = 0; i < 8; i++){
///     hwlib::wait_ms(5000);
///     radio.radioData.update();
///     hwlib::cout << hwlib::left << hwlib::setw(30) << "Frequency: " << radio.getIntFrequency() << hwlib::endl;
///     hwlib::cout << hwlib::left << hwlib::setw(30) << "Station Name: " << radio.radioData.stationName() << hwlib::endl;
///     hwlib::wait_ms(3000);
///     radio.seekChannel(1);
/// }
/// ~~~~~~~~~~~~~~~
class RDA5807 {
    private:
        TwoWire & bus;

        uint16_t data[8] = {0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x9881, 0x0000, 0x8000};    //First two bytes contain device specific info; are never send but here for completeness.
        uint16_t status[6] = {0x0000, 0x0000, 0x0000, 0x0000, 0x0000, 0x0000};

        const uint8_t indexAddress;         //0x11 for RDA5807
        const uint8_t address;
        const uint8_t firstReadRegister;    //0x10 for RDA5807

        uint8_t bandLimit;

        void setData();
        void setData(const unsigned int regNumber);
        void setRegister(const unsigned int regNumber, const uint16_t value);

        void getStatus();
        void getStatus(const unsigned int regNumber);

        //Specific Powerfull Setting; let user handle this through standBy().
        void powerUpEnable(const bool enable);
    public:
        RDA5807(TwoWire & bus, const uint8_t address = 0x10, const uint8_t firstReadRegister = 0x0A, const uint8_t bandLimit = 0);

        //Basic Settings
        void setClockSupply(const bool clockSource, const bool directInput = false);
        void setClockFrequency(const unsigned int frequency = 32);
        unsigned int getClockFrequency();

        void begin(const bool muted = false);
        void reset();

        void setBandLimit(const unsigned int limit = 0);
        unsigned int hasBandLimit();

        void setSpacing(const unsigned int spacing = 100000);       //Spacing between channels; 100000kHz default (Country Specific).
        unsigned int getSpacing();

        void standBy(const bool sleep = true);
        bool isStandBy();

        //Basic Tuning
        bool setFrequency(const float frequency, const bool autoTune);
        void setFrequency(const float frequency);
        float getFrequency();
        unsigned int getIntFrequency();

        void setTune(const bool tune = true);
        bool isTuned();

        void demodulateMethod(const bool newMethod = true);     //Can Improve Signal Strength
        bool newDemodulate();

        bool isReady();
        bool isStation();

        //Audio Quality
        unsigned int signalStrength();

        void normalAudio(const bool normal = true);     //High Impedance (false) or Normal (true) Output
        bool isNormalAudio();

        void setBassBoost(const bool boost = true);
        bool bassBoosted();

        void setVolume(const uint8_t volume = 15);
        unsigned int getVolume();

        void setMute(const bool mute = true);
        bool isMuted(const char side = 'a');

        void setStereo(const bool stereo = true);
        bool isStereo();
        bool stereoReception();

        //Search Settings
        void seek(const unsigned int direction);

        void seekChannel(const unsigned int direction, const bool wrapContinue = true);
        bool seekCompleted();
        void setSeekThreshold(const uint8_t threshold = 8);

        //Automated Tests
        bool testCorrectFunctioning();
};

#endif //__RDA5807_HPP