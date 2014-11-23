# LoopingThread

A LoopingThread can be used to:
- Repetitively do the same task (the *tick*)
- Allow for additional tasks (*callbacks*) to be scheduled for execution

# Use for main threads
The idea is very simple and can be useful in a variety of cases, the most common one being for a main thread. In a typical graphical application you have a *main loop* that is run in the initial thread. The main loop handles a variety of tasks such as rendering a user interface, updating program logic, etc.

This works fine unless you decide to run something asynchronously, for instance loading a file. Executing such a long-lasting task in the main thread would potentially prevent any interaction with the user interface, and the interface feels *unresponsive*. Thus, things like loading files from the disk or fetching data from a remote computer should be done in a secondary thread. However, here is where a problem occurs: When the user interface starts a new thread to - for instance - load a file and passes a callback to be called as soon as the task is done, the callback will run into issues when it tries to modify the user interface to illustrate the successfully loaded file. This is due to the fact that the callback will be run in the secondary thread, although all the user interface handling is done in the main thread.

The solution to this issue is using the LoopingThread. The secondary task can simply schedule the callback to be run from the main thread, and any complications that occured due to thread safety or race conditions in the past will be gone.

# Usage

Have a look at the [main.cpp](main.cpp) file for an example with a MainThread.

# MIT License

Copyright (c) 2014 Robert Böhm

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
THE SOFTWARE.
