#include <fstream>
#include "Core.h"

int main(int argc, char* argv[]) {
    Core core;

    if (argc > 1) {
        std::string path = argv[1];
        core.start(path);
    }
    else {
        core.start();
    }

    return 0;
}
