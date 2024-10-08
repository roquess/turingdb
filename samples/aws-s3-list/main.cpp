#include <cstdlib>
#include <string>
#include <vector>

#include <spdlog/spdlog.h>

#include "TuringS3Client.h"
#include "TuringS3ClientConfig.h"

using namespace db;

int main(int argc, const char** argv) {
    const std::string remotePath = "db";
    const std::string bucket = "turing";
    const std::string endpointUrl = "s3.fr-par.scw.cloud";

    TuringS3ClientConfig turingConfig {endpointUrl};
    TuringS3Client turingClient {turingConfig};
    std::vector<std::string> files;

    if (!turingClient.listFiles(bucket, remotePath, files)) {
        spdlog::info("[{}] The operation failed.", SAMPLE_NAME);
        return EXIT_FAILURE;
    }

    spdlog::info("[{}] {} objects found in the path '{}'.", SAMPLE_NAME, files.size(), remotePath);
    spdlog::info("[{}] {} ... {}.", SAMPLE_NAME, files.at(0), files.at(files.size() - 1));
    return EXIT_SUCCESS;
}
