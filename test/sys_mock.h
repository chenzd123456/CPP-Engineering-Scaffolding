#ifndef ___system_api_mockH__
#define ___system_api_mockH__

#include "test.h"

// 系统调用模拟类
// 用于模拟open、write、close系统调用的行为
class SystemApiMock {
  public:
    MOCK_METHOD3(open, int(const char *pathname, int flags, mode_t mode));
    MOCK_METHOD3(write, ssize_t(int fd, const void *buf, size_t count));
    MOCK_METHOD1(close, int(int fd));
};

class SystemApiSingleton {
  public:
    static SystemApiMock& instance();
    static void setMock(SystemApiMock* mock);
    static SystemApiMock* getMock();

  private:
    static SystemApiMock* instancePtr;
};

// 全局mock实例指针
// 在测试用例中会被设置为当前测试的mock对象

// 系统调用包装函数实现
// 这些函数会调用mock对象的方法
// 声明原始系统调用函数
#ifdef __cplusplus
extern "C" {
#endif
int __real_open(const char *pathname, int flags, mode_t mode);
int __wrap_open(const char *pathname, int flags, mode_t mode);

ssize_t __real_write(int fd, const void *buf, size_t count);
ssize_t __wrap_write(int fd, const void *buf, size_t count);

int __real_close(int fd);
int __wrap_close(int fd);

#ifdef __cplusplus
}
#endif
#endif // ___system_api_mockH__
