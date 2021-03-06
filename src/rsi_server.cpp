#include "rsi_server.h"

#include <iostream>

#include "message.h"
#include "simple_xml.h"

using boost::asio::ip::udp;

const int kBufferSize = 1024;

std::string default_send_data = "<Sen Type=\"ImFree\">\
<EStr>KRCnexxt - RSI Object ST_ETHERNET</EStr>\
<AKorr A1=\"0.0\" A2=\"0.0\" A3=\"0.0\" A4=\"0.0\" A5=\"0.0\" A6=\"0.0\" />\
<Tech T21=\"1.09\" T22=\"2.08\" T23=\"3.07\" T24=\"4.06\" T25=\"5.05\" T26=\"6.04\" T27=\"7.03\" T28=\"8.02\" T29=\"9.01\" T210=\"10.00\" />\
<DiO>125</DiO>\
<IPOC>0000000000</IPOC>\
</Sen>";

RSIServer::RSIServer(int port)
    : port_(port), front_server_(50050) {
    try {
        socket_ = new udp::socket(io_service_, udp::endpoint(udp::v4(), port));
    }
    catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
}

RSIServer::~RSIServer() {
    front_server_.Stop();
    front_server_thread_->join();
    socket_->close();
    delete socket_;
    socket_ = 0;
}

void RSIServer::Run() {

    boost::array<char, kBufferSize> recv_buf;
    boost::system::error_code ignored_error;

    udp::endpoint remote_endpoint;

    front_server_thread_
        = new boost::thread(boost::bind(&FrontServer::Run, &front_server_));
    std::string last_sent_data = default_send_data;

    try {
        for (;;) {
            boost::system::error_code error;

            recv_buf.assign(0);

            std::size_t transferred_size = socket_->receive_from(
                boost::asio::buffer(recv_buf), remote_endpoint, 0, error);

            if (error && error != boost::asio::error::message_size)
                throw boost::system::system_error(error);

            std::string recv_data(recv_buf.begin(),
                recv_buf.begin() + transferred_size);

			SimpleXML xml_recv_data(recv_data);
            std::string recv_ipoc(xml_recv_data.Node("Rob").Node("IPOC").GetValue());
			xml_recv_data.GetXML();

			//Set Robot's current axis angles to front server.
			//std::cout << xml_recv_data.Node("Rob").Node("AIPos").GetAttribute("A1") << std::endl;
			xml_recv_data.Node("Rob").Node("AIPos");
			front_server_.SetRobotAxisAngles(FrontServer::RobotAxisAngles(
						strtof(xml_recv_data.GetAttribute("A1").c_str(), NULL),
						strtof(xml_recv_data.GetAttribute("A2").c_str(), NULL),
						strtof(xml_recv_data.GetAttribute("A3").c_str(), NULL),
						strtof(xml_recv_data.GetAttribute("A4").c_str(), NULL),
						strtof(xml_recv_data.GetAttribute("A5").c_str(), NULL),
						strtof(xml_recv_data.GetAttribute("A6").c_str(), NULL)
					)
				);
            //Pump message from buffering module
            Message message = front_server_.PopMsgQueue();

			std::string pop_data = message.GetData();

            if (pop_data != "") {
                SimpleXML xml_send_data(pop_data);
                std::string send_data = xml_send_data.Node("Sen")
                                                     .Node("IPOC")
                                                     .SetValue(recv_ipoc)
                                                     .GetXML();
                last_sent_data = send_data;
                socket_->send_to(boost::asio::buffer(send_data),
                                 remote_endpoint, 0, ignored_error);
            }
            else {
                SimpleXML xml_send_data(last_sent_data);
                last_sent_data = xml_send_data.Node("Sen")
                                              .Node("IPOC")
                                              .SetValue(recv_ipoc)
                                              .GetXML();
                socket_->send_to(boost::asio::buffer(last_sent_data),
                                 remote_endpoint, 0, ignored_error);
            }

        }
    }
    catch (std::exception& e) {
        std::cerr << e.what() << std::endl;
    }
}
