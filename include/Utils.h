#ifndef UTILS_H
#define UTILS_H

#include <string>
#include <functional>

inline std::string hashPassword(const std::string& password) {
    return std::to_string(std::hash<std::string>{}(password));
}

#endif // UTILS_H