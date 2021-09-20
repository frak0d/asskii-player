#include <cmath>
#include <cstdio>
#include <string>
#include <format>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <filesystem>

using namespace std;
namespace fs = std::filesystem;

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
#endif

    throw runtime_error("Unsupported OS !");
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

void DeleteFile(const string& filename)
{
	#if defined(__linux__) || defined(__unix__) || defined(__APPLE__)
		system(format("rm {}", QuoteShellArg(filename)).c_str());
	#endif

	#if defined(_WIN32) || defined(_WIN64)
		system(format("del {}", QuoteShellArg(filename)).c_str());
	#endif
}

void DeleteFolder(const string& foldername)
{
	#if defined(__linux__) || defined(__unix__) || defined(__APPLE__)
		system(format("rm -r {}", QuoteShellArg(foldername)).c_str());
	#endif

	#if defined(_WIN32) || defined(_WIN64)
		system(format("rmdir {}", QuoteShellArg(foldername)).c_str());
	#endif
}

void save_frames(const string& ffpath, const string& filepath, const uint32_t start_num, const uint32_t end_num)
{
	string command = format("{0} -i {1} -vf select='between(n,{2},{3})' -vsync 0 -start_number {2} frame-%03d.png",
							QuoteShellArg(ffpath), QuoteShellArg(filepath), start_num, end_num);
	system(command.c_str());
}

void delete_frames(const string& folderpath, const uint32_t start_num, const uint32_t end_num)
{
	for (int i = start_num; i <= end_num; ++i)
	{
		DeleteFile(format("{}/frame-{:3d}", folderpath, i));
	}
}

int main(int argc, const char* argv[])
{
	string ff = "ffmpeg";
	string fl = "test.3gp";

	auto self_path = fs::path(argv[0]);
	cout << self_path << endl;
/*
	Pipe pipeIn, pipeOut;
	pipeIn.Open("ffmpeg.exe -i input.mp3 -map_metadata -1 -f wav -c:a pcm_f32le -");
	pipeOut.Open("opusenc.exe --ignorelength --bitrate 128 - output.opus", true);

	char buf[16384];
	size_t res = 0;
	do {
	  res = pipeIn.Read(buf, sizeof(buf));
	  pipeOut.Write(buf, res);
	} while (res != 0);

	pipeIn.Close();
	pipeOut.Close();
*/
	//save_frames(ff, fl, 20, 50);
	return 0;
}
