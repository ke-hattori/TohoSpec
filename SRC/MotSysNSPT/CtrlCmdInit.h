
// 初期化データ
typedef struct {
	int		P501;	// ファームウェア機能 ストロークリミット X+ [count]
	int		P502;	//                                       X-
	int		P503;	//                                       Y+
	int		P504;	//                                       Y-
	int		P505;	//                                       Z+
	int		P506;	//                                       Z+
	int		P507;	//                                       T+
	int		P508;	//                                       T-
	int		P509;	// ソフトウェア機能 ストロークリミット X+ [count]
	int		P510;	//                                     X-
	int		P511;	//                                     Y+
	int		P512;	//                                     Y-
	int		P513;	//                                     Z+
	int		P514;	//                                     Z-
	int		P515;	//                                     T+
	int		P516;	//                                     T-
	int		P517;	// 非常停止 減速時間 X [0.001count/msec2]
	int		P518;	//                   Y
	int		P519;	//                   Z
	int		P520;	//                   T
	int		P521;	// 非常停止 Abort→Kill ウェイト時間
	int		P522;	// インポジションバンド X [1/16count]
	int		P523;	//                      Y
	int		P524;	//                      Z
	int		P525;	//                      T
	int		P526;	// インポジションフィルタ数 X [サイクル]
	int		P527;	//                          Y
	int		P528;	//                          Z
	int		P529;	//                          T
	int		P530;	// 最高加速度 X [0.001count/msec2]
	int		P531;	//            Y
	int		P532;	//            Z
	int		P533;	//            T
	int		P534;	// 位置決め時 最高速度 X [count/msec]
	int		P535;	//                     Y
	int		P536;	//                     Z
	int		P537;	//                     T
	int		P538;	// 原点オフセット X [count]
	int		P539;	//                Y
	int		P540;	//                Z
	int		P541;	//                T
	int		P542;	// 原点復帰実行時 ストロークリミット T+ [count]
	int		P543;	//                                   T-
	int		P544;	// インポジション2バンド X [1/16count]
	int		P545;	//                       Y
	int		P546;	//                       Z
	int		P547;	//                       T
	int		P548;	// インポジション2フィルタ数 X [サイクル]
	int		P549;	//                           Y
	int		P550;	//                           Z
	int		P551;	//                           T
	int		P552;	// 停止命令時 減速時間計算モード X軸
	int		P553;	// 停止命令時 減速度 X軸
	int		P554;	// 停止命令時 減速時間計算モード Y軸
	int		P555;	// 停止命令時 減速度 Y軸
	int		P556;	// 停止命令時 減速時間計算モード Z軸
	int		P557;	// 停止命令時 減速度 Z軸
	int		P558;	// 停止命令時 減速時間計算モード T軸
	int		P559;	// 停止命令時 減速度 T軸
	int		P560;	// 原点復帰 - リミット時 速度係数 X軸
	int		P561;	// 原点復帰 - リミット時 移動距離 X軸
	int		P562;	// 原点復帰 - リミット時 速度係数 Y軸
	int		P563;	// 原点復帰 - リミット時 移動距離 Y軸
	int		P564;	// (システム予約)
	int		P565;	// 原点復帰時 減速時間 Y軸
	int		P566;	// 原点復帰 Y軸 YS軸サーチモード
	int		P567;	// 原点復帰 Y軸 YS軸移動距離
	int		P568;	// 原点復帰 Y軸 YS軸原点位置
	int		P569;	// 原点復帰 Y軸 YS軸補正 安全保護値
	int		P570;
	int		P571;
	int		P572;
	int		P573;
	int		P574;
	int		P575;
	int		P576;
	int		P577;
	int		P578;
	int		P579;
	int		P580;	// ジョイスティック時 X軸 加速時間
	int		P581;	// ジョイスティック時 X軸 S字加速時間
	int		P582;	// ジョイスティック時 X軸 動作速度１
	int		P583;	// ジョイスティック時 X軸 動作速度２
	int		P584;	// ジョイスティック時 X軸 動作速度３
	int		P585;	// ジョイスティック時 Y軸 加速時間
	int		P586;	// ジョイスティック時 Y軸 S字加速時間
	int		P587;	// ジョイスティック時 Y軸 動作速度１
	int		P588;	// ジョイスティック時 Y軸 動作速度２
	int		P589;	// ジョイスティック時 Y軸 動作速度３
	int		P590;	// ジョイスティック時 X軸 動作方向選択
	int		P591;	// ジョイスティック時 Y軸 動作方向選択
	int		P592;	// ジョイスティック時 XY軸 入替え
	int		P593;
	int		P594;
	int		P595;
	int		P596;
	int		P597;
	int		P598;
	int		P599;
} CC_INITDATA_COMMON;

