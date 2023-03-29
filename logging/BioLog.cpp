#include "BioLog.h"

#include <iostream>
#include <cctype>
#include <sstream>

#include "Message.h"

#include "BioAssert.h"

using namespace Log;

BioLog* BioLog::_instance = nullptr;

BioLog::BioLog()
{
}

void BioLog::init() {
    if (_instance) {
        return;
    }

    _instance = new BioLog();
}

void BioLog::openFile(const std::string& file) {
    if (_instance) {
        _instance->open(file);
    }
}

bool BioLog::isOk() const {
    return (_errorCount >= 1) || (_fatalCount >= 1);
}

void BioLog::open(const std::string& logFile) {
    _outStream.open(logFile);
    if (!_outStream.is_open()) {
        std::cerr << "ERROR: failed to open the log file '" << logFile << "' for write.\n";
        exit(EXIT_FAILURE);
        return;
    }
}

void BioLog::close() {
    _outStream.close();
}

void BioLog::destroy() {
    if (_instance) {
        _instance->close();
        delete _instance;
    }
    _instance = nullptr;
}

BioLog* BioLog::getInstance() {
    bioassert(_instance);
    return _instance;
}

void BioLog::log(const Message& msg) {
    bioassert(_instance);
    _instance->emit(msg);
}

void BioLog::echo(const std::string& str, bool newline) {
    bioassert(_instance);
    _instance->print(str);
    if (newline) {
        _instance->print("\n");
    }

    _instance->flush();
}

void BioLog::print(const std::string& str) {
    _outStream << str;
    std::cout << str;
}

void BioLog::emit(const Message& msg) {
    const std::string& format = msg.getFormat();
    bioassert(!format.empty());
    bioassert(_instance);

    const MessageArgStorage* args = msg.getArgStorage();

    std::string errorStr;
    errorStr = "["; 
    errorStr += std::to_string(msg.getID());
    errorStr += "] ";

    switch (msg.getSeverity()) {
        case Message::INFO:
            _infoCount++;
            errorStr += "Info: ";
            break;

        case Message::WARNING:
            _warningCount++;
            errorStr += "Warning: ";
            break;

        case Message::ERROR:
            _errorCount++;
            errorStr += "Error: ";
            break;

        case Message::FATAL:
            _fatalCount++;
            errorStr += "Fatal: ";
            break;
    }

    const size_t formatSize = format.size();
    for (size_t i = 0; i < formatSize; i++) {
        if (format[i] == '$' && i < formatSize-1 && isdigit(format[i+1])) {
            const unsigned argPos = format[i+1]-'0';
            bioassert(argPos < args->_size);

            errorStr += args->_args[argPos];
            i++;
        } else {
            errorStr.push_back(format[i]);
        }
    }

    print(errorStr);
    print("\n");
    flush();

    if (_fatalCount >= 1) {
        printSummary();
        destroy();
        exit(EXIT_FAILURE);
    }
}

void BioLog::printSummary() {
    if (_instance) {
        _instance->_printSummary();
    }
}

void BioLog::_printSummary() {
    std::ostringstream out;
    out << "\nSummary:\n";
    out << "-------------------------------------------------------------------\n";
    out << "Terminated";
    
    if (_fatalCount == 0 && _errorCount == 0 && _warningCount == 0) {
        out << " successfully";
    }
    out << " with ";

    out << _fatalCount;
    out << " Fatal, ";
    out << _errorCount;
    out << ((_errorCount == 1) ? " Error, " : " Errors, ");
    out << _warningCount;
    out << ((_warningCount == 1) ? " Warning" : " Warnings");
    out << ".\n";
    out << "-------------------------------------------------------------------";
    print(out.str());
    print("\n");
    flush();
}

void BioLog::flush() {
    _outStream.flush();
}
