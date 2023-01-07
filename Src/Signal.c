#include "Signal.h"

/* private variables */
static const Signal_Driver* signalDriver;
#if SIGNAL_MAX_NUM == -1
    static Signal* lastSignal = SIGNAL_NULL;

    #define __signals()         lastSignal
    #define __next(S)           S = (S)->Previous
#else
    static Signal signals[SIGNAL_MAX_NUM] = {0};

    #define __signals()         signals
    #define __next(S)           S++
#endif // SIGNAL_MAX_NUM == -1

#if KEY_USE_INIT
    #define __initPin(CONF)          if (signalDriver->initPin) { signalDriver->initPin(CONF); }
#else
    #define __initPin(CONF)
#endif

#if KEY_USE_DEINIT
    #define __deinitPin(CONF)        if (signalDriver->deinitPin) { signalDriver->deinitPin(CONF); }
#else
    #define __deinitPin(CONF)
#endif

/**
 * @brief use for initialize
 * 
 * @param driver 
 */
void Signal_init(const Signal_Driver* driver) {
    signalDriver = driver;
}
/**
 * @brief user must place it in timer 
 * all of callbacks handle and fire in this function
 */
void Signal_handle(void) {
    Signal_State state;
    Signal* pSignal = __signals();
#if SIGNAL_MAX_NUM == -1
    while (SIGNAL_NULL != pSignal) {
#else
    uint8_t len = SIGNAL_MAX_NUM;
    while (len-- > 0) {
        if (pSignal->Configured) {
#endif
    #if SIGNAL_ENABLE_FLAG
        if (pSignal->Enabled) {
    #endif // SIGNAL_ENABLE_FLAG
        // update current state
        state = (Signal_State) (((pSignal->State << 1) | signalDriver->readPin(pSignal->Config)) & 0x03);
        pSignal->State = state;
        // call callback on new state
		if (pSignal->NotActive == Signal_NotHandled
        #if SIGNAL_IDLE != -1
            && SIGNAL_IDLE != state
        #endif  
        ) {
        #if SIGNAL_MULTI_CALLBACK
            if (pSignal->Callbacks.callbacks[state]) {
            #if SIGNAL_IDLE != -1
                pSignal->NotActive = 
            #endif
                pSignal->Callbacks.callbacks[state](pSignal, state);
            }
        #else
            if (pSignal->Callbacks.onChange) {
            #if SIGNAL_IDLE != -1
                pSignal->NotActive = 
            #endif
                pSignal->Callbacks.onChange(pSignal, state);
            }
        #endif /* SIGNAL_MULTI_CALLBACK_ENABLE */
        }
    #if SIGNAL_IDLE != -1
        else if (SIGNAL_IDLE == state && pSignal->NotActive != Signal_NotHandled) {
            pSignal->NotActive = Signal_NotHandled;
        }
    #endif
    #if SIGNAL_ENABLE_FLAG
        }
    #endif // Signal_ENABLE_FLAG
    #if SIGNAL_MAX_NUM != -1
        }
    #endif // Signal_ENABLE_FLAG
        __next(pSignal);
    }
}

/**
 * @brief set new pin configuration for signal
 * 
 * @param signal address of signal instance
 * @param config new pin configuration
 * @return uint8_t return 1 if signal added, 0 if there is no space
 */
void Signal_setConfig(Signal* signal, const Signal_PinConfig* config) {
    signal->Config = config;
}
/**
 * @brief get signal pin config
 * 
 * @param signal 
 * @return const Signal_PinConfig* 
 */
const Signal_PinConfig* Signal_getConfig(Signal* signal) {
    return signal->Config;
}
#if SIGNAL_MAX_NUM > 0
/**
 * @brief finding a empty space for new signal in array
 * 
 * @return Signal* return null if not found empty space
 */
Signal* Signal_new(void) {
    uint8_t len = SIGNAL_MAX_NUM;
    Signal* pSignal = signals;
    while (len-- > 0) {
        if (!pSignal->Configured) {
            return pSignal;
        }
        pSignal++;
    }
    return SIGNAL_NULL;
}
#endif // SIGNAL_MAX_NUM
/**
 * @brief add signal into list for process
 * 
 * @param signal address of signal
 * @param config signal pin configuration
 */
uint8_t Signal_add(Signal* signal, const Signal_PinConfig* config) {
    // check for null
    if (SIGNAL_NULL == signal) {
        return 0;
    }
    // add new signal to list
    Signal_setConfig(signal, config);
    // init IOs
    __initPin(config);
    signal->State = (signalDriver->readPin(signal->Config) << 1 | signalDriver->readPin(signal->Config)) & 3;
#if SIGNAL_MAX_NUM == -1
    // add signal to linked list
    signal->Previous = lastSignal;
    lastSignal = signal;
#endif // SIGNAL_MAX_NUM == -1
    signal->Configured = 1;
    signal->Enabled = 1;
    return 1;
}
/**
 * @brief remove signal from list
 * 
 * @param remove address of signal
 * @return uint8_t return 1 if signal found, 0 if not found
 */
uint8_t Signal_remove(Signal* remove) {
#if SIGNAL_MAX_NUM == -1
    Signal* pSignal = lastSignal;
    // check last signal first
    if (remove == pSignal) {
        // deinit IO
        __deinitPin(remove->Config);
        // remove signal dropped from link list
        pSignal->Previous = remove->Previous;
        remove->Previous = SIGNAL_NULL;
        remove->Configured = 0;
        remove->Enabled = 0;
        return 1;
    }
    while (SIGNAL_NULL != pSignal) {
        if (remove == pSignal->Previous) {
            // deinit IO
            __deinitPin(remove->Config);
            // remove signal dropped from link list
            pSignal->Previous = remove->Previous;
            remove->Previous = SIGNAL_NULL;
            remove->Configured = 0;
            remove->Enabled = 0;
            return 1;
        }
        pSignal = pSignal->Previous;
    }
#else
    uint8_t len = SIGNAL_MAX_NUM;
    Signal* pSignal = signals;
    while (len--) {
        if (remove == pSignal && pSignal->Configured) {
            pSignal->Configured = 0;
            pSignal->Enabled = 0;
            return 1;
        }
        pSignal++;
    }
#endif // SIGNAL_MAX_NUM == -1
    return 0;
}
/**
 * @brief finding signal based on PinConfig in list
 * 
 * @param config 
 * @return Signal* 
 */
Signal* Signal_find(const Signal_PinConfig* config) {
#if SIGNAL_MAX_NUM == -1
    Signal* pSignal = lastSignal;
    while (SIGNAL_NULL != pSignal) {
        if (config == pSignal->Config) {
            return pSignal;
        }
        pSignal = pSignal->Previous;
    }
#else
    uint8_t len = SIGNAL_MAX_NUM;
    Signal* pSignal = signals;
    while (len--) {
        if (config == pSignal->Config) {
            return pSignal;
        }
        pSignal++;
    }
#endif // SIGNAL_MAX_NUM == -1
    return SIGNAL_NULL;
}

#if SIGNAL_MULTI_CALLBACK
void Signal_onLow(Signal* signal, Signal_Callback cb) {
    signal->Callbacks.onLow = cb;
}
void Signal_onRising(Signal* signal, Signal_Callback cb) {
    signal->Callbacks.onRising = cb;
}
void Signal_onFalling(Signal* signal, Signal_Callback cb) {
    signal->Callbacks.onFalling = cb;
}
void Signal_onHigh(Signal* signal, Signal_Callback cb) {
    signal->Callbacks.onHigh = cb;
}
#else
void Signal_onChange(Signal* signal, Signal_Callback cb) {
    Signal->Callbacks.onChange = cb;
}
#endif // SIGNAL_MULTI_CALLBACK

#if SIGNAL_ENABLE_FLAG
void Signal_setEnabled(Signal* Signal, uint8_t enabled) {
    signal->Enabled = enabled;
}
uint8_t Signal_isEnabled(Signal* signal) {
    return signal->Enabled;
}
#endif

#if SIGNAL_ARGS
void Signal_setArgs(Signal*, void* args) {
    Signal->Args = args;
}
void* Signal_getArgs(Signal* signal) {
    return signal->Args;
}
#endif

