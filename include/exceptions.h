#pragma once
#include <exception>

class ConfigIsMissingError : public std::exception {
public:
    const char* what() const noexcept override;
};

class EmptyConfigError : public std::exception {
public:
    const char* what() const noexcept override;
};

class IncorrectVersionError : public std::exception {
public:
    const char* what() const noexcept override;
};