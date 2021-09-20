#include <cstdint>
#include <cstdio>

#define USE_POPEN 1

#if PLATFORM_WINDOWS
#define popen _popen
#define pclose _pclose
#endif

class Pipe {
private:
#if USE_POPEN
  FILE *fp;
#else
  HANDLE hChildStd_IN_Rd;
  HANDLE hChildStd_IN_Wr;

  HANDLE hChildStd_OUT_Rd;
  HANDLE hChildStd_OUT_Wr;

  HANDLE hChildStd_ERR_Rd;
  HANDLE hChildStd_ERR_Wr;

  PROCESS_INFORMATION piProcInfo;
#endif

public:
  Pipe();
  ~Pipe() { Close(); }

  int Open(const char *cmdline, bool write = false);
  int Close();
  size_t Read(uint8_t* buffer, size_t size);
  size_t Write(uint8_t* buffer, size_t size);
};

#if USE_POPEN
Pipe::Pipe() { fp = NULL; }

int Pipe::Open(const char *cmdLine, bool write)
{
  fp = popen(cmdLine, write ? "wb" : "rb");
  if (!fp) return -1; return 0;
}

int Pipe::Close()
{
  if (fp)
  {
    pclose(fp);
    fp = NULL;
  }
  return 0;
}

size_t Pipe::Read(uint8_t *buffer, size_t size)
{
  return fread(buffer, 1, size, fp);
}

size_t Pipe::Write(uint8_t* buffer, size_t size)
{
  return fwrite(buffer, 1, size, fp);
}

#else

Pipe::Pipe()
{
  hChildStd_IN_Rd = NULL;
  hChildStd_IN_Wr = NULL;

  hChildStd_OUT_Rd = NULL;
  hChildStd_OUT_Wr = NULL;

  hChildStd_ERR_Rd = NULL;
  hChildStd_ERR_Wr = NULL;
}

