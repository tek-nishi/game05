
#pragma once

//
// OS依存処理(Windows版)
//

#if defined (_MSC_VER)

#include <iostream>
#include <string>
#include <streambuf>
#include <cstring>
#include "co_defines.hpp"

namespace ngs {

class DbgStreambuf : public std::streambuf
{
	std::vector<char> str_;
public:
	int_type overflow(int_type c = EOF)
	{
		str_.push_back(c);
		return c;
	}
	int sync()
	{
		str_.push_back('\0');
		// 念のため
		OutputDebugString(&str_[0]);
		str_.clear();
		return 0;
	}
};


class Os {
	enum { osTimerPeriod = 1 };

	DbgStreambuf dbgStream_;
	std::streambuf *stream_;

	std::string loadPath_;
	std::string savePath_;

public:
	Os() :
		loadPath_("devdata/"),
		savePath_("")
	{
		_set_error_mode(_OUT_TO_MSGBOX);
		stream_ = std::cout.rdbuf(&dbgStream_);

		DOUT << "Os()" << std::endl;
#ifdef _DEBUG
		_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif
		timeBeginPeriod(osTimerPeriod);
		// FIXME:これを設定しないと60fpsで動作しない
	}

	~Os()
	{
		DOUT << "~Os()" << std::endl;
		timeEndPeriod(osTimerPeriod);
		std::cout.rdbuf(stream_);
	}

	const std::string& loadPath() const { return loadPath_; }
	const std::string& savePath() const { return savePath_; }
};

}

#endif
