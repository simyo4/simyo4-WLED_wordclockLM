#pragma once

#include "wled.h"

/*
 * Usermods allow you to add own functionality to WLED more easily
 * See: https://github.com/Aircoookie/WLED/wiki/Add-own-functionality
 */

class WordClockLisaAndMaxUsermod : public Usermod 
{
  private:
    unsigned long lastTime = 0;
    int lastTimeMinutes = -1;

    // set your config variables to their boot default value (this can also be done in readFromConfig() or a constructor if you prefer)
    bool usermodActive = true;
    bool displayOnlyLisaAndMax = false;
    bool displayLisaAndMaxAtTwelve = true;
    
    // defines for mask sizes
    #define maskSizeLeds        341
    #define maskSizeMinutesMea  12
    #define maskSizeHoursMea    6
    #define maskSizeItIs        5

    // "minute" masks

    // Meander wiring
    const int maskMinutesMea[14][maskSizeMinutesMea] = 
    {
      { -1, -1, -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1}, // 0 - 00
      {  111, 114, 117, 120,  145, 148, 151, 154,  -1,  -1,  -1,  -1}, // 1 - 05 fünf nach (4,7-10) (5,8-11)
      { 96,  99,  102,  105,  145, 148, 151, 154,  -1,  -1,  -1,  -1}, // 2 - 10 zehn (4, 2-5) nach
      { 74,  77,  80,  83,  86,  89,  92,  -1,  -1,  -1,  -1,  -1}, // 3 - 15 viertel (3, 5-11)
      { 37,  40,  43,  46,  49,  52,  55,  145, 148, 151, 154,  -1}, // 4 - 20 zwanzig (2, 3-9) nach
      {  111, 114, 117, 120,  124,  127,  130,  158,  161,  164,  167,  -1}, // 5 - 25 fünf vor (5, 1-3) halb (6, 2-5)
      { 158,  161,  164,  167,  -1,  -1,  -1,  -1,  -1,  -1,  -1,  -1}, // 6 - 30 halb
      {  111, 114, 117, 120,  145, 148, 151, 154, 158, 161,  164,  167}, // 7 - 35 fünf nach halb
      { 37,  40,  43,  46,  49,  52,  55,  124,  127,  130,  -1,  -1}, // 8 - 40 zwanzig vor
      { 62,  65,  68,  71,  74,  77,  80,  83,  86,  89,  92,  -1}, // 9 - 45 dreiviertel (3, 1-11)
      { 96,  99,  102,  105,  124,  127,  130,  -1,  -1,  -1,  -1,  -1}, // 10 - 50 zehn vor
      {  111, 114, 117, 120,  124,  127,  130,  -1,  -1,  -1,  -1,  -1}, // 11 - 55 fünf vor
    };


    // hour masks
    const int maskHoursMea[13][maskSizeHoursMea] = 
    {
      { 282,  285,  288,  291,  -1,  -1}, // 01: eins //changed to ein's' because 'Uhr' is missing
      { 282,  285,  288,  291,  -1,  -1}, // 01: eins
      { 313,  316,  319,  322,  -1,  -1}, // 02: zwei
      { 328,  331,  334,  337,  -1,  -1}, // 03: drei
      { 176,  179,  182,  185,  -1,  -1}, // 04: vier
      { 238,  241,  244,  247,  -1,  -1}, // 05: fünf
      { 260,  263,  266,  269,  272,  -1}, // 06: sechs
      { 291,  294,  297,  300,  303,  306}, // 07: sieben
      { 248,  251,  254,  257,  -1,  -1}, // 08: acht
      { 226, 229, 232, 235,  -1,  -1}, // 09: neun
      { 217, 220, 223, 226,  -1,  -1}, // 10: zehn
      { 210,  213,  216,  -1,  -1,  -1}, // 11: elf
      { 195,  198,  201,  204,  207,  -1}  // 12: zwölf and 00: null
    };

    // mask "it is"
    const int maskItIs[maskSizeItIs] = {0, 3, 9, 12, 15};

