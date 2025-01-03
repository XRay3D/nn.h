#pragma once

#include <chrono>
#include <print>
#include <source_location>

#include <map>

namespace Chrono = std::chrono;
struct ElapsedTimer {
    using Sl = std::source_location;

    using Clock = Chrono::high_resolution_clock;
    using Now = decltype(Clock::now());
    using Elapsed = decltype(Clock::now() - Now{});

    using Key = std::pair<const char*, unsigned>;
    using Avg = std::pair<Elapsed, double>;
    static inline std::map<Key, Avg> map;

    Now start = Clock::now();
    Sl sl = Sl::current();

    // static inline Elapsed average;
    // static inline double count;
    static inline size_t counter;

public:
    static void reset() {
        // average = {}, count = {};
        map.clear();
    }
    ~ElapsedTimer() {
        const auto elapsed = Clock::now() - start;
        auto&& [average, count] = map[{sl.file_name(), sl.line()}];
        average += elapsed, ++count;
        // if(!(static_cast<size_t>(count) % 1000)) {
        if(!(++counter % 100)) {
            std::string_view file_name{sl.file_name()};
            file_name = file_name.substr(file_name.find_last_of('/') + 1);
            std::println("Elapsed {} (avg {}) file:{} func:{} line:{}",
                Chrono::duration_cast<Chrono::microseconds>(elapsed),
                Chrono::duration_cast<Chrono::microseconds>(average) / count,
                file_name,
                sl.function_name(),
                sl.line());
        }
    }
};
