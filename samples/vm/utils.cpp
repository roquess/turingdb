#include "utils.h"

std::vector<db::Instruction> getTestProgram() {
    return {
        db::Instruction::getDBCount(0),     // Stores db count in reg0
        db::Instruction::createDatabase(1), // Create database with name "DB #n" with n == value stored in reg1
        db::Instruction::getDBCount(2),     // Stores db count in reg2
        db::Instruction::listDatabases(3),  // populates a std::vector<std::string_view> object with db names
        db::Instruction::exit(),  // populates a std::vector<std::string_view> object with db names
    };
}
