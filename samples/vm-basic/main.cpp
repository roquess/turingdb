#include "vmutils.h"

using namespace db;

int main() {
    auto sample = VMSample::createSample(SAMPLE_NAME);

    if (!sample.generateFromFile(sample._sampleDir + "/program.turing")) {
        return 1;
    }

    sample.execute();
    spdlog::info("Sum: {}", sample._vm->readRegister(0));
    sample.destroy();

    return 0;
}
