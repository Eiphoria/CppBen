#include "print_macro.h"


std::string time_stomp() {
	std::time_t timenow = std::chrono::system_clock::to_time_t(std::chrono::system_clock::now());
	std::tm timeinfo;
	localtime_s(&timeinfo, &timenow);

	std::stringstream ss;
	ss << std::put_time(&timeinfo, "%y-%m-%d %OH:%OM:%OS");
	return ss.str();
}