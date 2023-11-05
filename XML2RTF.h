
#if !defined(AFX_XML2RTF_H__EAA1F837_8EAD_4345_9CCB_79ED77DFDC35__INCLUDED_)
#define AFX_XML2RTF_H__EAA1F837_8EAD_4345_9CCB_79ED77DFDC35__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "resource.h"


class CStringEx : public CString {
	public:
		enum Options {
			NO_OPTIONS=0,
			COMMA_DELIMIT=1,
			};
		static const int BASE64_MAXLINE;
		static const char *EOL;
		static const char decimalChar,thousandChar;
		static const char CRchar,LFchar,TABchar;
	public:
		static const char m_base64tab[];
	public:
		CString Tokenize(CString delimiter, int& first);
		static CStringEx CommaDelimitNumber(const char *);
		static CStringEx CommaDelimitNumber(CString);
		static CStringEx CommaDelimitNumber(DWORD);
		CStringEx SubStr(int begin, int len) const;					// substring from s[begin] to s[begin+len]
		BYTE Asc(int);
		int Val(int base=10);
		double Val();
		void Repeat(int);
		void Repeat(const char *,int);
		void Repeat(char,int);
		void AddCR() { CStringEx::operator+=(LFchar); }
		void RemoveLeft(int n) { CStringEx::operator=(Mid(n)); }
		void RemoveRight(int n) { CStringEx::operator=(Mid(1,GetLength()-n)); }		// era un'idea per fare LEFT$ di tot char, ma Trimright c' gi anche se diversa...
		void Trim() { CString::TrimLeft(); CString::TrimRight(); }	
		static BOOL IsAlpha(char);
		BOOL IsAlpha(int);
		static BOOL IsAlnum(char);
		BOOL IsAlnum(int);
		static BOOL IsDigit(char);
		BOOL IsDigit(int);
		static BOOL IsPrint(char);
		BOOL IsPrint(int);
		int FindNoCase(CString substr,int start=0);
		WORD GetAsciiLength();
		CStringEx Encode64();
		int Decode64();
		CStringEx FormatTime(int m=0,CTime mT=0);
		CStringEx FormatSize(DWORD);
		void Print();
		void Debug();
		CStringEx() : CString() {};		// servono tutti i costruttori "perch non ne ha di virtual, la CString" !
		// https://www.codeguru.com/cpp/cpp/string/ext/article.php/c2793/CString-Extension.htm
		// https://www.codeproject.com/Articles/2396/Simple-CString-Extension
		CStringEx(const CString& stringSrc) : CString(stringSrc) {};
		// bah, eppure non sembra... 2021...
//		CStringEx(const CStringEx& stringSrc) : CString(stringSrc) {};
		CStringEx(TCHAR ch, int nRepeat = 1) : CString(ch, nRepeat) {};
//		CStringEx(LPCTSTR lpch, int nLength) : CString(lpch, nLength) {};
//		CStringEx(const unsigned char *psz) : CString(psz) {};
		CStringEx(LPCWSTR lpsz) : CString(lpsz) {};
		CStringEx(LPCSTR lpsz) : CString(lpsz) {};
//		CStringEx(const char c) {char s[2]={'\0', '\0'}; s[0]=c; CString::operator=(s);}
		CStringEx(int i, const char* format="%d", DWORD options=NO_OPTIONS);
		CStringEx(double d, const char* format="%02lf", DWORD options=NO_OPTIONS);
		virtual ~CStringEx() {};
private:
	CString InsertSeparator(DWORD);
	};


class CTimeEx : public CTime {
public:
	CTimeEx(int nYear, int nMonth, int nDay,
		int nHour, int nMin, int nSec, int nDST = -1) { CTime(nYear, nMonth, nDay,
      nHour, nMin, nSec, nDST); }
	CTimeEx() { CTime(); }
	static CString Num2Mese(int);
	static CString Num2Giorno(int);
	static CString Num2Month3(int);
	static CString Num2Day3(int);
	static CString getNow(int ex=0);
	static CString getNowGMT(BOOL bAddCR=TRUE);
	static CString getNowGoogle(BOOL bAddCR=TRUE);
	static int getMonthFromString(const CString);
	static int getMonthFromGMTString(const CString);
	static CString getMese() { return Num2Giorno(GetCurrentTime().GetDay()); };
	static CString getGiorno() { return Num2Mese(GetCurrentTime().GetMonth()); };
	static CString getFasciaDellaGiornata();
	static CString getSaluto();
	static WORD GetDayOfYear();
	static CTime parseGMTTime(const CString);
	static CTime parseTime(const CString);
	static BOOL isWeekend();
	BOOL isWeekend(CTime);
	};

#endif // !defined(AFX_XML2RTF_H__EAA1F837_8EAD_4345_9CCB_79ED77DFDC35__INCLUDED_)
