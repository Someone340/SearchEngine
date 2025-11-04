#include "exceptions.h"

const char* ConfigIsMissingError::what() const noexcept{
    return "Config file is missing";
}

const char* EmptyConfigError::what() const noexcept{
    return "Config file is empty";
}

const char* IncorrectVersionError::what() const noexcept{
    return "Config.json has incorrect file version";
}