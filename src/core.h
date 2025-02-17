#pragma once

#include "libretro_script.h"
#include "util.h"
#include <hcdebug.h>

// TODO: clear these on deinit
extern struct core_t
{
    // these are set by the front-end
    RETRO_SCRIPT_DECLT(retro_set_environment) retro_set_environment;
    RETRO_SCRIPT_DECLT(retro_get_memory_data) retro_get_memory_data;
    RETRO_SCRIPT_DECLT(retro_get_memory_size) retro_get_memory_size;
    RETRO_SCRIPT_DECLT(retro_init) retro_init;
    RETRO_SCRIPT_DECLT(retro_deinit) retro_deinit;
    RETRO_SCRIPT_DECLT(retro_run) retro_run;
    
    // this is set by the backend via retro_set_environment callback.
    retro_get_proc_address_t retro_get_proc_address;
    
    // optional, set via retro_get_proc_address
    struct
    {
        hc_Set set_debugger;
        bool debugger_is_init;
        hc_DebuggerIf debugger;
        void* userdata;
    } hc;
} core;

typedef void (* breakpoint_cb_t)(unsigned id);

extern struct frontend_callbacks_t
{
    retro_environment_t retro_environment;
    breakpoint_cb_t breakpoint_cb;
} callbacks;

typedef void (*core_init_cb_t)();

// attach a function that runs once every time a new core is created/destroyed
void retro_script_register_on_init(core_init_cb_t);
void retro_script_register_on_deinit(core_init_cb_t);

// write code that runs directly when a core is created/destroyed
#define ON_INIT() \
    static void _core_init_fn(); \
    INITIALIZER(_init_core_init_fn) { retro_script_register_on_init(_core_init_fn); }; \
    static void _core_init_fn()

#define ON_DEINIT() \
    static void _core_deinit_fn(); \
    INITIALIZER(_init_core_deinit_fn) { retro_script_register_on_deinit(_core_deinit_fn); }; \
    static void _core_deinit_fn()
