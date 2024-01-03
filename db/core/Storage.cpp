#include "Storage.h"

using namespace db;

Storage::Storage()
{
}

Storage::~Storage() {
}

StorageAccessor Storage::access() {
    return StorageAccessor(StorageAccessor::SharedAccess(), this);
}

StorageAccessor Storage::uniqueAccess() {
    return StorageAccessor(StorageAccessor::UniqueAccess(), this);
}