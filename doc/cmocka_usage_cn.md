# CMocka 单元测试使用指南

## 1. 简介

CMocka 是一个功能强大的 C 语言单元测试框架，支持 mock 函数和异常测试。本文介绍如何在项目中配置和使用 CMocka。

## 2. 环境配置

### 2.1 添加依赖

在 `CMakeLists.txt` 中添加：

```cmake
find_package(cmocka REQUIRED)
target_link_libraries(your_target_name cmocka)
```

### 2.2 包含头文件

在测试文件中包含：

```c
#include <cmocka.h>
```

## 3. 编写测试用例

### 3.1 基本结构

```c
#include <cmocka.h>

static void test_case(void **state) {
    // 测试代码
    assert_int_equal(actual, expected);
}

int main(void) {
    const struct CMUnitTest tests[] = {
        cmocka_unit_test(test_case),
    };
    return cmocka_run_group_tests(tests, NULL, NULL);
}
```

## 4. Mock 函数

### 4.1 创建 Mock 函数

使用 `__wrap_` 前缀创建 mock 函数：

```c
int __wrap_function_name(parameters) {
    // 验证参数
    check_expected(param);

    // 返回 mock 值
    return mock_type(int);
}
```

### 4.2 设置期望值

```c
expect_value(__wrap_function_name, param_name, expected_value);
will_return(__wrap_function_name, return_value);
```

## 5. 常用断言

| 断言函数                          | 描述       |
| --------------------------------- | ---------- |
| `assert_true(expression)`         | 表达式为真 |
| `assert_false(expression)`        | 表达式为假 |
| `assert_int_equal(a, b)`          | 整数相等   |
| `assert_string_equal(a, b)`       | 字符串相等 |
| `assert_memory_equal(a, b, size)` | 内存相等   |

## 6. 编译与运行测试

### 6.1 CMake 配置

在项目的 `CMakeLists.txt` 中添加测试配置：

```cmake
# 查找 CMocka 包
find_package(cmocka REQUIRED)

# 创建测试可执行文件
add_executable(test_program
    test/test_case1.c
    test/test_case2.c
    src/implementation.c
)

# 链接 CMocka 库
target_link_libraries(test_program cmocka)

# 添加 wrap 链接选项
target_link_options(test_program PRIVATE
    -Wl,--wrap=function_to_mock1
    -Wl,--wrap=function_to_mock2
)

# 安装测试程序
install(TARGETS test_program DESTINATION bin)
```

### 6.2 编译测试

使用以下命令编译测试程序：
```bash
mkdir build
cd build
cmake ..
make
```

### 6.3 运行测试

编译完成后，测试程序位于 `build/bin/` 目录下：
```bash
./bin/test_program
```

### 6.4 自动化测试

可以使用 CTest 集成测试：
```cmake
enable_testing()
add_test(NAME unit_tests COMMAND test_program)
```

然后通过以下命令运行：
```bash
ctest --output-on-failure
```

## 7. 示例

参考 `test/write_file_test.c` 中的实现：

- Mock 系统调用 (open, write, close)
- 参数验证
- 返回值控制
- 测试用例组织

## 8. 最佳实践

1. 每个测试用例应独立
2. 使用 setup/teardown 函数管理测试环境
3. 为每个测试添加详细注释
4. 保持测试代码简洁
5. 定期运行测试

## 9. 高级用法

### 9.1 多模块 Mock 管理

当需要处理多个模块的 mock 时，可以采用以下策略：

1. **模块化 Mock 实现**
   为每个模块创建单独的 mock 文件：

   ```c
   // mock_module_a.c
   int __wrap_module_a_function() {
       return mock_type(int);
   }

   // mock_module_b.c
   int __wrap_module_b_function() {
       return mock_type(int);
   }
   ```

