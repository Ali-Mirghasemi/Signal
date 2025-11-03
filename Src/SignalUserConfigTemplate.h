/**
 * @file SignalUserConfigTemplate.h
 * @author Ali Mirghasemi (ali.mirghasemi1376@gmail.com)
 * @brief This is template configuration for Signal library
 * @version 0.1
 * @date 2025-11-03
 * 
 * @copyright Copyright (c) 2025
 * 
 */
#ifndef _SIGNAL_USER_CONFIG_H_
#define _SIGNAL_USER_CONFIG_H_

/**
 * @brief define SIGNAL_MULTI_CALLBACK if you want have separate callback functions
 * for each state such as Signal_onFalling, Signal_onLow, Signal_onRising, Signal_onHigh
 */
//#define SIGNAL_MULTI_CALLBACK              1
/**
 * @brief if you enable this option you can disable or enable Signal in runtime
 */
//#define SIGNAL_ENABLE_FLAG                 0
/**
 * @brief user must define initPin function in Signal_Driver
 */
//#define SIGNAL_IO_INIT                     0
/**
 * @brief user must define deinitPin function in Signal_Driver
 */
//#define SIGNAL_IO_DEINIT                   0
/**
 * @brief if Signal is based on pair of GPIO and Pin num must enable it
 * for arduino must disable it
 */
//#define SIGNAL_CONFIG_IO                   1
/**
 * @brief enable it if you want have a feature to ignore incoming events
 * set it to -1 for disable it and get all callbacks
 */
//#define SIGNAL_IDLE                        Signal_State_High

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
//typedef uint16_t Signal_Pin;

/**
 * @brief maximum number of Signals
 * -1 for unlimited, lib use linked list 
 * x for limited Signals, lib use pointer array
 */
//#define SIGNAL_MAX_NUM                     -1

/**
 * @brief user can store some args in Signal struct and retrieve them in callbacks
 */
//#define SIGNAL_ARGS                        0

#endif // _SIGNAL_USER_CONFIG_H_
