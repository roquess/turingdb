#ifndef _ITERATOR_RANGE_
#define _ITERATOR_RANGE_

template <class IteratorType>
class IteratorRange {
public:
    IteratorRange() = default;

    IteratorRange(const IteratorType& begin, const IteratorType& end)
        : _begin(begin), _end(end)
    {
    }

    IteratorType begin() const { return _begin; }
    IteratorType end() const { return _end; }

private:
    IteratorType _begin;
    IteratorType _end;
};

#endif
