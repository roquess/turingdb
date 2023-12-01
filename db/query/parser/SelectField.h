#pragma once

namespace db {

class ASTContext;

class SelectField {
public:
    friend ASTContext;

    static SelectField* create(ASTContext* ctxt);

    void setAll(bool isAll) { _isAll = isAll; }
    bool isAll() const { return _isAll; }     

private:
    bool _isAll {false};

    SelectField();
    ~SelectField();
};

}
