# SimpleObjectLanguage
A simple data switching language

"SOL.hpp" is a simple one-header toolkit/library for you to use SOL

## Grammar
The file looks like this:
```
{
    sample_key = "sample_value",
    sample_object = {
        sample_key1 = "",
        sample_key2 = {},
        sample_key3 = []
    },
    sample_array = [
        "",
        {},
        []
    ]
}
```

BNF:
```
<Key> ::= ("a..z" | "A..Z" | "_") {"0..9" | "a..z" | "A..Z" | "_"}
<String> ::= """ {UTF-8 string} """
<Value> ::= <String> | <Object> | <Array>
<Pair> ::= <Key> "=" <Value>
<Array> ::= "[" {<Value> ","} <Value> "]"
<object> ::= "{" {<Pair> ","} <Pair> "}"
```

## Something important
Only supports '\0', '\t', '\n', '\r', '\"', '\\' and UCS-2('\uxxxx') to be escaped and unescaped in strings.

It looks really similar to json, if you want to convert json to sol, just replace ':' to '=', delete double quotes before and after each key, and add double quotes before and after each value.

## How to use
```cpp
#include <iostream>

#include "SOL.hpp"

sol::Parser parser;
sol::Object obj;

int main() {
    // Read .sol file
    if (!parser.parseFromFile("test.sol")) {
        // Get error
        std::cerr << parser.getError() << std::endl;
        return 0;
    }
    
    // Get result
    obj = parser.getResult();
    
    // Modify
    obj["settings"] = "nul";
    
    // Convert object to formatted string
    std::cout << sol::convertToString()(obj) << std::endl;
    
    return 0;
}
```

## Simple document
### sol::Parser
> #### const std::string& getError()
> Get error string

> #### bool parseFromFile(path)
> Parse .sol file from **path**(C-style string or `std::string`), return true if success

> #### bool parseFromMemory(mem)
> Parse .sol string from **mem**(C-style string or `std::string`), return true if success

> #### sol::Object getObject()
> Return the parse result, the result maybe undefine when it fails to parse

### sol::Object
Alias of `std::unordered_map<std::string, Value>`

### sol::Array
Alias of `std::vector<Value>`

### sol::Value
#### Using constructor to assign it:
> `sol::Value() // empty`
> 
> `sol::Value(value) // from another sol::Value`
> 
> `sol::Value(object) // from another sol::Object`
> 
> `sol::Value(array) // from another sol::Array`
> 
> `sol::Value("string") // from another C-style string or std::string`

#### Using assign operator:
> `value = another_vale`
> 
> `value = another_object`
> 
> ...

#### Get value type:
> `value.getType()`

#### Use it as Object/Array/String/Integer/Double/Bool:
> `value.asObject()`
> 
> `value.asArray()`
> 
> `value.asString()`
> 
> `value.asInteger() // Base on std::string, convert to long long`
> 
> `value.asDouble() // Base on std::string, convert to double`
> 
> `value.asBool() // Base on std::string, only if std::string is "true", it would return true`
> 
> `(Object)value`
> 
> `(Array)value`
> 
> `(std::string)value`
> 
If the type is not the same (eg. an object type value calls `asArray()`), they would return an undefined value

#### Operator\[\]:
> If it is an object value, it is same as `std::unordered_map<std::string, Value>::operator[]`
> 
> Or it would clear itself and assign it self an empty object, then call `std::unordered_map<std::string, Value>::operator[]`
>
> If it is an array value, it is same as `std::vector<Value>::operator[]`
> 
> Or it would clear itself and assign it self an empty array, then call `std::vector<Value>::operator[]`

### sol::getValue
> `sol::Value sol::getValue()(object, "a.b.c.d.e.f.g")`
> 
Get Value from Object, which path is "a.b.c.d.e.f.g"

If path is invalid, it would return a null `sol::Value`, which type is `sol::VALUE_NULL`

### sol::convertToString
> `std::string sol::convertToString()(object, falg = false)`
> 
Return **obj**(`sol::Object`) as a string, if **flag**(`bool`) is false, the string would be formatted

### sol::saveToFile
> `bool sol::saveToFile()(path, obj, flag = false)`
> 
Write **obj**(`sol::Object`) to file **path**(C-style string or `std::string`), if **flag**(`bool`) is false, the output would be formatted
