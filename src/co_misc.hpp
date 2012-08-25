
#pragma once

//
// 雑多な処理
//

#include <sys/stat.h>
#include <string>

namespace ngs {

// 切り上げて一番近い２のべき乗値を求める
int int2pow(const int value)
{
	int res = 1;

	while(res < (1 << 30))
	{
		if(res >= value)
		break;
		res *= 2;
	}

	return res;
}

// ファイルの存在判定
bool isFileExists(const std::string& file)
{
	struct stat info;
	int result = stat(file.c_str(),&info);
	return (result == 0);
	// TODO: ディレクトリかどうかも判定
}

template <typename T>
T rad2ang(const T rad)
{
	return rad * 180.0 / M_PI;
}

template <typename T>
T ang2rad(const T ang)
{
	return ang * M_PI / 180.0;
}

// キーワード置換
void replaceString(std::string& text, const std::string& src, const std::string& dst)
{
	std::string::size_type pos = 0;
	while((pos = text.find(src, pos)) != std::string::npos)
	{
		text.replace(pos, src.length(), dst);
		pos += dst.length();
	}
}

}