    // overall mask to define which LEDs are on
    int maskLedsOn[maskSizeLeds] = 
    {
      0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
      0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
      0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
      0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
      0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
      0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
      0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
      0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
      0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
      0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
      0,0,0,0,0,0,0,0,0,0,0
    };

    const int maskLisa[4] = {133, 136, 139, 142};
    const int maskMax[3] = {186, 189, 192};

    // update led mask
    void updateLedMask(const int wordMask[], int arraySize)
    {
      // loop over array
      for (int x=0; x < arraySize; x++) 
      {
        // check if mask has a valid LED number
        if (wordMask[x] >= 0 && wordMask[x] < maskSizeLeds)
        {
          // turn LED on
          maskLedsOn[wordMask[x]] = 1;
        }
      }
    }

    // set hours
    void setHours(int hours, bool fullClock)
    {
      int index = hours;

      // handle 00:xx as 12:xx
      if (hours == 0)
      {
        index = 12;
      }

      // check if we get an overrun of 12 o´clock
      if (hours == 13)
      {
        index = 1;
      }

      // special handling for "ein Uhr" instead of "eins Uhr"
      if (hours == 1 && fullClock == true)
      {
        index = 0;
      }

      // update led mask
      updateLedMask(maskHoursMea[index], maskSizeHoursMea);
    }

    // set minutes
    void setMinutes(int index)
    {
      // update led mask
      updateLedMask(maskMinutesMea[index], maskSizeMinutesMea);
    }

    // update the display
    void updateDisplay(uint8_t hours, uint8_t minutes) 
    {
      // disable complete matrix at the bigging
      for (int x = 0; x < maskSizeLeds; x++)
      {
        maskLedsOn[x] = 0;
      } 

      if(displayLisaAndMaxAtTwelve && minutes/5 == 0 && hours == 12)
      {
        updateLedMask(maskLisa, 4);
        updateLedMask(maskMax, 3);
      }
      else
      {
        updateLedMask(maskItIs, maskSizeItIs);
      }

      // switch minutes
      switch (minutes / 5) 
      {
        case 0:
            // full hour
            setMinutes(0);
            setHours(hours, true);            
            break;
        case 1:
            // 5 nach
            setMinutes(1);
            setHours(hours, false);
            break;
        case 2:
            // 10 nach
            setMinutes(2);
            setHours(hours, false);
            break;
        case 3:
            // viertel 
            setMinutes(3);
            setHours(hours + 1, false);
            break;
        case 4:
            // 20 nach
            setMinutes(4);
            setHours(hours, false);
            break;
        case 5:
            // 5 vor halb
            setMinutes(5);
            setHours(hours + 1, false);
            break;
        case 6:
            // halb
            setMinutes(6);
            setHours(hours + 1, false);
            break;
        case 7:
            // 5 nach halb
            setMinutes(7);
            setHours(hours + 1, false);
            break;
        case 8:
            // 20 vor
            setMinutes(8);
            setHours(hours + 1, false);
            break;
        case 9:
            // dreiviertel
            setMinutes(9);
            setHours(hours + 1, false);
            break;
        case 10:
            // 10 vor
            setMinutes(10);
            setHours(hours + 1, false);
            break;
        case 11:
            // 5 vor
            setMinutes(11);
            setHours(hours + 1, false);
            break;
        }
    }

  public:
    //Functions called by WLED

    /*
     * setup() is called once at boot. WiFi is not yet connected at this point.
     * You can use it to initialize variables, sensors or similar.
     */
    void setup() 
    {
    }

    /*
     * connected() is called every time the WiFi is (re)connected
     * Use it to initialize network interfaces
     */
    void connected() 
    {
    }

