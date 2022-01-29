#pragma once

#define RECVPACK_SIZE 8192 
struct ServerInfo
{
	std::wstring  LunchVer;
	std::wstring  HomeUrl;
	std::wstring  PayUrl;
	std::wstring  QQ;
	std::wstring  CommandLine;
	std::wstring  ExeName;
	std::wstring  GGUrl;
	std::wstring  UpLuchUrl;
	std::wstring  LunchName;
	std::wstring  UpdataUrl;
};
struct TextColor
{
	int Red;
	int Green;
	int Blue;
};
struct DlgRect
{
	int x;
	int y;
	int cx;
	int cy;
};

struct DownLoadInfo
{
	std::wstring Url;
	std::wstring Path;
};
enum ColorSet
{
	eTip,
	eFile,
	eVer,
	eFont,
	eProf,
	eProb,
	eProdf,
	eProdb,
	eWeb,
	eMaxColor,
};
enum DlgRectSet
{
	eStart,
	eMain,
	ePay,
	eHome,
	eClose,
	eWindow,
	ePro,
	eProd,
	eFenbianlv,
	eTipMsg,
	eFileMsg,
	eVerMsg,
	eWebRect,
	eMaxRect,
};
struct FontSet
{
	int Size;
	TCHAR FontName[64];
};
typedef std::vector<std::wstring> PathFile;