#include "config.h"
#include "logging.h"
#include "logging_win.h"
#include <windows.h>
#include <pathcch.h>
#include <ShlObj.h>
#include <stdlib.h>
#include <time.h>
#include <stdarg.h>
#include <stdio.h>
#include "str_conv.h"
#include "exe_info.h"


/**
 * get logging directory path
 */
static LPWSTR
logging_get_logging_dir();

/**
 * allocate heap object
 */
static void*
logging_alloc(
    size_t size);

/**
 * free heap object 
 */
static void
logging_free(
    void* obj);

/**
 * get logfile path 
 *
 */
static LPWSTR
logging_get_logfile_path();


/**
 * open logfile stream  
 */
static FILE*
logging_open_logfile_stream(
    LPCWSTR logfile);

/**
 * create logfile directory if you don't have
 */
static int
logging_create_logfile_dir_if_not(
    LPCWSTR logfile);

/**
 * log with va_list 
 */
static void
logging_log_v(
    int priority,
    const char* format,
    va_list v_arg);

/**
 * the messaige is written into log file if input priority is greater than or
 * equals level.
 */
static int
logging_level = DEFAULT_LOGGING_LEVEL;

/**
 * start logging
 */
void
logging_start()
{
}

/**
 * log
 */
void
logging_log(
    int priority,
    const char* format,
    ...)
{
    if (priority >= logging_level) {
        va_list v_arg;

        if (priority > LOG_LEVEL_DEBUG) {
            priority = LOG_LEVEL_DEBUG;
        } else if (priority < 0) {
            priority = LOG_LEVEL_EMERG;
        }
        va_start(v_arg, format);
        logging_log_v(priority, format, v_arg);
        va_end(v_arg);
    }
}

/**
 * set logging level
 */
void
logging_set_level(
    int priority)
{
    logging_level = priority;
}

/**
 * log with va_list 
 */
static void
logging_log_v(
    int priority,
    const char* format,
    va_list v_arg)
{
    LPWSTR logfile_path;
    FILE* fs;
    int state;
    char* message_buffer;
    char* time_str_buffer;
    int message_size;
    const static size_t time_str_buffer_size = 0xff;
    state = 0;
    message_size = 0;
    fs = NULL;
    logfile_path = NULL;
    message_buffer = NULL;
   
    logfile_path = logging_get_logfile_path();
    state = logfile_path ? 0 : -1;

    if (state == 0) {
        fs = logging_open_logfile_stream(logfile_path);
        state = fs ? 0 : -1;
    }
    if (state == 0) {
        message_buffer = (char*)logging_alloc(LOGGING_MAX_MESSAGE_SIZE);
        state = message_buffer ? 0 : -1;
    }
    if (state == 0)  {
        time_str_buffer = (char*)logging_alloc(time_str_buffer_size);
        state = time_str_buffer ? 0 : -1;
    }
    if (state == 0) {
        time_t time_now;
        time_t time_now_res;
        struct tm tm_now;
        time_now_res = time(&time_now);
        state = time_now_res != (time_t) -1 ? 0 : -1;
        if (state == 0) {
            struct tm* tm_res;
            tm_res = localtime(&time_now);
            state = tm_res ? 0 : -1;
            if (state == 0) {
                tm_now = *tm_res;
            }
        }
        if (state == 0) {
            strftime(time_str_buffer, time_str_buffer_size,
                "%F%T%z", &tm_now);
        }
    }
    if (state == 0) {
        va_list arg;
        va_copy(arg, v_arg);
        message_size = vsnprintf(
            message_buffer, LOGGING_MAX_MESSAGE_SIZE, format, arg);
        va_end(arg);
    }
    if (state == 0) {
        if (message_size) {
            fprintf(fs,
                "%s\n"
                "%0x\n"
                "%0x\n"
                "%d\n"
                "%s\n", 
                time_str_buffer, 
                GetCurrentProcessId(),
                GetCurrentThreadId(),
                priority, message_buffer);
        }
    }

    if (message_buffer) {
        logging_free(message_buffer);
    }
    if (time_str_buffer) {
        logging_free(time_str_buffer);
    }
    if (fs) {
        fclose(fs);
    }
    if (logfile_path) {
        logging_free(logfile_path);
    }
}



/**
 * stop logging
 */
void
logging_stop()
{
}

/**
 * open logfile stream  
 */
static FILE*
logging_open_logfile_stream(
    LPCWSTR logfile_path)
{
    FILE* result;
    int state;
    result = NULL;
    state = logging_create_logfile_dir_if_not(logfile_path);
    if (state == 0) {
        result = _wfopen(logfile_path, L"a+");
    }
    return result;
}


