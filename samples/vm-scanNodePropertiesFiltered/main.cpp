#include "LogUtils.h"
#include "vmutils.h"

#include "Time.h"

using namespace db;

int main() {
    auto sample = VMSample::createSample(SAMPLE_NAME);

    if (!sample.loadJsonDB(sample._turingHome + "/neo4j/pole-db/")) {
        return 1;
    }

    if (!sample.executeFile(sample._sampleDir + "/program.turing")) {
        return 1;
    }
    if (!sample.executeFile(sample._sampleDir + "/program.turing")) {
        return 1;
    }

    sample.printOutput({"Name", "NodeID"}, 0, 20, 25);

    auto access = sample.readDB();
    auto t0 = Clock::now();
    std::vector<std::string_view> names;
    const auto displayNameType = access.getDB()->getMetadata()->propTypes().get("surname (String)");
    for (const auto& displayName : access.scanNodeProperties<types::String>(displayNameType._id)) {
        if (displayName == "Hamilton" || displayName == "Smith") {
            names.push_back(displayName);
        }
    }
    spdlog::info(names.size());
    logt::ElapsedTime(Milliseconds(Clock::now() - t0).count(), "ms");

    sample.destroy();

    return 0;
}
