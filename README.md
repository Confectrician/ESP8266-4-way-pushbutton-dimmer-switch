# ESP8266 4-Way Pushbutton Dimmer Switch

This is a sketch that uses 4 pushbuttons, to interact as pushbutton, switch or dimmer over mqtt.
It is based on [homie convention](marvinroger/homie) and uses the [homie-esp8266](marvinroger/homie-esp8266) framework.

## Concepts

### Compare dimmer to pushbutton input

Caution.The following paragraph could be a bit *geeky*.

After some *practical research* the average time for one **button push to button release event** is about `247,4285714ms`.
Minimum time needed was `191ms`.
Maximum time needed was `287ms`.

Based on this values, i have set the `DIMMER_PUSH_INTERVAL` to `300ms` for now.
This value is used to decide wether a push is hold to start a dimmer or just tapped to update the `switchState`.

## Hardware

### Pinout Information (Wemos D1 Mini)

Please be aware that only pin 3 and 4 have an integrated Pull up resistor.
For the other pins of your choice, you will have to add a 10kΩ pullup resistor to each input.