    /*
     * loop() is called continuously. Here you can check for events, read sensors, etc.
     * 
     * Tips:
     * 1. You can use "if (WLED_CONNECTED)" to check for a successful network connection.
     *    Additionally, "if (WLED_MQTT_CONNECTED)" is available to check for a connection to an MQTT broker.
     * 
     * 2. Try to avoid using the delay() function. NEVER use delays longer than 10 milliseconds.
     *    Instead, use a timer check as shown here.
     */
    void loop() {

      // do it every 5 seconds
      if (millis() - lastTime > 5000) 
      {
        if(displayOnlyLisaAndMax)
        {
          for (int x = 0; x < maskSizeLeds; x++)
          {
            maskLedsOn[x] = 0;
          } 
          updateLedMask(maskLisa, 4);
          updateLedMask(maskMax, 3);
        }
        else
        {
          // check the time
          int minutes = minute(localTime);

          // check if we already updated this minute
          if (lastTimeMinutes != minutes)
          {
            // update the display with new time
            updateDisplay(hourFormat12(localTime), minute(localTime));

            // remember last update time
            lastTimeMinutes = minutes;
          }
        }


        // remember last update
        lastTime = millis();
      }
    }

    /*
     * addToJsonInfo() can be used to add custom entries to the /json/info part of the JSON API.
     * Creating an "u" object allows you to add custom key/value pairs to the Info section of the WLED web UI.
     * Below it is shown how this could be used for e.g. a light sensor
     */
    /*
    void addToJsonInfo(JsonObject& root)
    {
    }
    */

    /*
     * addToJsonState() can be used to add custom entries to the /json/state part of the JSON API (state object).
     * Values in the state object may be modified by connected clients
     */
    void addToJsonState(JsonObject& root)
    {
      JsonObject usermod = root["WordClockLisaAndMaxUsermod"];
      if (usermod.isNull()) usermod = root.createNestedObject("WordClockLisaAndMaxUsermod");

      usermod["active"] = usermodActive;
      usermod["displayOnlyLisaAndMax"] = displayOnlyLisaAndMax;
      usermod["displayLisaAndMaxAtTwelve"] = displayLisaAndMaxAtTwelve;
    }

    /*
     * readFromJsonState() can be used to receive data clients send to the /json/state part of the JSON API (state object).
     * Values in the state object may be modified by connected clients
     */
    void readFromJsonState(JsonObject& root)
    {
      JsonObject usermod = root["WordClockLisaAndMaxUsermod"];
      if (!usermod.isNull()) {
        if(usermod.containsKey("active"))
          usermodActive = usermod["active"];
        
        if(usermod.containsKey("displayOnlyLisaAndMax"))
          displayOnlyLisaAndMax = usermod["displayOnlyLisaAndMax"];

        if(usermod.containsKey("displayLisaAndMaxAtTwelve"))
          displayLisaAndMaxAtTwelve = usermod["displayLisaAndMaxAtTwelve"];      
      }
    }

    /*
     * addToConfig() can be used to add custom persistent settings to the cfg.json file in the "um" (usermod) object.
     * It will be called by WLED when settings are actually saved (for example, LED settings are saved)
     * If you want to force saving the current state, use serializeConfig() in your loop().
     * 
     * CAUTION: serializeConfig() will initiate a filesystem write operation.
     * It might cause the LEDs to stutter and will cause flash wear if called too often.
     * Use it sparingly and always in the loop, never in network callbacks!
     * 
     * addToConfig() will make your settings editable through the Usermod Settings page automatically.
     *
     * Usermod Settings Overview:
     * - Numeric values are treated as floats in the browser.
     *   - If the numeric value entered into the browser contains a decimal point, it will be parsed as a C float
     *     before being returned to the Usermod.  The float data type has only 6-7 decimal digits of precision, and
     *     doubles are not supported, numbers will be rounded to the nearest float value when being parsed.
     *     The range accepted by the input field is +/- 1.175494351e-38 to +/- 3.402823466e+38.
     *   - If the numeric value entered into the browser doesn't contain a decimal point, it will be parsed as a
     *     C int32_t (range: -2147483648 to 2147483647) before being returned to the usermod.
     *     Overflows or underflows are truncated to the max/min value for an int32_t, and again truncated to the type
     *     used in the Usermod when reading the value from ArduinoJson.
     * - Pin values can be treated differently from an integer value by using the key name "pin"
     *   - "pin" can contain a single or array of integer values
     *   - On the Usermod Settings page there is simple checking for pin conflicts and warnings for special pins
     *     - Red color indicates a conflict.  Yellow color indicates a pin with a warning (e.g. an input-only pin)
     *   - Tip: use int8_t to store the pin value in the Usermod, so a -1 value (pin not set) can be used
     *
     * See usermod_v2_auto_save.h for an example that saves Flash space by reusing ArduinoJson key name strings
     * 
     * If you need a dedicated settings page with custom layout for your Usermod, that takes a lot more work.  
     * You will have to add the setting to the HTML, xml.cpp and set.cpp manually.
     * See the WLED Soundreactive fork (code and wiki) for reference.  https://github.com/atuline/WLED
     * 
     * I highly recommend checking out the basics of ArduinoJson serialization and deserialization in order to use custom settings!
     */
    void addToConfig(JsonObject& root) override
    {
      JsonObject top = root.createNestedObject(F("WordClockLisaAndMaxUsermod"));
      top[F("active")] = usermodActive;
      top[F("displayOnlyLisaAndMax")] = displayOnlyLisaAndMax;
      top[F("displayLisaAndMaxAtTwelve")] = displayLisaAndMaxAtTwelve;
    }