2. **运行时切换实现**

   使用函数指针实现运行时切换：

   ```c
   // 定义函数指针类型
   typedef int (*module_a_func_t)(void);

   // 真实实现
   int real_module_a_function() {
       // 真实实现代码
   }

   // Mock 实现
   int mock_module_a_function() {
       return mock_type(int);
   }

   // 当前使用的实现
   module_a_func_t current_module_a_impl = real_module_a_function;

   // 公共接口
   int module_a_function() {
       return current_module_a_impl();
   }

   // 切换实现
   void set_module_a_implementation(module_a_func_t impl) {
       current_module_a_impl = impl;
   }
   ```

   使用示例：
   ```c
   // 测试用例中
   void test_case(void **state) {
       // 切换到 mock 实现
       set_module_a_implementation(mock_module_a_function);
       
       // 设置期望值
       will_return(mock_module_a_function, 42);
       
       // 测试代码
       assert_int_equal(module_a_function(), 42);
       
       // 恢复真实实现
       set_module_a_implementation(real_module_a_function);
   }
   ```

   注意事项：
   - 在多线程环境中需要添加线程同步机制
   - 确保在测试完成后恢复原始实现
   - 可以使用 setup/teardown 函数自动管理实现切换

3. **状态管理**
   使用全局变量或测试上下文管理 mock 状态：

   ```c
   struct mock_state {
       int module_a_ready;
       int module_b_ready;
   };

   static void setup_mocks(void **state) {
       struct mock_state *s = *state;
       s->module_a_ready = 1;
       s->module_b_ready = 0;
   }
   ```

4. **最佳实践**
   - 为每个模块创建单独的 mock 文件
   - 使用清晰的命名规范区分 mock 和真实实现
   - 在测试用例中明确说明使用的 mock 模块
   - 使用 setup/teardown 函数管理 mock 状态

## 10. 模块级别测试指南

### 10.1 模块设计与测试策略

1. **模块接口设计**
   - 定义清晰的模块接口
   - 使用头文件声明公共接口
   - 保持接口稳定

   ```c
   // module_a.h
   #ifndef MODULE_A_H
   #define MODULE_A_H

   // 初始化模块
   int module_a_init(void);

   // 处理数据
   int module_a_process(int input);

   // 清理资源
   void module_a_cleanup(void);

   #endif
   ```

2. **测试用例组织**
   - 为每个模块创建单独的测试文件
   - 按功能组织测试用例
   - 使用 setup/teardown 管理测试环境

   ```c
   // test_module_a.c
   #include "module_a.h"
   #include <cmocka.h>

   static void test_module_a_init(void **state) {
       assert_int_equal(module_a_init(), 0);
   }

   static void test_module_a_process(void **state) {
       module_a_init();
       assert_int_equal(module_a_process(10), 100);
       module_a_cleanup();
   }

   int main(void) {
       const struct CMUnitTest tests[] = {
           cmocka_unit_test(test_module_a_init),
           cmocka_unit_test(test_module_a_process),
       };
       return cmocka_run_group_tests(tests, NULL, NULL);
   }
   ```

3. **依赖管理**
   - 使用 mock 隔离模块依赖
   - 定义清晰的模块边界
   - 测试模块间的交互

   ```c
   // mock_module_b.c
   int __wrap_module_b_function(int input) {
       check_expected(input);
       return mock_type(int);
   }
   ```

4. **测试覆盖率**
   - 使用 gcov 分析测试覆盖率
   - 确保覆盖所有接口函数
   - 关注边界条件和错误处理

   ```bash
   gcc -fprofile-arcs -ftest-coverage -o test_program test_module_a.c module_a.c
   ./test_program
   gcov module_a.c
   ```

5. **集成测试**
   - 测试模块间的交互
   - 验证接口兼容性
   - 检查资源管理

   ```c
   // test_integration.c
   void test_module_a_b_integration(void **state) {
       module_a_init();
       module_b_init();
       
       // 测试模块交互
       assert_int_equal(module_a_process(module_b_get_data()), 0);
       
       module_b_cleanup();
       module_a_cleanup();
   }
   ```

### 10.2 最佳实践

1. 保持模块接口简单
2. 为每个模块维护独立的测试套件
3. 使用自动化测试工具
4. 定期审查测试覆盖率
5. 在 CI/CD 中集成模块测试

### 10.3 常见问题

#### 10.3.1 链接错误

确保正确链接 cmocka 库：

```cmake
target_link_libraries(your_target_name cmocka)
```

### 9.2 Mock 函数未生效

检查：

1. 是否正确使用 `__wrap_` 前缀
2. 是否在编译时添加了 `-Wl,--wrap=function_name` 参数

### 9.3 测试失败

检查：

1. 期望值设置是否正确
2. Mock 返回值是否正确
3. 断言条件是否合理