/**
 * create logfile directory if you don't have
 */
static int
logging_create_logfile_dir_if_not(
    LPCWSTR logfile_path)
{
    LPWSTR dir;
    size_t logfile_path_len;
    int result;
    logfile_path_len = wcslen(logfile_path);

    dir = (LPWSTR)logging_alloc((logfile_path_len + 1) * sizeof(WCHAR));
    result = dir ? 0 : -1;

    if (result == 0) {
        HRESULT hres;
        memcpy(dir, logfile_path, (logfile_path_len + 1) * sizeof(WCHAR));
        hres = PathCchRemoveFileSpec(dir, logfile_path_len + 1);
        result = SUCCEEDED(hres) ? 0 : -1;
    }
    if (result == 0) {
        int sh_res;
        sh_res = SHCreateDirectory(NULL, dir);
        switch (sh_res) {
        case ERROR_SUCCESS:
        case ERROR_ALREADY_EXISTS:
            result = 0;
            break;
        default:
            result = -1;
        }
    }
    if (dir) {
        logging_free(dir);
    }

    return result;
}


/**
 * get logfile path 
 *
 */
static LPWSTR
logging_get_logfile_path()
{
    LPWSTR logging_dir;
    LPWSTR logging_path;
    int state;
    LPWSTR result;
    state = 0;
    logging_path = NULL;
    result = NULL;
    logging_dir = logging_get_logging_dir();
    state = logging_dir ? 0 : -1;
    if (state == 0) {
        HRESULT h_res;
        h_res = PathAllocCombine(
            logging_dir, 
            LOGGING_FILE_NAME_W, 
            PATHCCH_ALLOW_LONG_PATHS, &logging_path);
        state = SUCCEEDED(h_res) ? 0 : -1;
    }
    if (state == 0) {
        size_t len;
        len = wcslen(logging_path);
        result = (LPWSTR)logging_alloc((len + 1) * sizeof(WCHAR));
        if (result) {
            memcpy(result, logging_path, (len + 1) * sizeof(WCHAR));
        }
    }
    if (logging_dir) {
        logging_free(logging_dir);
    }

    if (logging_path) {
        LocalFree(logging_path);
    }
    return result;
}


/**
 * get logging directory path
 */
static LPWSTR
logging_get_logging_dir()
{
    LPWSTR result;
    LPWSTR tmp_dir;
    LPWSTR logging_dir;
    char* exe_name_0;
    LPWSTR exe_name_1;
    int state;
    DWORD dw_state;
    result = NULL;
    exe_name_0 = NULL;
    logging_dir = NULL;
    state = 0;
    dw_state = 0;
    tmp_dir = (LPWSTR)logging_alloc((MAX_PATH + 1) * sizeof(WCHAR));

    state = tmp_dir ? 0 : -1;
    if (state == 0) {
        dw_state = GetTempPathW(MAX_PATH + 1, tmp_dir);
        state = dw_state ? 0 : -1;
    }

    if (state == 0) {
        exe_name_0 = exe_info_get_exe_name();
        state = exe_name_0 ? 0 : -1;
    }
    if (state == 0) {
        exe_name_1 = str_conv_utf8_to_utf16(exe_name_0,
            strlen(exe_name_0) + 1,
            logging_alloc, 
            logging_free);
        state = exe_name_1 ? 0 : -1;
    }

    if (state == 0) {
        HRESULT h_res;
        h_res = PathAllocCombine(
            tmp_dir, exe_name_1, PATHCCH_ALLOW_LONG_PATHS, &logging_dir);
        state = SUCCEEDED(h_res) ? 0 : -1;
    }
    if (state == 0) {
        size_t len;
        len = wcslen(logging_dir);
        result = (LPWSTR)logging_alloc((len + 1) * sizeof(WCHAR));
        if (result) {
            memcpy(result, logging_dir, (len + 1) * sizeof(WCHAR));
        }

    }

    if (logging_dir) {
        LocalFree(logging_dir);
    }
    if (exe_name_1) {
        logging_free(exe_name_1);
    }
    if (exe_name_0) {
        exe_info_free(exe_name_0);
    }
    if (tmp_dir) {
        logging_free(tmp_dir);
    }
    return result;
}

/**
 * allocate heap object
 */
static void*
logging_alloc(
    size_t size)
{
    return malloc(size);
}

/**
 * free heap object 
 */
static void
logging_free(
    void* obj)
{
    free(obj);
}

/* vi: se ts=4 sw=4 et: */
