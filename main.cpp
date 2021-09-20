#include <cerrno>
#include <cmath>
#include <cstdio>
#include <stdexcept>
#include <string>
#include <format>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <system_error>

using namespace std;

#include "pipe.hpp"

int WIDTH;
int HEIGHT;

string QuoteShellArg(const string &$arg)
{
#if defined(__linux__) || defined(__unix__) || defined(__APPLE__)
	string ret="'";
	ret.reserve($arg.length()+10); // ¯\_(ツ)_/¯ should avoid realloc in most cases
	for(size_t i=0;i<$arg.length();++i)
	{
		if($arg[i]=='\00')
			throw runtime_error("argument contains null bytes, it is impossible to escape null bytes on unix!");
		else if($arg[i]=='\'')
			ret+="'\\''";
		else
			ret += $arg[i];
	}
	ret+="'";
	return ret;

#elif defined(_WIN32) || defined(_WIN64)
	// todo
	// the Windows escape rules are *complex* and even differ between system()/.bat invocations and manual cmd invocations
	// maybe try porting https://stackoverflow.com/a/29215357/1067003
	return string("\"") + $arg + string("\"");

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
	Pipe ffpipe;
	int ret = ffpipe.Open("ffmpeg -i test.3gp -s 160x90 -vf select='between(n\\,1\\,100)' -vsync 0 -f image2pipe -vcodec png -");
    if (ret != 0)
	{
		throw runtime_error(format(" Unable to Open Pipe, error {} : {}", errno, strerror(errno)));
	}

    size_t rs;
	uint8_t buf[16384];
	
	rs = ffpipe.Read(buf, sizeof(buf));

    uint32_t image_size = *(uint32_t*)&buf[2];
    cout << image_size << endl;

	ffpipe.Close();
	return 0;
}
