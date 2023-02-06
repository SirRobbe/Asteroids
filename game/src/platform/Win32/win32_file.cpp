#include "../file.h"

#include "kswindows.h"
// TODO: Write cusotm assert
#include <cassert>
// TODO: Implement own
#include <iostream>

namespace ks::File
{
	INTERNAL bool Win32OpenFile(stringUTF8 path, HANDLE* fileHandle)
	{
		LPWSTR utf16Path = Win32UTF8ToUTF16(path);
		*fileHandle = CreateFile(utf16Path, GENERIC_READ, FILE_SHARE_READ,
			nullptr, OPEN_EXISTING, 0, nullptr);
		delete[] utf16Path;

		if (*fileHandle == INVALID_HANDLE_VALUE)
		{
			DWORD error = GetLastError();
			// TODO: Make string printable
			std::cout << "ERROR " << error << " OCCURED WHILE OPENING FILE " << (char)*path << std::endl;
			return false;
		}

		return true;
	}

	INTERNAL int64 Win32GetFileSize(HANDLE file)
	{
		LARGE_INTEGER fileSize;
		if (!GetFileSizeEx(file, &fileSize))
		{
			// TODO: Diagnostics ...
			assert(false);
			return -1;
		}

		return fileSize.QuadPart;
	}

	INTERNAL void Win32ReadIntoBuffer(HANDLE file, int64 size, byte* buffer)
	{
		DWORD bytesRead;
		if (!ReadFile(file, buffer, size, &bytesRead, nullptr))
		{
			// TODO: Diagnostics ...
			assert(false);
		}
	}

	INTERNAL void Win32CloseFile(HANDLE file)
	{
		if (!CloseHandle(file))
		{
			// TODO: Diagnostics ...
			assert(false);
		}
	}

	string ReadAllText(stringUTF8 path)
	{
		HANDLE file;
		string buffer = nullptr;
		if (Win32OpenFile(path, &file))
		{
			int64 fileSize = Win32GetFileSize(file);
			if (fileSize != -1)
			{
				int64 bufferSize = fileSize + 1;
				buffer = new char8[bufferSize];

				Win32ReadIntoBuffer(file, fileSize, (byte*)buffer);
				Win32CloseFile(file);

				buffer[fileSize] = '\0';

				// TODO: At some point I dont want that this functions has to care about line
				// endings. Since the engine shall only support \n extract this job to a util that
				// prepares assets before they get loaded
				// NOTE: Already removing \r from lineendings in case they exist
				// so the rest of the engine can work with this assumption
				for (int i = 0; i < bufferSize; i++)
				{
					if (buffer[i] == '\r')
					{
						for (int j = i; j < bufferSize; j++)
						{
							buffer[j] = buffer[j + 1];
						}
						bufferSize--;
					}
				}

				if (bufferSize != fileSize + 1)
				{
					string temp = buffer;
					buffer = new char8[bufferSize];
					for (int i = 0; i < bufferSize; i++)
					{
						buffer[i] = temp[i];
					}
					delete[] temp;
				}
			}
		}

		return buffer;
	}

	FixedArray<byte>* ReadAllBytes(stringUTF8 path)
	{
		HANDLE file = 0;
		FixedArray<byte>* buffer = nullptr;
		if(Win32OpenFile(path, &file))
		{
			int64 fileSize = Win32GetFileSize(file);
			if (fileSize != -1)
			{
				buffer = new FixedArray<byte>(fileSize);
				Win32ReadIntoBuffer(file, fileSize, buffer->Data());
				Win32CloseFile(file);
			}
		}

		return buffer;
	}
}