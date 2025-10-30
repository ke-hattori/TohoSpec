/*
 *	include files
 */

#ifdef	MASTER
	#define	_EXTCC
#else
	#define	_EXTCC	extern
#endif

#ifdef __cplusplus
extern "C" {
#endif

/*
 *	define macros
 */

//#define	CC_DEB_BUTTON	1
#define	CC_COM_LOG		FALSE	// �V���A���ʐM �̃��O
#define	CC_INIT_TIMEOUT			5000	// �����ݒ�l�����݊����^�C���A�E�g
#define	CC_SERVO_ONOFF_TIMEOUT	5000	// �T�[�{ ON/OFF �^�C���A�E�g
#define	CC_CLEAR_ALARM_TIMEOUT	5000	// �A���[���N���A �^�C���A�E�g
// ����`(�K���A�Ԃɂ��Ă��������D���ԋ֎~�D�����͕s��)
#define	CC_AXIS_X	0	// X �� (1)
#define	CC_AXIS_Y	1	// Y �� (2)
#define	CC_AXIS_Z	2	// Z �� (3)		(Nano �ł͖��g�p)
#define	CC_AXIS_T	3	// T �� (4)		(Nano �ł͖��g�p)
#define	CC_AXIS_NUM	4	// �ő厲��
// �o�̓|�[�g�r�b�g�ʒu
#define	CC_OUTBIT_SERVO			0	// �T�[�{ ON/OFF
#define	CC_OUTBIT_RUN_X			1	// X �� ���� �J�n/��~
#define	CC_OUTBIT_RUN_Y			2	// Y �� ���� �J�n/��~
#define	CC_OUTBIT_RUN_Z			3	// Z �� ���� �J�n/��~(Nano �ł͖��g�p)
#define	CC_OUTBIT_RUN_T			4	// T �� ���� �J�n/��~(Nano �ł͖��g�p)
#define	CC_OUTBIT_CLEAR_ALARM	5	// �A���[���N���A
#define	CC_OUTBIT_CHANGE_GAIN_X	6	// X �� �Q�C���؊��� hmenjo4 20030801
#define	CC_OUTBIT_CHANGE_GAIN_Y	7	// Y �� �Q�C���؊��� hmenjo4 20030801
#define	CC_OUTBIT_CHANGE_GAIN_Z	8	// Z �� �Q�C���؊��� hmenjo4 20030801(Nano �ł͖��g�p)
#define	CC_OUTBIT_CHANGE_GAIN_T	9	// T �� �Q�C���؊��� hmenjo4 20030801(Nano �ł͖��g�p)
#define	CC_OUTBIT_VAC_VALVE		10	// Vacuum Valve (���g�p)
#define	CC_OUTBIT_VAC_DEST		11	// Vacuum Destructive (���g�p)
#define	CC_OUTBIT_SPARE_12		12	// (�\��)
#define	CC_OUTBIT_SPARE_13		13	// (�\��)
#define	CC_OUTBIT_SPARE_14		14	// (�\��)
#define	CC_OUTBIT_SPARE_15		15	// (�\��)
#define	CC_OUTBIT_JOY_MODE		16	// JOYSTICK ���[�h�I��
#define	CC_OUTBIT_JOY_SPEED1	17	// JOYSTICK ���x�P
#define	CC_OUTBIT_JOY_SPEED2	18	// JOYSTICK ���x�Q
#define	CC_OUTBIT_JOY_SPEED3	19	// JOYSTICK ���x�R
#define	CC_OUTBIT_SPARE_20		20	// (�\��)
#define	CC_OUTBIT_SPARE_21		21	// (�\��)
#define	CC_OUTBIT_SPARE_22		22	// (�\��)
#define	CC_OUTBIT_SPARE_23		23	// (�\��)
#define	CC_OUTBIT_SPARE_24		24	// (�\��)
#define	CC_OUTBIT_SPARE_25		25	// (�\��)
#define	CC_OUTBIT_SPARE_26		26	// (�\��)
#define	CC_OUTBIT_SPARE_27		27	// (�\��)
#define	CC_OUTBIT_SPARE_28		28	// (�\��)
#define	CC_OUTBIT_SPARE_29		29	// (�\��)
#define	CC_OUTBIT_SPARE_30		30	// (�\��)
#define	CC_OUTBIT_SPARE_31		31	// (�\��)

// ���̓|�[�g�r�b�g�ʒu
#define	CC_INBIT_WD				0	// W.D.
#define	CC_INBIT_SERVO			1	// �T�[�{ ON/OFF
#define	CC_INBIT_ALARM_X		2	// X �� �A���[������
#define	CC_INBIT_INP_X			3	// X �� �ʒu���ߊ���
#define	CC_INBIT_ALARM_Y		4	// Y �� �A���[������
#define	CC_INBIT_INP_Y			5	// Y �� �ʒu���ߊ���
#define	CC_INBIT_ALARM_Z		6	// Z �� �A���[������(Nano �ł͖��g�p)
#define	CC_INBIT_INP_Z			7	// Z �� �ʒu���ߊ���(Nano �ł͖��g�p)
#define	CC_INBIT_ALARM_T		8	// T �� �A���[������(Nano �ł͖��g�p)
#define	CC_INBIT_INP_T			9	// T �� �ʒu���ߊ���(Nano �ł͖��g�p)
#define CC_INBIT_INP_VAC		10	// Chuck Vaccum (���g�p)
#define CC_INBIT_INTERLOCK		11	// Alignment Interlock (���g�p)
#define CC_INBIT_INP2_X			12	// X �� �ʒu���ߊ����Q
#define CC_INBIT_INP2_Y			13	// Y �� �ʒu���ߊ����Q
#define CC_INBIT_INP2_Z			14	// Z �� �ʒu���ߊ����Q(Nano �ł͖��g�p)
#define CC_INBIT_INP2_T			15	// T �� �ʒu���ߊ����Q(Nano �ł͖��g�p)
#define CC_INBIT_JOY_STATUS		16	// JOYSTICK ���
#define CC_INBIT_SPARE_17		17	// JOYSTICK ���x�P
#define CC_INBIT_SPARE_18		18	// JOYSTICK ���x�Q
#define CC_INBIT_SPARE_19		19	// JOYSTICK ���x�R
#define CC_INBIT_SPARE_20		20	// (�\��)
#define CC_INBIT_SPARE_21		21	// (�\��)
#define CC_INBIT_SPARE_22		22	// (�\��)
#define CC_INBIT_SPARE_23		23	// (�\��)
#define CC_INBIT_SPARE_24		24	// (�\��)
#define CC_INBIT_SPARE_25		25	// (�\��)
#define CC_INBIT_SPARE_26		26	// (�\��)
#define CC_INBIT_SPARE_27		27	// (�\��)
#define CC_INBIT_SPARE_28		28	// (�\��)
#define CC_INBIT_SPARE_29		29	// (�\��)
#define CC_INBIT_SPARE_30		30	// (�\��)
#define CC_INBIT_RESERVE_31		31	// (�V�X�e���\��)
// �t�@�C����
#define	SERVOPARAMNAME	_T("..\\cfg\\ServoParam.dat")	// �T�[�{�p�����^


/*
 *	define prototypes
 */
//_EXTCC	CWinThread* g_pCCThread;	// Thread pointer
UINT	CC_Main_Loop(LPVOID pParam);
//_EXTCC	void CC_Main_Loop(void);
_EXTCC	DWORD CC_Initialize_Controller(void);
_EXTCC	int CC_ReadCommand(char *SendData, char *RecvData);
_EXTCC	int CC_WriteCommand(char *SendData, char *RecvData);
_EXTCC	BOOL CC_LoadInitData(void);
_EXTCC	DWORD CC_DioOpen(void);
_EXTCC	DWORD CC_DioClose(void);
_EXTCC	DWORD CC_DioInpBit(WORD wRBitNo, BYTE *pucRBitData);
_EXTCC	DWORD CC_DioOutBit(WORD wWBitNo, BYTE ucWBitData);
_EXTCC	DWORD CC_DioInpDWord(DWORD *pdwRData);
_EXTCC	void CC_DioError(void);
_EXTCC	BOOL CC_checkWD(void);
_EXTCC	void CC_MsgReboot(void);
_EXTCC	void CC_PollingAlarm(void);
_EXTCC	WORD CC_PollingStatus(void);
_EXTCC	BOOL CC_ServoONOFF(int on_off);
_EXTCC	void CC_ServoON_Ctrl(void);
_EXTCC	void CC_Initial(void);
_EXTCC	int CC_ReadCommand2(char *SendData, char *RecvData, short DataNum, void *ReadData);
_EXTCC	void CC_ServoOFF_RunOFF(void);
_EXTCC	BOOL CC_LoadServoParam(void);
_EXTCC	BOOL CC_InitServo(LPVOID pParam);
_EXTCC	void CC_ServoFailure(void);
_EXTCC	void SetDefaultData(void);

_EXTCC	long CC_get_true_position(short axis);
_EXTCC	void CC_local_emergency_stop(short axis);
_EXTCC	int CC_motion_set_soft_home(short axis, long soft_home);
_EXTCC	int CC_motion_move_at_speed(short axis, short velocity);
_EXTCC	int CC_motion_move_to_position(short axis, long position, short velocity);
_EXTCC	int CC_motion_move_at_speed_to_dest(short axis, long destination, short velocity);
_EXTCC	int CC_motion_go_home(short axis);
_EXTCC	void CC_X_Manager(void);
_EXTCC	void CC_Y_Manager(void);
_EXTCC	void CC_Z_Manager(void);
_EXTCC	void CC_T_Manager(void);
_EXTCC	void CC_SendMotionMessage(short Axis, short Result);
_EXTCC	void CC_ThetaAdjust(long *position, long pluslimit, long minuslimit);
_EXTCC	long CC_ConvSpeed_Z(long org_speed);
_EXTCC	void CC_AdjustAccel_Y(long Dest, long *AccelTimeT, long *AccelTimeS);
_EXTCC	void CC_ChangeGainAll(BOOL onoff);				// hmenjo4 20030801
_EXTCC	void CC_ChangeGain(short sAxis, DWORD dwGainSel);
_EXTCC	long CC_ConvSpeed_XYT(short Axis, long org_speed);	// hmenjo10 20031009
_EXTCC	short CC_CnvAxisToSPT(short Axis);
_EXTCC	short CC_CnvAxisFromSPT(short Axis);
_EXTCC	BOOL CC_IsJoyMode(void);
_EXTCC	int CC_SetJoyMode(int iMode, int iSpeedSel);
_EXTCC	int CC_GetMaxSpeed(short Axis);
_EXTCC	int CC_GetMotionSize(short Axis);
_EXTCC	int CC_GetElectronicGear(short Axis);
_EXTCC	int CC_GetSoftLimit(short Axis, short signflag);
_EXTCC	int CC_SetJoySpeed(short Axis, int iSpeedSel, short velocity);

_EXTCC	void CC_DispOthers(void);
_EXTCC	void CC_DispFuncParam(short axis, short Speed, long position);
_EXTCC	void mmtest(void);
_EXTCC	void mmtest2(void);
_EXTCC	void mmtestORGX(int mode);
_EXTCC	void mmtestORGXdone(void);
_EXTCC	void mmtestORGY(int mode);
_EXTCC	void mmtestORGYdone(void);
_EXTCC	void mmtestORGZ(int mode);
_EXTCC	void mmtestORGZdone(void);
_EXTCC	void mmtestORGT(int mode);
_EXTCC	void mmtestORGTdone(void);
/* added 2009.11.11 hmenjo MotSys ���^�C���A�E�g ---------- { ---------- */
_EXTCC	int CC_MotionTimeout(int iMode, short wAxis);
_EXTCC	void CC_MotTimeoutCheck(short wAxis, short *wPrc);
/* added 2009.11.11 hmenjo MotSys ���^�C���A�E�g ---------- } ---------- */
/* added 2009.12.14 hmenjo MotSys DIO ���O�ǉ� ---------- { ---------- */
_EXTCC	void CC_DioLog(DWORD dwDi, DWORD dwDo);
/* added 2009.12.14 hmenjo MotSys DIO ���O�ǉ� ---------- } ---------- */


/*
 *	define structures
 */
// ���A���[��
typedef struct {
	BOOL	Axis0;	// ���O : BigX
	BOOL	Axis1;	// ���P : BigY
	BOOL	Axis2;	// ���Q : Z
	BOOL	Axis3;	// ���R : X Slow
	BOOL	Axis4;	// ���S : Theta
	BOOL	Axis5;	// ���T : Level
	BOOL	Axis6;	// ���U : X Fast
	BOOL	Axis7;	// ���V : (T)
} CC_AXISALARM;

// �����
typedef struct {
	WORD	Mode;			// ���샂�[�h
	long	Position;		// �ʒu
	long	Velocity;		// ���x
	long	Current;		// �d���l
	long	Order_Position;	// �w�߈ʒu
	long	Order_Velocity;	// �w�ߑ��x
	long	Order_Current;	// �w�ߓd���l
	WORD	Status;			// ���
	long	d_Position;		// �ʒu�΍� hmenjo5 20030801
} CC_AXIS_INFO;

// �T�[�{�p�����^
typedef struct {
	long	ORG1_AccelTimeT;	// ���_���A ��P����������
	long	ORG1_AccelTimeS;	//				�r������������
	long	ORG1_Speed;			//				�ړ����x
	long	ORG2_AccelTimeT;	//			��Q����������
	long	ORG2_AccelTimeS;	//				�r������������
	long	ORG2_Speed;			//				�ړ����x
	long	ORG3_AccelTimeT;	//			��R����������
	long	ORG3_AccelTimeS;	//				�r������������
	long	ORG3_Speed;			//				�ړ����x
	long	HP_AccelTimeT;		// HP �ړ�	����������
	long	HP_AccelTimeS;		//			�r������������
	long	HP_BaseBand;		//			�ړ��ϔ��f�
	long	HP_Speed;			//			���x
	long	ABS_AccelTimeT;		// ABS �ړ�	����������
	long	ABS_AccelTimeS;		//			�r������������
	long	ABS_Speed;			//			���x
	long	INC_AccelTimeT;		// INC �ړ�	����������
	long	INC_AccelTimeS;		//			�r������������
	long	INC_Speed;			//			���x
} SERVO_PARAM;


/*
 *	define constant values
 */


/*
 *	define variable values
 */
_EXTCC	BOOL	g_CC_CommAbort;							// �V���A���ʐM���f
_EXTCC	BOOL	g_CC_ThreadLoop;						// �X���b�h���[�v�t���O
_EXTCC	BOOL	g_CC_bInitialize;						// �������t���O
_EXTCC	DWORD	g_CC_OutPort;							// DIO �o�̓|�[�g(����)
_EXTCC	DWORD	g_CC_InPort;							// DIO ���̓|�[�g(����)
_EXTCC	BOOL	g_CC_bSetAlarm[CC_AXIS_NUM];			// �A���[���Z�b�g�t���O
_EXTCC	BOOL	g_CC_bClearAlarm[CC_AXIS_NUM];			// �A���[���N���A�t���O
_EXTCC	BOOL	g_CC_ServoONReq;						// �T�[�{ ON �v��
_EXTCC	BOOL	g_CC_ServoOFFReq;						// �T�[�{ OFF �v��
_EXTCC	BOOL	g_CC_ServoONOFFerr;						// �T�[�{�^�C���A�E�g�G���[
_EXTCC	BOOL	g_CC_ComError;							// �ʐM�G���[
_EXTCC	long	g_CC_Count[CC_AXIS_NUM];				// �e���̃J�E���g
_EXTCC	CC_AXIS_INFO	g_CC_AxisInfo[CC_AXIS_NUM + 1];	// �e���̏��
_EXTCC	SERVO_PARAM	ServoParam[CC_AXIS_NUM];			// �T�[�{�p�����^
_EXTCC	long	g_CC_HP_Pos[CC_AXIS_NUM];				// �e���� HP �ʒu
_EXTCC	short	g_CC_Axis_prc[CC_AXIS_NUM];				// �e���̏������
														//		0�F�N���҂�
														//		1�FINP �Ď��O�̑҂�����
														//		2�FINP �҂�
														//		3�F���샂�[�h�̒ʏ�ւ̕ω��҂�
_EXTCC	short	g_CC_Axis_mode[CC_AXIS_NUM];			// �e���̏������[�h
														//		0�F�ʏ�(��~��)
														//		1�F���_���A
														//		2�FHP �ړ�
														//		3�FABS �ړ�
														//		4�FINC �ړ�
_EXTCC	BOOL	g_CC_Axis_HP_Req[CC_AXIS_NUM];			// HP �ړ��v��				(���g�p)

_EXTCC	BOOL	g_CC_Axis_ORG_Req[CC_AXIS_NUM];			// ���_���A�v��
_EXTCC	short	g_CC_Axis_pattern[CC_AXIS_NUM];			// �e���̏����p�^�[��
														//		1 : CC_motion_move_at_speed()
														//		2 : CC_motion_move_to_position()
														//		3 : CC_motion_move_at_speed_to_dest()
														//		4 : CC_motion_go_home()
_EXTCC	long	g_CC_MaxSpeed[CC_AXIS_NUM];				// �ő呬�x�����l

#ifdef __cplusplus
}
#endif
