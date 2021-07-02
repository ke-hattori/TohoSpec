#include "stdafx.h"
#include "System.h"
//Saiki 20090527 Add ----->
#include "resource.h"
//Saiki 20090527 Add <-----
/////////////////////////////////////////////////////////////////////////////
// Name       : CheckMinMaxDouble
// Purpose    : 文字列が指定された範囲の数値であるかをチェック(double型)
//              dMin,dMaxが同じ値(指定しない)の場合は範囲のチェックはしない
// Parameters : lpszBuff ---> 文字列
//              dMin     ---> 最小値
//              dMax     ---> 最大値
//
// Returns    : CHECK_DATA_OK                       ---> OK
//              CHECK_DATA_STRING_EMPTY             ---> 文字列が空っぽ
//              CHECK_DATA_NO_NUMBER                ---> 文字列が数値ではない
//              CHECK_DATA_OUTSIDE_EFFECTIVE_NUMBER ---> 有効桁数外(小数点第3位まで)
//              CHECK_DATA_OUTSIDE_RANGE            ---> 数値が範囲外
int CheckMinMaxDouble(char* lpszBuff, double dMin /* = 0 */, double dMax /* = 0 */)
{
	char szBuff[256], szMessage[256];
	int iLen, iIndex = 0, iDecimalCount = 0;
	BOOL bCheckDot = FALSE;
	double dData;
	//Saiki 20090526 Add ----->
	CString strBuffer;
	//Saiki 20090526 Add <-----

	_tcscpy(szBuff, lpszBuff);
	iLen = strlen(szBuff);
	// 文字列が空っぽの場合
	if(iLen <= 0){
//		sprintf(szMessage, "Please enter a number");
		//Saiki 20090526 Change ----->
		//sprintf(szMessage, "数値を入力してください");
		LoadStringML(IDS_ENTER_NUMBER, strBuffer, "Please enter a number");
		MessageBox(NULL, strBuffer, NULL, MB_OK|MB_ICONSTOP);
		//Saiki 20090526 Change <-----
		return CHECK_DATA_STRING_EMPTY;
	}

	while(iIndex < iLen){
		// 一文字目，もしくは最後が「.」の場合
		if((iIndex == 0 || iIndex == iLen - 1) && szBuff[iIndex] == '.'){
//			sprintf(szMessage, "Please enter a number");
			//Saiki 20090526 Change ----->
			//sprintf(szMessage, "数値を入力してください");
			LoadStringML(IDS_ENTER_NUMBER, strBuffer, "Please enter a number");
			MessageBox(NULL, strBuffer, NULL, MB_OK|MB_ICONSTOP);
			//Saiki 20090526 Change <-----
			return CHECK_DATA_NO_NUMBER;
		}

		// 一文字目が「-」の場合はそのまま続ける
		if(iIndex == 0 && szBuff[iIndex] == '-'){
			iIndex++;
			continue;
		}

		switch(szBuff[iIndex]){
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
			if(bCheckDot){
				iDecimalCount++;
				if(iDecimalCount > 3){
//					sprintf(szMessage, "Please enter a number in the third place of decimal point");
					//Saiki 20090526 Change ----->
					//sprintf(szMessage, "小数点第3位まで数値を入力してください");
					LoadStringML(IDS_ENTER_THIRD_DECIMAL_POINT, strBuffer, "Please enter a number in the third place of decimal point");
					MessageBox(NULL, strBuffer, NULL, MB_OK|MB_ICONSTOP);
					//Saiki 20090526 Change <-----
					return CHECK_DATA_OUTSIDE_EFFECTIVE_NUMBER;
				}
			}
			break;
		case '.':
			// 「.」が二個ある場合
			if(bCheckDot){
//				sprintf(szMessage, "Please enter a number");
				//Saiki 20090526 Change ----->
				//sprintf(szMessage, "数値を入力してください");
				LoadStringML(IDS_ENTER_NUMBER, strBuffer, "Please enter a number");
				MessageBox(NULL, strBuffer, NULL, MB_OK|MB_ICONSTOP);
				//Saiki 20090526 Change <-----
				return CHECK_DATA_NO_NUMBER;
			}
			bCheckDot = TRUE;
			break;
		default:
//			sprintf(szMessage, "Please enter a number");
			//Saiki 20090526 Change ----->
			//sprintf(szMessage, "数値を入力してください");
			LoadStringML(IDS_ENTER_NUMBER, strBuffer, "Please enter a number");
			MessageBox(NULL, strBuffer, NULL, MB_OK|MB_ICONSTOP);
			//Saiki 20090526 Change <-----
			return CHECK_DATA_NO_NUMBER;
			break;
		}
		iIndex++;
	}

	if(dMin != dMax){
		dData = atof(szBuff);
		if(dData < dMin || dData > dMax){
//			sprintf(szMessage, "Please enter a number between %.3lf and %.3lf", dMin, dMax);
			//Saiki 20090526 Change ----->
			//sprintf(szMessage, "%.3lf から %.3lfの間で数値を入力してください", dMin, dMax);
			LoadStringML(IDS_ENTER_NUMBER_BETWEEN_3DOUBLE, strBuffer, "Please enter a number between %.3lf and %.3lf");
			sprintf(szMessage, strBuffer, dMin, dMax);
			//Saiki 20090526 Change <-----
			MessageBox(NULL, szMessage, NULL, MB_OK|MB_ICONSTOP);
			return CHECK_DATA_OUTSIDE_RANGE;
		}
	}

	return CHECK_DATA_OK;
}

