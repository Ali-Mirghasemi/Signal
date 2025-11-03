 /**
 * @file Signal.h
 * @author Ali Mirghasemi (ali.mirghasemi1376@gmail.com)
 * @brief this library use for drive Signal, button and input signals
 * @version 0.1.0
 * @date 2022-12-10
 * 
 * @copyright Copyright (c) 2022
 */

#ifndef _SIGNAL_H_
#define _SIGNAL_H_

#ifdef __cplusplus
extern "C" {
#endif 

#define SIGNAL_VER_MAJOR    0
#define SIGNAL_VER_MINOR    2
#define SIGNAL_VER_FIX      0

#include "SignalConfig.h"

#define __SIGNAL_VER_STR(major, minor, fix)     #major "." #minor "." #fix
#define _SIGNAL_VER_STR(major, minor, fix)      __SIGNAL_VER_STR(major, minor, fix)
/**
 * @brief show signal version in string format
 */
#define SIGNAL_VER_STR                          _SIGNAL_VER_STR(SIGNAL_VER_MAJOR, SIGNAL_VER_MINOR, SIGNAL_VER_FIX)
/**
 * @brief show signal version in integer format, ex: 0.2.0 -> 200
 */
#define SIGNAL_VER                              ((SIGNAL_VER_MAJOR * 10000UL) + (SIGNAL_VER_MINOR * 100UL) + (SIGNAL_VER_FIX))

#define SIGNAL_NULL            ((Signal*) 0)
#define SIGNAL_CONFIG_NULL     ((Signal_PinConfig*) 0)

/**
 * @brief hold pin configuration that use for handle Signal
 */
typedef struct {
#if SIGNAL_CONFIG_IO
    Signal_IO     	IO;
#endif
    Signal_Pin      Pin;
} Signal_PinConfig;

/**
 * @brief Signal state
 *     _______               ___(High)___
 * (Falling)<-|____(Low)____|-> (Rising)
 */
typedef enum {
    Signal_State_Low           = 0x00,
    Signal_State_Rising        = 0x01,
    Signal_State_Falling       = 0x02,
    Signal_State_High          = 0x03,
} Signal_State;

/**
 * @brief show next callbacks can fire or ignore incoming callbacks
 */
typedef enum {
    Signal_NotHandled           = 0,
    Signal_Handled	            = 1
} Signal_HandleStatus;

struct _Signal;
typedef struct _Signal Signal;

/**
 * @brief initialize pin in input mode, remember if your pin is pull-up, or pull-down 
 * must configured in init function
 * this function call when new Signal add to queue
 */
typedef void (*Signal_InitPinFn)(const Signal_PinConfig* config);
/**
 * @brief de-initialize pin and change pin to reset mode
 * this function call on remove Signal
 */
typedef void (*Signal_DeInitPinFn)(const Signal_PinConfig* config);
/**
 * @brief this function must return value of a pin
 * 0 -> LOW, 1 -> HIGH
 */
typedef uint8_t (*Signal_ReadPinFn)(const Signal_PinConfig* config);
/**
 * @brief this callback call when Signal state change
 * 
 * @param signal show which Signal changed
 * @param state show current state of Signal
 * @return user can return Signal_NotHandled (0) if wanna get callback on other events 
 *                  otherwise can return Signal_Handled (1) that mean Signal handled nad next event is onPressed
 */
typedef Signal_HandleStatus (*Signal_Callback)(Signal* signal, Signal_State state);
/**
 * @brief hold minimum function for Signal lib to work
 * user must pass atleast init and read functions to Signal library
 */
typedef struct {
    Signal_ReadPinFn     readPin;
#if SIGNAL_IO_INIT
    Signal_InitPinFn     initPin;
#endif
#if SIGNAL_IO_DEINIT
    Signal_DeInitPinFn   deinitPin;
#endif
} Signal_Driver;


// determine how many callbacks need
#if SIGNAL_MULTI_CALLBACK 
    #define SIGNAL_CALLBACKS_NUM 4
#else
    #define SIGNAL_CALLBACKS_NUM 1
#endif

typedef union {
    Signal_Callback            callbacks[SIGNAL_CALLBACKS_NUM];
    struct {
    #if SIGNAL_MULTI_CALLBACK
        Signal_Callback        onLow;
        Signal_Callback        onRising;
        Signal_Callback        onFalling;
        Signal_Callback        onHigh;
    #else
        Signal_Callback        onChange;
    #endif // SIGNAL_MULTI_CALLBACK
    };
} Signal_Callbacks;

/**
 * @brief this struct hold Signal parameters
 * do not change the parameters or something else with it
 */
struct _Signal {
#if SIGNAL_MAX_NUM == -1
    struct _Signal*             Previous;               			/**< point to previous Signal, if it's null show they Signal is end of linked list */
#endif // SIGNAL_MAX_NUM == -1
#if SIGNAL_ARGS
    void*                       Args;
#endif
    const Signal_PinConfig*     Config;                 			/**< hold pointer to pin configuration */
    Signal_Callbacks            Callbacks;                          /**< hold user separate callbacks for each Signal state */
    uint8_t                     State           : 2;    			/**< show current state of Signal*/
    uint8_t                     NotActive       : 1;    			/**< show other states will be ignore or not */
    uint8_t                     Configured      : 1;                /**< this flag shows Signal is configured or not, just useful fo fixed Signal num */
    uint8_t                     Enabled         : 1;                /**< check this flag in irq */
    uint8_t                     Reserved        : 3;
};

void Signal_init(const Signal_Driver* driver);
void Signal_handle(void);

void Signal_setConfig(Signal* signal, const Signal_PinConfig* config);
const Signal_PinConfig* Signal_getConfig(Signal* signal);

#if SIGNAL_MAX_NUM > 0
    Signal* Signal_new(void);
#endif // SIGNAL_MAX_NUM

uint8_t  Signal_add(Signal* signal, const Signal_PinConfig* config);
uint8_t  Signal_remove(Signal* remove);
Signal*  Signal_find(const Signal_PinConfig* config);

#if SIGNAL_MULTI_CALLBACK
    void Signal_onLow(Signal* signal, Signal_Callback cb);
    void Signal_onRising(Signal* signal, Signal_Callback cb);
    void Signal_onFalling(Signal* signal, Signal_Callback cb);
    void Signal_onHigh(Signal* signal, Signal_Callback cb);
#else
    void Signal_onChange(Signal* Signal, Signal_Callback cb);
#endif // SIGNAL_MULTI_CALLBACK

#if SIGNAL_ENABLE_FLAG
    void Signal_setEnabled(Signal* signal, uint8_t enabled);
    uint8_t Signal_isEnabled(Signal* signal);
#endif

#if SIGNAL_ARGS
    void  Signal_setArgs(Signal*, void* args);
    void* Signal_getArgs(Signal* signal);
#endif

#ifdef __cplusplus
};
#endif

#endif /* _SIGNAL_H_ */
