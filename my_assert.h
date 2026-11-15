#pragma once
#include <cassert>

static void my_assert(bool exp) {
    if (!exp) {
        assert(false);
    }
}
