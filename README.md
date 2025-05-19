![image](https://github.com/user-attachments/assets/57152f2c-f8ee-4853-b017-c5379bd5c2ca)
# 1、如何构建chat的cpp微服务

推荐使用docker构建，避免出现依赖等问题

执行:
docker build --build-arg APP_DIR=$PWD  -t chat-dev-container -f Dockerfile .

# 2、如何编译chat的cpp微服务

执行
./container_build.sh
里面指定了容器镜像名称为chat-dev-container，如果你使用了其他名称，请修改脚本中的名称。
