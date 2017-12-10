/*
    main.cpp - Library for ESP8266 4 way pushbutton dimmer switch project
    Creadted by Jerome Luckenbach, 2017-12-08
    Released under MIT License
*/
#include <Homie.h>
#include <pushbutton.h>
#include <constants.h>
#include <pins.h>

// Pin config
const int pushbutton1Pin = PUSHBUTTON1_PIN; // Caution -> 10k pullup needed here
const int pushbutton2Pin = PUSHBUTTON2_PIN; // Caution -> 10k pullup needed here
const int pushbutton3Pin = PUSHBUTTON3_PIN;
const int pushbutton4Pin = PUSHBUTTON4_PIN;

// Button meta variables
// TODO Maybe let pushButton inherit from HomieNode, to keep all metadata in one class
pushButton pushButton1;
pushButton pushButton2;
pushButton pushButton3;
pushButton pushButton4;

// Node instances
HomieNode pushbutton1Node(NODE_1, NODE_TYPE);
HomieNode pushbutton2Node(NODE_2, NODE_TYPE);
HomieNode pushbutton3Node(NODE_3, NODE_TYPE);
HomieNode pushbutton4Node(NODE_4, NODE_TYPE);

// function declarations
bool globalInputHandler(const HomieNode& node, const String& property, const HomieRange& range, const String& value);
void checkPushButton(const HomieNode& node, const int& reading, int& state, int& lastReading);
void loopHandler();


void setup() {
    Serial.begin(115200);
    Serial << endl << endl;

    // Hardware configuration
    pinMode(pushbutton1Pin, INPUT);
    pinMode(pushbutton2Pin, INPUT);
    pinMode(pushbutton3Pin, INPUT);
    pinMode(pushbutton4Pin, INPUT);

    digitalWrite(pushbutton1Pin, HIGH);
    digitalWrite(pushbutton2Pin, HIGH);
    digitalWrite(pushbutton3Pin, HIGH);
    digitalWrite(pushbutton4Pin, HIGH);
    
    // Debouncer configuration
    pushButton1.debouncer.attach(pushbutton1Pin);
    pushButton2.debouncer.attach(pushbutton2Pin);
    pushButton3.debouncer.attach(pushbutton3Pin);
    pushButton4.debouncer.attach(pushbutton4Pin);

    pushButton1.debouncer.interval(DEBOUNCE_INTERVAL);
    pushButton2.debouncer.interval(DEBOUNCE_INTERVAL);
    pushButton3.debouncer.interval(DEBOUNCE_INTERVAL);
    pushButton4.debouncer.interval(DEBOUNCE_INTERVAL);


    // Homie configuration
    Homie_setFirmware("4-way-pushbutton-dimmer-switch", "1.0.0"); 
    Homie.disableResetTrigger(); // Needed to prevent resetting the device by mistake, since we are using D3 input.
    Homie.setGlobalInputHandler(globalInputHandler);
    Homie.setLoopFunction(loopHandler);

    // Node configurations
    pushbutton1Node.advertise("pressed");
    pushbutton2Node.advertise("pressed");
    pushbutton3Node.advertise("pressed");
    pushbutton4Node.advertise("pressed");

    pushbutton1Node.advertise("switchState").settable();
    pushbutton2Node.advertise("switchState").settable();
    pushbutton3Node.advertise("switchState").settable();
    pushbutton4Node.advertise("switchState").settable();

    pushbutton1Node.advertise("dimmerValue").settable();
    pushbutton2Node.advertise("dimmerValue").settable();
    pushbutton3Node.advertise("dimmerValue").settable();
    pushbutton4Node.advertise("dimmerValue").settable();

    Homie.setup();
}

void loop() {
    Homie.loop();
    pushButton1.debouncer.update();
    pushButton2.debouncer.update();
    pushButton3.debouncer.update();
    pushButton4.debouncer.update();
}

