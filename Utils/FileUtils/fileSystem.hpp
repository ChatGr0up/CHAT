#pragma once
#include <string>
namespace CHAT::Utils::FileUtils {
class fileSystem {
public:        
    static std::string currentPath(const std::string& filename);

    static bool fileExists(const std::string& fileName);

    static bool createDirectory(const std::string& dirPath);

    static bool isDirectoryExists(const std::string& dirPath);

    static bool renameFile(const std::string& srcFile, const std::string& dstFile);

private:
    static bool isPathCorrect(const std::string& dirPath);
};
}