#ifndef __SERVER_H
#define __SERVER_H

#include <string>
#include <iostream>
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/enable_shared_from_this.hpp>

using namespace boost::asio;
using ip::tcp;
using std::cout;
using std::endl;

class Server : public boost::enable_shared_from_this {
  tcp::socket sock;
  std::string message="Hello From Server!";
  enum { max_length = 1024 };
  char data[max_length];

public:
    Server(std::string address, int port);
    void Start();
};


#endif // !__SERVER_H