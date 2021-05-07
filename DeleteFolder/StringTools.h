#pragma once

#include <Windows.h>
#include <string>
#include <vector>
#include <algorithm>
#include <tchar.h>

#pragma warning(disable:4190)
using namespace std;

#ifndef xstring 
#ifdef UNICODE
#define xstring wstring
#else
#define xstring string
#endif
#endif

std::string  UnicodeToAnsi(const std::wstring& strSource);
std::wstring AnsiToUnicode(const std::string& strSource);
std::wstring Utf8ToUnicode(const std::string& strSrouce);
std::string UnicodeToUtf8(const std::wstring& strSource);
std::string AnsiToUtf8(const std::string& strSource);
std::string Utf8ToAnsi(const std::string& strSource);
//std::string URLEncodeGB2312(const char* szSrc/*, char* pBuf, int cbBufLen*/);			//�ո�תΪ'+'
std::string URLEncodeGB2312Forspace(const char* szSrc/*, char* pBuf, int cbBufLen*/);	//�ո�תΪ'%20'
string Utf8ToGBK(const string& strSource);
char* GBKToUtf8(const char* strSource);
//bool UrlEncode(const char* szSrc, char* pBuf, int cbBufLen, bool bUpperCase);
//bool Utf8UrlDecode(const char* szSrc, char* pBuf, int cbBufLen);
//bool UrlDecode(const char* szSrc, char* pBuf, int cbBufLen);
std::string EscapeToAnsi(const std::string& strSource);

//������������������� URLEncodeGB2312 URLEncodeGB2312Forspace������ֱ���������,���Գ��Դ���utf8�������
//�����Ȼ��������Ҫ������������Ժ�������������
char dec2hexChar(short int n);
string escape(const string &URL);
string escapeURL(const string &URL);
string deescapeURL(const std::string &URL);

std::string deescapeUnicodeURL(const std::string &URL);

string Native2Ascii(const wstring& strUnicode,bool bPercentPre = false,bool bUpper = false);
wstring Ascii2Native(const string& strAnsi,bool bPercentPre = false);

#ifndef _W
#define _W(x) AnsiToUnicode(x).data()
#endif

#ifndef _A
#define _A(x) UnicodeToAnsi(x).data()
#endif

//�ַ����滻 ���strText�е�����strOld�������滻,��������滻��Ĵ��ٴμ���滻.���罫"1001"�е�"1"�滻Ϊ"1234",��ô����ֵΪ"1234001234"
//strText-ȫ������ strOld-��Ҫ���滻������ strNew-�µ������滻��
xstring StringReplace(const xstring& strText, const xstring& strOld, const xstring& strNew);
string StringReplaceA(const string& strText, const string& strOld, const string& strNew);

void SplitStringW(const wstring& src,vector<wstring> &dst,const wstring& strDelim);
void SplitStringA(const string& src,vector<string> &dst,const string& strDelim);

void TrimLeftStringA(string &strVal, LPCSTR lpszOneOfFinds=" ");
void TrimStringA(string &strVal, LPCSTR lpszOneOfFinds=" ");
void TrimString(xstring &strVal, LPCTSTR lpszOneOfFinds=_T(" \t\n"));

void ReplaceSubString(xstring &strSrc, LPCTSTR lpszStart, LPCTSTR lpszEnd, LPCTSTR lpszNew);

xstring GetSubString(const xstring &strSrc, LPCTSTR lpszStart, LPCTSTR lpszEnd);

//��Сдת��
xstring StringConvertUpperOrLower(bool bUpper,const xstring& strValue);
string StringConvertUpperOrLowerA(bool bUpper,const string& strValue);

//base64����
string base64Encode(const char* source,int nLength);
int base64Decode(const char* source, unsigned char* pOut);


inline int jwSnprintf(char* pszBuf, size_t nCount, const char * pszFormat, ...)
{
	va_list ap;
	va_start(ap, pszFormat);
	int n = _vsnprintf_s(pszBuf, nCount, _TRUNCATE, pszFormat, ap); //д���ַ���s
	va_end(ap); //�ͷ���Դ,������va_start�ɶԵ���

	return n; //����д����ַ�����
}

