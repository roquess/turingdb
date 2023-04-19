// Copyright 2023 Turing Biosystems Ltd.

#pragma once

#include <stddef.h>

template <class STLType>
class STLRangeIterator {
public:
    using STLTypeIterator = typename STLType::const_iterator;

    STLRangeIterator(const STLType* container, bool beginOrEnd)
        : _container(container),
        _it(beginOrEnd ? container->begin() : container->end())
    {
    }

    bool isValid() const {
        return _it != _container->end();
    }

    typename STLType::value_type operator*() const { return *_it; }

    bool operator==(const STLRangeIterator& other) const {
        return this->_it == other._it;
    }

    bool operator!=(const STLRangeIterator& other) const {
        return !(*this == other);
    }

    STLRangeIterator& operator++() { ++_it; return *this; }

private:
    const STLType* _container {nullptr};
    STLTypeIterator _it;
};

template <class STLType>
class STLRange {
public:
    using Iterator = STLRangeIterator<STLType>;

    STLRange(const STLType* container)
        : _container(container)
    {}

    STLRange(const STLRange& other) = delete;
    STLRange(STLRange&& other) = delete;

    size_t size() const { return _container->size(); }

    bool empty() const { return _container->empty(); }

    Iterator begin() const {
        return STLRangeIterator(_container, true);
    }

    Iterator end() const {
        return STLRangeIterator(_container, false);
    }

private:
    const STLType* _container {nullptr};
};
