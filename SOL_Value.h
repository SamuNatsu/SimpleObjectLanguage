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

#ifndef SOL_VALUE_H_INCLUDED
#define SOL_VALUE_H_INCLUDED

#include <string>
#include <vector>
#include <unordered_map>

namespace SOL {

class Value;
using Object = std::unordered_map<std::string, Value>;
using Array = std::vector<Value>;

class Value {
    public:
        using ValueType = enum {
            Value_Null = 0,
            Value_String,
            Value_Object,
            Value_Array
        };
        static const char* ValueTypeName[4];

        Value() = default;
        Value(const Value&);
        Value(Value&&);
        Value(const std::string&);
        Value(std::string&&);
        Value(const char*);
        Value(const Object&);
        Value(Object&&);
        Value(const Array&);
        Value(Array&&);
        Value(long long);
        Value(double);
        ~Value();

        Value& operator=(const Value&);
        Value& operator=(Value&&);
        Value& operator=(const std::string&);
        Value& operator=(std::string&&);
        Value& operator=(const char*);
        Value& operator=(const Object&);
        Value& operator=(Object&&);
        Value& operator=(const Array&);
        Value& operator=(Array&&);
        Value& operator=(long long);
        Value& operator=(double);
        Value& operator[](const std::string&);
        Value& operator[](std::string&&);
        Value& operator[](size_t);

        void Clear();

        std::string AsString() const;
        Object AsObject() const;
        Array AsArray() const;
        long long AsInteger();
        double AsFloat();

        ValueType GetType() const;

    private:
        ValueType m_Type = Value_Null;
        void* m_Data = nullptr;
};

const char* Value::ValueTypeName[4] = {
    "Value_Null",
    "Value_String",
    "Value_Object",
    "Value_Array"
};

Value::Value(const Value& tmp) {
    *this = tmp;
}
Value::Value(Value&& tmp) {
    *this = std::forward<Value>(tmp);
}
Value::Value(const std::string& tmp) {
    *this = tmp;
}
Value::Value(std::string&& tmp) {
    *this = std::forward<std::string>(tmp);
}
Value::Value(const char* tmp) {
    *this = tmp;
}
Value::Value(const Object& tmp) {
    *this = tmp;
}
Value::Value(Object&& tmp) {
    *this = std::forward<Object>(tmp);
}
Value::Value(const Array& tmp) {
    *this = tmp;
}
Value::Value(Array&& tmp) {
    *this = std::forward<Array>(tmp);
}
Value::Value(long long tmp) {
    *this = tmp;
}
Value::Value(double tmp) {
    *this = tmp;
}
Value::~Value() {
    Clear();
}

Value& Value::operator=(const Value& tmp) {
    Clear();
    m_Type = tmp.m_Type;
    switch (m_Type) {
        case (Value_Null):
            m_Data = nullptr;
            break;
        case (Value_String):
            m_Data = new std::string(*(std::string*)tmp.m_Data);
            break;
        case (Value_Object):
            m_Data = new Object(*(Object*)tmp.m_Data);
            break;
        case (Value_Array):
            m_Data = new Array(*(Array*)tmp.m_Data);
            break;
    }
    return *this;
}
Value& Value::operator=(Value&& tmp) {
    Clear();
    m_Type = tmp.m_Type;
    m_Data = tmp.m_Data;
    tmp.m_Type = Value_Null;
    tmp.m_Data = nullptr;
    return *this;
}
Value& Value::operator=(const std::string& tmp) {
    if (m_Type == Value_String)
        *(std::string*)m_Data = tmp;
    else {
        Clear();
        m_Type = Value_String;
        m_Data = new std::string(tmp);
    }
    return *this;
}
Value& Value::operator=(std::string&& tmp) {
    if (m_Type == Value_String)
        ((std::string*)m_Data)->swap(tmp);
    else {
        Clear();
        m_Type = Value_String;
        m_Data = new std::string(std::forward<std::string>(tmp));
    }
    return *this;
}
Value& Value::operator=(const char* tmp) {
    (*this) = std::string(tmp);
    return *this;
}
Value& Value::operator=(const Object& tmp) {
    if (m_Type == Value_Object)
        *(Object*)m_Data = tmp;
    else {
        Clear();
        m_Type = Value_Object;
        m_Data = new Object(tmp);
    }
    return *this;
}
Value& Value::operator=(Object&& tmp) {
    if (m_Type == Value_Object)
        ((Object*)m_Data)->swap(tmp);
    else {
        Clear();
        m_Type = Value_Object;
        m_Data = new Object(std::forward<Object>(tmp));
    }
    return *this;
}
Value& Value::operator=(const Array& tmp) {
    if (m_Type == Value_Array)
        *(Array*)m_Data = tmp;
    else {
        Clear();
        m_Type = Value_Array;
        m_Data = new Array(tmp);
    }
    return *this;
}
Value& Value::operator=(Array&& tmp) {
    if (m_Type == Value_Array)
        ((Array*)m_Data)->swap(tmp);
    else {
        Clear();
        m_Type = Value_Array;
        m_Data = new Array(std::forward<Array>(tmp));
    }
    return *this;
}
Value& Value::operator=(long long tmp) {
    if (m_Type == Value_String)
        *(std::string*)m_Data = std::to_string(tmp);
    else {
        Clear();
        m_Type = Value_String;
        m_Data = new std::string(std::to_string(tmp));
    }
    return *this;
}
Value& Value::operator=(double tmp) {
    if (m_Type == Value_String)
        *(std::string*)m_Data = std::to_string(tmp);
    else {
        Clear();
        m_Type = Value_String;
        m_Data = new std::string(std::to_string(tmp));
    }
    return *this;
}
Value& Value::operator[](const std::string& tmp) {
    if (m_Type == Value_Object)
        return (*(Object*)m_Data)[tmp];
    else {
        *this = Object();
        return (*(Object*)m_Data)[tmp];
    }
}
Value& Value::operator[](std::string&& tmp) {
    return (*this)[tmp];
}
Value& Value::operator[](size_t tmp) {
    if (m_Type == Value_Array) {
        while (tmp >= ((Array*)m_Data)->size())
            ((Array*)m_Data)->emplace_back("");
        return (*(Array*)m_Data)[tmp];
    }
    else {
        *this = Array();
        while (tmp >= ((Array*)m_Data)->size())
            ((Array*)m_Data)->emplace_back("");
        return (*(Array*)m_Data)[tmp];
    }
}

void Value::Clear() {
    switch (m_Type) {
        case (Value_String):
            delete (std::string*)m_Data;
            break;
        case (Value_Object):
            delete (Object*)m_Data;
            break;
        case (Value_Array):
            delete (Array*)m_Data;
            break;
        default:
            break;
    }
    m_Type = Value_Null;
    m_Data = nullptr;
}

std::string Value::AsString() const {
    return m_Type == Value_String ? *(std::string*)m_Data : "";
}
Object Value::AsObject() const {
    return m_Type == Value_Object ? *(Object*)m_Data : Object();
}
Array Value::AsArray() const {
    return m_Type == Value_Array ? *(Array*)m_Data : Array();
}
long long Value::AsInteger() {
    if (m_Type != Value_String)
        return 0;
    long long _rtn = 0;
    sscanf(((std::string*)m_Data)->c_str(), "%lld", &_rtn);
    return _rtn;
}
double Value::AsFloat() {
    if (m_Type != Value_String)
        return 0.0;
    double _rtn = 0.0;
    sscanf(((std::string*)m_Data)->c_str(), "%lf", &_rtn);
    return _rtn;
}

Value::ValueType Value::GetType() const {
    return m_Type;
}

}

#endif