inline int jwStrncpy(char* pszDst, size_t nBufSize, const char * pszSrc)
{
	return strncpy_s(pszDst, nBufSize, pszSrc, _TRUNCATE);
}

inline string UintToStr(unsigned int uiVal)
{
	string strRet;
	char szBuff[20];

	_snprintf_s(szBuff, _countof(szBuff), _TRUNCATE, "%I32u", uiVal);
	strRet = szBuff;
	return strRet;
}


inline string DblToStr(double dblVal)
{
	string strRet;
	char szBuff[50];

	_snprintf_s(szBuff, _countof(szBuff), _TRUNCATE, "%f", dblVal);
	strRet = szBuff;
	return strRet;
}

inline bool IsGB2312(const unsigned char *pszIn) 
{ 
	if (*pszIn>=0xB0 && *pszIn<=0xF7 && *(pszIn+1)>=0xA0 && *(pszIn+1)<=0xFE) 
		return true; 
	else 
		return false; 
}

inline bool IsGBK(const unsigned char *pszIn) 
{ 
	if (*pszIn>=0x81 && *pszIn<=0xFE && *(pszIn+1)>=0x40 && *(pszIn+1)<=0xFE) 
		return true; 
	else 
		return false; 
}

inline bool IsBIG5(const unsigned char *pszIn) 
{ 
	if (*pszIn>=0x81 && *pszIn<=0xFE && ((*(pszIn+1)>=0x40 && *(pszIn+1)<=0x7E) || (*(pszIn+1)>=0xA1 && *(pszIn+1)<=0xFE)))
		return true; 
	else 
		return false; 
}

inline bool IsValidEnglishChar(const unsigned char *pszIn) 
{
	if((*pszIn>='0' && *pszIn <= '9') || (*pszIn>='a' && *pszIn <= 'z') || (*pszIn>='A' && *pszIn <= 'Z'))
	{
		return true;
	}
	else
		return false;
}

inline bool IsValidName(const unsigned char *pszIn, int nLen) 
{
	for (int i=0; i < nLen; )
	{
		if(IsValidEnglishChar(pszIn+i))
		{
			i++;
			continue;
		}
		else if(((i+1) < nLen) && IsGBK(pszIn+i))
		{
			i += 2;
			continue;
		}
		else
			return false;
	}
	return true;
}

inline bool IsValidQQ(const unsigned char *pszIn, int nLen) 
{
	for (int i=0; i < nLen; )
	{
		if(isdigit(*(pszIn+i)))
		{
			i++;
			continue;
		}
		else
			return false;
	}
	return true;

}

inline void CutChineseChar(char *pszSrc,unsigned int nNeedLen)
{
	if (strlen(pszSrc) >= nNeedLen)
	{
		//�������λ����Ӣ���ַ���Ҳ����һ����Ч�ĺ��֣������λ��һ�����ֶ����һ���ֽ�
		if(!IsValidEnglishChar((const unsigned char *)(pszSrc+nNeedLen-2)) && !IsGBK((const unsigned char *)(pszSrc+nNeedLen-2)))
		{
			*(pszSrc+nNeedLen-3) = '.';
		}
		*(pszSrc+nNeedLen-2) = '.';
		*(pszSrc+nNeedLen-1) = '.';
		*(pszSrc+nNeedLen) = '\0';
	}
}

inline DWORD IPSTR2DWORD(const char *pszIP)
{
	if (pszIP == NULL || pszIP[0] == '\0')
		return 0;

	vector<string> vecStr;
	SplitStringA(pszIP, vecStr, ".");

	DWORD dwRet = 0;
	for (unsigned int i = 0; i < vecStr.size(); i++)
	{
		int nTemp = atoi(vecStr[i].c_str());
		dwRet += (nTemp << (i * 8));
	}
	return dwRet;
}