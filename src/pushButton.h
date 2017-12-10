/*
    pushButton.h - Library for ESP8266 4 way pushbutton dimmer switch project
    Creadted by Jerome Luckenbach, 2017-12-09
    Released under MIT License
*/
#ifndef pushButton_h
#define pushButton_h
#include <Homie.h>

class  pushButton {

    public:int lastReading;
    public:unsigned long lastReadingTime;
    public:int switchState;
    public:int dimmerValue;
    public:int dimmerReadingTime;
    public:bool dimmerMode;
    public:Bounce debouncer;

    public:
        pushButton();
};
#endif