#include <Windows.h>
#include <iostream>
#include <string>

HANDLE CopyToClipboard(UINT format, std::string content);

int main()
{
	if (!OpenClipboard(NULL))
	{
		std::cout << "Unable to open clipboard" << std::endl;
		return 1;
	}
	EmptyClipboard();
	UINT skypeClipboardFormat = RegisterClipboardFormat(L"SkypeMessageFragment");
	if (skypeClipboardFormat == 0)
	{
		std::cout << "Unable to register clipboard format" << std::endl;
		return 2;
	}
	std::string message;
	std::string author;
	std::string date;
	std::cout << "Enter skype username of author: ";
	std::getline(std::cin, author);
	std::cout << "\nEnter date of message (unix time stamp): ";
	std::getline(std::cin, date);
	std::cout << "\nEnter message:\n";
	std::getline(std::cin, message);
	std::string txt = "<quote author=\"" + author + "\" timestamp=\"" + date + "\">" + message + "</quote>";
	CopyToClipboard(CF_TEXT, message);
	CopyToClipboard(skypeClipboardFormat, txt);
	CloseClipboard();
	return 0;
}

HANDLE CopyToClipboard(UINT format, std::string content)
{
	HGLOBAL hg = GlobalAlloc(GMEM_MOVEABLE, content.size() + 1);
	if (!hg)
	{
		std::cout << "Unable to allocate memory for text" << std::endl;
		return NULL;
	}
	memcpy(GlobalLock(hg), content.c_str(), content.size()+1);
	GlobalUnlock(hg);
    HANDLE result = SetClipboardData(format, hg);
	GlobalFree(hg);
	return result;
}