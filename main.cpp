#include <cmath>
#include <cstdio>
#include <string>
#include <format>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <filesystem>

using namespace std;

int WIDTH;
int HEIGHT;

void clear()
{
	#if defined(__linux__) || defined(__unix__) || defined(__APPLE__)
		system("clear");
	#endif

	#if defined(_WIN32) || defined(_WIN64)
		system("cls");
	#endif
}

void save_frames(string& ffpath, string& filepath, uint32_t start, uint32_t end)
{
	string command = format("{0} -i {1} -vf select=\'between(n\,{2}\,{3})\' -vsync 0 -start_number {2} frames%d.png",
							ffpath, filepath, start, end);
	system(command.c_str());
}

int main(int argc, char const *argv[])
{
	string ff = "./ffmpeg/ffmpeg.exe";
	string fl = "./ffmpeg/test.3gp";

	save_frames(ff, fl, 20, 50);
	return 0;
}