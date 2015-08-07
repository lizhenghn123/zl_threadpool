#include <iostream>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <pthread.h>

#include "ThreadPool.h"  
  
  
class MyTask
{  
public:  
    MyTask(){}
      
    int run(int i, const char* p)  
    {  
        printf("thread[%lu] : (%d, %s)\n", pthread_self(), i, (char*)p);  
        sleep(1);  
        return 0;  
    }  
};  
  
int main()  
{  
    zl::ThreadPool threadPool(10);  
    MyTask taskObj[20];
    for(int i = 0; i < 20; i++)  
    {
        threadPool.addTask(std::bind(&MyTask::run, &taskObj[i], i, "helloworld"));
    }

    while(1)  
    {  
        printf("there are still %d tasks need to process\n", threadPool.size());  
        if (threadPool.size() == 0)
        {  
            threadPool.stop();
            printf("Now I will exit from main\n"); 
            exit(0);   
        }  
        sleep(2);  
    }  

    return 0;
}  
