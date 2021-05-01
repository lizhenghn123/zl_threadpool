#include "ThreadPool.h" 
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

namespace zl
{
    ThreadPool::ThreadPool(int threadNum)
    {
        isRunning_ = true;
        threadsNum_ = threadNum;
        createThreads();
    }

    ThreadPool::~ThreadPool()
    {
        stop();
        for(std::deque<Task*>::iterator it = taskQueue_.begin(); it != taskQueue_.end(); ++it)
        {
            delete *it;
        }
        taskQueue_.clear();
    }

    int ThreadPool::createThreads()
    {
        pthread_mutex_init(&mutex_, NULL);
        pthread_cond_init(&condition_, NULL);
        threads_ = (pthread_t*)malloc(sizeof(pthread_t) * threadsNum_);
        for (int i = 0; i < threadsNum_; i++)
        {
            pthread_create(&threads_[i], NULL, threadFunc, this);
        }
        return 0;
    }

    size_t ThreadPool::addTask(Task *task)
    {
        pthread_mutex_lock(&mutex_);
        taskQueue_.push_back(task);
        int size = taskQueue_.size();
        pthread_mutex_unlock(&mutex_);
        pthread_cond_signal(&condition_);
        return size;
    }

    void ThreadPool::stop()
    {
        if (!isRunning_)
        {
            return;
        }

        isRunning_ = false;
        pthread_cond_broadcast(&condition_);

        for (int i = 0; i < threadsNum_; i++)
        {
            pthread_join(threads_[i], NULL);
        }

        free(threads_);
        threads_ = NULL;

        pthread_mutex_destroy(&mutex_);
        pthread_cond_destroy(&condition_);
    }

    int ThreadPool::size()
    {
        pthread_mutex_lock(&mutex_);
        int size = taskQueue_.size();
        pthread_mutex_unlock(&mutex_);
        return size;
    }

    void* ThreadPool::threadFunc(void* arg)
    {
        pthread_t tid = pthread_self();
        ThreadPool* pool = static_cast<ThreadPool*>(arg);
        while (pool->isRunning_)
        {
            while (pool->isRunning_)
            {
                pthread_mutex_lock(&pool->mutex_);
                while (pool->taskQueue_.empty() && pool->isRunning_) {
                    pthread_cond_wait(&pool->condition_, &pool->mutex_);
                }

                if (!pool->isRunning_) {
                    pthread_mutex_unlock(&pool->mutex_);
                    printf("thread %lu will exit\n", tid);
                    break;
                }

                Task* task = pool->taskQueue_.front();
                pool->taskQueue_.pop_front();

                pthread_mutex_unlock(&pool->mutex_);
                task->run();
            }
        }
        return 0;
    }

}
