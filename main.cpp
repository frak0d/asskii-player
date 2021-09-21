#include <cerrno>
#include <cmath>
#include <cstdio>
#include <cstring>
#include <string>
#include <format>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <unistd.h>

using namespace std;

#include "pipe.hpp"

uint WIDTH  = 160;
uint HEIGHT = 100;

string QuoteShellArg(const string& arg)
{
#if defined(__linux__) || defined(__unix__) || defined(__APPLE__)
	string ret="'";
	ret.reserve(arg.length()+10); // ¯\_(ツ)_/¯ should avoid realloc in most cases
	for(size_t i=0;i<arg.length();++i)
	{
		if(arg[i]=='\00')
			throw runtime_error("argument contains null bytes, it is impossible to escape null bytes on unix!");
		else if(arg[i]=='\'')
			ret+="'\\''";
		else
			ret += arg[i];
	}
	ret+="'";
	return ret;

#elif defined(_WIN32) || defined(_WIN64)
	// todo
	// the Windows escape rules are *complex* and even differ between system()/.bat invocations and manual cmd invocations
	// maybe try porting https://stackoverflow.com/a/29215357/1067003
	return string("\"") + arg + string("\"");

#else
	#error "Unsupported OS !"
#endif
}

void ClearScreen()
{
	#if defined(__linux__) || defined(__unix__) || defined(__APPLE__)
		system("clear");
	#endif

	#if defined(_WIN32) || defined(_WIN64)
		system("cls");
	#endif
}

struct config
{
	string vid_path;
	bool color = false;
	bool blocks = false;
};

config ArgumentParser(int argc, const char* argv[])
{
	if (argc == 1)
	{
		cout << "Usage :-\n"
			 << "./asski-player [video path] <optional arguments>\n\n"
			 << "--color   -->  Display the Video in 24-bit Colors\n"
			 << "--blocks  -->  Use Filled Blocks instead of Ascii Symbols\n"
			 << "              (Requires --color, --color will be enabled)\n" << endl;
		exit(-1);
	}
	else if (string(argv[1]) == "--color" || string(argv[1]) == "--blocks")
	{
		puts("\033[91;1;3m==> Error : No Video File Specified !\033[m");
		exit(-1);
	}
	config cfg;
	cfg.vid_path = argv[1];
	
	string tok;
	for (int i=2 ; i < argc ; i++)	//argv[0] is exe name, argv[1] is video name
	{
		tok = argv[i];
		if 		(tok == "--color" ) cfg.color  = true;
		else if (tok == "--blocks") cfg.blocks = true;
	}
    return cfg;
}

int main(int argc, const char* argv[])
{
	config cfg = ArgumentParser(argc, argv);

	uint8_t buf[102400/*100KB*/];

	char test_cmd[500];
	snprintf(test_cmd, 500, "ffmpeg -i %s -v quiet -vsync 0 -s %ux%u "
							"-f image2pipe -vcodec rawvideo -pix_fmt rgb24 -",
							cfg.vid_path.c_str(), WIDTH, HEIGHT);
	
	FILE* ffpipe = popen(test_cmd, "r");
	if (!ffpipe)
	{
		printf("\033[91;1;3m==> Unable to Open Pipe, error %d : %s\033[m\n", errno, strerror(errno));
		exit(-1);
	}

	size_t rs = fread(buf, 1, sizeof(buf), ffpipe);
    printf("\n\033[94;1;3m ++++++++ Read %lu Bytes +++++++++ \033[m\n", rs);
    printf("\n\033[96;1m ==> Closing Pipe, %s\033[m\n", strerror(pclose(ffpipe)));
    return 0;
}
