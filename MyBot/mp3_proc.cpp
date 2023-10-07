#include "mp3_proc.h"

std::vector<uint8_t> mp3_to_pcm(const char* file) {

	std::vector<uint8_t> pcmdata;

	mpg123_init();

	int err = 0;
	unsigned char* buffer;
	size_t buffer_size, done;
	int channels, encoding;
	long rate;

	/* Note it is important to force the frequency to 48000 for Discord compatibility */
	mpg123_handle* mh = mpg123_new(NULL, &err);
	mpg123_param(mh, MPG123_FORCE_RATE, 48000, 48000.0);

	/* Decode entire file into a vector. You could do this on the fly, but if you do that
	* you may get timing issues if your CPU is busy at the time and you are streaming to
	* a lot of channels/guilds.
	*/
	buffer_size = mpg123_outblock(mh);
	buffer = new unsigned char[buffer_size];

	/* Note: In a real world bot, this should have some error logging */
	mpg123_open(mh, file);
	mpg123_getformat(mh, &rate, &channels, &encoding);

	unsigned int counter = 0;
	for (int totalBytes = 0; mpg123_read(mh, buffer, buffer_size, &done) == MPG123_OK; ) {
		for (auto i = 0; i < buffer_size; i++) {
			pcmdata.push_back(buffer[i]);
		}
		counter += buffer_size;
		totalBytes += done;
	}
	delete buffer;
	mpg123_close(mh);
	mpg123_delete(mh);

	return pcmdata;
}