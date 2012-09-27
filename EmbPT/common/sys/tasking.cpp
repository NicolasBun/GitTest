// ======================================================================== //
// Copyright 2009-2011 Intel Corporation                                    //
//                                                                          //
// Licensed under the Apache License, Version 2.0 (the "License");          //
// you may not use this file except in compliance with the License.         //
// You may obtain a copy of the License at                                  //
//                                                                          //
//     http://www.apache.org/licenses/LICENSE-2.0                           //
//                                                                          //
// Unless required by applicable law or agreed to in writing, software      //
// distributed under the License is distributed on an "AS IS" BASIS,        //
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied. //
// See the License for the specific language governing permissions and      //
// limitations under the License.                                           //
// ======================================================================== //

#include "tasking.h"
#include "sysinfo.h"
#include <xmmintrin.h>

#if defined(__WIN32__)
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

namespace embree
{
  TaskScheduler::TaskScheduler (size_t numThreads) : numTasks(0), activeTasks(0)
  {
    terminateThreads = false;
    for (size_t i=0; i<numThreads; i++)
      threads.push_back(createThread((thread_func)threadFunction,new Thread(i,this),4*1024*1024,int(i)));
  }

  TaskScheduler::~TaskScheduler()
  {
    if (threads.size() == 0) return;
    terminateThreads = true;
    taskMutex.lock(); 
    taskCondition.broadcast(); 
    taskMutex.unlock();
    for (size_t i=0; i<threads.size(); i++) join(threads[i]);
    threads.clear();
    terminateThreads = false;
  }

  void TaskScheduler::addTask(Task::runFunction run, void* runData, size_t elts, Task::completeFunction complete, void* completeData)
  {
    Task* task = new Task(run,runData,elts,complete,completeData);
    activeTasks++;
    
    Lock<MutexSys> lock(taskMutex);
    if (numTasks >= MAX_TASKS) throw std::runtime_error("too many tasks generated");
    tasks[numTasks++] = task;
    taskCondition.broadcast();
  }

  void TaskScheduler::go() 
  {
    finishMutex.lock();
    while (activeTasks != 0) finishCondition.wait(finishMutex);
    finishMutex.unlock();
  }

  void TaskScheduler::threadFunction(Thread* thread)
  {
    try
    {
      /* get thread ID */
      TaskScheduler* This = thread->scheduler;
      size_t tid = thread->tid;
      delete thread;

      /* flush to zero and no denormals */
      _mm_setcsr(_mm_getcsr() | /*FTZ:*/ (1<<15) | /*DAZ:*/ (1<<6));

      while (true)
      {
        This->taskMutex.lock();
        while (!This->numTasks && !This->terminateThreads) This->taskCondition.wait(This->taskMutex);
        if (This->terminateThreads) { This->taskMutex.unlock(); break; }
        Task* task = This->tasks[int32(This->numTasks)-1];
        size_t elt = --task->started;
        if (elt == 0) This->numTasks--;
        This->taskMutex.unlock();
        
        /* run the task */
        if (task->run)
          task->run(tid,task->runData,elt);
        
        /* complete the task */
        if (--task->completed == 0) {
          if (task->complete) task->complete(tid,task->completeData);
          delete task;
          if (--This->activeTasks == 0) {
            This->finishMutex.lock();
            This->finishCondition.broadcast();
            This->finishMutex.unlock();
          }
        }
      }
    }
    catch (const std::exception& e) {
      std::cout << "Error: " << e.what() << std::endl;
      exit(1);
    }
  }

  void TaskScheduler::init(int numThreads) 
  {
    if (numThreads < 0) scheduler = new TaskScheduler(getNumberOfLogicalThreads());
    else scheduler = new TaskScheduler(numThreads);
  }

  void TaskScheduler::cleanup()
  {
    if (scheduler) {
      delete scheduler; 
      scheduler = NULL;
    }
  }

  TaskScheduler* scheduler;
}

