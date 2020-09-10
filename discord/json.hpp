#pragma once
#include <nlohmann/json.hpp>

#define JS_D(k, t)         \
    do {                   \
        j.at(k).get_to(t); \
    } while (0)

#define JS_O(k, t)                            \
    do {                                      \
        if (j.contains(k)) j.at(k).get_to(t); \
    } while (0)

#define JS_N(k, t)                                 \
    do {                                           \
        if (!j.at(k).is_null()) j.at(k).get_to(t); \
    } while (0)

#define JS_ON(k, t)                              \
    do {                                         \
        if (j.contains(k) && !j.at(k).is_null()) \
            j.at(k).get_to(t);                   \
    } while (0)
