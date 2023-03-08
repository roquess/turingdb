// Copyright 2023 Turing Biosystems Ltd.

#ifndef _COMMON_RANGE_
#define _COMMON_RANGE_

template <typename IteratorType>
class Range {
public:
    Range() = default;

    Range(IteratorType begin, IteratorType end)
        : _beginIt(begin), _endIt(end)
    {
    }

    bool empty() const { return _beginIt == _endIt; }

    IteratorType begin() const { return _beginIt; }
    IteratorType end() const { return _endIt; }

private:
    IteratorType _beginIt;
    IteratorType _endIt;
};

template <typename ContainerType>
auto MakeRange(const ContainerType& container) {
    return Range(container.cbegin(), container.cend());
}

#endif