typedef struct {
	int		P600;	// X 軸 Fetal Following Error Limit (I111) [1/16count]
	int		P601;	//      Warning Following Error Limit (I112) [1/16count]
	int		P602;	//      PID Proportional Gain(I130)
	int		P603;	//      PID Derivative Gain(I131)
	int		P604;	//      PID Velocity Feed Forward Gain (I132)
	int		P605;	//      PID Integral Gain (I133)
	int		P606;	//      PID Integration Mode (I134)
	int		P607;	//      PID Acceleration Feed Forward Gain (I135)
	int		P608;	//      PID Notch Filter Coefficient N1 (I136)
	int		P609;	//      PID Notch Filter Coefficient N2 (I137)
	int		P610;	//      PID Notch Filter Coefficient D1 (I138)
	int		P611;	//      PID Notch Filter Coefficient D2 (I139)
	int		P612;	//      Friction Feedforward (I168)
	int		P613;	//      Deadband Gain Factor (I164)
	int		P614;	//      Deadband Gain Size (I165)
	int		P615;	//      Gain2 PID Proportional Gain
	int		P616;	//      Gain2 PID Derivative Gain
	int		P617;	//      Gain2 PID Velocity Feed Forward Gain
	int		P618;	//      Gain2 PID Integral Gain
	int		P619;	//      Gain2 PID Integration Mode
	int		P620;	//      Gain2 PID Acceleration Feed Forward Gain
	int		P621;	//      Gain2 Friction Feedforward
	int		P622;	//      Gain2 Deadband Gain Factor
	int		P623;	//      Gain2 Deadband Gain Size
	int		P624;	//		原点復帰時 Deadband Gain Size
} CC_INITDATA_X;

typedef struct {
	int		P625;	// YM 軸 Fetal Following Error Limit [1/16count]
	int		P626;	//       Warning Following Error Limit [1/16count]
	int		P627;	//       PID Proportional Gain
	int		P628;	//       PID Derivative Gain
	int		P629;	//       PID Velocity Feed Forward Gain
	int		P630;	//       PID Integral Gain
	int		P631;	//       PID Integration Mode
	int		P632;	//       PID Acceleration Feed Forward Gain
	int		P633;	//       PID Notch Filter Coefficient N1
	int		P634;	//       PID Notch Filter Coefficient N2
	int		P635;	//       PID Notch Filter Coefficient D1
	int		P636;	//       PID Notch Filter Coefficient D2
	int		P637;	//       Friction Feedforward
	int		P638;	//       Deadband Gain Factor
	int		P639;	//       Deadband Gain Size
	int		P640;	//       Gain2 PID Proportional Gain
	int		P641;	//       Gain2 PID Derivative Gain
	int		P642;	//       Gain2 PID Velocity Feed Forward Gain
	int		P643;	//       Gain2 PID Integral Gain
	int		P644;	//       Gain2 PID Integration Mode
	int		P645;	//       Gain2 PID Acceleration Feed Forward Gain
	int		P646;	//       Gain2 Friction Feedforward
	int		P647;	//       Gain2 Deadband Gain Factor
	int		P648;	//       Gain2 Deadband Gain Size
	int		P649;	//		 原点復帰時 Deadband Gain Size
} CC_INITDATA_Y1;

typedef struct {
	int		P650;	// YS 軸 Fetal Following Error Limit [1/16count]
	int		P651;	//       Warning Following Error Limit [1/16count]
	int		P652;	//       PID Proportional Gain
	int		P653;	//       PID Derivative Gain
	int		P654;	//       PID Velocity Feed Forward Gain
	int		P655;	//       PID Integral Gain
	int		P656;	//       PID Integration Mode
	int		P657;	//       PID Acceleration Feed Forward Gain
	int		P658;	//       PID Notch Filter Coefficient N1
	int		P659;	//       PID Notch Filter Coefficient N2
	int		P660;	//       PID Notch Filter Coefficient D1
	int		P661;	//       PID Notch Filter Coefficient D2
	int		P662;	//       Friction Feedforward
	int		P663;	//       Deadband Gain Factor
	int		P664;	//       Deadband Gain Size
	int		P665;	//       Gain2 PID Proportional Gain
	int		P666;	//       Gain2 PID Derivative Gain
	int		P667;	//       Gain2 PID Velocity Feed Forward Gain
	int		P668;	//       Gain2 PID Integral Gain
	int		P669;	//       Gain2 PID Integration Mode
	int		P670;	//       Gain2 PID Acceleration Feed Forward Gain
	int		P671;	//       Gain2 Friction Feedforward
	int		P672;	//       Gain2 Deadband Gain Factor
	int		P673;	//       Gain2 Deadband Gain Size
	int		P674;	//		 原点復帰時 Deadband Gain Size
} CC_INITDATA_Y2;

