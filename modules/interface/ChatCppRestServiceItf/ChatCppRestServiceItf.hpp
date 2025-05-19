#pragma once

#include "Module/ModuleBase.hpp"

namespace CHAT::Module {
class ChatCppRestServiceItf : public CHAT::Utils::Module::ModuleBase {
public:
    ChatCppRestServiceItf() = default;

    virtual ~ChatCppRestServiceItf() = default;

    std::string name() const override;
};
}