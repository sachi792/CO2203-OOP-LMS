#pragma once
#include <exception>
#include <string>

namespace domain {

// Base class for the exceptions used in the enrollment part.
class LMSException : public std::exception {
public:
    explicit LMSException(std::string message);
    ~LMSException() override = 0;       // makes the class abstract

    LMSException(const LMSException&) = default;
    LMSException& operator=(const LMSException&) = default;
    LMSException(LMSException&&) noexcept = default;
    LMSException& operator=(LMSException&&) noexcept = default;

    const char* what() const noexcept override;

protected:
    std::string message_;
};

} 