#include "FileHandler.h"
#include <iostream>

string FileHandler::openFile(OpenFileMode mode)
{
#ifdef WIN32
	OPENFILENAME ofn;
	char filenameLength[100];

	ZeroMemory(&ofn, sizeof(ofn));

#ifdef _MSC_VER // Visual studio
	wchar_t dd[100]; // Dummy variable to initialize filename
	LPWSTR filename = dd;
	mode == OpenFileMode::NEWGAME ? 
		ofn.lpstrFilter = L"*.tar" :
		ofn.lpstrFilter = L"*.sav";
#else // g++
	char dd[100];
	LPSTR filename = dd;
	mode == OpenFileMode::NEWGAME ? 
		ofn.lpstrFilter = "*.tar" :
		ofn.lpstrFilter = "*.sav";
#endif

	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = NULL;
	ofn.lpstrFile = filename;
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = sizeof(filenameLength);
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;

	GetOpenFileName(&ofn);
	
#ifdef _MSC_VER // Convert LPWSTR to string
	wcstombs(filenameLength, ofn.lpstrFile, 100); // using filenameLength as buffer
	string returnString(filenameLength);
#else // Convert LPSTR to string
	string returnString = ofn.lpstrFile;
#endif
	
	return returnString;
#elif __APPLE__
    cout << "Enter full path to file: ";
    string s;
    cin >> s;
    return s;
#endif
}

string FileHandler::saveFile()
{
#ifdef WIN32
	OPENFILENAME ofn;
	char filenameLength[100];

	ZeroMemory(&ofn, sizeof(ofn));

#ifdef _MSC_VER // Visual studio
	wchar_t dd[100]; // Dummy variable to initialize filename
	LPWSTR filename = dd;
	ofn.lpstrFilter = L"*.sav";
#else // g++
	char dd[100];
	LPSTR filename = dd;
	ofn.lpstrFilter = "*.sav";
#endif

	ofn.lStructSize = sizeof(ofn);
	ofn.hwndOwner = NULL;
	ofn.lpstrFile = filename;
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = sizeof(filenameLength);
	ofn.nFilterIndex = 1;
	ofn.lpstrFileTitle = NULL;
	ofn.nMaxFileTitle = 0;
	ofn.lpstrInitialDir = NULL;
	ofn.Flags = OFN_PATHMUSTEXIST | OFN_FILEMUSTEXIST;
	
	GetSaveFileName(&ofn);

	#ifdef _MSC_VER // Convert LPWSTR to string
	wcstombs(filenameLength, ofn.lpstrFile, 100); // using filenameLength as buffer
	string returnString(filenameLength);
#else // Convert LPSTR to string
	string returnString = ofn.lpstrFile;
#endif

	return returnString;
#elif __APPLE__
    cout <<"Enter savefile name (.sav will be appended): ";
    string s;
    cin >> s;
    return s;
#endif
}