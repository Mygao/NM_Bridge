#include "../include/front_server.h"

#include <algorithm>
#include <iostream>
#include <vector>

#include "../include/simple_xml.h"

using boost::asio::ip::tcp;

namespace {

std::string default_send_data = "<Sen Type=\"ImFree\">\
  <EStr>KRCnexxt - RSI Object ST_ETHERNET</EStr>\
  <AKorr A1=\"0.0\" A2=\"0.0\" A3=\"0.0\" A4=\"0.0\" A5=\"0.0\" A6=\"0.0\" />\
  <Tech T21=\"1.09\" T22=\"2.08\" T23=\"3.07\" T24=\"4.06\" T25=\"5.05\" T26=\"6.04\" T27=\"7.03\" T28=\"8.02\" T29=\"9.01\" T210=\"10.00\" />\
  <DiO>125</DiO>\
  <IPOC>0000000000</IPOC>\
</Sen>";

void Tokenize(const std::string& str,
                  std::vector<std::string>& tokens,
                  const std::string& delimiters = " ")
{
    std::string::size_type lastPos = str.find_first_not_of(delimiters, 0);

    std::string::size_type pos     = str.find_first_of(delimiters, lastPos);

    while (std::string::npos != pos || std::string::npos != lastPos)
    {
        tokens.push_back(str.substr(lastPos, pos - lastPos));
        lastPos = str.find_first_not_of(delimiters, pos);
        pos = str.find_first_of(delimiters, lastPos);
    }
}

}//namespace

FrontServer::FrontServer(int port)
  : acceptor_(io_service_, tcp::endpoint(tcp::v4(), port)),
    socket_(io_service_), stopflag_(false) {

}

FrontServer::~FrontServer() {
    socket_.close();
}

void FrontServer::Run() {
    try {
        while (!stopflag_) {

            std::cout << "Waiting..." << std::endl;
            acceptor_.accept(socket_);
            std::cout << "Accepted." << std::endl;

            for (;;) {
                boost::array<char, 1024> buf;
                boost::system::error_code error;

                size_t len = socket_.read_some(boost::asio::buffer(buf), error);

                if (error == boost::asio::error::eof){
                    socket_.close();
                    break;
                }
                else if (error) {
                    socket_.close();
                    break;//throw boost::system::system_error(error);
                }

                //Parse message and Make XMl and Push queue
                std::string str_buf(buf.begin(), buf.begin() + len);
                str_buf.erase(std::remove(
                                str_buf.begin(), str_buf.end(), ' '),
                                str_buf.end());

                std::vector<std::string> tokens;
                Tokenize(str_buf, tokens, ",");

                copy(tokens.begin(), tokens.end(), std::ostream_iterator<std::string>(std::cout, "\n"));

                SimpleXML xml_send_data(default_send_data);
                std::string send_data = xml_send_data.Node("Sen").Node("AKorr").
                                        ResetAttributes().
                                        AppendAttribute("A1", tokens[1]).
                                        AppendAttribute("A2", tokens[2]).
                                        AppendAttribute("A3", tokens[3]).
                                        AppendAttribute("A4", tokens[4]).
                                        AppendAttribute("A5", tokens[5]).
                                        AppendAttribute("A6", tokens[6]).GetXML();
                {
                    std::lock_guard<std::mutex> lock(msg_queue_mtx_);
                    msg_queue_.push_front(send_data);
                }
                socket_.write_some(boost::asio::buffer(send_data));
            }
        }
    }
    catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
}

void FrontServer::Stop() {
    stopflag_ = true;
}

std::string FrontServer::PopMsgQueue() {
    std::lock_guard<std::mutex> lock(msg_queue_mtx_);
    if (msg_queue_.empty()) {
        return "";
    }

    std::string pop = msg_queue_.back();
    msg_queue_.pop_back();
    return pop;
}
