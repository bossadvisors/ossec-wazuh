/*
 * Wazuh Module Manager
 * Copyright (C) 2016 Wazuh Inc.
 * April 22, 2016.
 *
 * This program is a free software; you can redistribute it
 * and/or modify it under the terms of the GNU General Public
 * License (version 2) as published by the FSF - Free Software
 * Foundation.
 */

#ifndef W_MODULES
#define W_MODULES

#ifndef ARGV0
#define ARGV0 "wazuh-moduled"
#endif // ARGV0

#include <pthread.h>
#include "shared.h"
#include "config/config.h"

#define WM_DEFAULT_DIR  DEFAULTDIR "/wodles"        // Default modules directory.
#define WM_STATE_DIR    DEFAULTDIR "/var/wodles"    // Default directory for states.
#define WM_STRING_MAX   1048576                     // Max. dynamic string size.
#define WM_BUFFER_MAX   1024                        // Max. static buffer size.
#define WM_MAX_ATTEMPTS 3                           // Max. number of attempts.
#define WM_MAX_WAIT     1                           // Max. wait between attempts.
#define WM_IO_WRITE     0
#define WM_IO_READ      1
#define WM_ERROR_TIMEOUT 1                          // Error code for timeout.

typedef void* (*wm_routine)(void*);     // Standard routine pointer

// Module context: this should be defined for every module

typedef struct wm_context {
    const char *name;                   // Name for module
    wm_routine start;                   // Main function
    wm_routine destroy;                 // Destructor
} wm_context;

// Main module structure

typedef struct wmodule {
    pthread_t thread;                   // Thread ID
    const wm_context *context;          // Context (common structure)
    void *data;                         // Data (module-dependent structure)
    struct wmodule *next;               // Pointer to next module
} wmodule;

// Inclusion of modules

#include "wm_oscap.h"

extern wmodule *wmodules;               // Loaded modules.
extern int wm_task_nice;                // Nice value for tasks.

// Check general configuration
void wm_check();

// Destroy configuration data
void wm_destroy();

/* Execute command with timeout of secs. exitcode can be NULL.
 *
 * command is a mutable string.
 * output is a pointer to dynamic string. Caller is responsible for freeing it!
 * On success, return 0. On another error, returns -1.
 * If the called program timed-out, returns WM_ERROR_TIMEOUT and output may
 * contain data.
 */
int wm_exec(char *command, char **output, int *exitcode, int secs);

// Check whether the last execution timed out
int wm_exec_timeout();

/* Concatenate strings with optional separator
 *
 * str1 must be a valid pointer to NULL or a string at heap
 * Returns 0 if success, or -1 if fail.
 */
int wm_strcat(char **str1, const char *str2, char sep);

// Trim whitespaces from string
char* wm_strtrim(char *string);

// Tokenize string separated by spaces, respecting double-quotes
char** wm_strtok(char *string);

/* Load or save the running state
 * op: WM_IO_READ | WM_IO_WRITE
 * Returns 0 if success, or 1 if fail.
 */
int wm_state_io(const wm_context *context, int op, void *state, size_t size);

#endif // W_MODULES
