#pragma once

#include <vector>

namespace db {

class InputPort;
class OutputPort;

class Processor {
public:
    using InputPorts = std::vector<InputPort*>;
    using OutputPorts = std::vector<OutputPort*>;

    enum class Status {
        Finished,
        Ready
    };

    Processor();
    virtual ~Processor();

    const InputPorts& inputs() const { return _inputPorts; }
    const OutputPorts& outputs() const { return _outputPorts; }

    virtual Status prepare() = 0;
    virtual void work() = 0;

private:
    InputPorts _inputPorts;
    OutputPorts _outputPorts;
};

}
