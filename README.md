![image](https://github.com/user-attachments/assets/57152f2c-f8ee-4853-b017-c5379bd5c2ca)
# 1、如何构建chat的cpp微服务

推荐使用docker构建，避免出现依赖等问题

执行:
docker build --build-arg APP_DIR=$PWD  -t chat-dev-container -f Dockerfile .

# 2、如何编译chat的cpp微服务

执行
./container_build.sh
里面指定了容器镜像名称为chat-dev-container，如果你使用了其他名称，请修改脚本中的名称。

# 3、chat的cpp微服务依赖

| 库名称             | 版本                    | 库的作用                 |
| ------------------ | ----------------------- | ------------------------ |
| protobuf           | 31.0-dev                | 提供序列化协议为gRPC支持 |
| libjsoncpp-dev     | 1.9.5-6build1           | 提供json解析             |
| CMake              | 3.28.3                  | 提供构建支持             |
| build-essential    | 12.10ubuntu1            | 提供基本构建依赖         |
| libmysqlclient-dev | 8.0.41-0ubuntu0.24.04.1 | mysql的cpp客户端代码依赖 |
|                    |                         |                          |
