#pragma once

namespace db {

class Processor;
class InputPort;
class OutputPort;

class Port {
public:
    virtual ~Port();

    Processor* getProcessor() const { return _processor; }

protected:
    Port();

private:
    Processor* _processor {nullptr};
};

class InputPort : public Port {
public:
    OutputPort* getOutputPort() const { return _outPort; }

private:
    OutputPort* _outPort {nullptr};
};

class OutputPort : public Port {
public:
    InputPort* getInputPort() const { return _inPort; }

private:
    InputPort* _inPort {nullptr};
};

}
