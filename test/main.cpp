#include "test.h"

// 测试程序入口
int main(int argc, char **argv) {
    // 初始化Google Test框架
    ::testing::InitGoogleTest(&argc, argv);
    
    // 运行所有Google Test测试用例
    return RUN_ALL_TESTS();
}
