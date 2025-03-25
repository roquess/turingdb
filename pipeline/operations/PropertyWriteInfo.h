#pragma once

#include "columns/ColumnVector.h"

namespace db {

template <SupportedType T>
struct PropertyWriteInfo {
    ColumnVector<size_t>* _indices {nullptr};
    ColumnVector<typename T::primitive>* _values {nullptr};
};

}
