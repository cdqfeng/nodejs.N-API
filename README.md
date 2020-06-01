## 使用步骤

### 1、安装必要nodejs依赖
```
npm i node-gyp -g
```

### 2、设置C++源文件
##### 当源代码已被编写，它必须被编译成二进制 addon.node 文件。 要做到这点，需在项目的顶层创建一个名为 binding.gyp 的文件，它使用一个类似 JSON 的格式来描述模块的构建配置。 该文件会被 node-gyp（一个用于编译 Node.js 插件的工具）使用。
```
{
  "targets": [
    {
      "target_name": "addon",
      "sources": [ "main.cc" ]
    }
  ]
}
```

### 3、调用 node-gyp build 命令生成编译后的 addon.node 的文件。 它会被放进 build/Release/ 目录。

### 4、当构建完成时，二进制插件就可以在 Node.js 中被使用，通过 require() 构建后的 addon.node 模块
```
// main.js
const addon = require('./build/Release/addon');

addon.setParent();
```