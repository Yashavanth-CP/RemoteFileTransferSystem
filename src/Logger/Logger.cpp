#include<iostream>
#include<ctime>
#include<stdlib.h>
#include "../Queue/FifoQueue.h"
#include "Logger.h"

std::shared_ptr<Logger> Logger::_pInstance = NULL;
std::once_flag Logger::once;
std::atomic<bool> Logger::stopSignal;

Logger::Logger() {

  if(logFilePath.empty()) {
    logFilePath = "/home/Documents/log.txt";
  }

  std::string lpath = getLogFile();
  try {
    if(lpath.empty()) {
      std::cout <<" Inside First try " << std::endl;
      throw "No Logfile Path specified ";
    } else {
      std::cout << "log file" << lpath << std::endl;
      this->logFileStream.open(this->logFilePath.c_str(), std::ios::app);
      if(!this->logFileStream.is_open()) {
        throw "Unable to open logfile . Please check permissions";
      }
    }
  }
  catch(const char* s) {
    std::cout << "Error: " << s << std::endl;
    exit(1);
  }
}

Logger::~Logger() {

  if(_pInstance != NULL) {
    _pInstance->logFileStream.close();
    _pInstance = NULL;
  }
 //TODO clear all messages in all queues and deallocate all queues.
}

std::shared_ptr<Logger> Logger::getLogger() {

  std::string path = "/home/Documents/log.txt";
  std::call_once( Logger::once, [] () {
                  Logger::_pInstance.reset( new Logger()); });
  return Logger::_pInstance;
}

std::string Logger::getLogFile() const {
  return this->logFilePath;
}

void Logger::setLogFile(std::string s) {
  if(!s.empty()){
    this->logFilePath = s;
  }
}

void Logger::logWrite(std::string* s) {
  if(!s->empty()) {
    this->logFileStream << Logger::getDate() << *s << '\n';
    this->logFileStream.flush();
  }
}

std::string Logger::getDate() {

  time_t now = time(0);
  char* tm = ctime(&now);
  std::string tme(tm);
  tme.erase(tme.length()-1);
  return tme;
}

FifoQueue<std::string, MAX_MESSAGES>* Logger::subScribe() {
  FifoQueue<std::string, MAX_MESSAGES> * toAdd = new FifoQueue<std::string, MAX_MESSAGES>();
  this->sreqs++;
  std::lock_guard<std::mutex> guard(this->logLock);
  this->subscribers.push_back(toAdd);
  this->sreqs--;
  return toAdd;
}

void Logger::unSubscribe(FifoQueue<std::string, MAX_MESSAGES> *toDel) {
  std::cout << "unSubscribe called here " << std::endl;
  if (toDel == NULL)
    return;

  this->usreqs++;
  std::lock_guard<std::mutex> guard(this->logLock);
  std::vector<FifoQueue<std::string, MAX_MESSAGES>*>::iterator it;
  it = std::find(this->subscribers.begin(), this->subscribers.end(), toDel);
  this->usreqs--;
  if(it == this->subscribers.end()) {
    return;
  } else {
    std::string* s;
    while ((s = (*it)->pop())!= NULL ) {
          this->logWrite(s);
          delete s;
    }
    delete *it;
    this->subscribers.erase(it);
  }
}

void Logger::stop() {
  std::cout<<"Stop invoked  " << std::endl;
  this->stopSignal.store(true);
}

void Logger::start() {

  this->stopSignal.store(false);
  std::vector<FifoQueue<std::string, MAX_MESSAGES>*>::iterator it;
  std::string* s;
  std::cout<< "Logger::Start called " << std::endl;
  while(!this->stopSignal.load()) {
    this->logLock.lock();
    for(it = this->subscribers.begin(); it != this->subscribers.end(); it++) {
      s = (*it)->pop();
      if(s != NULL) {
        this->logWrite(s);
        delete s;
      }
    }

    this->logLock.unlock();
    if(this->sreqs > 0 || this->usreqs > 0) {
      std::cout <<" Handling the sub and unsub requests " << std::endl;
      std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }

    std::this_thread::sleep_for(std::chrono::milliseconds(20));

  }

  std::cout << "start to clear up " << std::endl;
    /* delete everything before joining main thread */
  this->logLock.lock();
  for(it = this->subscribers.begin(); it != this->subscribers.end(); ++it) {
    while((s = (*it)->pop()) != NULL ) {
      this->logWrite(s);
      delete s;
    }
    delete *it;
  }
  this->logLock.unlock();
  return;
}

