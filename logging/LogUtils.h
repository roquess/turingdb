#pragma once

#include <string>
#include <spdlog/fmt/bundled/core.h>

namespace logt {

void TuringHomeUndefined();

void FileNotFound(const std::string& path);
void NotADirectory(const std::string& path);
void DirectoryDoesNotExist(const std::string& path);

void CanNotRead(const std::string& path);
void CanNotWrite(const std::string& path);
void CanNotRemove(const std::string& path);
void CanNotCreateDir(const std::string& path);

void ExecutableNotFound(const std::string& cmd);
void ImpossibleToRunCommand(const std::string& cmd);

void ElapsedTime(float time, std::string_view unit);

template <typename... Args>
void warn(fmt::format_string<Args...>&& fmt_str, Args&&... args) {
    fmt::print(stderr,
               "[warning]: {}\n",
               fmt::format(std::forward<fmt::format_string<Args...>>(fmt_str),
                           std::forward<Args>(args)...));
}

template <typename... Args>
void error(fmt::format_string<Args...>&& fmt_str, Args&&... args) {
    fmt::print(stderr,
               "[error]: {}\n",
               fmt::format(std::forward<fmt::format_string<Args...>>(fmt_str),
                           std::forward<Args>(args)...));
}

template <typename... Args>
void info(fmt::format_string<Args...>&& fmt_str, Args&&... args) {
    fmt::print("[info]: {}\n",
               fmt::format(std::forward<fmt::format_string<Args...>>(fmt_str),
                           std::forward<Args>(args)...));
}

}
