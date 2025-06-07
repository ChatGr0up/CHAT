#include <string>

namespace CHAT::Module::RestDefine {
enum class RequestType {
    UserLogin,
    UserLogout,
    UserRegister,
    UserInfoUpdate,
    UserInfoFetch,
    UserPasswordChange,
    UserPasswordReset,
    UserFriendAdd,
    UserFriendRemove,
    UserFriendListFetch,
    UserGroupCreate,
    UserGroupJoin,
    UserGroupLeave,
    UserGroupListFetch,
    UserGroupMessageSend,
    UserGroupMessageFetch    
};
}