/////////////////////////////////////////////////////////////////////////////
// Name       : CheckMinMaxDouble
// Purpose    : 文字列が指定された範囲の数値であるかをチェック(double型)
//              dMin,dMaxが同じ値(指定しない)の場合は範囲のチェックはしない
// Parameters : strBuff ---> 文字列
//              dMin    ---> 最小値
//              dMax    ---> 最大値
//
// Returns    : CHECK_DATA_OK                       ---> OK
//              CHECK_DATA_STRING_EMPTY             ---> 文字列が空っぽ
//              CHECK_DATA_NO_NUMBER                ---> 文字列が数値ではない
//              CHECK_DATA_OUTSIDE_EFFECTIVE_NUMBER ---> 有効桁数外(小数点第3位まで)
//              CHECK_DATA_OUTSIDE_RANGE            ---> 数値が範囲外
int CheckMinMaxDouble(CString strBuff, double dMin /* = 0 */, double dMax /* = 0 */)
{
	TCHAR szBuffer[256];
	_tcscpy(szBuffer, strBuff);

	return CheckMinMaxDouble(szBuffer, dMin, dMax);
}

/////////////////////////////////////////////////////////////////////////////
// Name       : CheckMinMaxLong
// Purpose    : 文字列が指定された範囲の数値であるかをチェック(long型)
//              lMin,lMaxが同じ値(指定しない)の場合は範囲のチェックはしない
// Parameters : lpszBuff ---> 文字列
//              lMin     ---> 最小値
//              lMax     ---> 最大値
//
// Returns    : CHECK_DATA_OK            ---> OK
//              CHECK_DATA_STRING_EMPTY  ---> 文字列が空っぽ
//              CHECK_DATA_NO_NUMBER     ---> 文字列が数値ではない
//              CHECK_DATA_OUTSIDE_RANGE ---> 数値が範囲外
int CheckMinMaxLong(char* lpszBuff, long lMin /* = 0 */, long lMax /* = 0 */)
{
	char szBuff[256], szMessage[256];
	int iLen, iIndex = 0;
	long lData;
	//Saiki 20090526 Add ----->
	CString strBuffer;
	//Saiki 20090526 Add <-----

	_tcscpy(szBuff, lpszBuff);
	iLen = strlen(szBuff);
	// 文字列が空っぽの場合
	if(iLen <= 0){
//		sprintf(szMessage, "Please enter a integer");
		//Saiki 20090526 Change ----->
		//sprintf(szMessage, "整数値を入力してください");
		LoadStringML(IDS_ENTER_INTEGER, strBuffer, "Please enter a integer");
		MessageBox(NULL, strBuffer, NULL, MB_OK|MB_ICONSTOP);
		//Saiki 20090526 Change <-----
		return CHECK_DATA_STRING_EMPTY;
	}

	while(iIndex < iLen){
		// 一文字目が「-」の場合はそのまま続ける
		if(iIndex == 0 && szBuff[iIndex] == '-'){
			iIndex++;
			continue;
		}

		switch(szBuff[iIndex]){
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
			break;
		default:
//			sprintf(szMessage, "Please enter a integer");
			//Saiki 20090526 Change ----->
			//sprintf(szMessage, "整数値を入力してください");
			LoadStringML(IDS_ENTER_INTEGER, strBuffer, "Please enter a integer");
			MessageBox(NULL, strBuffer, NULL, MB_OK|MB_ICONSTOP);
			//Saiki 20090526 Change <-----
			return CHECK_DATA_NO_NUMBER;
			break;
		}
		iIndex++;
	}

	if(lMin != lMax){
		lData = atol(szBuff);
		if(lData < lMin || lData > lMax){
//			sprintf(szMessage, "Please enter a integer between %ld and %ld", lMin, lMax);
			//Saiki 20090526 Change ----->
			//sprintf(szMessage, "%ld から %ld の間の整数値を入力してください", lMin, lMax);
			LoadStringML(IDS_ENTER_NUMBER_BETWEEN_1DOUBLE, strBuffer, "Please enter a integer between %ld and %ld");
			sprintf(szMessage, strBuffer, lMin, lMax);
			//Saiki 20090526 Change <-----
			MessageBox(NULL, szMessage, NULL, MB_OK|MB_ICONSTOP);
			return CHECK_DATA_OUTSIDE_RANGE;
		}
	}

	return CHECK_DATA_OK;
}

