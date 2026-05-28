
static LIBMONO_DATA_ATTRIBUTE libmono_runtime_tick_t libmono_runtime_tick_current=0;

void libmono_runtime_tick_inc(uint8_t time_ms) LIBMONO_FUNCTION_ATTRIBUTE
{
    libmono_runtime_criticalsection_enter();
    libmono_runtime_tick_current+=time_ms;
    libmono_runtime_criticalsection_leave();
}


libmono_runtime_tick_t libmono_runtime_tick_get(void) LIBMONO_REENTRANT_FUNCTION_ATTRIBUTE
{
    libmono_runtime_criticalsection_enter();
    libmono_runtime_tick_t ret=libmono_runtime_tick_current;
    libmono_runtime_criticalsection_leave();
    return ret;
}