    void appendConfigData()
    {

    }

    /*
     * readFromConfig() can be used to read back the custom settings you added with addToConfig().
     * This is called by WLED when settings are loaded (currently this only happens immediately after boot, or after saving on the Usermod Settings page)
     * 
     * readFromConfig() is called BEFORE setup(). This means you can use your persistent values in setup() (e.g. pin assignments, buffer sizes),
     * but also that if you want to write persistent values to a dynamic buffer, you'd need to allocate it here instead of in setup.
     * If you don't know what that is, don't fret. It most likely doesn't affect your use case :)
     * 
     * Return true in case the config values returned from Usermod Settings were complete, or false if you'd like WLED to save your defaults to disk (so any missing values are editable in Usermod Settings)
     * 
     * getJsonValue() returns false if the value is missing, or copies the value into the variable provided and returns true if the value is present
     * The configComplete variable is true only if the "exampleUsermod" object and all values are present.  If any values are missing, WLED will know to call addToConfig() to save them
     * 
     * This function is guaranteed to be called on boot, but could also be called every time settings are updated
     */
    bool readFromConfig(JsonObject& root)
    {
      // default settings values could be set here (or below using the 3-argument getJsonValue()) instead of in the class definition or constructor
      // setting them inside readFromConfig() is slightly more robust, handling the rare but plausible use case of single value being missing after boot (e.g. if the cfg.json was manually edited and a value was removed)

      JsonObject top = root[F("WordClockLisaAndMaxUsermod")];

      bool configComplete = !top.isNull();

      configComplete &= getJsonValue(top[F("active")], usermodActive);
      configComplete &= getJsonValue(top[F("displayOnlyLisaAndMax")], displayOnlyLisaAndMax);
      configComplete &= getJsonValue(top[F("displayLisaAndMaxAtTwelve")], displayLisaAndMaxAtTwelve);
      return configComplete;
    }

    /*
     * handleOverlayDraw() is called just before every show() (LED strip update frame) after effects have set the colors.
     * Use this to blank out some LEDs or set them to a different color regardless of the set effect mode.
     * Commonly used for custom clocks (Cronixie, 7 segment)
     */
    void handleOverlayDraw()
    {
      // check if usermod is active
      if (usermodActive == true || displayOnlyLisaAndMax == true)
      {
        // loop over all leds
        for (int x = 0; x < maskSizeLeds; x++)
        {
          // check mask
          if (maskLedsOn[x] == 0)
          {
            // set pixel off
            strip.setPixelColor(x, RGBW32(0,0,0,0));
          }
        }
      }
    }

    /*
     * getId() allows you to optionally give your V2 usermod an unique ID (please define it in const.h!).
     * This could be used in the future for the system to determine whether your usermod is installed.
     */
    uint16_t getId()
    {
      return USERMOD_ID_WORDCLOCK;
    }

   //More methods can be added in the future, this example will then be extended.
   //Your usermod will remain compatible as it does not need to implement all methods from the Usermod base class!
};