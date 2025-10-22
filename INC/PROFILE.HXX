#if !defined( _PROFILE_HXX_ )
#define _PROFILE_HXX_

#pragma once

#include <tchar.h>
#include <string>

#define PARAMS_IGNORED NULL
#define END_OF_PROFILE_TABLE   { PARAMS_IGNORED, PARAMS_IGNORED, PARAMS_IGNORED, PARAMS_IGNORED, PARAMS_IGNORED }

template <class T>
void CopyMemoryEx(T* lhs, CONST T* rhs, UINT num = 1L) { ::CopyMemory(lhs, rhs, sizeof(T) * num); }

class Profile
{
public:
	inline BOOL IsNotEndOfTable(void) const { return pszSection != PARAMS_IGNORED; }
	inline BOOL IsMatchSection(PCSTR pszTarget) const { return _tcscmp(pszTarget, pszSection) == 0; }
	inline BOOL IsEmptySection(void) const { return _tcslen(pszSection) == 0; }

	PCSTR pszSection;
	PCSTR pszKey;
	TCHAR chType;
	PCSTR pszDefault;
	DWORD dwOffset;
	DWORD dwSize;
};

#define PROFILE_MEMBER_OF(s, m) (size_t)&(((s *)0)->m), sizeof(((s *)0)->m)

#define BEGIN_PROFILE_MEMBER_MAP(theClass, name) \
	typedef theClass _Class; \
	static const Profile name[] = \
	{

#define PROFILE_MEMBER_ENTRY(sec, key, type, value, member) \
		{ sec, key, type, value, PROFILE_MEMBER_OF(_Class, member) },

#define END_PROFILE_MEBER_MAP() \
		END_OF_PROFILE_TABLE, \
	};

class TProfile
{
public:
	TProfile(bool sec_offset = true) { this->sec_offset = sec_offset; };
	TProfile(const Profile* const profile_table, PCSTR pszFilePath, bool sec_offset = true)
	{
		Attach(profile_table);
		SetFilePath(pszFilePath);
		this->sec_offset = sec_offset;
	}
	TProfile& operator=(const TProfile& rhs)
	{
		ProfileTable = rhs.ProfileTable;
		filepath	 = rhs.filepath;
		sec_offset	 = rhs.sec_offset;

		return *this;
	}
	BOOL IsExist(PCSTR pszFilePath) { return ::GetFileAttributes(pszFilePath) != 0xFFFFFFFF; }
	BOOL IsExist(void) { return IsExist(filepath.c_str()); }
	BOOL Delete(PCSTR pszFilePath) { return ::DeleteFile(pszFilePath); }
	BOOL Delete(void) { return Delete(filepath.c_str()); }

	virtual TProfile& Attach(const Profile* const profile_table)
	{
		assert(profile_table != NULL);

		ProfileTable = profile_table;

		return *this;
	}

	virtual TProfile& SetFilePath(PCSTR pszFilePath)
	{
		if ( pszFilePath != NULL ) {
			filepath = pszFilePath;
		}

		return *this;
	}

	template <class T>
	void Write(const T* rhs, PCSTR pszSection, const Profile* const profile_table = NULL)
	{
		const Profile* profile = profile_table ? profile_table : &ProfileTable[0];
		DWORD dwOffset = profile->dwOffset;

		for ( ; profile->IsNotEndOfTable(); profile++ ) {
			if ( profile->IsMatchSection(pszSection) ) {
				dwOffset = profile->dwOffset;
				break;
			}
		}

		while ( WriteProfile(rhs, pszSection, profile, dwOffset) ) {
			profile++;
			if ( !profile->IsEmptySection() ) break;
		}
	}

	template <class T>
	void Write(const T* rhs, const Profile* const profile_table = NULL)
	{
		const Profile* profile = profile_table ? profile_table : &ProfileTable[0];
		DWORD dwOffset = sec_offset ? profile->dwOffset : 0L;
		PCSTR pszSection;

		do {
			if ( profile->IsNotEndOfTable() && !profile->IsEmptySection() ) {
				pszSection = profile->pszSection;
			}
		} while ( WriteProfile(rhs, pszSection, profile++, dwOffset) );
	}

	template <class T>
	void Get(const T* rhs, PCSTR pszSection, const Profile* const profile_table = NULL)
	{
		const Profile* profile = profile_table ? profile_table : &ProfileTable[0];
		DWORD dwOffset = profile->dwOffset;

		for ( ; profile->IsNotEndOfTable(); profile++ ) {
			if ( profile->IsMatchSection(pszSection) ) {
				dwOffset = profile->dwOffset;
				break;
			}
		}

		while ( GetProfile(rhs, pszSection, profile, dwOffset) ) {
			profile++;
			if ( !profile->IsEmptySection() ) break;
		}
	}

	template <class T>
	void Get(const T* rhs, const Profile* const profile_table = NULL)
	{
		const Profile* profile = profile_table ? profile_table : &ProfileTable[0];
		DWORD dwOffset = sec_offset ? profile->dwOffset : 0L;
		PCSTR pszSection;

		do {
			if ( profile->IsNotEndOfTable() && !profile->IsEmptySection() ) {
				pszSection = profile->pszSection;
			}
		} while ( GetProfile(rhs, pszSection, profile++, dwOffset) );
	}

