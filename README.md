# SimpleObjectLanguage
A simple data switching language

## Quick Tutorial
You can simply use it like this:

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