/////////////////////////////////////////////////////////////////////////////
// Name       : CheckMinMaxLong
// Purpose    : 文字列が指定された範囲の数値であるかをチェック(long型)
//              lMin,lMaxが同じ値(指定しない)の場合は範囲のチェックはしない
// Parameters : strBuff ---> 文字列
//              lMin    ---> 最小値
//              lMax    ---> 最大値
//
// Returns    : CHECK_DATA_OK            ---> OK
//              CHECK_DATA_STRING_EMPTY  ---> 文字列が空っぽ
//              CHECK_DATA_NO_NUMBER     ---> 文字列が数値ではない
//              CHECK_DATA_OUTSIDE_RANGE ---> 数値が範囲外
int CheckMinMaxLong(CString strBuff, long lMin /* = 0 */, long lMax /* = 0 */)
{
	TCHAR szBuffer[256];
	_tcscpy(szBuffer, strBuff);

	return CheckMinMaxLong(szBuffer, lMin, lMax);
}

/////////////////////////////////////////////////////////////////////////////
// Name       : CheckMinMaxInt
// Purpose    : 文字列が指定された範囲の数値であるかをチェック(int型)
//              iMin,iMaxが同じ値(指定しない)の場合は範囲のチェックはしない
// Parameters : lpszBuff ---> 文字列
//              iMin     ---> 最小値
//              iMax     ---> 最大値
//
// Returns    : CHECK_DATA_OK            ---> OK
//              CHECK_DATA_STRING_EMPTY  ---> 文字列が空っぽ
//              CHECK_DATA_NO_NUMBER     ---> 文字列が数値ではない
//              CHECK_DATA_OUTSIDE_RANGE ---> 数値が範囲外
int CheckMinMaxInt(char* lpszBuff, int iMin /* = 0 */, int iMax /* = 0 */)
{
	TCHAR szBuffer[256], szMessage[256];
	int iLen, iIndex = 0;
	int iData;
	//Saiki 20090527 Add ----->
	CString strBuffer;
	//Saiki 20090527 Add <-----

	_tcscpy(szBuffer, lpszBuff);
	iLen = _tcslen(szBuffer);
	// 文字列が空っぽの場合
	if(iLen <= 0){
//		sprintf(szMessage, "Please enter a integer");
		//Saiki 20090526 Change ----->
		//sprintf(szMessage, "整数値を入力してください");
		LoadStringML(IDS_ENTER_INTEGER, strBuffer, "Please enter a integer");
		MessageBox(NULL, strBuffer, NULL, MB_OK|MB_ICONSTOP);
		//Saiki 20090526 Change <-----
		return CHECK_DATA_STRING_EMPTY;
	}

	while(iIndex < iLen){
		// 一文字目が「-」の場合はそのまま続ける
		if(iIndex == 0 && szBuffer[iIndex] == '-'){
			iIndex++;
			continue;
		}

		switch(szBuffer[iIndex]){
		case '0':
		case '1':
		case '2':
		case '3':
		case '4':
		case '5':
		case '6':
		case '7':
		case '8':
		case '9':
			break;
		default:
//			sprintf(szMessage, "Please enter a integer");
			//Saiki 20090526 Change ----->
			//sprintf(szMessage, "整数値を入力してください");
			LoadStringML(IDS_ENTER_INTEGER, strBuffer, "Please enter a integer");
			MessageBox(NULL, strBuffer, NULL, MB_OK|MB_ICONSTOP);
			//Saiki 20090526 Change <-----
			return CHECK_DATA_NO_NUMBER;
			break;
		}
		iIndex++;
	}

	if(iMin != iMax){
		iData = atoi(szBuffer);
		if(iData < iMin || iData > iMax){
//			sprintf(szMessage, "Please enter a integer between %d and %d", iMin, iMax);
			//Saiki 20090526 Change ----->
			//sprintf(szMessage, "%ld から %ld の間の整数値を入力してください", lMin, lMax);
			LoadStringML(IDS_ENTER_NUMBER_BETWEEN_1DOUBLE, strBuffer, "Please enter a integer between %ld and %ld");
			sprintf(szMessage, strBuffer, iMin, iMax);
			//Saiki 20090526 Change <-----
			MessageBox(NULL, szMessage, NULL, MB_OK|MB_ICONSTOP);
			return CHECK_DATA_OUTSIDE_RANGE;
		}
	}

	return CHECK_DATA_OK;
}

