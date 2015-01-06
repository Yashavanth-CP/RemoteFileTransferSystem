#ifndef _LOGGER_H_
#define _LOGGER_H_

#include<fstream>
#include<mutex>
#include<memory>
#include<vector>
#include<algorithm>
#include<chrono>
#include<thread>
#include "../Queue/FifoQueue.h"

#define MAX_MESSAGES 40
/* This class provides logging capability to a file.
   It uses singleton design pattren, so that only one
   instance of the Logger is present throughout the
   application.
*/

class Logger {

  private:
    /* Attributes */
    std::string logFilePath;
    std::ofstream logFileStream;
    static std::shared_ptr<Logger> _pInstance;
    static std::once_flag once;
    static std::atomic<bool> stopSignal;

    /* Make all constructors private */
    Logger();
    Logger(const Logger&);
    Logger & operator = (const Logger&);
    /* an array of all subscribers queues */
    std::vector<FifoQueue<std::string, MAX_MESSAGES>*> subscribers;
    /* Mutex for array */
    std::mutex logLock;
    std::atomic<size_t> sreqs;
    std::atomic<size_t> usreqs;
  public:
    ~Logger();
    std::ofstream getLogStream();
    void setLogFile(std::string);
    std::string getLogFile() const;
    void logWrite(std::string*);
    static std::string getDate();
    /* Public  Method to get Instance */
    static std::shared_ptr<Logger> getLogger();
    FifoQueue<std::string, MAX_MESSAGES>* subScribe();
    void unSubscribe(FifoQueue<std::string, MAX_MESSAGES>*);
    void start();
    void stop();
};

#endif
