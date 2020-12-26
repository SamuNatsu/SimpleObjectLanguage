#ifndef SOL_H_INCLUDED
#define SOL_H_INCLUDED

#include <fstream>

#include "SOL_Parser.h"

#define SOL_VERSION "0.1.1"

namespace {

void fWriteValue(const SOL::Value&, std::ofstream&, size_t layer);

void fWriteObject(const SOL::Object& tmp, std::ofstream& f, size_t layer) {
    f << '{';
    size_t _lmt = tmp.size(), _itx = 0;
    for (auto& i : tmp) {
        f << '\n';
        for (size_t j = 0; j <= layer; ++j)
            f << '\t';
        f << i.first << " = ";
        fWriteValue(i.second, f, layer + 1);
        if (++_itx < _lmt)
            f << ',';
    }
    if (_lmt) {
        f << '\n';
        for (size_t j = 0; j < layer; ++j)
            f << '\t';
    }
    f << '}';
}
void fWriteArray(const SOL::Array& tmp, std::ofstream& f, size_t layer) {
    f << '[';
    size_t _lmt = tmp.size(), _itx = 0;
    for (auto& i : tmp) {
        f << '\n';
        for (size_t j = 0; j <= layer; ++j)
            f << '\t';
        fWriteValue(i, f, layer + 1);
        if (++_itx < _lmt)
            f << ',';
    }
    if (_lmt) {
        f << '\n';
        for (size_t j = 0; j < layer; ++j)
            f << '\t';
    }
    f << ']';
}
void fWriteValue(const SOL::Value& tmp, std::ofstream& f, size_t layer) {
    switch (tmp.GetType()) {
        case (SOL::Value_String):
            f << '"' << tmp.AsString() << '"';
            break;
        case (SOL::Value_Object):
            fWriteObject(tmp.AsObject(), f, layer);
            break;
        case (SOL::Value_Array):
            fWriteArray(tmp.AsArray(), f, layer);
        default:
            break;
    }
}

void cWriteValue(const SOL::Value&, std::ofstream&);

void cWriteObject(const SOL::Object& tmp, std::ofstream& f) {
    f << '{';
    size_t _lmt = tmp.size(), _itx = 0;
    for (auto& i : tmp) {
        f << i.first << '=';
        cWriteValue(i.second, f);
        if (++_itx < _lmt)
            f << ',';
    }
    f << '}';
}
void cWriteArray(const SOL::Array& tmp, std::ofstream& f) {
    f << '[';
    size_t _lmt = tmp.size(), _itx = 0;
    for (auto& i : tmp) {
        cWriteValue(i, f);
        if (++_itx < _lmt)
            f << ',';
    }
    f << ']';
}
void cWriteValue(const SOL::Value& tmp, std::ofstream& f) {
    switch (tmp.GetType()) {
        case (SOL::Value_String):
            f << '"' << tmp.AsString() << '"';
            break;
        case (SOL::Value_Object):
            cWriteObject(tmp.AsObject(), f);
            break;
        case (SOL::Value_Array):
            cWriteArray(tmp.AsArray(), f);
        default:
            break;
    }
}

}

namespace SOL {

bool SaveFormatted(const Object& root, const std::string& file) {
    std::ofstream _fout(file);
    if (!_fout.is_open())
        return false;
    ::fWriteObject(root, _fout, 0);
    _fout.close();
    return true;
}
bool SaveFormatted(const Object& root, std::string&& file) {
    return SaveFormatted(root, file);
}
bool SaveFormatted(Object&& root, const std::string& file) {
    return SaveFormatted(root, file);
}
bool SaveFormatted(Object&& root, std::string&& file) {
    return SaveFormatted(root, file);
}

bool SaveCompressed(const Object& root, const std::string& file) {
    std::ofstream _fout(file);
    if (!_fout.is_open())
        return false;
    ::cWriteObject(root, _fout);
    _fout.close();
    return true;
}
bool SaveCompressed(const Object& root, std::string&& file) {
    return SaveCompressed(root, file);
}
bool SaveCompressed(Object&& root, const std::string& file) {
    return SaveCompressed(root, file);
}
bool SaveCompressed(Object&& root, std::string&& file) {
    return SaveCompressed(root, file);
}

}

#endif