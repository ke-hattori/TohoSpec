#ifndef _CHECK_DATA_H_
#define _CHECK_DATA_H_

enum CHECK_DATA
{
	CHECK_DATA_OK = 0,
	CHECK_DATA_STRING_EMPTY,
	CHECK_DATA_NO_NUMBER,
	CHECK_DATA_OUTSIDE_EFFECTIVE_NUMBER,
	CHECK_DATA_OUTSIDE_RANGE,
};

int CheckMinMaxDouble(char* lpszBuff, double dMin = 0, double dMax = 0);
int CheckMinMaxDouble(CString strBuff, double dMin = 0, double dMax = 0);
int CheckMinMaxLong(char* lpszBuff, long lMin = 0, long lMax = 0);
int CheckMinMaxLong(CString strBuff, long lMin = 0, long lMax = 0);
int CheckMinMaxInt(char* lpszBuff, int iMin = 0, int iMax = 0);
int CheckMinMaxInt(CString strBuff, int iMin = 0, int iMax = 0);
BOOL CheckName(char* lpszName, int iLenMax);
BOOL CheckName(CString strName, int iLenMax);

#endif // _CHECK_DATA_H_