/////////////////////////////////////////////////////////////////////////////
// Name       : CheckMinMaxInt
// Purpose    : 文字列が指定された範囲の数値であるかをチェック(int型)
//              iMin,iMaxが同じ値(指定しない)の場合は範囲のチェックはしない
// Parameters : strBuff ---> 文字列
//              iMin    ---> 最小値
//              iMax    ---> 最大値
//
// Returns    : CHECK_DATA_OK            ---> OK
//              CHECK_DATA_STRING_EMPTY  ---> 文字列が空っぽ
//              CHECK_DATA_NO_NUMBER     ---> 文字列が数値ではない
//              CHECK_DATA_OUTSIDE_RANGE ---> 数値が範囲外
int CheckMinMaxInt(CString strBuff, int iMin /* = 0 */, int iMax /* = 0 */)
{
	TCHAR szBuffer[256];
	_tcscpy(szBuffer, (PCSTR) strBuff);

	return CheckMinMaxInt(szBuffer, iMin, iMax);
}

/////////////////////////////////////////////////////////////////////////////
// Name     : CheckName
// Purpose  : 入力した名前に禁止文字が入っていないかを確認
// Parameters : lpszName ---> 名前
//
// Returns  : TRUE  ---> OK
//              FALSE ---> 禁止文字が含まれている
BOOL CheckName(char* lpszName, int iLenMax)
{
	int iLen = 0;
	while(*lpszName != '\0'){
//		if(!isascii(*lpszName) || !isgraph(*lpszName)){
//			return FALSE;
//		}


		if(IsDBCSLeadByte(*lpszName) == 0) //先行バイトではない
		{

			switch(*lpszName){
			case '!':
			case '"':
			case '\'':
			case '*':
			case '+':
			case ',':
			case '.':
			case '/':
			case ':':
			case ';':
			case '<':
			case '=':
			case '>':
			case '?':
			case '@':
			case '[':
			case '\\':
			case ']':
			case '^':
			case '`':
			case '|':
			case '~':
			case '$':
			case '&':
			case '(':
			case ')':
				return FALSE;
				break;
			}

		}
		else //先行バイト
		{
			//先行バイトの場合は次の文字も無視する
			lpszName++;
			iLen++;
		}

		lpszName++;
		iLen++;
	}

	if(iLen > iLenMax || iLen <= 0){
		return FALSE;
	}

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////
// Name       : CheckName
// Purpose    : 入力した名前に禁止文字が入っていないかを確認
// Parameters : strName ---> 名前
//
// Returns    : TRUE  ---> OK
//              FALSE ---> 禁止文字が含まれている
BOOL CheckName(CString strName, int iLenMax)
{
	return CheckName(strName.GetBuffer(0), iLenMax);
}
