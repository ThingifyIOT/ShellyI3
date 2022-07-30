#include <Arduino.h>
#include <ThingifyEsp.h>
#include <Lib\DebounceButton\DebounceButton.h>

ThingifyEsp thing("Shelly I3");


// pin definitions
const int Switch1Pin = 14;
const int Switch2Pin = 12;
const int Switch3Pin = 13;

const int ResetButtonPin = 2;

// nodes
Node *switch1Node, *switch2Node, *switch3Node;

//buttons
DebounceButton switch1(Switch1Pin, INPUT);
DebounceButton switch2(Switch2Pin, INPUT);
DebounceButton switch3(Switch3Pin, INPUT);

// variables
bool _wasResetPressed = false;
uint64_t _resetPressStartTime = 0;
uint64_t _lastTemperatureRead = 0;

void setup()
{
    Serial.begin(500000);
    thing.Initialize();
    thing.AddDiagnostics();

    switch1.Init();
    switch2.Init();
    switch3.Init();

    pinMode(ResetButtonPin, INPUT);

    switch1Node = thing.AddBoolean("Switch1");
    switch2Node = thing.AddBoolean("Switch2");
    switch3Node = thing.AddBoolean("Switch3");

    thing.Start();
}

void loop()
{
    switch1.Loop();
    switch2.Loop();
    switch3.Loop();
    
    switch1Node->Set(switch1.IsPressed());
    switch2Node->Set(switch2.IsPressed());
    switch3Node->Set(switch3.IsPressed());

   // detect button reset long press
    bool isResetPressed = !digitalRead(ResetButtonPin);

    if(!_wasResetPressed && isResetPressed)
    {
        _resetPressStartTime = millis();
    }

    if(!isResetPressed)
    {
        _resetPressStartTime = 0;
    }    

    if(_resetPressStartTime != 0 && millis() - _resetPressStartTime > 5000)
    {
        thing.ResetConfiguration();
    }
    _wasResetPressed = isResetPressed;
    // end detect button reset long press

    thing.Loop();
}
