#pragma once
#include <string>
namespace CHAT::Utils::FileUtils {
class fileSystem {    
    static std::string currentPath(const std::string& filename);
    static bool fileExists(const std::string& fileName);
};
}