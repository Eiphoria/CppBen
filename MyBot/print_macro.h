#pragma once
#include <iostream>
#include <chrono>
#include <iomanip>
#include <sstream>

/*
Prints
INFO, path to file, function, line, MSG 
into console
*/
#define PRINT_INFO std::cout << time_stomp() << " [INFO] " << __FILE__ << "(" << __FUNCTION__ << ":" << __LINE__ << ") >> "
/*
Prints
ERROR, path to file, function, line, MSG
into console
*/
#define PRINT_ERROR std::cout << time_stomp() << " [ERROR] " << __FILE__ << "(" << __FUNCTION__ << ":" << __LINE__ << ") >> "
#define LOG(msg, data) log_msg(msg, __FILE__, __LINE__, __FUNCTION__, data)

//Just catch time in call moment and convert to str in pleasure format
std::string time_stomp();