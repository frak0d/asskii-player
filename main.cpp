#include <cerrno>
#include <cmath>
#include <cstdio>
#include <cstring>
#include <string>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <unistd.h>

using namespace std;

uint WIDTH  = 60;
uint HEIGHT = 40;

char shade_list[] = "$@B%8&WM#*oahkbdpqwmZO0QLCJUYXzcvunxrjft/|()1{}[]?-_+~<>i!lI;:,\"^`'. ";

string QuoteShellArg(const string& arg)
{
#if defined(__linux__) || defined(__unix__) || defined(__APPLE__)
	string ret="'";
	ret.reserve(arg.length()+10); // ¯\_(ツ)_/¯ should avoid realloc in most cases
	for(size_t i=0;i<arg.length();++i)
	{
		if(arg[i]=='\0')
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
	uint8_t shades = 32;
	bool color = false;
	char block = false;
};

config ArgumentParser(int argc, const char* argv[])
{
	if (argc == 1)
	{
		cout << "Usage :-\n"
			 << "./asski-player [video path] <optional arguments>\n\n"
			 << "--color   -->  Display the Video in 24-bit Colors\n"
			 << "--block   -->  Specify the character to use as pixel in Color Mode\n"
			 << "               (it will be ignored when not using color)\n" << endl;
		exit(-1);
	}
	else if (string(argv[1]) == "--color" || string(argv[1]) == "--block")
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
		if 		(tok == "--color") cfg.color = true;
		else if (tok == "--block") cfg.block = *argv[i+1];
	}
    return cfg;
}

int main(int argc, const char* argv[])
{
	config cfg = ArgumentParser(argc, argv);

	int pos = 0;
	size_t rs;
	float sf1 = 256/cfg.shades;
	float sf2 = sizeof(shade_list) / cfg.shades;
	uint8_t brightness = 0;
	
	uint8_t buf[WIDTH*3*HEIGHT];	// Save one image in buffer
	
	char test_cmd[500];
	snprintf(test_cmd, 500, "ffmpeg -i %s -v quiet -vsync 1 -s %ux%u "
							"-f image2pipe -vcodec rawvideo -pix_fmt rgb24 -",
							cfg.vid_path.c_str(), WIDTH, HEIGHT);
	
	FILE* ffpipe = popen(test_cmd, "r");
	if (!ffpipe)
	{
		printf("\033[91;1;3m==> Unable to Open Pipe, error %d : %s\033[m\n", errno, strerror(errno));
		exit(-1);
	}

	do {
		rs = fread(buf, 1, sizeof(buf), ffpipe);
    	//printf("\n\033[94;1;3m ++++++++ Read %lu Bytes +++++++++ \033[m\n", rs);
    	for (int y=0 ; y < HEIGHT ; ++y)
    	{
    		for (int x=0 ; x < WIDTH ; ++x)
    		{
    			++pos;
    			if (!cfg.color)
    			{
    				brightness = (buf[3*pos-2]/sf1 + buf[3*pos-1]/sf1 + buf[3*pos]/sf1) / 3;
    				printf(" %c", shade_list[(int)(brightness*sf2)]);
    			}
    			else
    			{
    				puts("Color Mode Not Yet Supported !!");
    				exit(-2);
    			}
    		}
    		puts("");	// Newline
    	}
    	pos = 0;
    	ClearScreen();
	} while (rs == sizeof(buf));
    
    printf("\n\033[96;1m ==> Closing Pipe, %s\033[m\n", strerror(pclose(ffpipe)));
    return 0;
}