	template <class T>
	LPVOID GetAddress(const T* rhs, const Profile* const profile, DWORD dwOffset)
	{
		return LPVOID((PBYTE) rhs + profile->dwOffset - dwOffset);
	}
	template <class T>
	BOOL WriteProfile(const T* rhs, PCSTR pszSection, const Profile* const profile, DWORD dwOffset = 0L)
	{
		assert(!filepath.empty());

		if ( !profile->IsNotEndOfTable() ) return FALSE;

		TCHAR szBuffer[1024 + 1];
		switch ( profile->chType ) {
		case 'b':
			_tcscpy(szBuffer, *(PBOOL) GetAddress(rhs, profile, dwOffset) ? "TRUE" : "FALSE");
			break;
		case 'B':
			_itot(*(PBYTE) GetAddress(rhs, profile, dwOffset), szBuffer, 10);
			break;
		case 'S':
			_itot(*(PSHORT) GetAddress(rhs, profile, dwOffset), szBuffer, 10);
			break;
		case 'W':
			_itot(*(PWORD) GetAddress(rhs, profile, dwOffset), szBuffer, 10);
			break;
		case 'I':
			_itot(*(int*) GetAddress(rhs, profile, dwOffset), szBuffer, 10);
			break;
		case 'U':
			_itot(*(PDWORD) GetAddress(rhs, profile, dwOffset), szBuffer, 10);
			break;
		case 'L':
			_ltot(*(PLONG) GetAddress(rhs, profile, dwOffset), szBuffer, 10);
			break;
		case 'D':
			_ltot(*(PDWORD) GetAddress(rhs, profile, dwOffset), szBuffer, 10);
			break;
		case 'F':
			_stprintf(szBuffer, "%.3f", *(float*) GetAddress(rhs, profile, dwOffset));
			break;
		case 'd':
			_stprintf(szBuffer, "%.3f", *(double*) GetAddress(rhs, profile, dwOffset));
			break;
		case 'C':
			_tcsncpy(szBuffer, (PCSTR) GetAddress(rhs, profile, dwOffset), min(sizeof(szBuffer), profile->dwSize) - 1);
			break;
		default:
			break;
		}

		::WritePrivateProfileString( pszSection,
									 profile->pszKey,
									 szBuffer,
									 filepath.c_str() );

		return TRUE;
	}

	template <class T>
	BOOL GetProfile(const T* rhs, PCSTR pszSection, const Profile* const profile, DWORD dwOffset = 0L)
	{
		assert(!filepath.empty());

		if ( !profile->IsNotEndOfTable() ) return FALSE;

		TCHAR szBuffer[1024 + 1];
		::GetPrivateProfileString( pszSection,
								   profile->pszKey,
								   profile->pszDefault,
								   szBuffer,
								   sizeof(szBuffer) - 1,
								   filepath.c_str() );

		switch ( profile->chType ) {
		case 'b':
			*(PBOOL) GetAddress(rhs, profile, dwOffset) = _tcscmp(szBuffer, "TRUE") == 0 ? TRUE : FALSE;
			break;
		case 'B':
			*(PBYTE) GetAddress(rhs, profile, dwOffset) = (BYTE) _ttoi(szBuffer);
			break;
		case 'S':
			*(PSHORT) GetAddress(rhs, profile, dwOffset) = (SHORT) _ttoi(szBuffer);
			break;
		case 'W':
			*(PWORD) GetAddress(rhs, profile, dwOffset) = (WORD) _ttoi(szBuffer);
			break;
		case 'I':
			*(int*) GetAddress(rhs, profile, dwOffset) = _ttoi(szBuffer);
			break;
		case 'U':
			*(PDWORD) GetAddress(rhs, profile, dwOffset) = (DWORD) _ttoi(szBuffer);
			break;
		case 'L':
			*(PLONG) GetAddress(rhs, profile, dwOffset) = _ttol(szBuffer);
			break;
		case 'D':
			*(PDWORD) GetAddress(rhs, profile, dwOffset) = (DWORD) _ttol(szBuffer);
			break;
		case 'F':
			*(float*) GetAddress(rhs, profile, dwOffset) = (float) atof(szBuffer);
			break;
		case 'd':
			*(double*) GetAddress(rhs, profile, dwOffset) = (double) atof(szBuffer);
			break;
		case 'C':
			_tcscpy((PSTR) GetAddress(rhs, profile, dwOffset), szBuffer);
			break;
		default:
			break;
		}
		return TRUE;
	}
	virtual void Get(void)	 {}
	virtual void Write(void) {}

private:
	const Profile* ProfileTable;
	std::string    filepath;
	bool		   sec_offset;
};

template <class T>
class TProfileData : public TProfile, T
{
public:
	TProfileData() {}
	TProfileData(const Profile* const profile_table, PCSTR pszFilePath, bool sec_offset = true)
		: TProfile(profile_table, pszFilePath, sec_offset) { Get(); }

	operator T*()  { return (T*) this; }
	T* operator&() { return operator T*(); }

public:
	void Get(void)	 { TProfile::Get(operator T*());   }
	void Write(void) { TProfile::Write(operator T*()); }
};

#endif // _PROFILE_HXX_
