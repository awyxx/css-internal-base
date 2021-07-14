#pragma once

#include <utility>
#include <vector>

#include "utils.h"

// scanner[interface_name] = interface_ptr
class interface_scanner {
    struct s {
        const char* first;
        void* second;
    };
    std::vector<s> results;

public:
    interface_scanner(uintptr_t connect_interfaces_fn = 0);
    ~interface_scanner();

    void* operator[](const char* sz) {
        for (auto& r : results)
            if (strcmp(sz, r.first) == 0)
                return r.second;
        return nullptr;
    }
    void* operator[](int hash) {}
};