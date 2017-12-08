#include <Homie.h>

#pragma region Functions declarations
bool globalInputHandler(const HomieNode& node, const String& property, const HomieRange& range, const String& value);
void checkPushButton(const HomieNode& node, const int& reading, int& state, int& lastReading);
void loopHandler();
#pragma endregion

#pragma region Variable declarations
const int DEBOUNCE_INTERVAL = 100;

const int pushbutton1Pin = D1;
const int pushbutton2Pin = D2;
const int pushbutton3Pin = D3;
const int pushbutton4Pin = D4;

int pushbutton1LastReading = -1;
int pushbutton2LastReading = -1;  
int pushbutton3LastReading = -1;  
int pushbutton4LastReading = -1; 

int pushbutton1State = LOW;
int pushbutton2State = LOW;  
int pushbutton3State = LOW;  
int pushbutton4State = LOW;

Bounce debouncer1 = Bounce();
Bounce debouncer2 = Bounce();
Bounce debouncer3 = Bounce();
Bounce debouncer4 = Bounce();
#pragma endregion

#pragma region Node declarations
HomieNode pushbutton1Node("pushbutton1", "pushbutton");
HomieNode pushbutton2Node("pushbutton2", "pushbutton");
HomieNode pushbutton3Node("pushbutton3", "pushbutton");
HomieNode pushbutton4Node("pushbutton4", "pushbutton");
#pragma endregion

void setup() {
    Serial.begin(115200);
    Serial << endl << endl;

    // Hardware configuration
    #pragma region
    pinMode(pushbutton1Pin, INPUT);
    pinMode(pushbutton2Pin, INPUT);
    pinMode(pushbutton3Pin, INPUT);
    pinMode(pushbutton4Pin, INPUT);

    digitalWrite(pushbutton1Pin, HIGH);
    digitalWrite(pushbutton2Pin, HIGH);
    digitalWrite(pushbutton3Pin, HIGH);
    digitalWrite(pushbutton4Pin, HIGH);
    #pragma endregion
    
    // Debouncer configuration
    #pragma region
    debouncer1.attach(pushbutton1Pin);
    debouncer2.attach(pushbutton2Pin);
    debouncer3.attach(pushbutton3Pin);
    debouncer4.attach(pushbutton4Pin);

    debouncer1.interval(DEBOUNCE_INTERVAL);
    debouncer2.interval(DEBOUNCE_INTERVAL);
    debouncer3.interval(DEBOUNCE_INTERVAL);
    debouncer4.interval(DEBOUNCE_INTERVAL);
    #pragma endregion

    // Homie configuration
    Homie_setFirmware("4-way-pushbutton-dimmer-switch", "1.0.0"); // The underscore is not a typo! See Magic bytes
    Homie.setGlobalInputHandler(globalInputHandler);
    Homie.setLoopFunction(loopHandler);

    // Node configurations
    #pragma region
    pushbutton1Node.advertise("pressed");
    pushbutton2Node.advertise("pressed");
    pushbutton3Node.advertise("pressed");
    pushbutton4Node.advertise("pressed");

    pushbutton1Node.advertise("switchState").settable();
    pushbutton2Node.advertise("switchState").settable();
    pushbutton3Node.advertise("switchState").settable();
    pushbutton4Node.advertise("switchState").settable();
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
        
        if(node.getId() == "pushbutton1"){
            pushbutton1State = (value == "true") ? HIGH : LOW;
        } else if(node.getId() == "pushbutton2"){
            pushbutton2State = (value == "true") ? HIGH : LOW;
        } else if(node.getId() == "pushbutton3"){
            pushbutton3State = (value == "true") ? HIGH : LOW;
        } else if(node.getId() == "pushbutton4"){
            pushbutton4State = (value == "true") ? HIGH : LOW;            
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

    checkPushButton(pushbutton1Node, debouncer1.read(), pushbutton1State, pushbutton1LastReading);
    checkPushButton(pushbutton2Node, debouncer2.read(), pushbutton2State, pushbutton2LastReading);
    checkPushButton(pushbutton3Node, debouncer3.read(), pushbutton3State, pushbutton3LastReading);
    checkPushButton(pushbutton4Node, debouncer4.read(), pushbutton4State, pushbutton4LastReading);

}

#pragma endregion