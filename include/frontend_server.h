#ifndef FRONTEND_SERVER_H_
#define FRONTEND_SERVER_H_

#include <cppzmq-master/zmq.hpp>

class FrontendServer {
  public:
    explicit FrontendServer(int port);
    ~FrontendServer();

    void Run();
    void Stop();

  private:
    zmq::context_t context_;
    zmq::socket_t socket_;

    bool stopflag_;

};

#endif //FRONTEND_SERVER_H_
