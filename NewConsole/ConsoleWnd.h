#pragma once

#include <memory>
#include <string>
#include <list>

#include <Windows.h>
#include <gdiplus.h>

#include "ConsoleEventListener.h"

class ConsoleHost;
class NewConsole;
class ConsoleWnd : public ConsoleEventListener, public std::enable_shared_from_this<ConsoleWnd>
{
private:
	std::shared_ptr<ConsoleHost> host_;

	std::shared_ptr<Gdiplus::Bitmap> cacheBitmap_;
	int cacheScrollx_;
	int cacheScrolly_;
	int cacheWidth_;
	int cacheHeight_;

	std::list<std::string> buffer_;
	std::string lastLine_;
	std::weak_ptr<NewConsole> mainWnd_;
private:
	virtual void handleWrite(const std::string &buffer);
	void updateCache(int width, int height, int scrollx, int scrolly);
	void invalidateCache();
	void bufferUpdated();
	void appendStringToBuffer(const std::string &buffer);
public:
	ConsoleWnd(const std::wstring &cmdline, std::weak_ptr<NewConsole> mainWnd);
	~ConsoleWnd();

	void drawScreenContents(HDC hdc, int x, int y, int width, int height, int scrollx, int scrolly);
	void appendInputBuffer(const std::string &buffer);
};
