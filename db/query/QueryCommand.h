#pragma once

namespace db::query {

class ASTContext;

class QueryCommand {
public:
    friend ASTContext;

    enum Kind {
        QCOM_LIST_COMMAND
    };

    virtual Kind getKind() const = 0;

protected:
    QueryCommand() = default;
    virtual ~QueryCommand() = default;
    void registerCmd(ASTContext* ctxt);
};

class ListCommand : public QueryCommand {
public:
    enum SubType {
        LCOM_UNKNOWN,
        LCOM_DATABASES
    };

    Kind getKind() const override { return QCOM_LIST_COMMAND; }

    SubType getSubType() const { return _subType; }
    void setSubType(SubType subType) { _subType = subType; }

    static ListCommand* create(ASTContext* ctxt);

private:
    SubType _subType {LCOM_UNKNOWN};

    ListCommand();
    ~ListCommand();
};

}
