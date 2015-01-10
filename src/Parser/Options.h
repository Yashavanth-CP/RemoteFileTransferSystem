#ifndef _OPTIONS_H_
#define _OPTIONS_H_

#include<iostream>

class Options {
	
  private:
    std::string _name;
    size_t _port;
    std::string _ipAddr;
    Options(std::string&, std::string&, size_t);
    void setIpAddr(std::string& s);   
 
  public:

    static size_t _maxPort;
    static size_t _minPort;
    static size_t _dPort;
    static Options* create(std::string&, std::string&, size_t);
    std::string getName() const;
    void setName(const std::string&);
    static bool valIpAddr(std::string&);
    size_t getMaxPort() const;
    size_t getMinPort() const;
    size_t getDefaultPort() const;
    bool setPort(size_t);
    size_t getPort() const;
    std::string getIpAddr() const;
    //bool getIPfromName(const std::string &);
};

#endif
