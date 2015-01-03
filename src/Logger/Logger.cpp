
#include<iostream>
#include<ctime>
#include<stdlib.h>

#include "Logger.h"

std::shared_ptr<Logger> Logger::_pInstance = NULL;
std::once_flag Logger::once;

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

void Logger::logWrite(std::string s) {
  if(!s.empty()) {
    this->logFileStream << Logger::getDate() << "\n";
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