int Pipe::Open(const char *cmdLine, bool write)
{
  SECURITY_ATTRIBUTES saAttr;

  // Set the bInheritHandle flag so pipe handles are inherited.

  saAttr.nLength = sizeof(SECURITY_ATTRIBUTES);
  saAttr.bInheritHandle = TRUE;
  saAttr.lpSecurityDescriptor = NULL;

  // Create a pipe for the child process's STDOUT.

  if (!CreatePipe(&hChildStd_OUT_Rd, &hChildStd_OUT_Wr, &saAttr, 0))
    return -1;

  // Ensure the read handle to the pipe for STDOUT is not inherited.
  if (!write) {
    if (!SetHandleInformation(hChildStd_OUT_Rd, HANDLE_FLAG_INHERIT, 0))
      return -1;
  } else {
    if (!SetHandleInformation(hChildStd_OUT_Wr, HANDLE_FLAG_INHERIT, 0))
      return -1;
  }

  // Create a pipe for the child process's STDERR.

  if (!CreatePipe(&hChildStd_ERR_Rd, &hChildStd_ERR_Wr, &saAttr, 0))
    return -1;

  // Ensure the read handle to the pipe for STDERR is not inherited.

  if (!write) {
    if (!SetHandleInformation(hChildStd_ERR_Rd, HANDLE_FLAG_INHERIT, 0))
      return -1;
  } else {
    if (!SetHandleInformation(hChildStd_ERR_Wr, HANDLE_FLAG_INHERIT, 0))
      return -1;
  }

  // Create a pipe for the child process's STDIN.

  if (!CreatePipe(&hChildStd_IN_Rd, &hChildStd_IN_Wr, &saAttr, 0))
    return -1;

  // Ensure the write handle to the pipe for STDIN is not inherited.

  if (!SetHandleInformation(hChildStd_IN_Wr, HANDLE_FLAG_INHERIT, 0))
    return -1;

  STARTUPINFOA siStartInfo;
  BOOL bSuccess = FALSE;
  ZeroMemory(&piProcInfo, sizeof(PROCESS_INFORMATION));
  ZeroMemory(&siStartInfo, sizeof(STARTUPINFO));
  siStartInfo.cb = sizeof(STARTUPINFO);
  siStartInfo.hStdError = hChildStd_ERR_Wr;
  siStartInfo.hStdOutput = hChildStd_OUT_Wr;
  siStartInfo.hStdInput = hChildStd_IN_Rd;
  siStartInfo.dwFlags |= STARTF_USESTDHANDLES;
  bSuccess = CreateProcessA(NULL,
                            (char *)cmdLine, // command line
                            NULL,            // process security attributes
                            NULL,         // primary thread security attributes
                            TRUE,         // handles are inherited
                            0,            // creation flags
                            NULL,         // use parent's environment
                            NULL,         // use parent's current directory
                            &siStartInfo, // STARTUPINFO pointer
                            &piProcInfo); // receives PROCESS_INFORMATION

  // If an error occurs, exit the application.
  if (!bSuccess)
    return -1;
  else {
    // Close handles to the child process and its primary thread.
    // Some applications might keep these handles to monitor the status
    // of the child process, for example.

    CloseHandle(piProcInfo.hProcess);
    CloseHandle(piProcInfo.hThread);
  }

  if (write) {
    CloseHandle(hChildStd_OUT_Rd);
    hChildStd_OUT_Rd = NULL;
    CloseHandle(hChildStd_OUT_Wr);
    hChildStd_OUT_Wr = NULL;
    CloseHandle(hChildStd_ERR_Rd);
    hChildStd_ERR_Rd = NULL;
    CloseHandle(hChildStd_ERR_Wr);
    hChildStd_ERR_Wr = NULL;
    CloseHandle(hChildStd_IN_Rd);
    hChildStd_IN_Rd = NULL;
  } else {
    CloseHandle(hChildStd_IN_Rd);
    hChildStd_IN_Rd = NULL;
    CloseHandle(hChildStd_IN_Wr);
    hChildStd_IN_Wr = NULL;
    //		CloseHandle(hChildStd_OUT_Rd);
    //		hChildStd_OUT_Rd = NULL;
    CloseHandle(hChildStd_OUT_Wr);
    hChildStd_OUT_Wr = NULL;
    CloseHandle(hChildStd_ERR_Rd);
    hChildStd_ERR_Rd = NULL;
    CloseHandle(hChildStd_ERR_Wr);
    hChildStd_ERR_Wr = NULL;
  }
  return 0;
}

int Pipe::Close() {
  if (hChildStd_IN_Rd) {
    CloseHandle(hChildStd_IN_Rd);
    hChildStd_IN_Rd = NULL;
  }

  if (hChildStd_IN_Wr) {
    CloseHandle(hChildStd_IN_Wr);
    hChildStd_IN_Wr = NULL;
  }

  if (hChildStd_OUT_Rd) {
    CloseHandle(hChildStd_OUT_Rd);
    hChildStd_OUT_Rd = NULL;
  }

  if (hChildStd_OUT_Wr) {
    CloseHandle(hChildStd_OUT_Wr);
    hChildStd_OUT_Wr = NULL;
  }

  if (hChildStd_ERR_Rd) {
    CloseHandle(hChildStd_ERR_Rd);
    hChildStd_ERR_Rd = NULL;
  }

  if (hChildStd_ERR_Wr) {
    CloseHandle(hChildStd_ERR_Wr);
    hChildStd_ERR_Wr = NULL;
  }

  return 0;
}
size_t Pipe::Read(char *buffer, size_t size) {
  // check if the process terminated
  DWORD dwRead;
  BOOL bSuccess = ReadFile(hChildStd_OUT_Rd, buffer, size, &dwRead, NULL);
  return bSuccess ? dwRead : 0;
}
size_t Pipe::Write(char *buffer, size_t size) {
  DWORD dwWritten;
  BOOL bSuccess = WriteFile(hChildStd_IN_Wr, buffer, size, &dwWritten, NULL);
  return bSuccess ? dwWritten : 0;
}
#endif
