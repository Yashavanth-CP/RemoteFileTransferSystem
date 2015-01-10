#include<iostream>
#include "Options.h"
#include<string>

size_t Options::_maxPort = 65535;
size_t Options::_minPort = 1;
size_t Options::_dPort = 4956;

Options* Options::create(std::string& n, std::string& ip, size_t p) {

  std::cout << "options create : " << ip << std::endl;
  if(true == Options::valIpAddr(ip)) {
    return new Options(n, ip, p);
  } else {
    std::cout << "Validation failed" << std::endl;
    return NULL;
}

}

Options::Options(std::string& n, std::string& ip, size_t p) : _name(n) {
 
  this->setPort(p);
  this->setName(n);
  this->setIpAddr(ip);

}

std::string Options::getIpAddr() const {
  return this->_ipAddr;
}

void Options::setIpAddr(std::string& ip) {
  this->_ipAddr = ip;
}

std::string Options::getName() const {
  return this->_name;
}

void Options::setName(const std::string& s) {
  if(s.empty()) {
     std::cout<< " Option name is empty " << std::endl;
     return;
   }

  this->_name = s;
}

size_t Options::getMaxPort() const {
  return this->_maxPort;
}

size_t  Options::getMinPort() const {
  return this->_minPort;
}

size_t  Options::getDefaultPort() const {
  return this->_dPort;
}

bool Options::setPort(size_t p) {
  if(p > this->getMaxPort() || p < this->getMinPort()) {
     this->_port = this->getDefaultPort();
     return false;
  }

  this->_port = p;
  return true;
}

size_t Options::getPort() const {
   return this->_port;
}

bool Options::valIpAddr(std::string& s) {

   if (s.empty() || s.length() < 7 || s.length() > 15)
     return false;

   size_t pos = s.find_first_not_of("1234567890.");
   if (pos != std::string::npos)
     return false;

   int dots = 0;
   size_t oldfound = -1;
   size_t found = -1;
   size_t i = 0;
   while (i < s.length())
   {
     found = s.find_first_of(".", found+1);
     if (found != std::string::npos) {
       dots++;
     } else {
       found = s.length();
     }

     if (dots > 3)
       return false;

     if ((found - oldfound) > 4 || (found - oldfound) < 2)
       return false;

     if ((found - oldfound) == 4) {

       if (s.at(oldfound+1) > '2')
         return false;

       if (s.at(oldfound+1) == '2') {
         if (s.at(oldfound+2) > '5')
           return false;

         if (s.at(oldfound+2) == '5' && s.at(oldfound+3) > '5')
           return false;
       }
     }
     oldfound = found;
     i = oldfound;
   }

   if(dots == 3)
      return true;

   return false;
} 

       
