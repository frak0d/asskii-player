#include <cerrno>
#include <cmath>
#include <cstdio>
#include <string>
#include <format>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <unistd.h>

using namespace std;

#include "pipe.hpp"

int WIDTH;
int HEIGHT;

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

int main(int argc, const char* argv[])
{
	uint8_t buf[102400/*100KB*/];
	char test_cmd[] = "ffmpeg -i test.3gp -vsync 0 -s 160x90 -v quiet "
					  "-f image2pipe -vcodec rawvideo -pix_fmt rgb24 -";

	FILE* ffpipe = popen(test_cmd, "r");
	if (!ffpipe)
	{
		printf("\033[91;1;3m==> Unable to Open Pipe, error %d : %s\033[m\n", errno, strerror(errno));
		exit(-1);
	}

	size_t rs = fread(buf, 1, sizeof(buf), ffpipe);
    cout << "\n\033[94;1;3m ++++++++ Read " << rs << " Bytes +++++++++ \033[m\n" << endl;
    cout << "\033[96;1m ==> Closing Pipe, " << strerror(pclose(ffpipe)) << "\033[m" << endl;
    return 0;
}
