#include "FileHandler.h"
#include <iostream>

string FileHandler::openFile()
{
#ifdef WIN32
	OPENFILENAME ofn;
	char filenameLength[100];
	wchar_t dd[100]; // Dummy variable to initialize filename
	LPWSTR filename = dd;

	ZeroMemory(&ofn, sizeof(ofn));
	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = NULL;
	ofn.lpstrFile = filename;
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = sizeof(filenameLength);
	ofn.lpstrFilter = L"All";
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	GetOpenFileName(&ofn);

	// Convert LPWSTR to string
	wcstombs(filenameLength, ofn.lpstrFile, 100); // using filenameLength as buffer
	string returnString(filenameLength);

	return returnString;
#elif APPLE
#endif
}
void FileHandler::saveFile()
{
#ifdef WIN32
#elif APPLE
#endif
}