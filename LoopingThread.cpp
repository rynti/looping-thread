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


#include "LoopingThread.hpp"


LoopingThread::LoopingThread() : thread(nullptr), scheduledCallbacks() {

}

LoopingThread::~LoopingThread() {
  if (this->thread) {
    delete this->thread;
  }
}

void LoopingThread::runCallbacks() {
  this->callbacksMutex.lock();
  if (this->scheduledCallbacks.size() > 0) {
    std::vector<std::function<void()>> currentCallbacks = std::move(this->scheduledCallbacks);
    this->scheduledCallbacks.clear();
    this->callbacksMutex.unlock();
    for (auto callback : currentCallbacks) {
      callback();
    }
  } else {
    this->callbacksMutex.unlock();
  }
}

void LoopingThread::run() {
  for (;;) {
    // This is to allow for new callbacks to be scheduled from within a callback
    this->runCallbacks();

    // Run the tick
    if (!this->tick()) {
      break;
    }
  }

  // Run pending callbacks, this might result in an infinite loop if there are more
  // callbacks scheduled from within scheduled callbacks
  this->callbacksMutex.lock();
  while (this->scheduledCallbacks.size() > 0) {
    this->callbacksMutex.unlock();
    this->runCallbacks();
    this->callbacksMutex.lock();
  }
  this->callbacksMutex.unlock();
}

void LoopingThread::scheduleCallback(std::function<void()> callback) {
  this->callbacksMutex.lock();
  this->scheduledCallbacks.push_back(callback);
  this->callbacksMutex.unlock();
}

void LoopingThread::start() {
  if (!this->thread) {
    this->thread = new std::thread(&LoopingThread::run, this);
  }
}

void LoopingThread::join() {
  if (this->thread && this->thread->joinable()) {
    this->thread->join();
  }
}
