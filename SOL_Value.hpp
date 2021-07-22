#ifndef SOL_VALUE_HPP_INCLUDED
#define SOL_VALUE_HPP_INCLUDED

#include <vector>
#include <string>
#include <algorithm>
#include <unordered_map>

namespace sol {

class Value;
using Array = std::vector<Value>;
using Object = std::unordered_map<std::string, Value>;
using String = std::string;

using ValueType = enum ___ {
    VALUE_NULL,
    VALUE_ARRAY,
    VALUE_OBJECT,
    VALUE_STRING
};

class Value {
    public:
        Value(): p_type(VALUE_NULL), p_data(nullptr) {}
        Value(const Value& t) {*this = t;}
        Value(Value&& t) {*this = std::forward<Value>(t);}
        Value(const Array& t) {*this = t;}
        Value(Array&& t) {*this = std::forward<Array>(t);}
        Value(const Object& t) {*this = t;}
        Value(Object&& t) {*this = std::forward<Object>(t);}
        Value(const String& t) {*this = t;}
        Value(String&& t) {*this = std::forward<String>(t);}
        Value(long long t) {*this = t;}
        Value(double t) {*this = t;}
        Value(bool t) {*this = t;}
        ~Value() {p_clear();}

        Value& operator=(const Value& t) {
            p_clear();
            p_type = t.p_type;
            switch (p_type) {
                case (VALUE_NULL):
                    break;
                case (VALUE_ARRAY):
                    p_data = new Array(*(Array*)t.p_data);
                    break;
                case (VALUE_OBJECT):
                    p_data = new Object(*(Object*)t.p_data);
                    break;
                case (VALUE_STRING):
                    p_data = new String(*(String*)t.p_data);
                    break;
            }
            return *this;
        }
        Value& operator=(Value&& t) {
            p_clear();
            std::swap(p_type, t.p_type);
            std::swap(p_data, t.p_data);
            return *this;
        }
        Value& operator=(const Array& t) {
            p_clear();
            p_type = VALUE_ARRAY;
            p_data = new Array(t);
            return *this;
        }
        Value& operator=(Array&& t) {
            p_clear();
            p_type = VALUE_ARRAY;
            p_data = new Array(std::forward<Array>(t));
            return *this;
        }
        Value& operator=(const Object& t) {
            p_clear();
            p_type = VALUE_OBJECT;
            p_data = new Object(t);
            return *this;
        }
        Value& operator=(Object&& t) {
            p_clear();
            p_type = VALUE_OBJECT;
            p_data = new Object(std::forward<Object>(t));
            return *this;
        }
        Value& operator=(const String& t) {
            p_clear();
            p_type = VALUE_STRING;
            p_data = new String(t);
            return *this;
        }
        Value& operator=(String&& t) {
            p_clear();
            p_type = VALUE_STRING;
            p_data = new String(std::forward<String>(t));
            return *this;
        }
        Value& operator=(long long t) {
            p_clear();
            p_type = VALUE_STRING;
            p_data = new String(std::to_string(t));
            return *this;
        }
        Value& operator=(double t) {
            p_clear();
            p_type = VALUE_STRING;
            p_data = new String(std::to_string(t));
            return *this;
        }
        Value& operator=(bool t) {
            p_clear();
            p_type = VALUE_STRING;
            p_data = new String(t ? "true" : "false");
            return *this;
        }

        Value& operator[](size_t t) {
            if (p_type != VALUE_ARRAY)
                *this = Array();
            if (((Array*)p_data)->size() <= t)
                ((Array*)p_data)->resize(t + 1);
            return (*(Array*)p_data)[t];
        }
        Value& operator[](const String& t) {
            if (p_type != VALUE_OBJECT)
                *this = Object();
            return (*(Object*)p_data)[t];
        }

        bool isNull() const {
            return p_type == VALUE_NULL;
        }
        bool isArray() const {
            return p_type == VALUE_ARRAY;
        }
        bool isObject() const {
            return p_type == VALUE_OBJECT;
        }
        bool isString() const {
            return p_type == VALUE_STRING;
        }
        ValueType type() const {
            return p_type;
        }

        Array& array() {
            if (!isArray())
                *this = Array();
            return *(Array*)p_data;
        }
        Object& object() {
            if (!isObject())
                *this = Object();
            return *(Object*)p_data;
        }
        String& string() {
            if (!isString())
                *this = String();
            return *(String*)p_data;
        }
        long long integer() const {
            return isString() ? std::stoll(*(String*)p_data) : 0;
        }
        double real() const {
            return isString() ? std::stod(*(String*)p_data) : 0.0;
        }
        bool boolean() const {
            return isString() ? *(String*)p_data == "true" : false;
        }

    private:
        ValueType p_type;
        void* p_data;

        void p_clear() {
            switch (p_type) {
                case (VALUE_NULL):
                    break;
                case (VALUE_ARRAY):
                    delete (Array*)p_data;
                    break;
                case (VALUE_OBJECT):
                    delete (Object*)p_data;
                    break;
                case (VALUE_STRING):
                    delete (String*)p_data;
                    break;
            }
            p_type = VALUE_NULL;
            p_data = nullptr;
        }
};

}

#endif