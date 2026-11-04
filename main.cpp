#include <chrono>
#include <cstdint>
#include <iostream>
#include <thread>

#include "dm.hpp"

uintptr_t DecryptState(uintptr_t encryptedState) {
  uint32_t key = (uint32_t)encryptedState;
  uint32_t low = memory::read<uint32_t>(encryptedState) ^ key;
  uint32_t high = memory::read<uint32_t>(encryptedState + 0x4) ^ key;
  return ((uintptr_t)high << 32) | low;
}

int main() {
  std::cout << std::unitbuf;

  if (!memory::attach()) {
    std::cout << "attach failed\n";
    return 0;
  }

  uintptr_t dataModel = memory::read<uintptr_t>(memory::base() + Offsets::FakeDataModel);
  if (memory::valid(dataModel))
    dataModel = memory::read<uintptr_t>(dataModel + Offsets::RealDataModel);
  if (!memory::valid(dataModel) || instance::className(dataModel) != "DataModel") {
    std::cout << "DataModel not found\n";
    return 0;
  }

  uintptr_t scriptContext = 0;
  for (auto child : instance::children(dataModel)) {
    if (instance::className(child) == "ScriptContext") {
      scriptContext = child;
      break;
    }
  }
  if (!scriptContext) {
    std::cout << "ScriptContext not found\n";
    return 0;
  }

  auto bit = [](uintptr_t ptr, int b) -> int {
    if (!memory::valid(ptr))
      return 1;
    uintptr_t global = memory::read<uintptr_t>(ptr + Offsets::LGlobal);
    if (!memory::valid(global))
      return 1;
    return (memory::read<uint32_t>(global + Offsets::RegistryFree) >> b) & 1;
  };

  uintptr_t encryptedL1 = scriptContext + Offsets::StateFacet + Offsets::StateSlot;
  uintptr_t encryptedL2 = encryptedL1 + Offsets::StateStride;

  while (true) {
    if (memory::read<uint32_t>(scriptContext + Offsets::FacetState) >= 3)
      break;

    uintptr_t L1 = DecryptState(encryptedL1);
    uintptr_t L2 = DecryptState(encryptedL2);

    if (!memory::valid(L1) && !memory::valid(L2)) {
      std::cout << "no Luau states\n";
    } else {
      auto result =
        (bit(L1, 28) & bit(L2, 28)) |
        ((bit(L1, 29) & bit(L2, 29)) << 1) |
        ((bit(L1, 30) & bit(L2, 30)) << 2);

      if (result == 0x7)
        std::cout << "Luau Violations: 0x" << std::hex << result << " (clean)\n";
      else
        std::cout << "Luau Violations: 0x" << std::hex << result << " (detected)\n";
    }

    std::this_thread::sleep_for(std::chrono::seconds(1));
  }

  return 0;
}
