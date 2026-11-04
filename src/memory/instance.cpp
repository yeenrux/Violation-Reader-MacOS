#include "dm.hpp"

namespace instance {

std::string className(uptr inst) {
    auto d = memory::read<uptr>(inst + Offsets::ClassDescriptor);
    if (!memory::valid(d))
        return {};

    auto ptr = memory::read<uptr>(d + Offsets::ClassName);
    if (!memory::valid(ptr))
        return {};

    return memory::readString(ptr);
}

std::vector<uptr> children(uptr inst) {
    std::vector<uptr> out;

    auto list = memory::read<uptr>(inst + Offsets::Children);
    if (!memory::valid(list))
        return out;

    auto start = memory::read<uptr>(list);
    auto end = memory::read<uptr>(list + 0x8);

    if (!memory::valid(start) || !memory::valid(end) || end <= start ||
        end - start > 0x200000) {
        return out;
    }

    for (uptr at = start; at < end; at += 0x10) {
        uptr child = memory::read<uptr>(at);
        if (memory::valid(child))
            out.push_back(child);
    }

    return out;
}

}
