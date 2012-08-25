
#pragma once

//
// OS依存処理(iOS版)
//

#if defined (TARGET_OS_IPHONE)

#import <UIKit/UIKit.h>
#include <string>
#include "co_defines.hpp"

namespace ngs {


NSString *GetDocumentPath()
{
	NSArray *paths = NSSearchPathForDirectoriesInDomains(NSDocumentDirectory, NSUserDomainMask, YES);
	return [paths objectAtIndex:0];
}


class Os {
	std::string loadPath_;
	std::string savePath_;

public:
	Os()
	{
		DOUT << "Os()" << std::endl;

		loadPath_ = [[[NSBundle mainBundle] resourcePath] UTF8String] + std::string("/devdata/");
		savePath_ = [GetDocumentPath() UTF8String] + std::string("/");
		// TODO:テスト時にはパスをプロジェクトのパスに切り替えたい
	}

	~Os()
	{
		DOUT << "~Os()" << std::endl;
	}

	const std::string& loadPath() const { return loadPath_; }
	const std::string& savePath() const { return savePath_; }
};

}

#endif
