#include <string>
#include <format>

using namespace std;

string QuoteShellArg(const string& arg);

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
