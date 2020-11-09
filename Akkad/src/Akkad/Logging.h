#pragma once
#include <spdlog/spdlog.h>

#define AK_INFO(...) :: spdlog::info(__VA_ARGS__)
#define AK_WARNING(...) :: spdlog::warn(__VA_ARGS__)
#define AK_ERROR(...) :: spdlog::error(__VA_ARGS__)