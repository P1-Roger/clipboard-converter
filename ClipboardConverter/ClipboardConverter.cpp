// ClipboardConverter.cpp : This file contains the 'main' function. Program execution begins and ends there.
//

#include <iostream>
#include <Windows.h>
#include <string>
#include <cctype>
#include <cwctype>
#include <locale>

void ConvertClipboardText(char format)
{
	if (!OpenClipboard(NULL))
	{
		std::cout << "Failed to open clipboard." << std::endl;
		return;
	}

	HANDLE hData = GetClipboardData(CF_UNICODETEXT);
	if (hData == NULL)
	{
		std::cout << "Failed to get clipboard data." << std::endl;
		CloseClipboard();
		return;
	}

	wchar_t* pData = static_cast<wchar_t*>(GlobalLock(hData));
	if (pData == NULL)
	{
		std::cout << "Failed to lock clipboard data." << std::endl;
		CloseClipboard();
		return;
	}

	std::wstring text(pData);

	GlobalUnlock(hData);
	CloseClipboard();

	switch (format)
	{
	case 'U':
		std::cout << "You chose U" << std::endl;
		for (wchar_t& c : text)
		{
			c = std::towupper(c);
		}
		break;
	case 'L':
		std::cout << "You chose L" << std::endl;
		for (wchar_t& c : text)
		{
			c = std::towlower(c);
		}
		break;
	case 'P':
		std::cout << "You chose P" << std::endl;
		bool capitalizeNext = true;
		for (wchar_t& c : text)
		{
			if (std::iswalpha(c))
			{
				if (capitalizeNext)
				{
					c = std::towupper(c);
					capitalizeNext = false;
				}
				else
				{
					c = std::towlower(c);
				}
			}
			else
			{
				capitalizeNext = true;
			}
		}
	}

	if (!OpenClipboard(NULL))
	{
		std::cout << "Failed to open clipboard." << std::endl;
		return;
	}

	HGLOBAL hModifiedData = GlobalAlloc(GMEM_MOVEABLE, (text.size() + 1) * sizeof(wchar_t));
	if (hModifiedData == NULL)
	{
		std::cout << "Failed to allocate memory for modified data." << std::endl;
		CloseClipboard();
		return;
	}

	wchar_t* pModifiedData = static_cast<wchar_t*>(GlobalLock(hModifiedData));
	if (pModifiedData == NULL)
	{
		std::cout << "Failed to lock memory for modified data." << std::endl;
		CloseClipboard();
		return;
	}

	wcscpy_s(pModifiedData, text.size() + 1, text.c_str());

	GlobalUnlock(hModifiedData);

	EmptyClipboard();
	SetClipboardData(CF_UNICODETEXT, hModifiedData);

	CloseClipboard();

	std::wcout << "Text converted to: " << text << std::endl;
	//std::cout << "Text copied back to clipboard." << std::endl;
}

int main()
{
	std::locale::global(std::locale(""));
	std::wcout.imbue(std::locale());

	std::cout << "---- Clipboard Text Format by Roger ----" << std::endl;
	std::cout << "----------------------------------------" << std::endl;
	std::cout << "Press 'U' to convert text to uppercase." << std::endl;
	std::cout << "Press 'L' to convert text to lowercase." << std::endl;
	std::cout << "Press 'P' to convert text to proper case." << std::endl;
	std::cout << "Press 'Q' to quit the program." << std::endl;
	
	char choice;
	while (true)
	{
		std::cout << "Enter your choice: ";
		std::cin >> choice;

		switch (toupper(choice))
		{
		case 'U':
			ConvertClipboardText('U');
			break;
		case 'L':
			ConvertClipboardText('L');
			break;
		case 'P':
			ConvertClipboardText('P');
			break;
		case 'Q':
			return 0;
		default:
			std::cout << "Invalid choice. Please try again." << std::endl;
			break;
		}
	}
}
