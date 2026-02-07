# PenFramework

### 简介

这是一个简单的游戏开发框架，包含引擎核心，编辑器以及其他的开发套件。

### 注意

这只是作为游戏项目Undefined Containment的附属引擎开源，不保证长期更新。

### 下载与部署

###### 从源代码库编译

1. 从仓库拷贝原始代码
```sh
git clone https://github.com/PenNineCat/PenFramework.git
```
2. 定向到代码目录
```sh
cd PenFramework
```
3. 通过cmake编译该项目
```sh
cmake -B build -S . -DCMAKE_TOOLCHAIN_FILE=[path/to/vcpkg]/scripts/buildsystems/vcpkg.cmake
cmake --build build --config Release
```

cmake vcpkg集成会自动下载与安装[vcpkg.json](https://github.com/PenNineCat/PenFramework/blob/master/vcpkg.json)中涉及到的vcpkg第三方依赖项。

如果您使用Visual Studio等集成IDE，可以直接使用集成IDE打开项目目录，进行编译。

### 快速开始
您可以使用以下代码来将框架整合到您的项目中
```cmake
find_package(PenFramework REQUIRED 
	COMPONENTS
		PenEngine
		PenEngineExample 
)
add_executable(${PROJECT_NAME} Main.cpp)
target_link_libraries(${PROJECT_NAME} 
	PenFramework::PenEngine
	PenFramework::PenEngineExample
)
```
以下是一份示例代码
```cpp
Main.cpp

#include "PenFramework/Code/CoreApplication.h"
#include "PenFramework/Example/Example1/Code/CoreApplication.h"

int main()
{
// 这是实际您应该使用的内容
//	PenEngine::CoreApplication app;
//	return app.Exec();

//  示例项目
    PenEngine::Example1::CoreApplication app;
	return app.Exec();
}
```

### 平台支持
- Windows 10/11 x64

### 图形API支持
- DirectX 11/12

### 依赖版本，组件与库
#### C++ 依赖
1. C++标准 23 或更高

#### 

#### 工具链
1. cmake 3.10.0 或更高  
2. vcpkg 2025 或更高

#### 依赖库

###### Vcpkg 库
1. boost 1.90.0 或更高  
2. spdlog 1.16.0#1 或更高  
3. zlib 1.3.1 或更高  
4. freetype 2.13.3 或更高  
5. imgui 1.91.9 或更高  
详情参见[vcpkg.json](https://github.com/PenNineCat/PenFramework/blob/master/vcpkg.json)
###### 其他第三方库
1. SteamSDK 1.63
2. Wwise 2024.1.6.8842
详情参见[ThiryParty.md](https://github.com/PenNineCat/PenFramework/blob/master/ThiryParty/ThiryParty.md)

### 文件目录

```
Filetree 
├── LICENSE.txt
├── README.md
├── UpdateLog.txt 更新日志
├── /Code/ 项目核心代码
│    ├── /Engine/ 核心引擎
│    ├── /Editor/ 编辑器
│    ├── /CodeAnalyze/ 效率分析
│    ├── /CodeUnitTest/	单元测试  
├── /Doc/ Doxgen生成的API文档
├── /Example/ 一些示例项目
├── vcpkg.json vcpkg第三方库安装列表项
├── /ThiryParty/ 无法通过vcpkg安装的其他第三方库
│    ├── ThiryParty.md 其他第三方库cmake项
```

### 联系方式与社区
- 🐧QQ群：1079650584
- 📧邮箱：amingzhuo@163.com/PenNineCat@stu.ynu.com

### 参与与贡献

目前该项目由于性质特殊，暂不接受PR，不过仍然欢迎
- Bug报告与功能建议
- 文档改进
- 测试反馈  
您可以通过提交issue，或者通过上述联系方式反馈这些内容。

### 版权信息

Copyright 2025 PenNineCat 

该项目使用Mozilla Public License Version 2.0，详情参见[LICENSE.txt](https://github.com/PenNineCat/PenFramework/blob/master/LICENSE.txt)
