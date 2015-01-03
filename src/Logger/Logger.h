#ifndef _LOGGER_H_
#define _LOGGER_H_

#include<fstream>
#include<mutex>
#include<memory>

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

    /* Make all constructors private */
    Logger();
    Logger(const Logger&);
    Logger & operator = (const Logger&);

  public:
    ~Logger();
    std::ofstream getLogStream();
    void setLogFile(std::string);
    std::string getLogFile() const;
    void logWrite(std::string);
    static std::string getDate();
    /* Public  Method to get Instance */
    static std::shared_ptr<Logger> getLogger();
};

#endif
