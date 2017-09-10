#ifndef _DEBUG_H_
#define _DEBUG_H_

/**
 * 
 * @file debug.h
 * @author Bram Vlerick
 * @date 22/11/2016 20:50
 * @brief File containing the logger functions
 *
 * This file contains all the function prototypes for 
 * the logger functionality
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include <time.h>

#define MAX_HDR_LEN 64

/**< Loglevel defines */
#define LOG_LVL_INFO    0x01
#define LOG_LVL_WARNING 0x02
#define LOG_LVL_ERROR   0x04
#define LOG_LVL_OK      0x08

/**< Logger enable colors */
#define KNRM  "\x1B[0m"
#define KRED  "\x1B[31m"
#define KGRN  "\x1B[32m"
#define KYEL  "\x1B[33m"
#define KBLU  "\x1B[34m"
#define KMAG  "\x1B[35m"
#define KCYN  "\x1B[36m"
#define KWHT  "\x1B[37m"

/**< Static loglevel variable */
static int _loglvl;

/**< Static logger file pointer */
static FILE *_fp;

/**< Static logger counters */
static long _ok, _errors, _warnings, _info;

/**
 * @brief Set the loglevel of the logger
 * Set the loglevel of the logger. Examples:
 * To set to info LOG_LVL_INFO
 * To set to info and warning: LOG_LVL_INFO | LOG_LVL_WARNING
 * 
 * @param loglvl The loglevel that will be set
 */
void logger_set_loglevel(int loglvl);

/**
 * @brief Enable filelogging
 * @param filename Filename for the the logfile
 * @param timestap Add timestamp to the filename
 */
void logger_set_file_logging(char *filename, int timestamp);

/**
 * @brief Log some data
 * Log some data using a VAlist
 * 
 * @param loglvl The severity of the message
 * @param msg The message that will be logged
 */
void logger_log(int loglvl, char *format, ...);

/**
 * @brief Print log statistics
 * Print the logcounter providing information about logged messages
 */
void logger_print_stats();

#endif
