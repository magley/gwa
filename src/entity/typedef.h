#pragma once
#include <stdint.h>

using EntityID = uint32_t;
using EntityRefID = uint32_t;
using ComponentBit = uint32_t;
#define ENTITY_NULL -1

struct EntityManager;