void checkPushButton(const HomieNode& node, const int& reading, pushButton& button){
    
    // pushbutton stated changed
    if(reading != button.lastReading){

        Homie.getLogger() << node.getId() << " changed on " << millis() << " millis" << endl;        
        node.setProperty("pressed").send(reading ? "false" : "true");

        // update switchState when the pushbutton is released and was pushed not later than DIMMER_PUSH_INTERVAL
        if(reading == HIGH && button.lastReading== LOW && (millis() - button.lastReadingTime < DIMMER_PUSH_INTERVAL)){

            node.setProperty("switchState").send(button.switchState ? "false" : "true");
            node.setProperty("dimmerValue").send(button.switchState ? "0" : "100");

            button.dimmerValue = button.switchState? 0 : 100;
            button.switchState = button.switchState ? LOW : HIGH;
            
        }

        button.lastReading = reading;
        button.lastReadingTime = millis();
    }
    // pushbutton state is consant. 
    else if(reading == button.lastReading){

        // start dimmerModer when DIMMER_PUSH_INTERVAL time has reached and button is pressed
        if(!button.dimmerMode && reading == LOW && (millis() - button.lastReadingTime) > DIMMER_PUSH_INTERVAL){
            button.dimmerMode = true;
            button.dimmerReadingTime = button.lastReadingTime;            
        }

        if(button.dimmerMode){
            // start dimmer action when DIMMER_CONTINUE_INTERVAL time has reached and button is pressed
            if(reading == LOW && (millis() - button.dimmerReadingTime) > DIMMER_CONTINUE_INTERVAL){    
                
                if(button.switchState == LOW && button.dimmerValue <= 100){
                    button.dimmerValue += DIMMER_STEP_VALUE;
                    node.setProperty("dimmerValue").send(String(button.dimmerValue));

                    if(button.dimmerValue == 100){
                        node.setProperty("switchState").send("true");
                        button.switchState = HIGH;
                    }
                }
                else if(button.switchState == HIGH && button.dimmerValue >= 0){
                    button.dimmerValue -= DIMMER_STEP_VALUE;
                    node.setProperty("dimmerValue").send(String(button.dimmerValue));

                    if(button.dimmerValue == 0){
                        node.setProperty("switchState").send("false");
                        button.switchState = LOW;
                    }
                }

                Homie.getLogger() << "--> " << node.getId() << "dimmer action processed. DimmerValue = " << button.dimmerValue << endl;
                button.dimmerReadingTime = millis();

            }
            // stop dimmerMode when pushbutton is released 
            else if (reading == HIGH) {
                button.dimmerMode = false;
            }
        }

    }
}

bool globalInputHandler(const HomieNode& node, const String& property, const HomieRange& range, const String& value) {

    Homie.getLogger() << "Received on node " << node.getId() << ": " << property << " = " << value << endl;

    if(property == "switchState" && (value == "true" || value == "false")){

        Homie.getLogger() << "Property and value are valid. Checking for the correct Node now." << endl;
        
        if(node.getId() == "pushbutton1"){
            pushButton1.switchState = (value == "true") ? HIGH : LOW;
        } else if(node.getId() == "pushbutton2"){
            pushButton2.switchState = (value == "true") ? HIGH : LOW;
        } else if(node.getId() == "pushbutton3"){
            pushButton3.switchState = (value == "true") ? HIGH : LOW;
        } else if(node.getId() == "pushbutton4"){
            pushButton4.switchState = (value == "true") ? HIGH : LOW;            
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

    checkPushButton(pushbutton1Node, pushButton1.debouncer.read(), pushButton1);
    checkPushButton(pushbutton2Node, pushButton2.debouncer.read(), pushButton2);
    checkPushButton(pushbutton3Node, pushButton3.debouncer.read(), pushButton3);
    checkPushButton(pushbutton4Node, pushButton4.debouncer.read(), pushButton4);

}