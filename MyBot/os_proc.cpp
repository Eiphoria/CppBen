#include "os_proc.h"




std::string exec(std::string cmd) {
	char buffer[256];
	std::string result = "";
	FILE* pipe = _popen(cmd.c_str(), "r");
	if (!pipe) throw  std::runtime_error("popen() failed!");
	try {
		while (fgets(buffer, sizeof buffer, pipe) != NULL) {
			result += buffer;
		}
	}
	catch (...) {
		_pclose(pipe);
		throw;
	}
	_pclose(pipe);
	return result;
}

std::vector<uint8_t> executeFFmpegCommandAndGetAudioData(const std::string& command) {
	char buffer[11520];
	std::vector<uint8_t> pcmdata;

	
	FILE* pipe = _popen(command.c_str(), "rb");
	if (!pipe) {
		throw std::runtime_error("Unable to open pipe for command execution.");
	}

	while (true) {
		
		size_t bytesRead = fread(buffer, 1, sizeof(buffer), pipe);

		//PRINT_INFO;
		//std::cout << "Bytes read: " << bytesRead << std::endl;

		if (bytesRead != 11520) {
			break; 
		}

		
		for (size_t i = 0; i < bytesRead; ++i) {
			pcmdata.push_back(static_cast<uint8_t>(buffer[i]));
		}
	}

	_pclose(pipe);
	return pcmdata;
}
