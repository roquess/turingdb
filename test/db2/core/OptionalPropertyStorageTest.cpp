#include "OptionalPropertyStorage.h"

#include <gtest/gtest.h>

using namespace db;
using namespace std;
using namespace chrono;

TEST(OptionalPropertyStorageTest, Create) {

    //srand(0);
    //constexpr size_t count = 100000;

    //std::vector<bool> checks(count);
    //for (size_t i = 0; i < count; i++) {
    //    checks[i] = rand() % 2;
    //}

    //BoolContainer<uint64_t> bools;
    //OptionalContainer<uint64_t> opts;

    //constexpr auto measureInit = [](auto& container,
    //                                const std::vector<bool>& checks,
    //                                std::string_view msg) {
    //    auto t0 = std::chrono::high_resolution_clock::now();
    //    container.init(count, checks);
    //    auto t1 = std::chrono::high_resolution_clock::now();
    //    auto dur = duration_cast<microseconds>(t1 - t0).count();
    //    std::cout << msg << dur / 1000.0f << " ms" << std::endl;
    //};

    //constexpr auto measureIsValid = [](const auto& container,
    //                                   std::string_view msg) {
    //    std::vector<bool> valid(count);
    //    auto t0 = std::chrono::high_resolution_clock::now();
    //    for (size_t i = 0; i < count; i++) {
    //        valid[i] = container.isValid(i);
    //    }
    //    auto t1 = std::chrono::high_resolution_clock::now();
    //    auto dur = duration_cast<microseconds>(t1 - t0).count();
    //    std::cout << msg << dur / 1000.0f << " ms "
    //              << std::count_if(valid.begin(), valid.end(), [](bool v) {
    //                     return v;
    //                 })
    //              << std::endl;
    //};

    //measureInit(bools, checks, "BoolContainer init:");
    //measureInit(opts, checks, "OptionalContainer init:");
    //measureIsValid(bools, "BoolContainer isValid:");
    //measureIsValid(opts, "OptionalContainer isValid:");
}
