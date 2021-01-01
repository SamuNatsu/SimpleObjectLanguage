/**
 * MIT License
 * 
 * Copyright (c) 2020 SamuNatsu
 * 
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * 
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * 
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
**/

#ifndef SOL_HPP_INCLUDED
#define SOL_HPP_INCLUDED

#include <fstream>

#include "SOL_Parser.hpp"

#define SOL_VERSION "1.0.0"
#define SOL_VERSION_MAJOR 1
#define SOL_VERSION_MINOR 0
#define SOL_VERSION_PATCH 0

namespace {

void fWriteObject(const SOL::Object&, std::ofstream&, size_t);
void fWriteArray(const SOL::Array&, std::ofstream&, size_t);
void fWriteValue(const SOL::Value&, std::ofstream&, size_t);

void cWriteObject(const SOL::Object&, std::ofstream&);
void cWriteArray(const SOL::Array&, std::ofstream&);
void cWriteValue(const SOL::Value&, std::ofstream&);

std::string sWriteObject(const SOL::Object&);
std::string sWriteArray(const SOL::Array&);
std::string sWriteValue(const SOL::Value&);

}

namespace SOL {

bool SaveFormatted(const Object&, const std::string&);
bool SaveFormatted(const Object&, std::string&&);
bool SaveFormatted(Object&&, const std::string&);
bool SaveFormatted(Object&&, std::string&&);

bool SaveCompressed(const Object&, const std::string&);
bool SaveCompressed(const Object&, std::string&&);
bool SaveCompressed(Object&&, const std::string&);
bool SaveCompressed(Object&&, std::string&&);

std::string Convert2String(const Object&);
std::string Convert2String(Object&&);

}

#endif

#ifdef SOL_IMPLEMENTATION

namespace {

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

std::string sWriteObject(const SOL::Object& tmp) {
    std::string _s(1, '{');
    size_t _lmt = tmp.size(), _itx = 0;
    for (auto& i : tmp) {
        _s += i.first + '=' + sWriteValue(i.second);
        if (++_itx < _lmt)
            _s += ',';
    }
    return _s + '}';
}
std::string sWriteArray(const SOL::Array& tmp) {
    std::string _s(1, '[');
    size_t _lmt = tmp.size(), _itx = 0;
    for (auto& i : tmp) {
        _s += sWriteValue(i);
        if (++_itx < _lmt)
            _s += ',';
    }
    return _s + ']';
}
std::string sWriteValue(const SOL::Value& tmp) {
    switch (tmp.GetType()) {
        case (SOL::Value_String):
            return '"' + tmp.AsString() + '"';
        case (SOL::Value_Object):
            return sWriteObject(tmp.AsObject());
        case (SOL::Value_Array):
            return sWriteArray(tmp.AsArray());
        default:
            return "";
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

std::string Convert2String(const Object& root) {
    return sWriteObject(root);
}
std::string Convert2String(Object&& root) {
    return Convert2String(root);
}

}

#endif