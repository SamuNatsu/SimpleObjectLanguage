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
 * 
 * @file    SOL_Value.h
 * @author  SamuNatsu
 * @version 0.1.2
 * @brief   Value class
**/

#ifndef SOL_VALUE_H_INCLUDED
#define SOL_VALUE_H_INCLUDED

#include <string>
#include <vector>
#include <unordered_map>

/**
 * @namespace
 * @brief   Simple object language namespace
**/
namespace SOL {

/**
 * @enum
 * @brief   Value types
**/
using ValueType = enum _vt {
    Value_Null = 0,
    Value_String,
    Value_Object,
    Value_Array
};

class Value;
/**
 * @brief   Object
**/
using Object = std::unordered_map<std::string, Value>;
/**
 * @brief   Array
**/
using Array = std::vector<Value>;

/**
 * @class
 * @brief   Value
**/
class Value {
    public:
        /**
         * @brief   Value type names (C-style string)
        **/
        static const char* ValueTypeName[4];

        /**
         * @brief   Default constructor
        **/
        Value() = default;
        /**
         * @brief   Copy constructor
         * @param   tmp Source value
        **/
        Value(const Value& tmp);
        /**
         * @brief   Move constructor
         * @param   tmp Source value
        **/
        Value(Value&& tmp);
        /**
         * @brief   Construct value from a string
         * @param   tmp Source string
        **/
        Value(const std::string& tmp);
        /**
         * @brief   Construct value from a string
         * @param   tmp Source string
        **/
        Value(std::string&& tmp);
        /**
         * @brief   Construct value from a C-style string
         * @param   tmp Source string
        **/
        Value(const char* tmp);
        /**
         * @brief   Construct value from an object
         * @param   tmp Source object
        **/
        Value(const Object& tmp);
        /**
         * @brief   Construct value from an object
         * @param   tmp Source object
        **/
        Value(Object&& tmp);
        /**
         * @brief   Construct value from an array
         * @param   tmp Source array
        **/
        Value(const Array& tmp);
        /**
         * @brief   Construct value from an array
         * @param   tmp Source array
        **/
        Value(Array&& tmp);
        /**
         * @brief   Construct value from an integer
         * @param   tmp Source integer
        **/
        Value(long long tmp);
        /**
         * @brief   Construct value from an float
         * @param   tmp Source float
        **/
        Value(double tmp);
        /**
         * @brief   Default destructor
        **/
        ~Value();

        /**
         * @brief   Assign a value
         * @param   tmp Value
        **/
        Value& operator=(const Value& tmp);
        /**
         * @brief   Assign a value
         * @param   tmp Value
        **/
        Value& operator=(Value&& tmp);
        /**
         * @brief   Assign a value
         * @param   tmp Value
        **/
        Value& operator=(const std::string& tmp);
        /**
         * @brief   Assign a value
         * @param   tmp Value
        **/
        Value& operator=(std::string&& tmp);
        /**
         * @brief   Assign a value
         * @param   tmp Value
        **/
        Value& operator=(const char* tmp);
        /**
         * @brief   Assign a value
         * @param   tmp Value
        **/
        Value& operator=(const Object& tmp);
        /**
         * @brief   Assign a value
         * @param   tmp Value
        **/
        Value& operator=(Object&& tmp);
        /**
         * @brief   Assign a value
         * @param   tmp Value
        **/
        Value& operator=(const Array& tmp);
        /**
         * @brief   Assign a value
         * @param   tmp Value
        **/
        Value& operator=(Array&& tmp);
        /**
         * @brief   Assign a value
         * @param   tmp Value
        **/
        Value& operator=(long long tmp);
        /**
         * @brief   Assign a value
         * @param   tmp Value
        **/
        Value& operator=(double tmp);
        /**
         * @brief   Get a value through a string
         * @param   tmp Source string
        **/
        Value& operator[](const std::string& tmp);
        /**
         * @brief   Get a value through a string
         * @param   tmp Source string
        **/
        Value& operator[](std::string&& tmp);
        /**
         * @brief   Get a value through an integer
         * @param   tmp Source integer
        **/
        Value& operator[](size_t tmp);

        /**
         * @brief   Clear value
        **/
        void Clear();

        /**
         * @brief   Convert value as string
        **/
        std::string AsString() const;
        /**
         * @brief   Convert value as object
        **/
        Object AsObject() const;
        /**
         * @brief   Convert value as array
        **/
        Array AsArray() const;
        /**
         * @brief   Convert value as integer
        **/
        long long AsInteger();
        /**
         * @brief   Convert value as float
        **/
        double AsFloat();

        /**
         * @brief   Get value type
        **/
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

ValueType Value::GetType() const {
    return m_Type;
}

}

#endif 
