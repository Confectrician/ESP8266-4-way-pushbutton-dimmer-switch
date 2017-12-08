#include <Homie.h>

#pragma region Functions declarations
bool globalInputHandler(const HomieNode& node, const String& property, const HomieRange& range, const String& value);
void checkPushButton(const HomieNode& node, const int& reading, int& state, int& lastReading);
void loopHandler();
#pragma endregion

#pragma region Variable declarations
const int DEBOUNCE_INTERVAL = 100;

const int taster1Pin = D1;
const int taster2Pin = D2;
const int taster3Pin = D3;
const int taster4Pin = D4;

int taster1LastReading = -1;
int taster2LastReading = -1;  
int taster3LastReading = -1;  
int taster4LastReading = -1; 

int taster1State = LOW;
int taster2State = LOW;  
int taster3State = LOW;  
int taster4State = LOW;

Bounce debouncer1 = Bounce();
Bounce debouncer2 = Bounce();
Bounce debouncer3 = Bounce();
Bounce debouncer4 = Bounce();
#pragma endregion

#pragma region Node declarations
HomieNode taster1Node("taster1", "taster");
HomieNode taster2Node("taster2", "taster");
HomieNode taster3Node("taster3", "taster");
HomieNode taster4Node("taster4", "taster");
#pragma endregion

void setup() {
    Serial.begin(115200);
    Serial << endl << endl;

    // Hardware configuration
    #pragma region
    pinMode(taster1Pin, INPUT);
    pinMode(taster2Pin, INPUT);
    pinMode(taster3Pin, INPUT);
    pinMode(taster4Pin, INPUT);

    digitalWrite(taster1Pin, HIGH);
    digitalWrite(taster2Pin, HIGH);
    digitalWrite(taster3Pin, HIGH);
    digitalWrite(taster4Pin, HIGH);
    #pragma endregion
    
    // Debouncer configuration
    #pragma region
    debouncer1.attach(taster1Pin);
    debouncer2.attach(taster2Pin);
    debouncer3.attach(taster3Pin);
    debouncer4.attach(taster4Pin);

    debouncer1.interval(DEBOUNCE_INTERVAL);
    debouncer2.interval(DEBOUNCE_INTERVAL);
    debouncer3.interval(DEBOUNCE_INTERVAL);
    debouncer4.interval(DEBOUNCE_INTERVAL);
    #pragma endregion

    // Homie configuration
    Homie_setFirmware("luba-tasterTest", "1.0.0"); // The underscore is not a typo! See Magic bytes
    Homie.setGlobalInputHandler(globalInputHandler); // before Homie.setup()
    Homie.setLoopFunction(loopHandler);

    // Node configurations
    #pragma region
    taster1Node.advertise("pressed");
    taster2Node.advertise("pressed");
    taster3Node.advertise("pressed");
    taster4Node.advertise("pressed");

    taster1Node.advertise("switchState").settable();
    taster2Node.advertise("switchState").settable();
    taster3Node.advertise("switchState").settable();
    taster4Node.advertise("switchState").settable();
    #pragma endregion

    Homie.setup();
}

void loop() {
    Homie.loop();
    debouncer1.update();
    debouncer2.update();
    debouncer3.update();
    debouncer4.update();
}

#pragma region Custom functions implementation

void checkPushButton(const HomieNode& node, const int& reading, int& state, int& lastReading){
    if(reading != lastReading){

        Homie.getLogger() << node.getId() << " changed" << endl;        
        node.setProperty("pressed").send(reading ? "false" : "true");

        if(reading == HIGH && lastReading== LOW){
            node.setProperty("switchState").send(state ? "false" : "true");
            state = state ? LOW : HIGH;
        }

        lastReading = reading;
    }
}

bool globalInputHandler(const HomieNode& node, const String& property, const HomieRange& range, const String& value) {

    Homie.getLogger() << "Received on node " << node.getId() << ": " << property << " = " << value << endl;

    if(property == "switchState" && (value == "true" || value == "false")){

        Homie.getLogger() << "Property and value are valid. Checking for the correct Node now." << endl;
        
        if(node.getId() == "taster1"){
            taster1State = (value == "true") ? HIGH : LOW;
        } else if(node.getId() == "taster2"){
            taster2State = (value == "true") ? HIGH : LOW;
        } else if(node.getId() == "taster3"){
            taster3State = (value == "true") ? HIGH : LOW;
        } else if(node.getId() == "taster4"){
            taster4State = (value == "true") ? HIGH : LOW;            
        } else {
            return false; // Unknown Node. Return unhandled            
        }

        // Update the Property only, when any of the if cases has been handled.
        node.setProperty("switchState").send((value == "true") ?  "true" : "false");
        return true;
    }

    return false;
}

void loopHandler(){

    checkPushButton(taster1Node, debouncer1.read(), taster1State, taster1LastReading);
    checkPushButton(taster2Node, debouncer2.read(), taster2State, taster2LastReading);
    checkPushButton(taster3Node, debouncer3.read(), taster3State, taster3LastReading);
    checkPushButton(taster4Node, debouncer4.read(), taster4State, taster4LastReading);

}

#pragma endregion