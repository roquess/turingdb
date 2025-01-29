#pragma once

#include <vector>

namespace db {

class QueryCommand;
class SelectField;
class FromTarget;
class PathPattern;
class EntityPattern;
class TypeConstraint;
class NameConstraint;
class ExprConstraint;
class Expr;
class VarDecl;
class SelectProjection;

class ASTContext {
public:
    friend QueryCommand;
    friend SelectField;
    friend FromTarget;
    friend PathPattern;
    friend EntityPattern;
    friend TypeConstraint;
    friend NameConstraint;
    friend ExprConstraint;
    friend Expr;
    friend VarDecl;
    friend SelectProjection;

    ASTContext();
    ASTContext(ASTContext&& other) = delete;
    ASTContext(const ASTContext& other) = delete;
    ASTContext& operator=(ASTContext&& other) = delete;
    ASTContext& operator=(const ASTContext& other) = delete;

    ~ASTContext();

    QueryCommand* getRoot() const { return _root; } 

    void setRoot(QueryCommand* cmd) { _root = cmd; }

    bool hasError() const { return _isError; }
    void setError(bool hasError) { _isError = hasError; }

private:
    bool _isError {false};
    QueryCommand* _root {nullptr};
    std::vector<QueryCommand*> _cmds;
    std::vector<SelectField*> _selectFields;
    std::vector<FromTarget*> _fromTargets;
    std::vector<PathPattern*> _pathPatterns;
    std::vector<EntityPattern*> _entityPatterns;
    std::vector<TypeConstraint*> _typeConstraints;
    std::vector<NameConstraint*> _nameConstraints;
    std::vector<ExprConstraint*> _exprConstraints;
    std::vector<Expr*> _expr;
    std::vector<VarDecl*> _varDecls;
    std::vector<SelectProjection*> _selectProjections;

    void addCmd(QueryCommand* cmd);
    void addSelectField(SelectField* field);
    void addFromTarget(FromTarget* target);
    void addPathPattern(PathPattern* pattern);
    void addEntityPattern(EntityPattern* pattern);
    void addTypeConstraint(TypeConstraint* constr);
    void addNameConstraint(NameConstraint* constr);
    void addExprConstraint(ExprConstraint* constr);
    void addExpr(Expr* expr);
    void addVarDecl(VarDecl* decl);
    void addSelectProjection(SelectProjection* proj);
};

} 
