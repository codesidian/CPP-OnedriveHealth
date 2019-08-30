// onenotechecker.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include "pch.h"
#include <iostream>
#include <sys/stat.h>
#include <string>
#include <fstream>
#include <filesystem>
#include <cstdlib>
#include <shlwapi.h>
#pragma comment(lib,"shlwapi.lib")
#include "shlobj.h"
#include <tchar.h>
#include <iomanip>
#include <stdlib.h>
#include <chrono>
#include <windows.h>
#include <strsafe.h>
#include <ctime>


namespace fs = std::filesystem;
using namespace std::chrono_literals;

inline bool checkIfFileExists(const std::string& name) {
	std:: ifstream f(name.c_str());
	return f.good();

}

inline bool isFileDateOld(const std::string& name) {
	std::ifstream f(name.c_str());
	fs::path p = name;
//	auto ftime = fs::last_write_time(p);
//	std::time_t cftime = decltype(ftime)::clock::to_time(ftime);
//	std::cout << "File write time is " << std::asctime(std::localtime(&cftime)) << '\n';

}

BOOL GetLastWriteTime(HANDLE hFile, LPTSTR lpszString, DWORD dwSize)
{
	FILETIME ftCreate, ftAccess, ftWrite;
	SYSTEMTIME stUTC, stLocal;
	DWORD dwRet;

	// Retrieve the file times for the file.
	if (!GetFileTime(hFile, &ftCreate, &ftAccess, &ftWrite))
		return FALSE;

	// Convert the last-write time to local time.
	FileTimeToSystemTime(&ftWrite, &stUTC);
	SystemTimeToTzSpecificLocalTime(NULL, &stUTC, &stLocal);

	// Build a string showing the date and time.
	dwRet = StringCchPrintf(lpszString, dwSize,
		TEXT("%02d/%02d/%d"),
		stLocal.wMonth, stLocal.wDay, stLocal.wYear);

	if (S_OK == dwRet)
		return TRUE;
	else return FALSE;
}
int DisplayResourceNAMessageBox()
{
	int msgboxID = MessageBox(
		NULL,
		"Your OneDrive may not be working correctly.  Please contact [EMAIL] to have it checked as soon as possible.",
		"OneDrive Error!",
		MB_ICONSTOP | MB_OK | MB_DEFBUTTON1
	);

	switch (msgboxID)
	{
	case IDCANCEL:
		// TODO: add code
		break;
	case IDTRYAGAIN:
		// TODO: add code
		break;
	case IDCONTINUE:
		// TODO: add code
		break;
	}

	return msgboxID;
}

void HideConsole()
{
	::ShowWindow(::GetConsoleWindow(), SW_HIDE);
}

void ShowConsole()
{
	::ShowWindow(::GetConsoleWindow(), SW_SHOW);
}

bool IsConsoleVisible()
{
	return ::IsWindowVisible(::GetConsoleWindow()) != FALSE;
}
int main()
{	
	HideConsole();
	std::string filePathA;

	std::string pause = "";


	TCHAR szPath[MAX_PATH];
	// Get path for each computer, non-user specific and non-roaming data.
	if (SUCCEEDED(SHGetFolderPath(NULL, CSIDL_LOCAL_APPDATA, NULL, 0, szPath)))
	{
		// Append product-specific path
		PathAppend(szPath, _T("\\Microsoft\\OneDrive\\logs\\Business1\\SyncDiagnostics.log"));
	}


	if (checkIfFileExists(szPath))
	{	
		TCHAR szBuf[MAX_PATH];
		HANDLE hFile;

		hFile = CreateFile(szPath, GENERIC_READ, FILE_SHARE_READ, NULL,
			OPEN_EXISTING, 0, NULL);

		if (hFile == INVALID_HANDLE_VALUE)
		{
			printf("CreateFile failed with %d\n", GetLastError());
			return 0;
		}


		if (GetLastWriteTime(hFile, szBuf, MAX_PATH))


			CloseHandle(hFile);
		std::string modifiedDate = szBuf;

		TCHAR tBuf[MAX_PATH];
		GetDateFormat(LOCALE_CUSTOM_UI_DEFAULT, 0, NULL, "MM/dd/yyyy", tBuf, MAX_PATH);
		std::string currentDate = tBuf;
		TCHAR erBuf[MAX_PATH];
		std::string err = "test";
		if (modifiedDate != currentDate) {
			DisplayResourceNAMessageBox();
			std::cout << "Your OneDrive may not be working correctly.  Please contact [EMAIL] to have it checked as soon as possible." << std::endl;
		}
		
		
	}
	else {
		DisplayResourceNAMessageBox();
		std::cout << "Missing: Your OneDrive may not be working correctly.  Please contact [EMAIL] to have it checked as soon as possible." << std::endl;
	}

	
}