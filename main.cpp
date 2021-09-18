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

int WIDTH;
int HEIGHT;

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
		system(format("rm \"{}\"", filename).c_str());
	#endif

	#if defined(_WIN32) || defined(_WIN64)
		system(format("del \"{}\"", filename).c_str());
	#endif
}

void DeleteFolder(const string& foldername)
{
	#if defined(__linux__) || defined(__unix__) || defined(__APPLE__)
		system(format("rm -r \"{}\"", foldername).c_str());
	#endif

	#if defined(_WIN32) || defined(_WIN64)
		system(format("rmdir \"{}\"", foldername).c_str());
	#endif
}

void save_frames(const string& ffpath, const string& filepath, const uint32_t start_num, const uint32_t end_num)
{
	string command = format("{0} -i {1} -vf select='between(n,{2},{3})' -vsync 0 -start_number {2} frame-%03d.png",
							ffpath, filepath, start_num, end_num);
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
	
	//save_frames(ff, fl, 20, 50);
	return 0;
}