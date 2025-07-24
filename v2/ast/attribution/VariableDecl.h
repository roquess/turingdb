#pragma once

#include "attribution/DeclID.h"
#include "attribution/VariableType.h"

namespace db {

class ConstVariableDecl;

class VariableDecl {
public:
    ~VariableDecl() = default;

    VariableDecl(VariableType type = VariableType::Invalid, DeclID id = -1)
        : _id(id),
          _type(type)
    {
    }

    VariableDecl(const VariableDecl&) = delete;
    VariableDecl(VariableDecl&&) = delete;
    VariableDecl& operator=(const VariableDecl&) = delete;
    VariableDecl& operator=(VariableDecl&&) = delete;

    operator ConstVariableDecl() const;

    DeclID id() const {
        return _id;
    }

    bool valid() const {
        return _id.valid();
    }

    VariableType type() const {
        return _type;
    }

    const std::string_view& name() const {
        return _name;
    }

    void setName(std::string_view name) {
        _name = name;
    }

private:
    DeclID _id;
    VariableType _type {VariableType::Invalid};
    std::string_view _name;
};

class ConstVariableDecl {
public:
    ~ConstVariableDecl() = default;

    ConstVariableDecl(VariableType type = VariableType::Invalid, DeclID id = -1, std::string_view name = "")
        : _id(id),
          _type(type),
          _name(name)
    {
    }

    ConstVariableDecl(const ConstVariableDecl&) = default;
    ConstVariableDecl(ConstVariableDecl&&) = default;
    ConstVariableDecl& operator=(const ConstVariableDecl&) = delete;
    ConstVariableDecl& operator=(ConstVariableDecl&&) = delete;

    DeclID id() const {
        return _id;
    }

    bool valid() const {
        return _id.valid();
    }

    VariableType type() const {
        return _type;
    }

    const std::string_view& name() const {
        return _name;
    }

private:
    DeclID _id;
    VariableType _type {VariableType::Invalid};
    std::string_view _name;
};

inline VariableDecl::operator ConstVariableDecl() const {
    return ConstVariableDecl {_type, _id, _name};
}

}
