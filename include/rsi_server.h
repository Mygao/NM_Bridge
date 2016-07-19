#ifndef RSI_SERVER_H_
#define RSI_SERVER_H_

#include <boost/array.hpp>
#include <boost/asio.hpp>
#include <boost/thread.hpp>

#include "front_server.h"

class RSIServer {
  public:
    explicit RSIServer(int port);
    virtual ~RSIServer();

    void Run();

  private:
    int port_;

    boost::asio::ip::udp::socket* socket_;
    boost::asio::io_service io_service_;
    boost::thread* frontend_server_thread_;

    FrontServer frontend_server_;

};

#endif //RSI_SERVER_H_
