#include <cstdio>

#include "SOL.h"

SOL::Parser parser;
SOL::Object root;

int main() {
    if (!parser.Parse("test.sol")) {
        puts(parser.GetError().c_str());
        return 0;
    }
    root = parser.GetRoot();
    for (auto &i : root["server"].AsArray())
        printf("name: %s\nip: %s:%s\n\n", i.AsObject()["name"].AsString().c_str(), i.AsObject()["ip"].AsString().c_str(), i.AsObject()["port"].AsString().c_str());
    return 0;
}