#ifndef MESSAGE_H_
#define MESSAGE_H_

#include <chrono>
#include <string>

class Message {
public:
	explicit Message(const std::string& data) : 
		data_(data), 
		timestamp_ms_(
			std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now().time_since_epoch())
		)
	{};

	std::string GetData() { return data_; }

private:
	std::string data_;
	std::chrono::microseconds timestamp_ms_;
};

#endif //MESSAGE_H_