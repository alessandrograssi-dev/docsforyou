#pragma once
#include <string_view>

inline bool starts_with(std::string_view string, std::string_view content)
{
  return string.find(content) == 0;
}