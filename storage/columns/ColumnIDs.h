#pragma once

#include "ColumnVector.h"
#include "ColumnConst.h"
#include "EntityID.h"

namespace db {

using ColumnIDs = ColumnVector<EntityID>;
using ColumnID = ColumnConst<EntityID>;

}
