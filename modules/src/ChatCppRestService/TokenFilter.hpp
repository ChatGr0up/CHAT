#include "drogon/HttpFilter.h"

namespace CHAT::Utils::Module::RestFrame {
class TokenFilter : public drogon::HttpFilter<TokenFilter> {
public:
    void doFilter(const drogon::HttpRequestPtr &req,
                  drogon::FilterCallback &&fcb,
                  drogon::FilterChainCallback &&fcc) override {
        auto token = req->getHeader("Authorization");
        if (token.empty() || !isValidToken(token)) {
            auto resp = drogon::HttpResponse::newHttpResponse();
            resp->setStatusCode(drogon::k401Unauthorized);
            resp->setBody("Unauthorized");
            fcb(resp);
        } else {
            fcc(); // 继续请求
        }
    }

    bool isValidToken(const std::string &token) {
        // 这里可以添加实际的令牌验证逻辑
        // 例如，检查令牌是否在数据库中存在或是否有效
        return token == "valid_token"; // 示例：仅当令牌为"valid_token"时才通过验证
    }
};
}