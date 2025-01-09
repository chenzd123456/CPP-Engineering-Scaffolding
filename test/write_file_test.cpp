// 文件写入功能测试
// 使用Google Test框架和Google Mock进行单元测试
// 主要测试write_file函数的各种场景
#include "sys_mock.h"
#include "test.h"

#include "write_file.h"

// 测试夹具类
// 每个测试用例运行前会创建新的实例
class WriteFileTest : public ::testing::Test {
  protected:
    void SetUp() override {
        // 在每个测试用例运行前设置mock实例
        SystemApiSingleton::setMock(&_system_api_mock);
    }

    void TearDown() override {
        // 在每个测试用例运行后清理mock实例
        SystemApiSingleton::setMock(nullptr);
    }

  protected:
    SystemApiMock _system_api_mock;
};

// 测试成功场景
// 验证文件正常写入的情况
TEST_F(WriteFileTest, SuccessCase) {
    // Setup mock expectations
    EXPECT_CALL(_system_api_mock, open(testing::StrEq("test.txt"),
                                      O_WRONLY | O_CREAT | O_TRUNC, 0644))
        .WillOnce(Return(3));

    EXPECT_CALL(_system_api_mock, write(3, testing::_, 10)).WillOnce(Return(10));

    EXPECT_CALL(_system_api_mock, close(3)).WillOnce(Return(0));

    const char *filename = "test.txt";
    const char *data = "test data";
    size_t data_size = 10;

    // Execute
    int result = write_file(filename, data, data_size);

    // Verify
    EXPECT_EQ(result, 0);
}

// 测试打开文件失败场景
// 验证open系统调用失败时的处理
TEST_F(WriteFileTest, OpenFailure) {
    // Setup mock expectations
    EXPECT_CALL(_system_api_mock,
                open(StrEq("test.txt"), O_WRONLY | O_CREAT | O_TRUNC, 0644))
        .WillOnce(Return(-1));

    const char *filename = "test.txt";
    const char *data = "test data";
    size_t data_size = 10;

    // Execute
    int result = write_file(filename, data, data_size);

    // Verify
    EXPECT_EQ(result, -1);
}

// 测试写入文件失败场景
// 验证write系统调用失败时的处理
TEST_F(WriteFileTest, WriteFailure) {
    // Setup mock expectations
    EXPECT_CALL(_system_api_mock,
                open(StrEq("test.txt"), O_WRONLY | O_CREAT | O_TRUNC, 0644))
        .WillOnce(Return(3));

    EXPECT_CALL(_system_api_mock, write(3, _, 10)).WillOnce(Return(-1));

    EXPECT_CALL(_system_api_mock, close(3)).WillOnce(Return(0));

    const char *filename = "test.txt";
    const char *data = "test data";
    size_t data_size = 10;

    // Execute
    int result = write_file(filename, data, data_size);

    // Verify
    EXPECT_EQ(result, -1);
}
