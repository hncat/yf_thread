# 个人linux线程封装库
```
语言标准: c++14
cmake版本: 3.5或以上皆可

1. 在CMakeLists.txt同级文件下创建build、libs、bin目录
2. cd build
3. cmake .. （如果想生成测试文件请使用cmake -DUSE_TEST ..)
4. cmake --build .

build: 编译目录存放一堆中间文件
libs: 存放生成的动态库和静态库
bin: 用于存放生成的可执行文件一般是测试的时候才会用到
```
