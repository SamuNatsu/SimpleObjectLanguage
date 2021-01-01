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

#ifndef SOL_H_INCLUDED
#define SOL_H_INCLUDED

#include <fstream>

#include "SOL_Parser.hpp"

#define SOL_VERSION "0.1.3"
#define SOL_VERSION_MAJOR 0
#define SOL_VERSION_MINOR 1
#define SOL_VERSION_PATCH 3

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

std::string sWriteValue(const SOL::Value&);
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

/**
 * @namespace
 * @brief   Simple object language namespace
**/
namespace SOL {

/**
 * @brief   Save an object which is formatted
 * @param   root    Root object
 * @param   file    File name
**/
bool SaveFormatted(const Object& root, const std::string& file) {
    std::ofstream _fout(file);
    if (!_fout.is_open())
        return false;
    ::fWriteObject(root, _fout, 0);
    _fout.close();
    return true;
}
/**
 * @brief   Save an object which is formatted
 * @param   root    Root object
 * @param   file    File name
**/
bool SaveFormatted(const Object& root, std::string&& file) {
    return SaveFormatted(root, file);
}
/**
 * @brief   Save an object which is formatted
 * @param   root    Root object
 * @param   file    File name
**/
bool SaveFormatted(Object&& root, const std::string& file) {
    return SaveFormatted(root, file);
}
/**
 * @brief   Save an object which is formatted
 * @param   root    Root object
 * @param   file    File name
**/
bool SaveFormatted(Object&& root, std::string&& file) {
    return SaveFormatted(root, file);
}

/**
 * @brief   Save an object which is compressed
 * @param   root    Root object
 * @param   file    File name
**/
bool SaveCompressed(const Object& root, const std::string& file) {
    std::ofstream _fout(file);
    if (!_fout.is_open())
        return false;
    ::cWriteObject(root, _fout);
    _fout.close();
    return true;
}
/**
 * @brief   Save an object which is compressed
 * @param   root    Root object
 * @param   file    File name
**/
bool SaveCompressed(const Object& root, std::string&& file) {
    return SaveCompressed(root, file);
}
/**
 * @brief   Save an object which is compressed
 * @param   root    Root object
 * @param   file    File name
**/
bool SaveCompressed(Object&& root, const std::string& file) {
    return SaveCompressed(root, file);
}
/**
 * @brief   Save an object which is compressed
 * @param   root    Root object
 * @param   file    File name
**/
bool SaveCompressed(Object&& root, std::string&& file) {
    return SaveCompressed(root, file);
}

/**
 * @brief   Convert an object to string which is compressed
 * @param   root    Root object
**/
std::string Convert2String(const Object& root) {
    return sWriteObject(root);
}
/**
 * @brief   Convert an object to string which is compressed
 * @param   root    Root object
**/
std::string Convert2String(Object&& root) {
    return Convert2String(root);
}

}

#endif