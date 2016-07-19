#include "../include/frontend_server.h"

#include <iostream>
#include <string>

FrontendServer::FrontendServer(int port)
  : context_(1), socket_(context_, ZMQ_STREAM), stopflag_(false) {
    char port_buf[7] = "";
    sprintf(port_buf,"%d", port);
    std::string addr = "tcp://*:" + std::string(port_buf);
    std::cout << "On FrontendServer Constructed, address is : " << addr << std::endl;
    socket_.bind(addr.c_str());
}

FrontendServer::~FrontendServer() {
    socket_.close();
    context_.close();
}

void FrontendServer::Run() {
    while (!stopflag_) {
        zmq::message_t request;

        socket_.recv (&request);
        std::cout << (char*)request.data() << std::endl;

        std::string to_sent("sent");
        zmq::message_t reply (to_sent.size());
        memcpy (reply.data(), to_sent.data(), to_sent.size());

        int res = socket_.send (to_sent.data(), to_sent.size(),ZMQ_SNDMORE);
        if (res != 0) {
            std::cout << "Sent" << res << std::endl;
        }
        else {
            std::cout << "Fail" << std::endl;
        }

        socket_.send((void*)0, 0, ZMQ_SNDMORE);
    }
}

void FrontendServer::Stop() {
    stopflag_ = true;
}
