#include<iostream>
#include"FifoQueue.h"
#include<thread>
#include<mutex>
#include<chrono>
#define MAX  10
FifoQueue<std::string, 25> queue[MAX];

void produce(int i) {
  int n = 0;
  std::string id = std::to_string(i);
  id.append("==");
  while(n < 250) {
    std::string num = std::to_string(n);
    std::string* s1 = new std::string("Producer");
    s1->append("==");
    s1->append(id);
    s1->append(num);
    std::this_thread::sleep_for (std::chrono::milliseconds((i+1)*30));
    if(false != queue[i].push(s1)) {
      std::cout << " Pushed element " << *s1 << std::endl;
    } else {
      std::cout << "******** UNABLE TO PUSH ********* " << s1 << std::endl;
      delete s1;
    }
    n++;
  }
  return;
}

void consume() {
  int n = 0;
  int empty = 1;
  std::string* s;
  while(true) {
    for(int q = 0; q < MAX; q++) {
    s = queue[q].pop();;
    if(s != NULL) {
      std::cout << "Popped element : " << *s  << std::endl;
      delete s;
      if(q == MAX-1) {
        std::cout << "empty" << empty << std::endl;
        empty++;
      }
    }
    if(empty == 250) {
      return;
    }
  }

  if(empty >= MAX-1) {
    n++;
  }
  std::this_thread::sleep_for (std::chrono::milliseconds(20));
  }

  return;
}

int main() {

  std::thread t[MAX];
  for(int i = 0; i < MAX ; i++) {
    t[i] =  std::thread(produce,i);
  }
  std::thread c(consume);

  for (int i = 0; i < MAX ; i++) {
    t[i].join();
  }
  c.join();

  return 0;
}



