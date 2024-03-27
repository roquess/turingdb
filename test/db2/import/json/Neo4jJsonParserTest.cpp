#include <iomanip>
#include <iostream>
#include <nlohmann/json.hpp>
#include <sstream>

#include "FileUtils.h"
#include <gtest/gtest.h>

using namespace std;
using namespace chrono;

using json = nlohmann::json;

// a simple event consumer that collects string representations of the passed
// values; note inheriting from json::json_sax_t is not required, but can
// help not to forget a required function
class sax_event_consumer : public json::json_sax_t {
public:
    std::vector<std::string> events;

    bool null() override {
        events.emplace_back("null()");
        return true;
    }

    bool boolean(bool val) override {
        events.emplace_back("boolean(val=" + std::string(val ? "true" : "false") + ")");
        return true;
    }

    bool number_integer(number_integer_t val) override {
        events.emplace_back("number_integer(val=" + std::to_string(val) + ")");
        return true;
    }

    bool number_unsigned(number_unsigned_t val) override {
        events.emplace_back("number_unsigned(val=" + std::to_string(val) + ")");
        return true;
    }

    bool number_float(number_float_t val, const string_t& s) override {
        events.emplace_back("number_float(val=" + std::to_string(val) + ", s=" + s + ")");
        return true;
    }

    bool string(string_t& val) override {
        events.emplace_back("string(val=" + val + ")");
        return true;
    }

    bool start_object(std::size_t elements) override {
        events.emplace_back("start_object(elements=" + std::to_string(elements) + ")");
        return true;
    }

    bool end_object() override {
        events.emplace_back("end_object()");
        return true;
    }

    bool start_array(std::size_t elements) override {
        events.emplace_back("start_array(elements=" + std::to_string(elements) + ")");
        return true;
    }

    bool end_array() override {
        events.emplace_back("end_array()");
        return true;
    }

    bool key(string_t& val) override {
        events.push_back("key(val=" + val + ")");
        return true;
    }

    bool binary(json::binary_t& val) override {
        events.emplace_back("binary(val=[...])");
        return true;
    }

    bool parse_error(std::size_t position, const std::string& last_token, const json::exception& ex) override {
        events.emplace_back("parse_error(position=" + std::to_string(position) + ", last_token=" + last_token + ",\n            ex=" + std::string(ex.what()) + ")");
        return false;
    }
};

int main() {
    // a JSON text
    auto t0 = std::chrono::high_resolution_clock::now();

    const std::string path = "/net/db/reactome/json/nodes_1.json";
    std::string data;
    FileUtils::readContent(path, data);

    auto t1 = std::chrono::high_resolution_clock::now();

    // create a SAX event consumer object
    sax_event_consumer sec;

    // parse JSON
    bool result = json::sax_parse(data, &sec);
    auto t2 = std::chrono::high_resolution_clock::now();

    float readDur = duration_cast<nanoseconds>(t1 - t0).count();
    float parseDur = duration_cast<nanoseconds>(t2 - t1).count();
    std::cout << "Read node properties: "
              << readDur / 1000.0f << " us" << std::endl;
    std::cout << "Parse node properties: "
              << parseDur / 1000.0f << " us" << std::endl;

    // output the result of sax_parse
    std::cout << "\nresult: " << std::boolalpha << result << std::endl;
}

// #include "Neo4jJsonParser.h"
// #include "BioLog.h"
// #include "DB.h"
//
// #include <gtest/gtest.h>
//
// using namespace db;
// using namespace std;
// using namespace chrono;
// using namespace Log;
//
// TEST(Neo4jJsonParserTest, Load) {
//     const testing::TestInfo* const testInfo =
//         testing::UnitTest::GetInstance()->current_test_info();
//     std::string outDirName = testInfo->test_suite_name();
//     outDirName += "_";
//     outDirName += testInfo->name();
//     outDirName += ".out";
//     const auto outDir = FileUtils::Path(outDirName);
//     const auto logPath = FileUtils::Path(outDir) / "log";
//
//     // Remove the directory from the previous run
//     if (FileUtils::exists(outDirName)) {
//         FileUtils::removeDirectory(outDirName);
//     }
//     FileUtils::createDirectory(outDirName);
//
//     BioLog::init();
//     BioLog::openFile(logPath.string());
//
//     DB* db = new DB;
//
//     {
//         auto t0 = std::chrono::high_resolution_clock::now();
//
//         const std::string path = "/net/db/reactome/json/nodeProperties.json";
//         std::string data;
//         FileUtils::readContent(path, data);
//
//         auto t1 = std::chrono::high_resolution_clock::now();
//         Neo4jJsonParser parser(db);
//         parser.parseNodeProperties(data);
//         auto t2 = std::chrono::high_resolution_clock::now();
//
//         float readDur = duration_cast<nanoseconds>(t1 - t0).count();
//         float parseDur = duration_cast<nanoseconds>(t2 - t1).count();
//         std::cout << "Read node properties: "
//                   << readDur / 1000.0f << " us" << std::endl;
//         std::cout << "Parse node properties: "
//                   << parseDur / 1000.0f << " us" << std::endl;
//     }
//
//     {
//         auto t0 = std::chrono::high_resolution_clock::now();
//
//         const std::string path = "/net/db/reactome/json/nodes_1.json";
//         std::string data;
//         FileUtils::readContent(path, data);
//
//         auto t1 = std::chrono::high_resolution_clock::now();
//         Neo4jJsonParser parser(db);
//         parser.parseNodes(data);
//         auto t2 = std::chrono::high_resolution_clock::now();
//
//         float readDur = duration_cast<milliseconds>(t1 - t0).count();
//         float parseDur = duration_cast<milliseconds>(t2 - t1).count();
//         std::cout << "Read nodes: "
//                   << readDur / 1000.0f << " s" << std::endl;
//         std::cout << "Parse nodes: "
//                   << parseDur / 1000.0f << " s" << std::endl;
//     }
//
//     delete db;
//     Log::BioLog::destroy();
// }
