
#include "text_proc.h"
#include "print_macro.h"

bool handle_play_command(const std::string& command) {
	std::string playKeyword = ".play";
	std::size_t playPos = command.find(playKeyword);

	if (playPos != std::string::npos) {
		return true;
	}
	return false;
}

std::string handle_url_in_command(const std::string& command) {
	std::string playKeyword = ".play";
	std::size_t playPos = command.find(playKeyword);

	if (playPos != std::string::npos) {
		std::size_t spacePos = command.find(' ', playPos + playKeyword.length());
		if (spacePos != std::string::npos) {
			std::string videoUrl = command.substr(spacePos + 1);
			PRINT_INFO;
			std::cout << "YouTube video URL: " << videoUrl << std::endl;
			return videoUrl;
		}
		return "NEIN";
	}
	return "NEIN";
}

std::string get_video_link(std::string message_text) {
	if (message_text.size() <= 5)
	{
		return "Error";
	}
	std::string cut_command = message_text.substr(5, message_text.size());
	PRINT_INFO;
	std::cout << "Possible url by cutting command: " << cut_command << std::endl;
	if (!(std::regex_search(cut_command, std::regex("https:\/\/www\.youtube\.com\/")))) {
		PRINT_ERROR;
		std::cout << "Url not contain youtube domen, url is: " << cut_command << std::endl;
		cut_command = "Error";
		return cut_command;
	}
	else if (!(std::regex_search(cut_command, std::regex("v=.+")))) {
		PRINT_ERROR;
		std::cout << "Youtube domen not contain any video_id signature, url is: " << cut_command << std::endl;
		cut_command = "Error";
		return cut_command;
	}
	else {
		PRINT_INFO;
		std::cout << "Regex trigerred, url is: " << cut_command << std::endl;
		return cut_command;

	}

}

std::string validate_ytdl_link(std::string ytdl_link) {
	if (!(std::regex_search(ytdl_link, std::regex("https:\/\/")))) {
		PRINT_ERROR;
		std::cout << "Url not contain https signature, url is: " << ytdl_link << std::endl;
		ytdl_link = "Error";
		return ytdl_link;
	}
	return ytdl_link;
}