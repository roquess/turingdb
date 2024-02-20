#pragma once

#include <optional>

template <class TError>
class BasicStatus {
public:
    BasicStatus() = default;

    BasicStatus(const TError& error)
        : _error(error)
    {
    }

    static BasicStatus<TError> ok() { return BasicStatus(); }

    operator bool() const { return !hasError(); }

    bool hasError() const { return _error.has_value(); }

    const TError& getError() const { return *_error; }

private:
    std::optional<TError> _error;
};
