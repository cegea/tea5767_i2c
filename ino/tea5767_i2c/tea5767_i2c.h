/**
 ********************************************************************************
 * @file    tea5737_i2c.c
 * @author  Carlos Egea
 * @date    06/06/2023
 * @brief   I2C Driver for NXP FM Radio module.
 ********************************************************************************
 */

#ifndef _HARDWARE_TEA5767_I2C_H
#define _HARDWARE_TEA5767_I2C_H

/************************************
 * INCLUDES
 ************************************/
#include <Wire.h>

/************************************
 * MACROS AND DEFINES
 ************************************/
#define DUMY_LED_PIN 0 // Pin number for a dummy LED
#define MAX_FREQ_JP 91.0 // Maximum frequency limit for the Japanese band
#define MAX_FREQ_EU 108.0 // Maximum frequency limit for the European band
#define MIN_FREQ_JP 76.0 // Minimum frequency limit for the Japanese band
#define MIN_FREQ_EU 87.5 // Minimum frequency limit for the European band
#define JP_BAND 1 // Constant for the Japanese band mode
#define EU_BAND 0 // Constant for the European band mode
#define ADC_LOW 5 // Constant for the low level of ADC readings
#define ADC_MID 7 // Constant for the medium level of ADC readings
#define ADC_HIGH 10 // Constant for the high level of ADC readings
#define TEA5767_REGISTERS 5 // Number of registers in the TEA5767 chip

class tea5767_i2c
{
public:  

    /*! \brief   Class constructor.
    *  \ingroup tea5767_i2c
    *
    */
    tea5767_i2c(uint8_t band_mode); // Constructor

    /************************************
     * GLOBAL FUNCTION PROTOTYPES
     ************************************/
    // ----------------------------------------------------------------------------
    // Setup

    /*! \brief   Initialize the parameters needed for setup().
    *  \ingroup tea5767_i2c
    *
    * TO DO
    *
    * \param radio Variable of type TEA5767_t.
    */
    void begin();

    /*! @brief Gets the current station frequency from the TEA5757 radio and prints it to stdout.
    * This function reads the raw data from the TEA5757 radio using the tea5767_read_raw() function,
    * extracts the frequency values from the read buffer, and calculates the frequency in MHz.
    * The extracted frequency is then printed to stdout with two decimal places.
    * @note This function assumes that the TEA5757 radio device has been initialized and is currently powered on.
    * @return void
    */
    float tea5767_getStation();

    /*! @brief Configures the search mode and direction of the TEA5757 tuner.
    * This function sets the search mode and direction of the TEA5757 tuner. It updates the values of the TEA5757_t structure
    * with the given search mode and search direction and then writes them to the tuner using the tea5767_write_registers() function.
    * @param searchMode An 8-bit unsigned integer specifying the search mode to use. Valid values are:

    * TEA5767_SEARCH_MUTE_LOW
    * TEA5767_SEARCH_MUTE_HIGH
    * TEA5767_SEARCH_MUTE_BW
    * TEA5767_SEARCH_MUTE_OFF
    * TEA5767_SEARCH_MUTE_SW
    * @param searchUpDown An 8-bit unsigned integer specifying the direction of the search. Valid values are:
    * TEA5767_SEARCH_UP
    * TEA5767_SEARCH_DOWN
    * 
    * */
    void tea5767_setSearch(uint8_t searchMode, uint8_t searchUpDown);

    /*! @brief Sets the frequency of the TEA5757 tuner.
    * This function sets the frequency of the TEA5757 tuner to the given value.
    * If the frequency is out of range for the current band mode, it will be adjusted to the nearest valid frequency.
    * After setting the frequency, the new value will be written to the tuner through the tea5767_write_registers function.
    * @param freq The desired frequency to set the tuner to.
    * @return void
    * @note The tuner must be initialized and ready before calling this function.
    */
    void tea5767_setStation(float freq);

    /*! @brief Increments the current frequency of the TEA5757 radio by a given value.
    * This function increases the current frequency of the TEA5757 radio by a given value.
    * If the given value is negative, the search mode is set to down, otherwise the search mode is set to up.
    * The frequency is then incremented by the given value, but it is first checked to ensure that it
    * is within the frequency limits specified by the radio's band mode. The resulting frequency is then
    * written to the radio using the tea5767_write_registers function.
    * @param freq The frequency increment value.
    * @note This function should be called only after the radio has been properly initialized and tuned to a station.
    */
    void tea5767_setStationInc(float freq);

