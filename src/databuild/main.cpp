#include <iostream>

#include "util/log.hpp"
#include "databuild/DataBuild.hpp"

int main(int argc, char* argv[]) {
    if (argc <= 1) {
        LogError("Please provide at least one valid databuild file.");
        return 1;
    }

    for (int i = 1; i < argc; i++) {
        std::cout << "---------------------------------" << std::endl;
        bool initSuccess;
        DataBuild::DataBuild dataBuild(argv[1], initSuccess);
        if (!initSuccess) {
            LogError("Building target \"", argv[1], "\"");
            continue;
        }

        if (!dataBuild.Run()) {
            LogError("Failed to preform databuild for target \"", argv[1], "\"");
            continue;
        }

        if (!dataBuild.SaveData()) {
            LogError("Failed to save databuild output for target \"", argv[1], "\"");
            continue;
        }

        LogSuccess("Built target \"", argv[1], "\" -----> \"", Strings_FilenameWithoutExtension(argv[1]), ".zip\"");
    }
}