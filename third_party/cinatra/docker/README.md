# 如何使用

## cinatra需要的依赖项：

1. C++17 编译器 (gcc 7.2, clang 4.0, Visual Studio 2017 update 15.5,或者更高的版本)
2. 开发使用了Asio网络组件，需要使用Boost.Asio, Boost.System。如果不想依赖Boost，使用独立版Asio即可.

## 使用docker进行开发部署，省去部分烦恼。

```
git clone https://github.com/qicosmos/cinatra.git
cd cinatra/docker
docker build  -t cinatra_env .
docker run -it cinatra_env

```

## 使用
cinatra是header-only的，直接引用头文件既可。


