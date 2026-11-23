#pragma once

#include <Uuid.h>

// Re-export Uuid::generate for backward compatibility and convenience
static inline Uuid generateUuid() {
    return Uuid::generate();
}