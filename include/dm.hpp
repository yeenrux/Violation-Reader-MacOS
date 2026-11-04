#pragma once

#include <string>
#include <vector>
#include <cstdint>
#include <mach/mach.h>
#include "offsets.hpp"

namespace memory {

using uptr = uintptr_t;

bool valid(uptr addr);
bool read(uptr addr, void* out, size_t size);
bool write(uptr addr, const void* in, size_t size);
std::string readString(uptr addr);
void setTask(task_t t);
task_t machTask();

bool attach();
uptr base();

template <typename T>
T read(uptr addr) {
    T v{};
    memory::read(addr, &v, sizeof(T));
    return v;
}

template <typename T>
bool write(uptr addr, T v) {
    return memory::write(addr, &v, sizeof(T));
}

}

namespace instance {

using uptr = uintptr_t;

std::string className(uptr inst);
std::vector<uptr> children(uptr inst);

}
