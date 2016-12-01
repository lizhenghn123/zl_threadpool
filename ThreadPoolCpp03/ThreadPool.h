#pragma once  
#include <deque>  
#include <pthread.h>
#include <functional>      // for std::function, std::bind

// 使用C++03/C++0x 语言规范实现的线程池： 基于对象做法，每一个job都是一个function对象
namespace zl
{
    class ThreadPool
    {
    public:
        typedef std::function<void()> Task;

    public:
        ThreadPool(int threadNum = 10);
        ~ThreadPool();

    public:
        size_t addTask(const Task& task);
        void   stop();
        int    size();
        Task take();

    private:
        int createThreads();
        static void* threadFunc(void * threadData);

    private:
        ThreadPool& operator=(const ThreadPool&);
        ThreadPool(const ThreadPool&);

    private:
        volatile  bool              isRunning_;
        int                         threadsNum_;
        pthread_t*                  threads_;

        std::deque<Task>            taskQueue_;
        pthread_mutex_t             mutex_;
        pthread_cond_t              condition_;
    };
}  
