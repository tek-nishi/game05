
#pragma once

//
// OS依存処理(OSX版)
//

#if defined (__APPLE__) && !(TARGET_OS_IPHONE)

#include <string>
#include <CoreFoundation/CoreFoundation.h>
#include "co_defines.hpp"

namespace ngs {

// OSXは自力でリソースパスを求める
std::string getCurrentPath()
{
	CFBundleRef mainBundle = CFBundleGetMainBundle();
	CFURLRef resourcesURL = CFBundleCopyResourcesDirectoryURL(mainBundle);
	std::string path("");

	char str[PATH_MAX];
	if(!CFURLGetFileSystemRepresentation(resourcesURL, TRUE, (UInt8 *)str, PATH_MAX))
	{
		DOUT << "error CFURLGetFileSystemRepresentation()" << std::endl;
	}
	else
	{
		path = std::string(str) + std::string("/");
	}
	CFRelease(resourcesURL);

	return path;
}


class Os {
	std::string loadPath_;
	std::string savePath_;

public:
	Os()
	{
		DOUT << "Os()" << std::endl;

		std::string path = getCurrentPath();
		loadPath_ = path + "devdata/";
		savePath_ = path;
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
