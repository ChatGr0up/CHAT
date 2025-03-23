#include "mysql/mysql.h"

namespace CHAT::Utils::DataBaseManager {
class DBThreadPool {
public:
    DBThreadPool& instance();

    DBThreadPool(const DBThreadPool&) = delete;

    DBThreadPool& operator=(const DBThreadPool&) = delete;

    DBThreadPool& operator=(DBThreadPool&&) = delete;

    DBThreadPool& operator=(const DBThreadPool&) = delete;

    DBThreadPool(DBThreadPool&&) = delete;

    ~DBThreadPool() = default;

private:
    DBThreadPool() = default;

};    
}