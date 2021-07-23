#ifndef SOL_HPP_INCLUDED
#define SOL_HPP_INCLUDED

#include <vector>

#include "SOL_Value.hpp"
#include "SOL_Parser.hpp"

#define SOL_VERSION "4.1.1"
#define SOL_VERSION_MAJAR 4
#define SOL_VERSION_MINOR 1
#define SOL_VERSION_PATCH 1

namespace sol {

inline bool check(const Value& v, const std::string& path, ValueType type) {
    size_t p = path.find_first_of('.');
    std::string lb;
    if (p == path.npos)
        lb = path;
    else 
        lb = path.substr(0, p);
    if (lb.length() == 0)
        return false;
    bool f = false;
    for (auto& i : lb)
        if (!isdigit(i)) {
            f = true;
            break;
        }
    if (f) {
        if (!v.isObject())
            return false;
        if (p == path.npos)
            return const_cast<Value&>(v).object()[lb].type() == type;
        else 
            return check(const_cast<Value&>(v).object()[lb], path.substr(p + 1), type);
    }
    else {
        if (!v.isArray())
            return false;
        size_t a = std::stoull(lb);
        if (const_cast<Value&>(v).array().size() <= a)
            return false;
        if (p == path.npos)
            return const_cast<Value&>(v).array()[a].type() == type;
        else 
            return check(const_cast<Value&>(v).array()[a], path.substr(p + 1), type);
    }
}

inline std::vector<int> check(const Value& v, const std::vector<std::string>& ls) {
    std::vector<int> rtn;
    int cnt = 0;
    for (auto& i : ls) {
        size_t p = i.find_first_of(':');
        if (p == i.npos) {
            rtn.emplace_back(-1);
            ++cnt;
            continue;
        }
        std::string lb = i.substr(p + 1);
        ValueType t;
        if (lb == "Array")
            t = VALUE_ARRAY;
        else if (lb == "Object")
            t = VALUE_OBJECT;
        else if (lb == "String")
            t = VALUE_STRING;
        else if (lb == "Null")
            t = VALUE_NULL;
        else {
            rtn.emplace_back(-2);
            ++cnt;
            continue;
        }
        if (check(v, i.substr(0, p), t))
            rtn.emplace_back(1);
        else {
            rtn.emplace_back(0);
            ++cnt;
        }
    }
    rtn.emplace_back(cnt);
    return rtn;
}

}

#endif