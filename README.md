# ESP8266 4-Way Pushbutton Dimmer Switch

This is a project that uses 4 inputs on a Wemos D1 Mini, to interact as pushbutton, switch or dimmer over mqtt.
It is based on [homie convention](marvinroger/homie) and uses the [homie-esp8266](marvinroger/homie-esp8266) framework.

## Concepts

According to the convention our topic will start with `base/decive-id` like it has been configured.

From here every pushbutton has its own main topic, which can be configured individually in `constants.h` for every pushbutton.
You can also configure the `NODE_TYPE` here for the device meta informations, that homie provides.

Every pushbutton has 3 functional mqqt topics configured.

- `pressed` indicates if the pushbutton is pressed currently
- `switchState` provides a switchmode, which will alternate between `true` and `false`
- `dimmerValue` provides a value between `0` and `100`

An example topic would for a dimmerValue would then be `homie/livingroom-multidimmer/pushbutton1/dimmerValue`.

The pushbuttons will react different depending on the time they have been pushed.
Short push actions will be interpreted as a *tap-action* and just affect the *switchState* topic.
Longer push actions will be interpreted as a *dimm-action* and change the *dimmerValue*.
The  *dimmerValue* is *increased* or *decreased* depending on the *switchState* **and** the current *dimmerValue* itself.

- *increased* when
`switchState` is LOW/false and the current `dimmerValue` is lower than or equal to `100`

- *decreased* when
`switchState` is HIGH/true and the current `dimmerValue` is greater than or equal to `0`


## Hardware

> partly tbd - just a testing setup @ the moment

- Wemos D1 Mini
- 2 10kΩ resistors (see pinout information)
- pushbuttons
- A DC source of your choice to power up the Wemos

### Custom PCB

tbd

### Pinout Information (Wemos D1 Mini)

Please be aware that only pin D3 and D4 have an integrated Pull up resistor.
For the other pins of your choice, you will have to add a 10kΩ pullup resistor to each input.

Also keep in mind that pin D3 is able to reset your device.
Make sure that `Homie.disableResetTrigger();` is called in the `setup()` function befor `Homie.setup()` is called.

## Pushbutton to dimmer handling

### Compare dimmer to pushbutton input

Caution. The following paragraph could be a bit *geeky*.

After some *practical research* the average time for one **button push to button release event** is about `247,4285714ms`.
Minimum time needed was `191ms`.
Maximum time needed was `287ms`.

Based on this values, i have set the `DIMMER_PUSH_INTERVAL` to `300ms` for now.
This value is used to decide wether a push is hold to start a dimmer or just tapped to update the `switchState`.