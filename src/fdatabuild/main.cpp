#include "util/log.hpp"
#include "fdatabuild/FDataBuild.hpp"

int main(int argc, char* argv[]) {
    if (argc <= 1) {
        LogError("Please provide at least one valid databuild file.");
        return 1;
    }

    for (int i = 1; i < argc; i++) {
        std::cout << "---------------------------------" << std::endl;
        bool initSuccess;
        FDataBuild::FDataBuild dataBuild(argv[i], initSuccess);
        if (!initSuccess) {
            LogError("Building target \"", argv[i], "\"");
            continue;
        }

        if (!dataBuild.Run()) {
            LogError("Failed to preform databuild for target \"", argv[i], "\"");
            continue;
        }

        if (!dataBuild.SaveData()) {
            LogError("Failed to save databuild output for target \"", argv[i], "\"");
            continue;
        }

        LogSuccess("Built target \"", argv[i], "\" -----> \"", Strings_FilenameWithoutExtension(argv[i]), ".zip\"");
    }
}