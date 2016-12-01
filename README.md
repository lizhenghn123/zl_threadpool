# zl_threadpool : 三个版本的线程池实现

[![Build Status](https://travis-ci.org/lizhenghn123/zl_threadpool.svg?branch=master)](https://api.travis-ci.org/lizhenghn123/zl_threadpool.svg?branch=master)

**Linux平台下C++(C++98、C++03、C++11)实现的线程池**

- ThreadPoolCpp98

        最古老的做法，只使用了C++98语言规范，采用**面向对象的思路**，每一个任务都是一个子类对象；

- ThreadPoolCpp03
 
        较新做法，使用C++03语言规范，还有C++0x（特指std::function + std::bind），与上面不同的是采用
        **基于对象的思路**，每一个任务都是一个std::function对象，std::function，std::bind真是好；

- ThreadPoolCpp11

        最新做法，**完全采用C++11**技术，比如std::thread, mutex, condition_variable, atomic组件，
        还有lambda技巧，packaged_task， future等等；