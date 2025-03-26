#include "CommitBuilder.h"

#include "reader/GraphReader.h"
#include "Graph.h"
#include "versioning/Commit.h"
#include "versioning/VersionController.h"
#include "versioning/CommitView.h"
#include "writers/DataPartBuilder.h"

using namespace db;

CommitBuilder::CommitBuilder() = default;

CommitBuilder::~CommitBuilder() = default;

std::unique_ptr<CommitBuilder> CommitBuilder::prepare(Graph& graph, const GraphView& view) {
    const auto reader = view.read();

    auto* ptr = new CommitBuilder {graph};

    ptr->_firstNodeID = reader.getNodeCount();
    ptr->_firstEdgeID = reader.getNodeCount();

    ptr->_commit = ptr->prepareCommit(view);

    return std::unique_ptr<CommitBuilder> {ptr};
}

GraphView CommitBuilder::viewGraph() const {
    return GraphView {*_graph, *_commit->_data};
}

GraphReader CommitBuilder::readGraph() const {
    return viewGraph().read();
}

DataPartBuilder& CommitBuilder::newBuilder() {
    std::scoped_lock lock {_mutex};
    GraphView view {*_graph, *_commit->_data};
    const size_t partIndex = view.dataparts().size() + _builders.size();
    auto& builder = _builders.emplace_back(DataPartBuilder::prepare(*_graph, view, partIndex));

    return *builder;
}

void CommitBuilder::buildAllPending(JobSystem& jobsystem) {
    std::scoped_lock lock {_mutex};
    size_t nodeCount = 0;
    size_t edgeCount = 0;

    for (const auto& builder : _builders) {
        nodeCount += builder->nodeCount();
        edgeCount += builder->edgeCount();
    }

    auto [firstNodeID, firstEdgeID] = _graph->allocIDRange(nodeCount, edgeCount);
    GraphView view {*_graph, *_commit->_data};

    for (const auto& builder : _builders) {
        auto part = _versionController->createDataPart(firstNodeID, firstEdgeID);

        firstNodeID += builder->nodeCount();
        firstEdgeID += builder->edgeCount();

        // TODO Use the jobsystem here
        part->load(view, jobsystem, *builder);
        _commit->_data->_history._allDataparts.emplace_back(part);
        _commit->_data->_history._commitDataparts.emplace_back(part);
    }

    _builders.clear();
}

std::unique_ptr<Commit> CommitBuilder::build(JobSystem& jobsystem) {
    buildAllPending(jobsystem);

    return std::move(_commit);
}

CommitBuilder::CommitBuilder(Graph& graph)
    : _graph(&graph),
      _versionController(graph._versionController.get())
{
}

std::unique_ptr<Commit> CommitBuilder::prepareCommit(const GraphView& view) {
    auto reader = view.read();

    auto commit = std::make_unique<Commit>();
    commit->_graph = _graph;
    commit->_data = _versionController->createCommitData(commit->hash());
    commit->_data->_hash = commit->hash();
    commit->_data->_graphMetadata = _graph->getMetadata();

    auto& history = commit->history();

    const DataPartSpan previousDataparts = reader.dataparts();
    const std::span<const CommitView> previousCommits = reader.commits();

    history.pushPreviousDataparts(previousDataparts);
    history.pushPreviousCommits(previousCommits);
    history.pushCommit(CommitView {commit.get()});

    return commit;
}
