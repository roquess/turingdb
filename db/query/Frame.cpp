#include "Frame.h"

using namespace db::query;

Frame::Frame(size_t size)
    : _tbl(size)
{
}

Frame::~Frame() {
}
