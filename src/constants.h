/*
    constants.h - Library for ESP8266 4 way pushbutton dimmer switch project
    Creadted by Jerome Luckenbach, 2017-12-10
    Released under MIT License
*/

#ifndef Constants_h
#define Constants_h

// Interval config
const int DEBOUNCE_INTERVAL = 70;
const int DIMMER_PUSH_INTERVAL = 300;
const int DIMMER_CONTINUE_INTERVAL = 100;
const int DIMMER_STEP_VALUE = 2;

// Node Config, for easy editing the topics and property types
const char* NODE_TYPE = "pushbutton";
const char* NODE_1 = "pushbutton1";
const char* NODE_2 = "pushbutton2";
const char* NODE_3 = "pushbutton3";
const char* NODE_4 = "pushbutton4";

#endif