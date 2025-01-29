#pragma once

#include <range/v3/view/zip.hpp>

#include "properties/PropertyManager.h"
#include "PropertyContainerComparator.h"

namespace db {

class PropertyManagerComparator {
public:
    [[nodiscard]] static bool same(const PropertyManager& a, const PropertyManager& b) {
        namespace rv = ranges::views;

        for (const auto& [itA, itB] : rv::zip(a, b)) {
            const auto& [ptA, containerA] = itA;
            const auto& [ptB, containerB] = itB;

            if (ptA != ptB) {
                return false;
            }

            if (!PropertyContainerComparator::same(containerA.get(), containerB.get())) {
                return false;
            }
        }

        return true;
    }
};

}
