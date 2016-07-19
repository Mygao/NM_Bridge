#ifndef FRONT_SERVER_H_
#define FRONT_SERVER_H_

#include <deque>
#include <mutex>
#include <boost/array.hpp>
#include <boost/asio.hpp>

class FrontServer {
  public:
    explicit FrontServer(int port);
    ~FrontServer();

    void Run();
    void Stop();

    std::string PopMsgQueue();

  private:
    boost::asio::io_service io_service_;
    boost::asio::ip::tcp::acceptor acceptor_;
    boost::asio::ip::tcp::socket socket_;

    bool stopflag_;

    std::deque<std::string> msg_queue_;
    std::mutex msg_queue_mtx_;
};

#endif //FRONT_SERVER_H_
