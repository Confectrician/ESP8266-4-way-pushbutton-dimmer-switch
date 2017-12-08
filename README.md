# ESP8266 4-Way Pushbutton Dimmer Switch

This is a sketch that uses 4 pushbuttons, to interact as pushbutton, switch or dimmer over mqtt.
It is based on [homie convention](marvinroger/homie) and uses the [homie-esp8266](marvinroger/homie-esp8266) framework.

### Pinout Information (Wemos D1 Mini)

Please be aware that only pin 3 and 4 have an integrated Pull up resistor.
For the other pins of your choice, you will have to add a 10kΩ pullup resistor to each input.