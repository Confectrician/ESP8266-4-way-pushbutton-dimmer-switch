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

pushButton::pushButton(){
    lastReading = -1;
    lastReadingTime = 0;
    switchState = 0;
    dimmerValue = 0;
    dimmerReadingTime = 0;
    dimmerMode = false;
    debouncer = Bounce();
}