#pragma once

#include "../../INC/Globals.hxx"
#include "../../INC/ConfigFile.hxx"
#include "SeHead.h"


class CSeMeasure
{
public:
	CSeMeasure();
	virtual ~CSeMeasure();
	virtual BOOL InitInstance();
	virtual void ExitInstance();
	virtual BOOL Measure(const MEAS_PROG_INFO* pMeasProgInfo);

	BOOL OpenShutter();
	BOOL CloseShutter();

private:
	CSeHead* m_pSeHead;
};
