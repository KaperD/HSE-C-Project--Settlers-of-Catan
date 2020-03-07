#include <boost/lockfree/queue.hpp>
#include <iostream>
#include <chrono>
#include <thread>
#include <mutex>
 
std::mutex g_lock;
 
void threadFunction()
{
     g_lock.lock();
 
     std::cout << "entered thread " << std::this_thread::get_id() << std::endl;
     std::this_thread::sleep_for(std::chrono::seconds(rand()%3));
     std::cout << "leaving thread " << std::this_thread::get_id() << std::endl;
 
     g_lock.unlock();
}
 
int main()
{
     srand((unsigned int)time(0));
     std::thread t1(threadFunction);
     std::thread t2(threadFunction);
     std::thread t3(threadFunction);
     t1.join();
     t2.join();
     t3.join();
     return 0;
}