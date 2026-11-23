#pragma once
#include <functional>
#include <string>
#include <Uuid.h>

using IdConsumer = std::function<void(Uuid)>;
