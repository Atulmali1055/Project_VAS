#pragma once
#include <cstddef>
#include <cstdint>

bool detectWakeWord(const int16_t* audioData, size_t len);
