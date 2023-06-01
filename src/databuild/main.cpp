#include <iostream>

#include "databuild/DataBuild.hpp"

int main(int argc, char* argv[]) {
    if (argc <= 1) {
        std::cout << "ERROR: Please provide at least one valid databuild file." << std::endl;
        return 1;
    }

    for (int i = 1; i < argc; i++) {
        bool initSuccess;
        DataBuild dataBuild(argv[i], initSuccess);
        if (!initSuccess) continue;

        if (!dataBuild.Run()) {
            std::cout << "ERROR: Failed to preform databuild for target \"" << argv[i] << "\"" << std::endl;
            continue;
        }

        if (!dataBuild.SaveData()) {
            std::cout << "ERROR: Failed to save databuild output for target \"" << argv[i] << "\"" << std::endl;
            continue;
        }

        std::cout << "SUCCESS: Built target \"" << argv[i] << "\" -----> \"" << FilenameWithoutExtension(argv[i]) << ".zip\"" << std::endl;
    }
}