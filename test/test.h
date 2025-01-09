#include <fcntl.h>
#include <setjmp.h>
#include <stdarg.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <sys/types.h>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

using ::testing::_;             // 任意参数匹配
using ::testing::Return;        // 模拟返回值
using ::testing::SetArgPointee; // 设置指针参数值
using ::testing::StrEq;         // 字符串相等匹配
