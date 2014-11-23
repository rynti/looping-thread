// Copyright (c) 2014 Robert Böhm
// 
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
// 
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
// 
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.


#include <thread>
#include <chrono>
#include <iostream>
#include <mutex>

#include "LoopingThread.hpp"


std::mutex stdoutMutex;


// Example usage of LoopingThread with a classic MainThread:
class MainThread : public LoopingThread {
private:
  MainThread();

public:
  virtual ~MainThread();

  static MainThread& getInstance();
  virtual bool tick();
};

MainThread::MainThread() {}
MainThread::~MainThread() {}

MainThread& MainThread::getInstance() {
  // Provide a global instance
  static MainThread instance;
  return instance;
}

bool MainThread::tick() {
  // Do something repeatedly here, e.g. render a new frame, update game objects, ...
  std::this_thread::sleep_for(std::chrono::seconds(1));
  stdoutMutex.lock();
  std::cout << "tick" << std::endl;
  stdoutMutex.unlock();

  // Return false to stop this thread
  return true;
}



void doLongAsyncTask() {
  std::thread longTask([] () {
    stdoutMutex.lock();
    std::cout << "Beginning long task..." << std::endl;
    stdoutMutex.unlock();

    std::this_thread::sleep_for(std::chrono::seconds(2));

    stdoutMutex.lock();
    std::cout << "Long task finished!" << std::endl;
    stdoutMutex.unlock();

    MainThread::getInstance().scheduleCallback([] () {
      stdoutMutex.lock();
      std::cout << "This is called within the main thread!" << std::endl <<
                   "No need to worry about thread safety or " <<
                   "race conditions here" << std::endl;
      stdoutMutex.unlock();
    });
  });
  longTask.detach();
}

int main() {
  doLongAsyncTask();
  MainThread::getInstance().run();
}








