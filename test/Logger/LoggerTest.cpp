#include<iostream>
#include"../Queue/FifoQueue.h"
#include "Logger.h"
#include<thread>
#include<mutex>
#include<chrono>
#define MAX  10

std::atomic<bool> lgrstatus;
void produce(int i, FifoQueue<std::string, MAX_MESSAGES>* q, std::shared_ptr<Logger> lgr) {
  int n = 0;
  std::string id = std::to_string(i);
  id.append("==");
  while(n < ((i+1) * 300)) {
    std::string num = std::to_string(n);
    std::string* s1 = new std::string("Producer");
    //std::cout << "Producer allocated  " << s1 << std::endl;
    s1->append("==");
    s1->append(id);
    s1->append(num);
    std::this_thread::sleep_for (std::chrono::milliseconds(50));
    if( false != q->push(s1)) {
      std::cout << " Pushed element " << *s1 << std::endl;
    } else {
      std::cout << "******** UNABLE TO PUSH ********* " << s1 << std::endl;
      delete s1;
    }
    n++;
  }
  lgr->unSubscribe(q);
  return;
}

int main() {

  std::shared_ptr<Logger> lgr = Logger::getLogger();
  std::thread logger = std::thread(&Logger::start, lgr);
  std::thread t[MAX];
  FifoQueue<std::string, MAX_MESSAGES> * q[MAX];
  for(int i = 0; i < MAX ; i++) {
    q[i] = lgr->subScribe();
    std::cout << "Subscribed : " << q[i] << std::endl;
    std::cout << "Subscribing thread with id : " << i << std::endl;
    t[i] =  std::thread(produce,i, q[i], lgr);
    std::this_thread::sleep_for (std::chrono::milliseconds(10000));
  }

  for(int i = 0; i < MAX ; i++) {
    t[i].join();
    std::cout << "Unsubscribing : " << q[i] << std::endl;
  }
  lgr->stop();
  logger.join();
  return 0;
}
