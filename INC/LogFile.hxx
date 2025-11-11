#if !defined( _LOGFILE_HXX_ )
#define _LOGFILE_HXX_

#pragma once

#define LOG_FILE_MAX_SIZE						(500000)
#define LOG_FILE_MAX_NUM						(50)
#define LOG_FILE_EXT							_TEXT(".log")

#define CRCHAR									TCHAR('\r') 	/* carriage return */
#define LFCHAR									TCHAR('\n') 	/* line feed */
#define NULLCHAR								TCHAR('\0')
#define SPACECHAR								TCHAR(' ')
#define TABCHAR 								TCHAR('\t')
#define DQUOTECHAR								TCHAR('\"')
#define SLASHCHAR								TCHAR('\\')
#define COLONCHAR								TCHAR(':')
#define COMMACHAR								TCHAR(',')

#define CR										TEXT("\r")
#define LF										TEXT("\n")
#define CRLF									TEXT("\r\n")
#define SPACE									TEXT(" ")
#define TAB 									TEXT("\t")
#define BLANK									TEXT("")

/*
#define PERMIT_FILE_SHARE_WRITE
*/

//===========================================================================
// class CLogFile
//===========================================================================
class CLogFile
{
public:
	CLogFile() {::InitializeCriticalSection(&m_csWait);}
	CLogFile(PSTR pszPathName, PSTR pszFileName, DWORD dwMaxFileSize = LOG_FILE_MAX_SIZE,
		DWORD dwMaxFileNum = LOG_FILE_MAX_NUM, PCSTR pszTitle = NULL);
	~CLogFile();

	void Create(PSTR pszPathName, PSTR pszFileName, DWORD dwMaxFileSize = LOG_FILE_MAX_SIZE,
		DWORD dwMaxFileNum = LOG_FILE_MAX_NUM, PCSTR pszTitle = NULL);
	void Logging(PCSTR pszBuff);
	void LoggingV(PCSTR pszFormat, ...);

	inline void Lock(void)	 {::EnterCriticalSection(&m_csWait);}
	inline void Unlock(void) {::LeaveCriticalSection(&m_csWait);}

	// misc
	BOOL IsExist(DWORD);
	void GetHistory(std::list<std::string>&, UINT);
	class history_node
	{
	private:
		typedef struct node_t {
			PSTR	text;
			node_t* next;
		} node_t;

	public:
		history_node() : head(NULL) {}
		~history_node() {
			while ( head != NULL ) {
				node_t* curr = head;
				head = head->next;
				delete [] curr->text;
				delete curr;
			}
		}

		void add(PCSTR text) {
			node_t* curr = new node_t;
			curr->text = new TCHAR[_tcslen(text) + 1];
			_tcscpy_s(curr->text, _tcslen(text) + 1, text);
			curr->next = head;
			head = curr;
		}
		PSTR find_first(void) {
			first = head;
			return first != NULL ? first->text : NULL;
		}
		PSTR find_next(void) {
			first = first->next;
			return first != NULL ? first->text : NULL;
		}
	private:
		node_t* head;
		node_t* first;
	};
	void GetHistory(history_node* ppHistoryNode, UINT nItemCount);

private:
	TCHAR m_szPathName[MAX_PATH];
	TCHAR m_szFileName[MAX_PATH];
	std::string m_strTitleName;
	DWORD m_dwMaxFileSize;
	DWORD m_dwMaxFileNum;

	BOOL IsOver(PCSTR pszFileName);
	BOOL Rename(DWORD dwFileNum);

	CRITICAL_SECTION m_csWait;
};

#endif // _LOGFILE_HXX_
