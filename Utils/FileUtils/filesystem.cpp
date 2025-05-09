#include <filesystem>
#include <cctype>
#include <algorithm>
#include "fileSystem.hpp"
namespace CHAT::Utils::FileUtils {
std::string fileSystem::currentPath(const std::string& filename) 
{
    try {
        return std::filesystem::current_path().string();
    } catch (const std::exception& e) {
        //  todo : add log
        return "";
    }
}
bool fileSystem::fileExists(const std::string& fileName) {
    if (!isPathCorrect(fileName)) {
        return false;
    }
    try {
        std::filesystem::path filePath(fileName);
        return std::filesystem::exists(filePath) && std::filesystem::is_regular_file(filePath);
    } catch (const std::filesystem::filesystem_error& e) {
        // todo : add log
        return false;  // 如果发生错误，返回 false
    }
}

bool fileSystem::createDirectory(const std::string &dirPath)
{
    if (!isPathCorrect(dirPath)) {
        return false;
    }
    if (isDirectoryExists(dirPath)) {
        return true;
    }
    try {
        return std::filesystem::create_directories(dirPath);
    } catch (const std::filesystem::filesystem_error& e) {
        // todo: add log, e.g., std::cerr << e.what() << std::endl;
        return false;
    } catch (...) {
        return false;
    }
}

bool fileSystem::isDirectoryExists(const std::string& dirPath)
{
    if (!isPathCorrect(dirPath)) {
        return false;
    }
    try {
        std::filesystem::path path(dirPath);
        return std::filesystem::exists(path) && std::filesystem::is_directory(path);
    } catch (const std::filesystem::filesystem_error& e) {
        // todo: add log, e.g., std::cerr << e.what() << std::endl;
        return false;
    } catch (...) {
        return false;
    }
}

bool fileSystem::renameFile(const std::string &srcFile, const std::string &dstFile)
{
    if (!isPathCorrect(srcFile) || !isPathCorrect(dstFile)) {
        return false;
    }
    try {
        std::filesystem::rename(srcFile, dstFile);
    } catch (...) {
        return false;
    }
    return true;
}

bool fileSystem::isPathCorrect(const std::string& dirPath)
{
    if (dirPath.empty()) {
        // log
        return false;
    }
    if (dirPath.size() > 1024) {
        return false;
    }
    if (dirPath.find("\\") != std::string::npos) {
        return false;
    }
    if (std::all_of(dirPath.begin(), dirPath.end(), [](unsigned char c) { 
        return std::isspace(c); 
    })) {
        return false;
    }
    if (dirPath.find_first_of("*?|<>\"") != std::string::npos) {
        return false;
    }
    return true;
}
}