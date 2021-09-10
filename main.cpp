#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <iostream>

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

int main(int argc, char const *argv[])
{
	return 0;
}