    /*! @brief Sets the mute mode of the TEA5757 tuner.
    * This function sets the mute mode of the TEA5757 tuner to the specified value. When mute mode is enabled,
    * the audio output is muted.
    * @param mute The desired mute mode value. true to enable mute mode, false to disable it.
    * @note The function tea5767_write_registers() is called to write the new mute mode value to the tuner.
    */
    void tea5767_setMute(bool mute);

    /*! @brief Sets the soft mute mode of the TEA5767 radio.
    * This function sets the soft mute mode of the TEA5767 radio to either on or off.
    * @param mute A boolean indicating whether the soft mute mode should be on (true) or off (false).
    * @return void
    * @note The soft mute mode reduces the hissing noise when tuning the radio but can also cause distortion in weak signals.
    */
    void tea5767_setSoftMute(bool mute);

    /*! @brief Sets the left channel mute mode for the TEA5767 radio.
    * @param mute Boolean value indicating whether the left channel should be muted.
    */
    void tea5767_setMuteLeft(bool mute);

    /*! @brief Sets the right channel mute mode for the TEA5767 radio.
    * @param mute Boolean value indicating whether the left channel should be muted.
    */
    void tea5767_setMuteRight(bool mute);

    /*! @brief Sets the standby mode of the TEA5757 radio.
    * This function sets the standby mode of the TEA5757 radio. When the radio is in standby mode, it consumes less power but cannot receive signals.
    * @param radio Pointer to the TEA5757_t structure.
    * @param standby Set to true to activate standby mode, false to deactivate it.
    */
    void tea5767_setStandby(bool standby);

    /*! @brief Sets the stereo mode of the TEA5757 radio.
    * This function sets the stereo mode of the TEA5757 radio. When the radio is in stereo mode, it receives stereo signals if available. When in mono mode, it receives only mono signals.
    * @param radio Pointer to the TEA5757_t structure.
    * @param stereo Set to true to activate stereo mode, false to activate mono mode.
    */
    void tea5767_setStereo(bool stereo);

 private:

    void printStatus();

    /*! \brief   Read all five bytes from TEA5767 w/o format.
    *  \ingroup tea5767_i2c
    *
    * Reads five bytes from the TEA5767_t variable and fills the buffer variable.
    *
    * \param radio From type TEA5767_t.
    * \param buffer \ref TEA5767_REGISTERS amount to this variable.
    */
    void tea5767_read_raw(uint8_t *buffer);


    /*! \brief   Writes all five bytes from TEA5767 with format.
    *  \ingroup tea5767_i2c
    *
    * Writtes five bytes to the TEA5767_t variable and fills the buffer variable.
    * The data from the radio variable is modified to fit the memory map of TEA5767.
    *
    * \param radio Variable of type TEA5767_t.
    */
    void tea5767_write_registers();

    /*! @brief Initializes the TEA5757_t structure for the TEA5757 tuner.
    * This function initializes the TEA5757_t structure with the default values for the TEA5757 tuner.
    * The default I2C address of the tuner is 0x60.
    * @note This function must be called before using any other function related to the TEA5757 tuner.
    */
    int tea5767_getReady();

    /*! @brief Checks if the given frequency is within the limits of the current band mode of the TEA5757 radio.
    * This function checks if the given frequency is within the limits of the current band mode of the TEA5757 radio, and adjusts it if necessary.
    * @param radio A pointer to a TEA5757_t structure that holds information about the radio, including its current band mode.
    * @param freq The frequency to check.
    * @return float The frequency adjusted if it is out of bounds for the current band mode, or the original frequency if it is within bounds.
    * @note The function assumes that the TEA5757_t structure pointed to by radio has its band_mode field set correctly.
    */
    float tea5767_checkFreqLimits(float freq);

    uint8_t _address;                //< I2C device address
    uint8_t _mute_mode;              //< Audio mute mode
    uint8_t _band_mode;              //< Frequency band mode
    uint8_t _standby;                // Standby mode
    uint8_t _searchMode;             // Search mode
    uint8_t _searchUpDown;           // Search up-down mode
    uint8_t _searchLevel;            // Search level
    uint8_t _stereoMode;             // Stereo mode
    uint8_t _muteLmode;              // Left channel mute mode
    uint8_t _muteRmode;              // Right channel mute mode
    uint8_t _stereoNoiseCancelling;  // Stereo noise cancelling mode
    uint8_t _softMuteMode;           // Soft mute mode
    uint8_t _hpfMode;                // High pass filter mode
    uint8_t _isReady;                // Radio is ready flag
    uint8_t _isStereo;               // Stereo mode flag
    uint8_t _stationLevel;           // Station level
    float   _frequency;               // Frequency in MHz

};

#endif