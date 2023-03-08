#ifndef _BIO_LOG_LOG_
#define _BIO_LOG_LOG_

#include <string>
#include <fstream>

namespace Log {

class Message;

class BioLog {
public:
    static void init(const std::string& logFile);

    static void log(const Message& msg);
    static void echo(const std::string& str);
    static void printSummary();

    static void destroy();

private:
    BioLog();
    void open(const std::string& logFile);
    void emit(const Message& msg);
    void print(const std::string& str);
    void _printSummary();
    void close();

    static BioLog* _instance;

    std::ofstream _outStream;
    size_t _errorCount {0};
    size_t _fatalCount {0};
    size_t _infoCount {0};
    size_t _warningCount {0};
};

}

#endif
