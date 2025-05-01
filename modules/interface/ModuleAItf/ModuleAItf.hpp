#include "Module/ModuleBase.hpp"
#include "mysql/mysql.h"

namespace CHAT::Module {
class ModuleAItf : public CHAT::Utils::Module::ModuleBase {
public:
    ModuleAItf() = default;
    ~ModuleAItf() = default;
    std::string name() const override;
    virtual void processResult(MYSQL_RES* result) = 0; 
};
}
