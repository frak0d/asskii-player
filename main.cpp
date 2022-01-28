#include <cerrno>
#include <cstdio>
#include <cstring>
#include <string>
#include <cstdint>
#include <cstdlib>
#include <csignal>

using namespace std;

const char shade_list[] = "$@B%8&WM#*oahkbdpqwmZO0QLCJUYXzcvunxrjft/|()1{}[]?-_+~<>i!lI;:,\"^`'. ";
const char* arg_list[] = {"-w", "-h", "--color", "--block", "--shades", "--nobg"};
const uint arg_count = 6;

inline void ClearScreen()
{
    printf("\x1b[0m\x1b[3J\x1b[H");
}

struct config
{
	string vid_path;
	uint8_t shades = 32;
	bool color = false;
	bool bg = true;
	bool clr = true;
	char block = '#';
	uint width = 80;
	uint height = 45;
};

bool isInArgList(const char* ag)
{
	string tok = ag;
	for (uint i=0 ; i < arg_count; ++i)
	{
		if (tok == arg_list[i]) return true;
	}
	return false;
}

config ArgumentParser(int argc, const char* argv[])
{
	config cfg;
	
	if (argc == 1)
	{
		printf( "Usage :-\n"
				"./asski-player [video path] <optional arguments>\n"
				"\n"
				"-w & -h   -->  Set Frame Width & Height (defaults are -w %u -h %u)\n"
				"--color   -->  Display the Video in 24-bit Colors (Slow on some Terminals)\n"
				"--block   -->  Specify the character to use as pixel in Color Mode (default is %c)\n"
				"               (ignored in non-color mode)\n"
				"--shades  -->  Specify the number of ascii shades (default is %u)\n"
				"               (ignored in color mode)\n"
				"--nobg    -->  Do not paint the background black.\n"
				"--noclr   -->  Preserve the last frame after playing.\n"
				"\n"
				"Note : Unknown Arguments are silently ignored.\n",
				 cfg.width, cfg.height, cfg.block, cfg.shades);
		exit(-1);
	}
	else if (isInArgList(argv[1]))
	{
		puts("\x1b[91;1;3m==> Error : No Video File Specified !\x1b[0m");
		exit(-1);
	}
	
	cfg.vid_path = argv[1];
	
	string tok;
	for (int i=2 ; i < argc ; i++)	//argv[0] is exe name, argv[1] is video name
	{
		tok = argv[i];
		if      (tok == "--color")  cfg.color  = true;
		else if (tok == "--block")  cfg.block  = *argv[i+1];
		else if (tok == "--shades") cfg.shades = stoi(argv[i+1]);
		else if (tok == "-w")       cfg.width  = stoi(argv[i+1]);
		else if (tok == "-h")       cfg.height = stoi(argv[i+1]);
		else if (tok == "--nobg")   cfg.bg     = false;
		else if (tok == "--noclr")  cfg.clr    = false;
	}
    return cfg;
}

void InterruptHandler(int signum)
{
	ClearScreen();
	puts("Interrupt Recieved, Exiting...");
	exit(signum);
}

int main(int argc, const char* argv[])
{
	signal(SIGINT, InterruptHandler);
	config cfg = ArgumentParser(argc, argv);

	int pos = 0;
	size_t rs;
	float sf1 = 256/cfg.shades;
	float sf2 = sizeof(shade_list) / cfg.shades;
	uint8_t brightness = 0;
	
	uint8_t buf[cfg.width*3*cfg.height];	// Save one image in buffer
	
	char test_cmd[1024];
	snprintf(test_cmd, 1024,"ffmpeg -re -i %s -v quiet -s %ux%u "
							"-f image2pipe -vcodec rawvideo -pix_fmt rgb24 -",
							cfg.vid_path.c_str(), cfg.width, cfg.height);
	
	FILE* ffpipe = popen(test_cmd, "r");
	if (!ffpipe)
	{
		printf("\x1b[91;1;3m==> Unable to Open Pipe, error %d : %s\x1b[0m\n", errno, strerror(errno));
		exit(-1);
	}

	ClearScreen();
	if (cfg.bg) printf("\x1b[48;2;0;0;0m"); // black background
	
	do {
		rs = fread(buf, 1, sizeof(buf), ffpipe);
		
    	for (uint y=0 ; y < cfg.height ; ++y)
    	{
    		for (uint x=0 ; x < cfg.width ; ++x)
    		{
    			if (cfg.color)
    			{
    				printf("\x1b[38;2;%u;%u;%um%c", buf[3*pos], buf[3*pos+1], buf[3*pos+2], cfg.block);
                }
                else
                {
    				brightness = (buf[3*pos]/sf1 + buf[3*pos+1]/sf1 + buf[3*pos+2]/sf1) / 3;
    				printf("%c", shade_list[(int)(brightness*sf2)]);
    			}
    			++pos;
    		}
    		if (y != cfg.height-1) puts("");	// Newline
    	}
    	
    	pos = 0;
    	printf("\x1b[H"); // go to top left
    	
	} while (rs == sizeof(buf));

    if (cfg.clr) ClearScreen(); // reset everything
    else printf("\x1b[0m"); // reset colors only
    
    int err = pclose(ffpipe); // closing pipe

    switch (err)
    {
    case 0: // no errors anywhere
    	printf("\n");
    	return 0;
    
    case -1: // error closing pipe
    	printf("\n\x1b[91;1m ==> Error %d while Closing Pipe, %s\x1b[0m\n", err, strerror(errno));
    	return -1;
    
    default: // error in ffmpeg
    	printf("\n\x1b[91;1m ==> Closing Pipe, FFmpeg Returned Code %d\x1b[0m\n", err);
    	return -2;
	}
}
