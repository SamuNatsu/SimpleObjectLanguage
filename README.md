# SimpleObjectLanguage
A simple data switching language

**-----Need help to write tutorial document-----**

## Grammar
The file looks like this:

```
{
    key = "value",
    object = {
        key = "object-key",
        object = {},
        array = ["object-array"]
    },
    array = [
        "array.key",
        {},
        []
    ]
}
```

BNF:
```
<Key> ::= <key> ("0..9" | "a..z" | "A..Z" | "_")
    
<String> ::= <String> (Any printable ASCII charactor)
    
<Value> ::= (""" <String> """) | <Object> | <Array>
    
<Pair> ::= <Key> "=" <Value>
    
<Array> ::= "[" {<Value> ","} <Value> "]"
    
<object> ::= "{" {<Pair> ","} <Pair> "}"
```

## Quick Tutorial
1. Download "SOL.h", "SOL_Parser.h", "SOL_Scanner.h", "SOL_Token.h", "SOL_Value.h"
2. You can simply use it like this:

```cpp
#include <cstdio>

// This is the header we need
#include "SOL.h"

// For common using, you just need a parser and an object
SOL::Parser parser;
SOL::Object root;

int main() {
    // Parse file
    if (!parser.Parse("test.sol")) {
        puts(parser.GetError().c_str());
        return 0;
    }
    
    // After parsing, you can get the object
    root = parser.GetRoot();
    
    // You can simply get/modify the elements just like objects in JavaScript(?)
    printf("%s\n", root["object"]["array"][0].AsString().c_str());
    
    // Or you can use SOL::SaveFormatted()/SOL::SaveCompressed() to save
    SOL::SaveFormatted(root, "save.sol");
    
    return 0;
}
```

## More Details
Once you successfully get the object for parser, all you can do with it are:
1. Use Key to get Value:`root["Key"]`
2. Convert Value to other types like: `root["Key"].AsString()`, `root["Key"].AsObject()` etc. (We native support AsString(), AsObject(), AsArray(), AsInteger(), AsFloat(). **Warning: If failed to convert, it would return a undefined data!**)
3. Cover/New a Value through assignment operator like: `root["Key"]="Hello"`, `root["Key"]=12345`, etc.
