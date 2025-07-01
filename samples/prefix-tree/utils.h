#pragma once

#include <vector>
#include <string>
#include <string_view>

std::vector<std::string> split(std::string_view str, std::string_view delim);

std::vector<std::string> preprocess(const std::string_view in);

