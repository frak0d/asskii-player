// This file needs some refactoring including windows compatibility

#include <cstdio>
#include <cerrno>
#include <cstdint>
#include <cstdlib>
#include <cstddef>
#include <fcntl.h>

class Pipe
{
private:
	FILE* fp = NULL;
public:
   ~Pipe() { Close(); }

	int Open(const char* cmdline, bool write = false)
	{
		fp = popen(cmdline, write ? "w" : "r");	// rb & wb are not working for some reason
		if (!fp) return -1; // return WEXITSTATUS(pclose(fp));
		fcntl(fileno(fp), F_SETPIPE_SZ, 67108864/*64MB*/);	// Increasing Pipe Buffer
		return 0;
	}

	int Close()
	{
		if (fp)
		{
			pclose(fp);
			fp = NULL;
		}
		return 0;
	}

	size_t Read(uint8_t* buffer, size_t size)
	{
        return fread(buffer, 1, size, fp);
    }

	size_t Write(uint8_t* buffer, size_t size)
	{
		return fwrite(buffer, 1, size, fp);
	}
};