#pragma once
#include <fmt/format.h>
#include <iomanip>
#include <sstream>

#include <vector>
#include <fstream>
#include <iostream>
#include <mpg123.h>
#include <out123.h>

//filename: Shakuhachi.mp3 or ablosute_path/Shakuhachi.mp3
std::vector<uint8_t> mp3_to_pcm(const char* file);