# SimpleObjectLanguage
A simple data switching language

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

## How to use
```cpp
#include <iostream>

#include "SOL.hpp"

sol::Parser parser;
sol::Object obj;

int main() {
    // Read .sol file
    if (!parser.ParseFromFile("test.sol")) {
        // Get error
        std::cerr << parser.GetError() << std::endl;
        return 0;
    }
    
    // Get result
    obj = parser.GetResult();
    
    // Modify
    obj["settings"] = "nul";
    
    // Convert object to formatted string
    std::cout << parser.ParseToString(obj) << std::endl;
    
    return 0;
}
```

## Simple document
### sol::Parser
> #### const std::string& GetError()
> Get error string

> #### bool ParseFromFile(path)
> Parse .sol file from **path**(C-style string or `std::string`), return true if success

> #### bool ParseFromMemory(mem)
> Parse .sol string from **mem**(C-style string or `std::string`), return true if success

> #### Object GetResult()
> return the result, the result maybe undefine when it fails to parse

> #### bool ParseToFile(path, obj, flag = false)
> Write **obj**(Object) to file **path**(C-style string or `std::string`), if **flag**(bool) is false, the output would be formatted

> #### std::string ParseToString(obj, flag = false)
> Return **obj**(Object) as a string, if **flag**(bool) is false, the string would be formatted

### sol::Object
Alias of `std::unordered_map<std::string, Value>`

### sol::Array
Alias of `std::vector<Value>`

### sol::Value
#### Using constructor to assign it:
> sol::Value() // empty
> 
> sol::Value(value) // from another sol::Value
> 
> sol::Value(object) // from another sol::Object
> 
> sol::Value(array) // from another sol::Array
> 
> sol::Value("string") // from another C-style string or std::string

#### Using assign operator:
> value = another_vale
> 
> value = another_object
> 
> ...

#### Get value type:
> value.GetType()

#### Use it as Object/Array/String:
> value.AsObject()
> 
> value.AsArray()
> 
> value.AsString()
> 
> (Object)value
> 
> (Array)value
> 
> (std::string)value
> 
If the type is not the same (eg. an object type value calls AsArray()), they would return a null value

#### Operator\[\]:
> If it is an object value, it is same as `std::unordered_map<std::string, Value>::operator[]`
> 
> Or it would clear itself and assign it self an empty object, then call `std::unordered_map<std::string, Value>::operator[]`
>
> If it is an array value, it is same as `std::vector<Value>::operator[]`
> 
> Or it would clear itself and assign it self an empty array, then call `std::vector<Value>::operator[]`
