#ifndef FRONT_SERVER_H_
#define FRONT_SERVER_H_

#include <deque>
#include <mutex>
#include <boost/array.hpp>
#include <boost/asio.hpp>

#include "macros.h"
#include "message.h"

class FrontServer {
  public:
    explicit FrontServer(int port);
    ~FrontServer();

    void Run();
    void Stop();

    Message PopMsgQueue();
	
	struct RobotAxisAngles {
		explicit RobotAxisAngles(
			float A1, float A2, float A3, float A4, float A5, float A6)
			: A1(A1), A2(A2), A3(A3), A4(A4), A5(A5), A6(A6) {}
		
		float A1;
		float A2;
		float A3;
		float A4;
		float A5;
		float A6;
	};

	void SetRobotAxisAngles(const RobotAxisAngles& angles) { robot_axis_angles_ = angles; }

  private:
    boost::asio::io_service io_service_;
    boost::asio::ip::tcp::acceptor acceptor_;
    boost::asio::ip::tcp::socket socket_;

    bool stopflag_;

    std::deque<Message> msg_queue_;
    std::mutex msg_queue_mtx_;

	RobotAxisAngles robot_axis_angles_;

	DISALLOW_COPY_AND_ASSIGN(FrontServer);
};

#endif //FRONT_SERVER_H_
