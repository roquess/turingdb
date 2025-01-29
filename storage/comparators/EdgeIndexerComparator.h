#pragma once

#include <range/v3/view/zip.hpp>

#include "indexers/EdgeIndexer.h"
#include "EdgeRecord.h"

namespace db {

class EdgeIndexerComparator {
public:
    [[nodiscard]] static bool same(const EdgeIndexer& a, const EdgeIndexer& b) {
        namespace rv = ranges::views;

        if (a.getFirstEdgeID() != b.getFirstEdgeID()) {
            return false;
        }

        if (a.getFirstNodeID() != b.getFirstNodeID()) {
            return false;
        }

        const auto& outIndexerA = a.getOutsByLabelSet();
        const auto& outIndexerB = b.getOutsByLabelSet();

        if (outIndexerA.size() != outIndexerB.size()) {
            return false;
        }

        for (const auto [itA, itB] : rv::zip(outIndexerA, outIndexerB)) {
            const auto& [labelsetA, rangesA] = itA;
            const auto& [labelsetB, rangesB] = itB;

            if (labelsetA != labelsetB) {
                return false;
            }

            if (rangesA.size() != rangesB.size()) {
                return false;
            }

            for (const auto& [rA, rB] : rv::zip(rangesA, rangesB)) {
                if (rA.size() != rB.size()) {
                    return false;
                }

                if (rA.size() == 0) {
                    continue;
                }

                if (rA.front()._edgeID != rB.front()._edgeID) {
                    return false;
                }
            }
        }

        const auto& inIndexerA = a.getInsByLabelSet();
        const auto& inIndexerB = b.getInsByLabelSet();

        if (inIndexerA.size() != inIndexerB.size()) {
            return false;
        }

        for (const auto [itA, itB] : rv::zip(inIndexerA, inIndexerB)) {
            const auto& [labelsetA, rangesA] = itA;
            const auto& [labelsetB, rangesB] = itB;

            if (labelsetA != labelsetB) {
                return false;
            }

            if (rangesA.size() != rangesB.size()) {
                return false;
            }

            for (const auto& [rA, rB] : rv::zip(rangesA, rangesB)) {
                if (rA.size() != rB.size()) {
                    return false;
                }

                if (rA.size() == 0) {
                    continue;
                }

                if (rA.front()._edgeID != rB.front()._edgeID) {
                    return false;
                }
            }
        }

        return true;
    }
};

}
