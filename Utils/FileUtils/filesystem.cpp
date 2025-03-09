#include <filesystem>
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
    try {
        std::filesystem::path filePath(fileName);
        return std::filesystem::exists(filePath) && std::filesystem::is_regular_file(filePath);
    } catch (const std::filesystem::filesystem_error& e) {
        // todo : add log
        return false;  // 如果发生错误，返回 false
    }
}
}