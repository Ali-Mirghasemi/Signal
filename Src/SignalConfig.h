/**
 * @file SignalConfig.h
 * @author Ali Mirghasemi (ali.mirgahsemi1376@gmail.com)
 * @brief This is configuration file for Signal library
 * @version 0.1
 * @date 2025-11-03
 * 
 * @copyright Copyright (c) 2025
 * 
 */
#ifndef _SIGNAL_CONFIG_H_
#define _SIGNAL_CONFIG_H_

#include <stdint.h>

/* Check User Config file exists to include it or not */
#ifndef __has_include
    #define __has_include(X)    0
#endif

#if defined(SIGNAL_USER_CONFIG) || __has_include("SignalUserConfig.h")
    #include "SignalUserConfig.h"
#endif

/******************************************************************************/
/*                                Configuration                               */
/******************************************************************************/

/**
 * @brief define SIGNAL_MULTI_CALLBACK if you want have separate callback functions
 * for each state such as Signal_onFalling, Signal_onLow, Signal_onRising, Signal_onHigh
 */
#ifndef SIGNAL_MULTI_CALLBACK
    #define SIGNAL_MULTI_CALLBACK              1
#endif
/**
 * @brief if you enable this option you can disable or enable Signal in runtime
 */
#ifndef SIGNAL_ENABLE_FLAG
    #define SIGNAL_ENABLE_FLAG                 0
#endif
/**
 * @brief user must define initPin function in Signal_Driver
 */
#ifndef SIGNAL_IO_INIT
    #define SIGNAL_IO_INIT                     0
#endif
/**
 * @brief user must define deinitPin function in Signal_Driver
 */
#ifndef SIGNAL_IO_DEINIT
    #define SIGNAL_IO_DEINIT                   0
#endif
/**
 * @brief if Signal is based on pair of GPIO and Pin num must enable it
 * for arduino must disable it
 */
#ifndef SIGNAL_CONFIG_IO
    #define SIGNAL_CONFIG_IO                   1
#endif
/**
 * @brief enable it if you want have a feature to ignore incoming events
 * set it to -1 for disable it and get all callbacks
 */
#ifndef SIGNAL_IDLE
    #define SIGNAL_IDLE                        Signal_State_High
#endif

#if SIGNAL_CONFIG_IO
/**
 * @brief hold Signal io
 * user can change it to GPIO_TypeDef or anything else that system want
 */
typedef void* Signal_IO;
#endif // SIGNAL_CONFIG_IO

/**
 * @brief hold Signal pin num or pin bit
 * user can change it to uint8_t for 8-bit systems like AVR
 */
#ifndef SIGNAL_PIN
    typedef uint16_t Signal_Pin;
#endif

/**
 * @brief maximum number of Signals
 * -1 for unlimited, lib use linked list 
 * x for limited Signals, lib use pointer array
 */
#ifndef SIGNAL_MAX_NUM
    #define SIGNAL_MAX_NUM                     -1
#endif

/**
 * @brief user can store some args in Signal struct and retrieve them in callbacks
 */
#ifndef SIGNAL_ARGS
    #define SIGNAL_ARGS                        0
#endif

/******************************************************************************/

#endif // _SIGNAL_CONFIG_H_
