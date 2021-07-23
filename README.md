# SimpleObjectLanguage
A simple data switching language.

# Install
1. Download `SOL.hpp`, `SOL_Parser.hpp`, `SOL_Scanner.hpp`, `SOL_Token.hpp` and `SOL_Value.hpp`, put them in the same folder. 

2. When you need to use it, just include `SOL.hpp`.

# Manual
## Some SOL type
`sol::Value`, `sol::Array`, `sol::Object` and `sol::String`.

`sol::Array` is the alias of `std::vector<sol::Value>`.

`sol::Object` is the alias of `std::unordered_map<std::string, sol::Value>`.

`sol::String` is the alias of `std::string`.
## Parse SOL
```cpp
...

// Parsing
if (!sol::Parser::fromFile("sample.sol")) {
    // Error ocucrred
    std::cerr << sol::Parser::error() << std::endl;
    return 0;
}

// Get result
Value sample = sol::Parser::result();

// Escape unicode as "\uxxxx"
sol::Parser::outputEscapeUnicode(true);

// Output SOL string with format
std::cout << sol::Parser::toString(v, 4) << std::endl;

...
```
### Get error
`const std::string& sol::Parser::error()`

Returns error message string. If there is no error occurred, the string will be uncertain.
### Get parse result
`Value sol::Parser::result()`

Returns parse result. If fails to parse, the result will be uncertain.
### From file
`bool sol::Parser::fromFile(const std::string& path)`

Returns `true` for success, `false` for error.
### From string
`bool sol::Parser::fromString(const std::string& str)`

Returns `true` for success, `false` for error.
### Set output hint - Escape unicode as "\uxxxx" or not
`void sol::Parser::outputEscapeUnicode(bool b)`

It would be set as `false` initially.
### To file without format
`bool sol::Parser::toFile(const std::string& path, const Value& v)`

Returns `true` for success, `false` for error.
### To string without format
`std::string sol::Parser::toString(const Value& v)`

Returns `true` for success, `false` for error.
### To file with format
`bool sol::Parser::toFile(const std::string& path, const Value& v, size_t n, size_t off = 0)`

n: Whitespace count

off: Initial offset

Returns `true` for success, `false` for error.
### To string with format
`std::string sol::Parser::toString(const Value& v, size_t n, size_t off = 0)`

n: Whitespace count

off: Initial offset

Returns SOL string.
## sol::Value operations
### Construction
It can accept some basic type to construct a sol::Value.
```cpp
...

sol::Value emptyValue;
sol::Value fromAnotherValue(emptyValue);
sol::Value fromArray(sol::Array());
sol::Value fromObject(sol::Object());
sol::Value fromString(sol::String());
sol::Value fromInt64(114514ll);
sol::Value fromDouble(3.1415926l);
sol::Value fromBoolean(true);

...
```
### Assignment
Similar to construction, it can accept some basic type.
### Type check
```cpp
...

sampleValue.isNull();
sampleValue.isArray();
sampleValue.isObject();
sampleValue.isString();

// Get type
sampleValue.type();

...
```
### Index
It can accept an integer or a string as an index. If the type of sol::Value is not an array, but you still use an integer as the index, it would automatically reconstruct itself as an array(AND YOU WILL LOST THE ORIGIN DATA STORAGED IN IT). Same as the string index.
```cpp
...

// As array
sampleValue[114514];

// As object
sampleValue["114514"];

...
```
### Convert
```cpp
...

sampleValue.array();
sampleValue.object();
sampleValue.string();
sampleValue.integer();
sampleValue.real();
sampleValue.boolean();

...
```
## Check
### Check element type
`bool sol::check(const Value& v, const std::string& path, ValueType type)`

path: Just like "a.b.x.ww.z.12.11". If it's a pure number, it would go through like an array, otherwise an object.

Return `true` for success, `false` for fail.
### Check elements
`std::vector<int> sol::check(const Value& v, const std::vector<std::string>& ls)`

ls: Path list, but you should add type name at the end of the path. e.g. `ls = {"a.b:String", "aaa.weqx.zxc:Array"};`

Return the list of result, -1 for invalid path, -2 for invalid type name, 0 for fails, 1 for success. The last element of the list is the total fails count.
