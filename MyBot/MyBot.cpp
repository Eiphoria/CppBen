#include "MyBot.h"
#include <dpp/dpp.h>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <cstdio>
#include <ctime>
#include <chrono>
#include <cstdlib>
#include <regex>
#include <vector>
#include <cstdio>
#include <memory>
#include <stdexcept>
#include <array>
#include <cstring>
#include <queue>

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <ogg/ogg.h>
#include <opus/opusfile.h>
#include <Windows.h>

#include "print_macro.h"
#include "text_proc.h"
#include "os_proc.h"
#include "mp3_proc.h"




/* Be sure to place your token in the line below.
 * Follow steps here to get a token:
 * https://dpp.dev/creating-a-bot-application.html
 * When you invite the bot, be sure to invite it with the
 * scopes 'bot' and 'applications.commands', e.g.
 * https://discord.com/oauth2/authorize?client_id=940762342495518720&scope=bot+applications.commands&permissions=139586816064
 */

std::map<dpp::snowflake, std::queue<std::string>> queue_links;
std::map<dpp::snowflake, dpp::snowflake> guild_id_bypass;

int main()
{
	std::string BOT_TOKEN = "";


	const char* env_var_name = "DPP_TOKEN";
	char* env_var_value = nullptr;
	size_t required_size;

	
	if (_dupenv_s(&env_var_value, &required_size, env_var_name) == 0 && env_var_value != nullptr) {
		
		std::cout << "Value of  " << env_var_name << " is: " << env_var_value << std::endl;
		BOT_TOKEN = env_var_value;
		
		free(env_var_value);
	}
	else {
		std::cerr << "Failed to retrieve environment variable " << env_var_name << std::endl;
		return 1;
	}


	dpp::cluster bot(BOT_TOKEN, dpp::i_default_intents | dpp::i_message_content);

	bot.on_log(dpp::utility::cout_logger());


	/* Use the on_message_create event to look for commands */
	bot.on_message_create([&bot](const dpp::message_create_t& event) {
		std::stringstream ss(event.msg.content);
		std::string command;
		ss >> command;

		dpp::snowflake author_id = static_cast<dpp::snowflake>(999705731244761219);
		if (event.msg.author.id == author_id) {
			return;
		}
		//PRINT_INFO;
		//std::cout << event.msg.content << " from author.id " << event.msg.author.id << " with name: " << event.msg.author.username << std::endl;

		if (handle_play_command(command)) {


			dpp::guild* g = dpp::find_guild(event.msg.guild_id);
			//auto id = event.msg.guild_id;
			
			if (!g->connect_member_voice(event.msg.author.id)) {
				event.reply("You don't seem to be in a voice channel!");
				return;
			}

			PRINT_INFO;
			std::cout << "event.msg.content: " << event.msg.content << std::endl;
			std::string url = get_video_link(event.msg.content);
			if (url == "Error") {
				PRINT_INFO;
				std::cout << "URL is not valid: " << url << std::endl;
				return;
			}
			
			PRINT_INFO;
			std::cout << "URL is:" << url << std::endl;

			dpp::voiceconn* v = event.from->get_voice(event.msg.guild_id);
			//if (!v || !v->voiceclient->is_ready() || !v->voiceclient) {
			//	event.reply("there was an issue with getting the voice channel. make sure i'm in a voice channel!");
			//	return;
			//}
			guild_id_bypass[v->channel_id] = event.msg.guild_id;
			std::cout << "v->is_ready(); is:" << v->is_ready() << std::endl;
			
			

			queue_links[v->channel_id].push(url);

			//std::string ytdl_command = "youtube-dl --get-url --format bestaudio";//https://www.youtube.com/watch?v=yUPPmIz2ZAs bajo la luvia
			//std::string command = ytdl_command + url;
			//std::cout << "command is:" << command << std::endl;
			//std::string link = exec(command);
			//link = validate_ytdl_link(link);
			//if (url == "Error") {
			//	PRINT_INFO;
			//	std::cout << "ytdl_link is not valid: " <<  std::endl;
			//	return;
			//}
			//PRINT_INFO;
			//std::cout << "ytdl link output: " << link << std::endl;
			//newline_pos = link.find('\n');
			//if (newline_pos != std::string::npos) {
			//	link.erase(newline_pos, 1);
			//}

			//std::string path_to_ffmpeg = "H:\\src\\vcpkg\\installed\\x64-windows-release\\tools\\ffmpeg\\";
			//std::string first_path_command = "ffmpeg.exe -loglevel quiet -i \"";
			//std::string second_path_command = "\" -acodec pcm_s16le -ar 48000 -ac 2 -f s16le -";
			//std::string commend = path_to_ffmpeg + first_path_command + link + second_path_command;
			//PRINT_INFO;
			//std::cout << "link output: " << std::endl;
			//std::cout << link << std::endl;
			//std::vector<uint8_t> pcmdata = executeFFmpegCommandAndGetAudioData(commend);

			//v->voiceclient->send_audio_raw(reinterpret_cast<uint16_t*>(pcmdata.data()), pcmdata.size());

			//bot.message_create(dpp::message(event.msg.channel_id, "Playing"));


		}

		/* Tell the bot to play the sound file */
		if (command == ".join") {
			dpp::guild* g = dpp::find_guild(event.msg.guild_id);
			if (!g->connect_member_voice(event.msg.author.id)) {
				bot.message_create(dpp::message(event.msg.channel_id, "You don't seem to be on a voice channel! :("));
			}
			bot.message_create(dpp::message(event.msg.channel_id, "Connecting"));
		}

		//if (command == ".pley") {

		//	if (!event.from->is_connected()) {
		//		return;
		//	}
		//	std::string link;

		//	dpp::voiceconn* v = event.from->get_voice(event.msg.guild_id);
		//	dpp::snowflake guild_id = 0; 
		//	dpp::snowflake voicechannel_id = v->channel_id;

		//	auto it = guild_id_bypass.find(voicechannel_id);
		//	if (it != guild_id_bypass.end()) {
		//		guild_id = it->second;
		//		std::cout << "Found Guild ID for Voice channel ID " << voicechannel_id << ": " << guild_id << std::endl;
		//	}
		//	else {

		//		std::cout << "Guild ID " << voicechannel_id << " not found in the map." << std::endl;
		//		return;
		//	}


		//	while (!queue_links[voicechannel_id].empty()) {

		//		std::string url = queue_links[voicechannel_id].front(); 
		//		queue_links[voicechannel_id].pop(); 
		//		PRINT_INFO;
		//		std::cout << "URL is:" << url << std::endl;
		//		size_t newline_pos = url.find('\n');
		//		if (newline_pos != std::string::npos) {
		//			url.erase(newline_pos, 1);
		//		}
		//		std::string ytdl_command = "youtube-dl --get-url --format bestaudio";//https://www.youtube.com/watch?v=yUPPmIz2ZAs bajo la luvia
		//		//queue_links[event.voice_channel_id].pop();
		//		std::string command = ytdl_command + url;
		//		std::cout << "command is:" << command << std::endl;
		//		std::string link = exec(command);
		//		link = validate_ytdl_link(link);
		//		if (link == "Error") {
		//			PRINT_INFO;
		//			std::cout << "ytdl_link is not valid: " << std::endl;
		//			return;
		//		}

		//		PRINT_INFO;
		//		std::cout << "ytdl link output: " << link << std::endl;
		//		newline_pos = link.find('\n');
		//		if (newline_pos != std::string::npos) {
		//			link.erase(newline_pos, 1);
		//		}

		//		std::string path_to_ffmpeg = "H:\\src\\vcpkg\\installed\\x64-windows-release\\tools\\ffmpeg\\";
		//		std::string first_path_command = "ffmpeg.exe -loglevel quiet -i \"";
		//		std::string second_path_command = "\" -acodec pcm_s16le -ar 48000 -ac 2 -f s16le -";
		//		std::string commend = path_to_ffmpeg + first_path_command + link + second_path_command;
		//		PRINT_INFO;
		//		std::cout << "link output: " << std::endl;
		//		std::cout << link << std::endl;
		//		std::vector<uint8_t> pcmdata = executeFFmpegCommandAndGetAudioData(commend);
		//		if (pcmdata.size() < 11520) {
		//			//event.from->disconnect_voice(guild_id);
		//			return;
		//		}
		//		else {
		//			while (v->voiceclient->send_audio_raw(reinterpret_cast<uint16_t*>(pcmdata.data()), pcmdata.size()).is_playing()) {
		//				
		//			};
		//			//while (event.voice_client->is_playing()) {
		//			//
		//			//}
		//		}
		//	}

		//	//std::cout << "No links in the queue for Channel ID: " << event.voice_channel_id << std::endl;
		//	
		//	dpp::snowflake remove_voice_channel_id = v->channel_id;
		//	guild_id_bypass.erase(remove_voice_channel_id);
		//}

		if (command == ".stop") {

			//dpp::voiceconn* v = event.from->get_voice(event.msg.guild_id);
			//if (!v == NULL ) {
		
			//	dpp::snowflake targetKey = v->channel_id;

			
			//	auto it = queue_links.find(targetKey);
			//	if (it != queue_links.end()) {
			//		
			//		queue_links.erase(it);
			//		std::cout << "Queue for Key " << targetKey << " has been removed from the map." << std::endl;
			//	}
			//	else {

			//		std::cout << "Key " << targetKey << " not found in the map." << std::endl;
			//		return;
			//	}
			//}
			if (event.from->is_connected()) {
				dpp::voiceconn* v = event.from->get_voice(event.msg.guild_id);
				if (v == NULL) {
					return;
				}
				dpp::snowflake targetKey = v->channel_id;
				
				auto it = queue_links.find(targetKey);
				if (it != queue_links.end()) {
					queue_links.erase(it);
					std::cout << "Queue for Key " << targetKey << " has been removed from the map." << std::endl;
				}
				else {

					std::cout << "Key " << targetKey << " not found in the map." << std::endl;
					return;
				}

				event.from->disconnect_voice(event.msg.guild_id);
				bot.message_create(dpp::message(event.msg.channel_id, "Disconecting"));
			}
		}
		if (command == ".queue") {
			dpp::voiceconn* v = event.from->get_voice(event.msg.guild_id);

			if (v == NULL) {
				return;
			}
			
			dpp::snowflake targetKey = v->channel_id;

			
			std::string allLinks = "Queue is: \n";

			
			auto it = queue_links.find(targetKey);
			if (it != queue_links.end()) {
				std::queue<std::string> linksQueue = it->second;

				
				while (!linksQueue.empty()) {
					allLinks += linksQueue.front() + "\n"; 
					linksQueue.pop();
				}

			
				std::cout << "Links for Key " << targetKey << ":\n" << allLinks << std::endl;
				bot.message_create(dpp::message(event.msg.channel_id, allLinks));
			}
			else {

				std::cout << "Key " << targetKey << " not found in the map." << std::endl;
				return;
			}
			
		}

		});

	bot.on_voice_client_speaking([&bot](const dpp::voice_client_speaking_t& event) {
		std::cout << "voice_client->is_playing(SUS): " << event.voice_client->is_playing() << std::endl;
		});
	bot.on_voice_state_update([&bot](const dpp::voice_state_update_t& event) {
		std::cout << "raw_event: "  << event.state.user_id << " " << event.raw_event << std::endl;
		});

	bot.on_voice_ready([&bot](const dpp::voice_ready_t& event) {

		std::cout << "voice ready: " << std::endl;
		//std::string link;

		dpp::snowflake guild_id = 0;
		dpp::snowflake voicechannel_id = event.voice_channel_id;

		auto it = guild_id_bypass.find(voicechannel_id);
		if (it != guild_id_bypass.end()) {
			guild_id = it->second;
			std::cout << "Found Guild ID for Voice channel ID " << voicechannel_id << ": " << guild_id << std::endl;
		}
		else {

			std::cout << "Guild ID " << voicechannel_id << " not found in the map." << std::endl;
			return;
		}


		/*while(!queue_links[voicechannel_id].empty()) {*/

			std::string url = queue_links[voicechannel_id].front(); 
			queue_links[voicechannel_id].pop(); 
			PRINT_INFO;
			std::cout << "URL is:" << url << std::endl;
			size_t newline_pos = url.find('\n');
			if (newline_pos != std::string::npos) {
				url.erase(newline_pos, 1);
			}
			std::string ytdl_command = "youtube-dl --get-url --format bestaudio";//https://www.youtube.com/watch?v=yUPPmIz2ZAs bajo la luvia
			//queue_links[event.voice_channel_id].pop();
			std::string command = ytdl_command + url;
			std::cout << "command is:" << command << std::endl;
			std::string link = exec(command);
			link = validate_ytdl_link(link);
			if (link == "Error") {
				PRINT_INFO;
				std::cout << "ytdl_link is not valid: " << std::endl;
				return;
			}

			PRINT_INFO;
			std::cout << "ytdl link output: " << link << std::endl;
			newline_pos = link.find('\n');
			if (newline_pos != std::string::npos) {
				link.erase(newline_pos, 1);
			}

			std::string path_to_ffmpeg = "H:\\src\\vcpkg\\installed\\x64-windows-release\\tools\\ffmpeg\\";
			path_to_ffmpeg = "";
			std::string first_path_command = "ffmpeg.exe -loglevel quiet -i \"";
			std::string second_path_command = "\" -acodec pcm_s16le -ar 48000 -ac 2 -f s16le -";
			std::string commend = path_to_ffmpeg + first_path_command + link + second_path_command;
			PRINT_INFO;
			std::cout << "link output: " << std::endl;
			std::cout << link << std::endl;
			std::vector<uint8_t> pcmdata = executeFFmpegCommandAndGetAudioData(commend);
			if (pcmdata.size() < 11520) {
				//event.from->disconnect_voice(guild_id);
				return;
			}
			else {
				event.voice_client->send_audio_raw(reinterpret_cast<uint16_t*>(pcmdata.data()), pcmdata.size());
				//while (event.voice_client->is_playing()) {
				//
				//}
			}
		//}
		
		//std::cout << "No links in the queue for Channel ID: " << event.voice_channel_id << std::endl;
	
		dpp::snowflake remove_voice_channel_id = event.voice_channel_id;
		guild_id_bypass.erase(remove_voice_channel_id);



		

		//event.from->disconnect_voice(event.from.)
		

		////bot.message_create(dpp::message(event.msg.channel_id, "Playing"));
		//std::cout << "Possible link values(on_voice_ready): " << std::endl;
		//std::cout << "event.voice_channel_id: " << event.voice_channel_id << " | event.voice_client->channel_id: " << event.voice_client->channel_id << " | event.voice_client->sessionid: " << event.voice_client->sessionid << " | event.voice_client->token: " << event.voice_client->token << std::endl;
		
		});

	bot.on_slashcommand([&bot](const dpp::slashcommand_t& event) {
		if (event.command.get_command_name() == "ping") {
			event.reply("PONG!");
		}
		if (event.command.get_command_name() == "play") {

			dpp::guild* g = dpp::find_guild(event.command.guild_id);
			if (!g->connect_member_voice(event.command.get_issuing_user().id)) {
				event.reply("You don't seem to be in a voice channel!");
				return;
			}
			//bot.message_create(dpp::message(event.command.channel_id, "Connecting"));
			//event.reply("Connecting");

			//std::string command_line = std::get<std::string>(event.get_parameter("value"));
			//dpp::message = event.command.get_resolved_message();

			PRINT_INFO;
			std::cout << "event.command.msg.content: " << event.command.msg.content << std::endl;
			dpp::voiceconn* v = event.from->get_voice(event.command.guild_id);
			std::cout << "Possible link values(on_slashcommand): " << std::endl;
			std::cout << "v->channel_id: " << v->channel_id << " | v->token: " << v->token << " | v->session_id: " << v->session_id << std::endl;

			//v->channel_id;
			//v->token;
			//v->session_id;
			//if (!v || !v->voiceclient || !v->voiceclient->is_ready()) {
			//	event.reply("there was an issue with getting the voice channel. make sure i'm in a voice channel!");
			//	return;
			//}

			//std::string path_to_ffmpeg = "H:\\src\\vcpkg\\installed\\x64-windows-release\\tools\\ffmpeg\\";
			//std::string first_path_command = "ffmpeg.exe -loglevel quiet -i \"";
			//std::string link = "https://rr7---sn-4upjvh-qv3z.googlevideo.com/videoplayback?expire=1694132541&ei=3RT6ZOjSEMzryAWckaioCQ&ip=5.153.158.50&id=o-AEDMwFTgKflurIx7P32wKRKmMWGu65HjFp6y4qC9kZFR&itag=251&source=youtube&requiressl=yes&mh=CD&mm=31%2C29&mn=sn-4upjvh-qv3z%2Csn-3c27sn7k&ms=au%2Crdu&mv=m&mvi=7&pcm2cms=yes&pl=24&initcwndbps=791250&spc=UWF9f-Ar4E0Vn3hgjmkkzzqYn83g9txT5-m4mrm5og&vprv=1&svpuc=1&mime=audio%2Fwebm&gir=yes&clen=1010638&dur=69.021&lmt=1651795539759624&mt=1694110657&fvip=17&keepalive=yes&fexp=24007246&c=ANDROID&txp=5318224&sparams=expire%2Cei%2Cip%2Cid%2Citag%2Csource%2Crequiressl%2Cspc%2Cvprv%2Csvpuc%2Cmime%2Cgir%2Cclen%2Cdur%2Clmt&sig=AOq0QJ8wRQIhAJQjd0J_IOcMXBOglzvvxxLpJMq9ltjM2_655Jj3-4wvAiAFzdv_bjYpTzRdvzE_5S7qO7F_FqXCJLcv6OMHW1959g%3D%3D&lsparams=mh%2Cmm%2Cmn%2Cms%2Cmv%2Cmvi%2Cpcm2cms%2Cpl%2Cinitcwndbps&lsig=AG3C_xAwRQIhAJKNUzJR0-PlNvANQsgDYGdnGjtwTFwf1YfAF5m-1YmEAiBU01goNQJOT2SgdGtozFth__htzUw1aHLisHK8vzWHEw%3D%3D";
			//size_t newline_pos = link.find('\n');
			//if (newline_pos != std::string::npos) {
			//	link.erase(newline_pos, 1);
			//}
			//std::string second_path_command = "\" -acodec pcm_s16le -ar 48000 -ac 2 -f s16le -";
			//std::string commend = path_to_ffmpeg + first_path_command + link + second_path_command;
			//PRINT_INFO;
			//std::cout << "link output: " << std::endl;
			//std::cout << link << std::endl;
			//std::vector<uint8_t> pcmdata = executeFFmpegCommandAndGetAudioData(commend);

			//v->voiceclient->send_audio_raw(reinterpret_cast<uint16_t*>(pcmdata.data()), pcmdata.size());

			////bot.message_create(dpp::message(event.msg.channel_id, "Playing"));
			//event.reply("Playing");
		}
		if (event.command.get_command_name() == "join") {
			dpp::guild* g = dpp::find_guild(event.command.guild_id);
			if (!g->connect_member_voice(event.command.get_issuing_user().id)) {
				event.reply("You don't seem to be in a voice channel!");
				return;
			}
			//bot.message_create(dpp::message(event.command.channel_id, "Connecting"));
			event.reply("Connecting");
		}
		if (event.command.get_command_name() == "stop") {
			event.from->disconnect_voice(event.command.guild_id);
			//bot.message_create(dpp::message(event.command.channel_id, "Disconecting"));
			event.reply("Disconecting");
		}
	});


	bot.on_ready([&bot](const dpp::ready_t& event) {
		if (dpp::run_once<struct register_bot_commands>()) {
			/* create a new command. */
			dpp::slashcommand pingcommand("ping", "ping ME!", bot.me.id);

			dpp::slashcommand joincommand("join", "joins your voice channel.", bot.me.id);

			dpp::slashcommand playcommand("play", "plays an YT url", bot.me.id);

			//const std::string description = "Youtube url";
			//playcommand.add_option(dpp::command_option(dpp::co_string, description, 0));

			dpp::slashcommand stopcommand("stop", "stop playing an YT url", bot.me.id);

			bot.global_bulk_command_create({ pingcommand, joincommand, playcommand, stopcommand });
		}
		});

	/* Start bot */
	bot.start(dpp::st_wait);
	return 0;
}
