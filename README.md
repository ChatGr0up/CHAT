# 1、如何构建chat的cpp微服务

1、先执行config目录下面的env_setup.sh脚本，设置环境
2、执行根目录下面的build.sh脚本进行构建

# 2、如何运行chat的cpp微服务

1、执行build目录中的main

# 3、chat的cpp微服务依赖

| 库名称             | 版本                    | 库的作用                 |
| ------------------ | ----------------------- | ------------------------ |
| protobuf           | 31.0-dev                | 提供序列化协议为gRPC支持 |
| libjsoncpp-dev     | 1.9.5-6build1           | 提供json解析             |
| CMake              | 3.28.3                  | 提供构建支持             |
| build-essential    | 12.10ubuntu1            | 提供基本构建依赖         |
| libmysqlclient-dev | 8.0.41-0ubuntu0.24.04.1 | mysql的cpp客户端代码依赖 |
|                    |                         |                          |
