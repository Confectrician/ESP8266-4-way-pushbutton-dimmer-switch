/*
    main.cpp - Library for ESP8266 4 way pushbutton dimmer switch project
    Creadted by Jerome Luckenbach, 2017-12-10
    Released under MIT License
*/
#include <pushbutton.h>

pushButton::pushButton(){
    lastReading = -1;
    lastReadingTime = 0;
    switchState = 0;
    dimmerValue = 0;
    dimmerReadingTime = 0;
    dimmerMode = false;
    debouncer = Bounce();
}