#ifndef MACROS_HPP
#define MACROS_HPP
 
#define num_to_ghz(x) num_to_mhz(x)*1000
#define num_to_mhz(x) num_to_khz(x)*1000
#define num_to_khz(x) x*1000 

static inline const char* f(const char* fmt, ...) {
    static char __fbuf[128];
    va_list args;
    va_start(args, fmt);
    vsnprintf(__fbuf, sizeof(__fbuf), fmt, args);
    va_end(args);
    return __fbuf;
}

static inline const char* f(const __FlashStringHelper* fmt, ...) { 
    return f(reinterpret_cast<const char *>(fmt));
}
#endif