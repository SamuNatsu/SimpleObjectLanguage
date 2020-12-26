#include <iostream>

#include "SOL.h"

SOL::Parser parser;
SOL::Object root;

int main() {
    if (!parser.Parse("test.sol")) {
        std::cerr << parser.GetError() << std::endl;
        return 0;
    }
    root = parser.GetRoot();

    // Output server list
    for (auto &server : root["server"].AsArray()) {
        auto svr = server.AsObject();
        std::cout << "name:\t" << svr["name"].AsString() << '\n'
            << "ip:\t" << svr["ip"].AsString() << ':' << svr["port"].AsString() << "\n\n";
    }
    std::cout << std::flush;

    return 0;
}
