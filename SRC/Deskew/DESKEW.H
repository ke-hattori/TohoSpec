#if !defined( _DESKEW_H_ )
#define _OVERLAY_

#pragma once

#include <cmath>

#if !defined( PI )
#define PI (4 * atan(1))
#endif

class CDeskew
{
public:
	CDeskew() { Clear(); }
	CDeskew(
		const STAGE_COORD& DeskewOriginal1,
		const STAGE_COORD& DeskewOriginal2,
		const STAGE_COORD& DeskewCurrent1,
		const STAGE_COORD& DeskewCurrent2)
	{ Set(DeskewOriginal1, DeskewOriginal2, DeskewCurrent1, DeskewCurrent2); }

	~CDeskew() {}

	void Clear(void)
	{
		// init the member vairables such that if "Set" if not called, any
		// CorrectToCurrent operation will just copy the original position
		// to the current position
		//
		DeltaTheta = 0L;

		DeskewRotate.lX 	= 0;   DeskewRotate.lY	   = 0;
		DeskewTranslate.lX	= 0;   DeskewTranslate.lY  = 0;
		CosTheta			= 1;   SinTheta 		   = 0;
	}

	long GetAngle(
		const STAGE_COORD& DeskewOriginal1,
		const STAGE_COORD& DeskewOriginal2,
		const STAGE_COORD& DeskewCurrent1,
		const STAGE_COORD& DeskewCurrent2)
	{
		// find the angle between the two line defined by the original and current deskew points
		double Theta = GetAngleInRad(DeskewOriginal1, DeskewOriginal2,
									 DeskewCurrent1,  DeskewCurrent2);

		// convert the angle from radians to a long in 0.01 deg
		Theta = Theta / PI * 18000L;
		DeltaTheta = long( Theta > 0 ? Theta + 0.5 : Theta - 0.5 );

		return DeltaTheta;
	}

	double GetAngleInRad(
		const STAGE_COORD& DeskewOriginal1,
		const STAGE_COORD& DeskewOriginal2,
		const STAGE_COORD& DeskewCurrent1,
		const STAGE_COORD& DeskewCurrent2)
	{
		long diff_x, diff_y;

		// find the angle between x_coord and the line defined by the original deskew points
		diff_x = DeskewOriginal2.lX - DeskewOriginal1.lX;
		diff_y = DeskewOriginal2.lY - DeskewOriginal1.lY;
		double Gama;
		if ( diff_x == 0 )
			Gama = (diff_y >= 0 ? PI / 2.0 : -PI / 2.0);
		else
			Gama = atan( (double)diff_y / diff_x );

		// find the angle between x_coord and the line defined by the current deskew points
		diff_x = DeskewCurrent2.lX - DeskewCurrent1.lX;
		diff_y = DeskewCurrent2.lY - DeskewCurrent1.lY;
		double Beta;
		if ( diff_x == 0 )
			Beta = (diff_y >= 0 ? PI / 2.0 : -PI / 2.0);
		else
			Beta = atan( (double)diff_y / diff_x );

		// find the angle between the two line defined by the original and current deskew points
		double angle = Beta - Gama;
		if ( angle > PI / 2.0 )
			angle = -PI + angle;
		else if ( angle < -PI / 2.0 )
			angle = PI + angle;

		return angle;
	}

	void GetOffset(
		const STAGE_COORD& DeskewOriginal1,
		const STAGE_COORD& DeskewOriginal2,
		const STAGE_COORD& DeskewCurrent1,
		const STAGE_COORD& DeskewCurrent2,
			  long* 	   xOff,
			  long* 	   yOff)
	{
		// find the offset in x and y directions
		*xOff = (DeskewOriginal2.lX - DeskewCurrent2.lX +
				 DeskewOriginal1.lX - DeskewCurrent1.lX) / (-2);

		*yOff = (DeskewOriginal2.lY - DeskewCurrent2.lY +
				 DeskewOriginal1.lY - DeskewCurrent1.lY) / (-2);
	}

	void Set(
		const STAGE_COORD& DeskewOriginal1,
		const STAGE_COORD& DeskewOriginal2,
		const STAGE_COORD& DeskewCurrent1,
		const STAGE_COORD& DeskewCurrent2)
	{
		// assign the translation and rotation points
		DeskewRotate	= DeskewOriginal1;
		DeskewTranslate = DeskewCurrent1;

		// find the angle between the two line defined by the original and current deskew points
		double Theta = GetAngleInRad(DeskewOriginal1, DeskewOriginal2,
									 DeskewCurrent1,  DeskewCurrent2);
		CosTheta = cos( Theta );
		SinTheta = sin( Theta );
	}

	void CorrectToCurrent(
		const STAGE_COORD& Original,
			  STAGE_COORD& Current)
	{
		STAGE_COORD Pt = Original;	 // the point to corrent

		// Do translation such that the rotation point is at origin
		Pt.lX -= DeskewRotate.lX;	Pt.lY -= DeskewRotate.lY;

		// Do rotation
		double temp_x = Pt.lX * CosTheta - Pt.lY * SinTheta;
		double temp_y = Pt.lX * SinTheta + Pt.lY * CosTheta;
		Current.lX = long( temp_x > 0 ? temp_x + 0.5 : temp_x - 0.5 );
		Current.lY = long( temp_y > 0 ? temp_y + 0.5 : temp_y - 0.5 );

		// Translate to the current Wafer position
		Current.lX += DeskewTranslate.lX;
		Current.lY += DeskewTranslate.lY;
	}

protected:
	// member variable for deskew for the first time
	long DeltaTheta;  // the angle (0.01 deg) between the prototype wafer and the current wafer

	// member variables for Deskew for the second time
	STAGE_COORD DeskewRotate;		 // the rotation reference point( use the prototype deskew point 1 )
	STAGE_COORD DeskewTranslate;	 // the translate reference point( use the current deskew point 1 )
	double		CosTheta;			   // cosine of the rotation angle
	double		SinTheta;			   // sine of the rotation angle
};

#endif // _DESKEW_H_
