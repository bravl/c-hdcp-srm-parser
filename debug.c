#include "debug.h"

//TODO: Finish Log to file

/**
 *
 * @file debug.c
 * @author Bram Vlerick
 * @date 22/11/2016 20:53
 * @brief File containing the logger functions
 *
 * This file contains all the functions for the logger
 *
 */

/**
 *
 * @brief Check if provided loglevel is correct
 * @param loglvl The loglevel that will be checked
 * @return 0 if correct -1 if failed
 */
static int check_loglvl(int loglvl) {
    if ((loglvl != LOG_LVL_INFO) | (loglvl != LOG_LVL_WARNING) |
        (loglvl != LOG_LVL_ERROR) | (loglvl != LOG_LVL_OK)) {
        return -1;
    }
    return 0;
}

/**
 * @brief Update the logger counters
 * This function will update the correct counter. Keeping
 * track of how many info/warning/error messages have been
 * logged. Usefull when not all messages are being printed
 *
 * @param loglvl the counter that has to be updated
 */
static void update_counters(int loglvl)
{
    switch (loglvl) {
        case LOG_LVL_INFO: _info++; break;
        case LOG_LVL_WARNING: _warnings++; break;
        case LOG_LVL_ERROR: _errors++; break;
        case LOG_LVL_OK: _ok++; break;
        default: break;
    }
    return;
}

/**
 * @brief Create a timestamp
 * 
 * @return Return a char pointer to a timestamp
 */
static void print_timestamp(FILE *out) {
    char outstr[MAX_HDR_LEN];
    time_t t;
    struct tm *tmp, tmbuf;
    t = time(NULL);
    tmp = localtime_r(&t, &tmbuf);
    if (tmp == NULL)
        return;
    if (strftime(outstr, sizeof(outstr), "%y-%m-%d %H:%M:%S", tmp) == 0)
        return;
    fprintf(out,"%s ",outstr);
    return;
}

/**
 * @brief Create the logger prefix
 *
 * @param loglvl The loglevel that is used for the
 * header
 * @return Returns a header as char pointer
 */
static void print_loglvl(FILE *out,int loglvl) {
    char header[MAX_HDR_LEN];
    if (out != stdout) {
        printf("not using stdout\n");
        return;
    } else {
        if (loglvl == LOG_LVL_ERROR)
            fprintf(out,"[%s%s%s] ",KRED,"ERROR",KNRM);
        if (loglvl == LOG_LVL_WARNING)
            fprintf(out,"[%s%s%s] ",KYEL,"WARN",KNRM);
        if (loglvl == LOG_LVL_INFO)
            fprintf(out,"[%s%s%s] ",KBLU,"INFO",KNRM);
        if (loglvl == LOG_LVL_OK)
            fprintf(out,"[%s%s%s] ",KGRN,"OK",KNRM);
    }
    return;
}

/**
 * @brief Set the loglevel of the logger
 * Set the loglevel of the logger. Examples:
 * To set to info LOG_LVL_INFO
 * To set to info and warning: LOG_LVL_INFO | LOG_LVL_WARNING
 * 
 * @param loglvl The loglevel that will be set
 */
void logger_set_loglevel(int loglvl) {
    _loglvl = loglvl;
    return;
}


/**
 * @brief Enable filelogging
 * @param filename Filename for the the logfile
 * @param timestap Add timestamp to the filename
 */
void logger_set_file_logging(char *filename, int timestamp) {
    _fp = fopen(filename, "rw");
    if (!_fp) {
        // log error
        return;
    }
    return;
}

/**
 * @brief Log some data
 * Log some data using a VAlist
 * 
 * @param loglvl The severity of the message
 * @param msg The message that will be logged
 */
void logger_log(int loglvl, char *format, ...) {
    if (!check_loglvl(loglvl)) {
        logger_log(LOG_LVL_ERROR,"Invalid loglevel provided\n");
        return;
    }
    update_counters(loglvl); 
    if ((_loglvl & loglvl)) {
        print_timestamp(stdout);
        print_loglvl(stdout, loglvl); 
        va_list args;
        va_start(args,format);
        vfprintf(stdout,format, args);
        va_end(args);
        printf("\n");
    }
    return;
}

/**
 * @brief Print log statistics
 * Print the logcounter providing information about logged messages
 */
void logger_print_stats() {
    logger_log(LOG_LVL_INFO, "%ld error(s), %ld warning(s) and %ld info"
                             " message(s)", _errors,_warnings,_info);
    return;
}

