#pragma once

#include <type_traits>
#include <filesystem>

namespace CHAT::Utils::FileUtils {
template<typename StreamType> 
class FileStream {
public:
    FileStream(const std::string& path, std::ios::openmode mode) {
        static_assert(
            std::is_same<StreamType, std::ifstream>::value ||
            std::is_same<StreamType, std::ofstream>::value ||
            std::is_same<StreamType, std::fstream>::value,
            "StreamType must be ifstream, ofstream, or fstream"
        );
        stream.open(path, mode);
        if (!stream.is_open() || !stream.good()) {
            throw std::runtime_error("Failed to open file: " + path);
        }
    }

    ~FileStream() {
        if (stream.is_open()) {
            stream.close();
        }
    }

    FileStream(const FileStream&) = delete;

    FileStream& operator=(const FileStream&) = delete;

    FileStream(FileStream&&) = default;

    FileStream& operator=(FileStream&&) = default;

    StreamType& get() {
        return stream;
    }

    bool isGood() const {
        return stream.good();
    }

    template <typename T>
    FileStream& operator<<(const T& data) {
        stream << data;
        return *this;
    }

    template <typename T>
    FileStream& operator>>(T& data) {
        stream >> data;
        return *this;
    }

    template<typename T = StreamType>
    typename std::enable_if<std::is_base_of<std::ostream, T>::value, void>::type
    flush() {
        stream.flush();
    }

    void close() {
        if (stream.is_open()) {
            stream.close();
        }
    }

    bool open(const std::string& path, std::ios::openmode mode) {
        if (stream.is_open()) {
            return true;
        }
        stream.open(path, mode);
        if (!stream.is_open() || !stream.good()) {
            return false;
        }
        return true;
    }

private:
    StreamType stream;    
};
}