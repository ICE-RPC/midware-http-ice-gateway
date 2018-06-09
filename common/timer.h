
#pragma once

#include <chrono>

#define TIME_SINCE_EPOCH(tv)                                                            \
        int64_t tv = std::chrono::duration_cast<std::chrono::milliseconds>(             \
                    std::chrono::system_clock::now().time_since_epoch()                 \
                    ).count();
#define TIME_SINCE_EPOCH_MICROSEC(tv)                                                   \
        int64_t tv = std::chrono::duration_cast<std::chrono::microseconds>(             \
                    std::chrono::system_clock::now().time_since_epoch()                 \
                    ).count();

#define TIMER_NOW(tv)                                                                   \
        tv = std::chrono::steady_clock::now();

#define TIMER_ELAPSE(start, end, elapse)                                                \
        do {                                                                            \
            std::chrono::microseconds __elapse__                                        \
                = std::chrono::duration_cast<std::chrono::microseconds>(end-start);     \
            elapse = __elapse__.count()/1000.0;                                         \
        } while(0);

#define TIMER(tv)                                                                       \
    std::chrono::steady_clock::time_point tv = std::chrono::steady_clock::now();

#define TIMER_END(tv, elapse)                                                           \
        double elapse = 0;                                                              \
        do {                                                                            \
            std::chrono::steady_clock::time_point _tv_end_                              \
                = std::chrono::steady_clock::now();                                     \
            std::chrono::microseconds __elapse__                                        \
                = std::chrono::duration_cast<std::chrono::microseconds>(_tv_end_-tv);   \
            elapse = __elapse__.count()/1000.0;                                         \
        } while(0);

