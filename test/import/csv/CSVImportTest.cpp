#include "CSVImport.h"
#include "BioLog.h"
#include "DB.h"
#include "StringBuffer.h"
#include "Writeback.h"
#include "gtest/gtest.h"

namespace db {
using namespace Log;

class CSVImportTest : public ::testing::Test {
protected:
    void SetUp() override {
        BioLog::init();

        _db = DB::create();

        Writeback wb(_db);
        Network* net = wb.createNetwork(_db->getString("example_net"));
        ASSERT_TRUE(net);

        const auto csvPath = "example.csv";

        StringBuffer* strBuffer = StringBuffer::readFromFile(csvPath);
        CSVImport csvImport(CSVImport::InitArgs{
            .buffer = strBuffer,
            .db = _db,
            .outNet = net,
            .delimiter = ',',
            .primaryColumn = "Patient",
        });
        const bool success = csvImport.run();
        ASSERT_TRUE(success);

        delete strBuffer;
    }

    void TearDown() override {
        delete _db;
    }

    DB* _db = nullptr;
};

//TEST_F(CSVImportTest, CSVImportEmptyLinesEnd) {
//    // no empty lines
//    // 1 empty lines
//    // 3 empty lines
//}

//TEST_F(CSVImportTest, CSVImportOneColumn) {
//    // Should succeeded
//    // Should not create any edges
//}
//
//TEST_F(CSVImportTest, CSVStringsExample) {
//    // Import a regular file with only string
//}
//
//TEST_F(CSVImportTest, CSVRealExample) {
//}
//
//TEST_F(CSVImportTest, CSVWithFloats) {
//}
//
//TEST_F(CSVImportTest, CSVDuplicates) {
//    // test handling of duplicates
//}
//
//TEST_F(CSVImportTest, CSVDuplicateNodeType) {
//    // test duplicate header cells -> should crash gracefully
//}
//
//TEST_F(CSVImportTest, CSVImportErrors) {
//    // For each test: should fail + compare output
//    //
//    // one line has a missing entry (3 cells instead of 4 par example)
//    // one line has an extra entry (5 cells instead of 4 for example)
//    // File does not exist
//}
//
//TEST_F(CSVImportTest, CSVInvalidPrimaryColumn) {
//    // Test invalid primary column
//}
//
//TEST_F(CSVImportTest, CSVDelimiters) {
//    // Test ',' and ';' delimiters
//}

}
