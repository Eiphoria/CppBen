#pragma once
#include <iostream>
#include <regex>
#include <string.h>


//check if message contain ".play" command
bool handle_play_command(const std::string& command);
//check if message contain ".play" command and tries to catch URL
std::string handle_url_in_command(const std::string& command);
//Exactly detect youtube url by regex expressions
std::string get_video_link(std::string message_text);
//Detect https signature in ytdl link
std::string validate_ytdl_link(std::string ytdl_link);