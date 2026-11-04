#pragma once

#include <cstdint>

namespace Offsets {
  // Instance
  constexpr uintptr_t FakeDataModel = 0x6fff590;
  constexpr intptr_t RealDataModel = -0xe0;
  constexpr int Children = 0x70;
  constexpr int ClassDescriptor = 0x18;
  constexpr int ClassName = 0x8;
  constexpr int StringFlag = 0x17;
  constexpr int StringLength = 0x8;

  // Structs
  constexpr int LGlobal = 0x50; // L->global
  constexpr int RegistryFree = 0x640; // global->registryfree

  // Luau
  constexpr int StateFacet = 0x168;
  constexpr int StateSlot = 0x88;
  constexpr int StateStride = 0x308;
  constexpr int FacetState = 0x14f8;
}
