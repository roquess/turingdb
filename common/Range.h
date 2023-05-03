// Copyright 2023 Turing Biosystems Ltd.

#pragma once

#include <stddef.h>
#include <iterator>

template <class ElementType>
class IteratorBase {
public:
    using iterator_category = std::input_iterator_tag;
    using value_type = ElementType;
    using difference_type = std::ptrdiff_t;
    using pointer = ElementType*;
    using reference = ElementType&;

protected:
    IteratorBase() = default;
    ~IteratorBase() = default;
};

template <class STLType>
class STLRangeIterator : public IteratorBase<typename STLType::value_type> {
public:
    using STLTypeIterator = typename STLType::const_iterator;

    STLRangeIterator() = delete;

    STLRangeIterator(const STLType* container, bool beginOrEnd)
        : _container(container)
    {
        if (_container) {
            _it = beginOrEnd ? _container->begin() : _container->end();
        }
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
    using ValueType = typename STLType::value_type;

    STLRange() = delete;

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

template <class STLIndexType>
class STLIndexIterator : public IteratorBase<typename STLIndexType::mapped_type> {
public:
    STLIndexIterator() = delete;
    STLIndexIterator(const STLIndexType* container, bool beginOrEnd)
        : _container(container)
    {
        if (_container) {
            _it = beginOrEnd ? _container->begin() : _container->end();
        }
    }

    bool isValid() const {
        return _it != _container->end();
    }

    typename STLIndexType::mapped_type operator*() const { return (*_it).second; }

    bool operator==(const STLIndexIterator& other) const {
        return this->_it == other._it;
    }

    bool operator!=(const STLIndexIterator& other) const {
        return !(*this == other);
    }

    STLIndexIterator& operator++() { ++_it; return *this; }

private:
    const STLIndexType* _container {nullptr}; 
    typename STLIndexType::const_iterator _it;
};

template <class STLIndexType>
class STLIndexRange {
public:
    using Iterator = STLIndexIterator<STLIndexType>;

    STLIndexRange() = delete;
    STLIndexRange(const STLIndexType* container)
        : _container(container)
    {}

    size_t size() const { return _container->size(); }

    bool empty() const { return _container->empty(); }

    Iterator begin() const {
        return STLIndexIterator(_container, true);
    }

    Iterator end() const {
        return STLIndexIterator(_container, false);
    }

private:
    const STLIndexType* _container {nullptr};
};
