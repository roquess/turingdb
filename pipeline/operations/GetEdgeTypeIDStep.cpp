#include "GetEdgeTypeIDStep.h"

#include <sstream>

#include "views/GraphView.h"
#include "reader/GraphReader.h"

using namespace db;

GetEdgeTypeIDStep::GetEdgeTypeIDStep(const ColumnIDs* edgeIDs, 
                  ColumnVector<EdgeTypeID>* edgeTypeIDs)
    : _edgeIDs(edgeIDs),
    _edgeTypeIDs(edgeTypeIDs)
{
}

GetEdgeTypeIDStep::~GetEdgeTypeIDStep(){
}

void GetEdgeTypeIDStep::execute() {
    const auto& edgeIDs = *_edgeIDs;
    ColumnVector<EdgeTypeID>& edgeTypeIDs = *_edgeTypeIDs;

    const auto reader = _view->read();

    edgeTypeIDs.clear();
    edgeTypeIDs.reserve(edgeIDs.size());
    for (EntityID edgeID : edgeIDs) {
        edgeTypeIDs.emplace_back(reader.getEdgeTypeID(edgeID));
    }

}

void GetEdgeTypeIDStep::describe(std::string& descr) const {
    std::stringstream ss;
    ss << "GetEdgeTypeIDStep";
    ss << " edgeIDs=" << std::hex << _edgeIDs;
    ss << " edgeTypeIDs=" << std::hex << _edgeTypeIDs;
    descr = ss.str();
}
