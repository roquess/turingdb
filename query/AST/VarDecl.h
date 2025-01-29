#pragma once

#include <string>

namespace db {

class ASTContext;
class DeclContext;
class Column;

class VarDecl {
public:
    friend ASTContext;

    static VarDecl* create(ASTContext* astCtxt,
                           DeclContext* declContext,
                           const std::string& name);

    const std::string& getName() const { return _name; }

    bool isSelected() const { return _selected; }
    
    void setSelected(bool selected) { _selected = selected; }

    void setColumn(Column* column) { _column = column; }

    Column* getColumn() const { return _column; }

private:
    std::string _name;
    bool _selected {false};
    Column* _column {nullptr};
    
    VarDecl(const std::string& name);
    ~VarDecl();
};

}
