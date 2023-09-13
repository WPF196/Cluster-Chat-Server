# 第三方库
这里的第三方库主要是包含了一个 `Json` 库，用以传输数据的`序列化`和`反序列化`

-------------------
### 使用方式
```
#include "json.hpp"
using json = nlohmann::json;
```
### 具体格式
```
msg_type: 消息类型
from: 谁发来的
to: 发给谁的
msg: 发送的内容
```
具体使用请看测试案例 test.cpp