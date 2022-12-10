# Signal Library
This library provide tracking input signals and generate event on signal change

## Abstract

This library used for handle input signals

it's based on HAL layer
user can port library to every hardware 

## Port

for port library user must implement following functions

initPin - configure pin based on input PinConfig
```C
void Signal_initPin(const Signal_PinConfig* config);
```
deinitPin - reset pin to register reset value
```C
void Signal_deInitPin(const Signal_PinConfig* config);
```
readPin - read pin and return 1 or 0
```C
uint8_t Signal_readPin(const Signal_PinConfig* config);
```

and in the final step give Signal_Driver to Signal_init function
remember deinitPin function it's optional and can be disable in Configuration 
```C
const Signal_Driver signalpadDriver = {
    Signal_initPin,
    Signal_readPin,
#if SIGNAL_USE_DEINIT
    Signal_deInitPin,
#endif
};
//...

Signal_init(&signalDriver);

```

## Configuration

in the configuration part in top of Key.h user can change and customize library based on what need in project

#### SIGNAL_MULTI_CALLBACK
define KEY_MULTI_CALLBACK if u want have sperate callback functions for each state such as Signal_onFalling, Signal_onLow, Signal_onRising

#### SIGNAL_USE_DEINIT
user must define deinitPin function in Signal_Driver

#### SIGNAL_CONFIG_IO
if signal is based on pair of GPIO and Pin num must enable it for arduino must disable it

#### SIGNAL_IO
hold signal io
user can change it to GPIO_TypeDef or anything else that system want

#### SIGNAL_PIN
hold signal pin num or pin bit
user can change it to uint8_t for 8-bit systems like AVR

#### SIGNAL_MAX_NUM
maximum number of signals
-1 for unlimited, lib use linked list 
x for limited signals, lib use pointer array