typedef struct {
	int		P675;	// Z 軸 Fetal Following Error Limit [1/16count]
	int		P676;	//      Warning Following Error Limit [1/16count]
	int		P677;	//      PID Proportional Gain
	int		P678;	//      PID Derivative Gain
	int		P679;	//      PID Velocity Feed Forward Gain
	int		P680;	//      PID Integral Gain
	int		P681;	//      PID Integration Mode
	int		P682;	//      PID Acceleration Feed Forward Gain
	int		P683;	//      PID Notch Filter Coefficient N1
	int		P684;	//      PID Notch Filter Coefficient N2
	int		P685;	//      PID Notch Filter Coefficient D1
	int		P686;	//      PID Notch Filter Coefficient D2
	int		P687;	//      Friction Feedforward
	int		P688;	//      Deadband Gain Factor
	int		P689;	//      Deadband Gain Size
	int		P690;	//      Gain2 PID Proportional Gain
	int		P691;	//      Gain2 PID Derivative Gain
	int		P692;	//      Gain2 PID Velocity Feed Forward Gain
	int		P693;	//      Gain2 PID Integral Gain
	int		P694;	//      Gain2 PID Integration Mode
	int		P695;	//      Gain2 PID Acceleration Feed Forward Gain
	int		P696;	//      Gain2 Friction Feedforward
	int		P697;	//      Gain2 Deadband Gain Factor
	int		P698;	//      Gain2 Deadband Gain Size
	int		P699;
} CC_INITDATA_Z;

typedef struct {
	int		P700;	// T 軸 Fetal Following Error Limit [1/16count]
	int		P701;	//      Warning Following Error Limit [1/16count]
	int		P702;	//      PID Proportional Gain
	int		P703;	//      PID Derivative Gain
	int		P704;	//      PID Velocity Feed Forward Gain
	int		P705;	//      PID Integral Gain
	int		P706;	//      PID Integration Mode
	int		P707;	//      PID Acceleration Feed Forward Gain
	int		P708;	//      PID Notch Filter Coefficient N1
	int		P709;	//      PID Notch Filter Coefficient N2
	int		P710;	//      PID Notch Filter Coefficient D1
	int		P711;	//      PID Notch Filter Coefficient D2
	int		P712;	//      Friction Feedforward
	int		P713;	//      Deadband Gain Factor
	int		P714;	//      Deadband Gain Size
	int		P715;	//      Gain2 PID Proportional Gain
	int		P716;	//      Gain2 PID Derivative Gain
	int		P717;	//      Gain2 PID Velocity Feed Forward Gain
	int		P718;	//      Gain2 PID Integral Gain
	int		P719;	//      Gain2 PID Integration Mode
	int		P720;	//      Gain2 PID Acceleration Feed Forward Gain
	int		P721;	//      Gain2 Friction Feedforward
	int		P722;	//      Gain2 Deadband Gain Factor
	int		P723;	//      Gain2 Deadband Gain Size
	int		P724;
	int		P725;
	int		P726;
	int		P727;
	int		P728;
	int		P729;
	int		P730;
	int		P731;
	int		P732;
	int		P733;
	int		P734;
	int		P735;
	int		P736;
	int		P737;
	int		P738;
	int		P739;
	int		P740;
	int		P741;
	int		P742;
	int		P743;
	int		P744;
	int		P745;
	int		P746;
	int		P747;
	int		P748;
	int		P749;
	int		P750;	// X 軸 指令分解能変換係数分子
	int		P751;	// X 軸 指令分解能変換係数分母
	int		P752;	// Y 軸 指令分解能変換係数分子
	int		P753;	// Y 軸 指令分解能変換係数分母
	int		P754;
	int		P755;
	int		P756;
	int		P757;
	int		P758;
	int		P759;
	int		P760;
	int		P761;
	int		P762;
	int		P763;
	int		P764;
	int		P765;
	int		P766;
	int		P767;
	int		P768;
	int		P769;
	int		P770;
	int		P771;
	int		P772;
	int		P773;
	int		P774;
	int		P775;
	int		P776;
	int		P777;
	int		P778;
	int		P779;
	int		P780;
	int		P781;
	int		P782;
	int		P783;
	int		P784;
	int		P785;
	int		P786;
	int		P787;
	int		P788;
	int		P789;
	int		P790;
	int		P791;
	int		P792;
	int		P793;
	int		P794;
	int		P795;
	int		P796;
	int		P797;
	int		P798;
	int		P799;
} CC_INITDATA_T;

#define	CC_INITDATA_PATH	_T("..\\cfg\\")
#define	CC_INITDATA_FILE	_T("CCInitData.dat")

int	CC_InitALL[799 - 501 + 1];
CC_INITDATA_COMMON	CC_InitDataCommon;
CC_INITDATA_X		CC_Initdata_X;
CC_INITDATA_Y1		CC_Initdata_Y1;
CC_INITDATA_Y2		CC_Initdata_Y2;
CC_INITDATA_Z		CC_Initdata_Z;
CC_INITDATA_T		CC_Initdata_T;

