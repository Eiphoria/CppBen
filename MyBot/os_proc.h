#pragma once
#include <iostream>
#include <cstdio>
#include <vector>
#include <string.h>

#include "print_macro.h"


//Do:
//Receive command ,execute command in win_OS and then reads output into variable which returns
//For:
//Exctract audio source link from youtube url
std::string exec(std::string cmd);

//Do:
//Receive ffmpeg command with audio source link, convert into PCM and writes byte into variable which returns
//For:
//Convert audio source link from youtube url into PCM bytes for send_audio_raw() function
std::vector<uint8_t> executeFFmpegCommandAndGetAudioData(const std::string& command);