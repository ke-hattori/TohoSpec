// SrHeadDesktop.cpp : Defines the SrHeadDesktop routines.
//

#include "stdafx.h"
#include <time.h>
#include <math.h>
#include "SrHead.h"
#include "..\\..\\INC\\Globals.hxx"
#include "..\\..\\INC\\MeaSys.hxx"

extern HWND g_hNotifyWnd;

//---------------------------------------------------------------------------
// InitInstance
BOOL CSrHeadDesktop::InitInstance()
{
	TRACE(_T("CSrHeadDesktop::InitInstance()\n"));

	return TRUE;
}

//---------------------------------------------------------------------------
// ExitInstance
void CSrHeadDesktop::ExitInstance()
{
	TRACE(_T("CSrHeadDesktop::ExitInstance()\n"));

}

//---------------------------------------------------------------------------
// GetVersion
BOOL CSrHeadDesktop::GetVersion(LPTSTR pszVersion)
{
	TRACE(_T("CSrHeadDesktop::GetVersion()\n"));

	_tcscpy(pszVersion, _T("NJPC025 Simulation"));

	return TRUE;
}

//---------------------------------------------------------------------------
// CcdScan
BOOL CSrHeadDesktop::CcdScan(int ccdScanData[], int iPixels, int iExposure, int iScans/*=1*/, BOOL bProhibitNotify/*=FALSE*/)	 // ccdScanData[iPixels]確保されていること
{
	TRACE(_T("CSrHeadDesktop::CcdScan()\n"));

// @@@ 高速化対応
// 正常リターンコード受信時、メッセージを通知する
	if ( !bProhibitNotify ) {
		if ( g_hNotifyWnd )
			::PostMessage(g_hNotifyWnd, WM_MEAS_RECVDATA, 0, 0);
	}

	ccdScanData[0] = 4860;
	ccdScanData[1] = 4847;
	ccdScanData[2] = 4851;
	ccdScanData[3] = 4850;
	ccdScanData[4] = 4855;
	ccdScanData[5] = 4856;
	ccdScanData[6] = 4860;
	ccdScanData[7] = 4850;
	ccdScanData[8] = 4850;
	ccdScanData[9] = 4848;
	ccdScanData[10] = 4854;
	ccdScanData[11] = 4853;
	ccdScanData[12] = 4852;
	ccdScanData[13] = 4857;
	ccdScanData[14] = 4855;
	ccdScanData[15] = 4856;
	ccdScanData[16] = 4858;
	ccdScanData[17] = 4852;
	ccdScanData[18] = 4852;
	ccdScanData[19] = 4847;
	ccdScanData[20] = 4851;
	ccdScanData[21] = 4857;
	ccdScanData[22] = 4854;
	ccdScanData[23] = 4850;
	ccdScanData[24] = 4854;
	ccdScanData[25] = 4839;
	ccdScanData[26] = 4857;
	ccdScanData[27] = 4852;
	ccdScanData[28] = 4851;
	ccdScanData[29] = 4850;
	ccdScanData[30] = 4859;
	ccdScanData[31] = 4854;
	ccdScanData[32] = 4853;
	ccdScanData[33] = 4865;
	ccdScanData[34] = 4866;
	ccdScanData[35] = 4858;
	ccdScanData[36] = 4854;
	ccdScanData[37] = 4853;
	ccdScanData[38] = 4852;
	ccdScanData[39] = 4852;
	ccdScanData[40] = 4847;
	ccdScanData[41] = 4855;
	ccdScanData[42] = 4854;
	ccdScanData[43] = 4852;
	ccdScanData[44] = 4854;
	ccdScanData[45] = 4854;
	ccdScanData[46] = 4865;
	ccdScanData[47] = 4857;
	ccdScanData[48] = 4860;
	ccdScanData[49] = 4852;
	ccdScanData[50] = 4852;
	ccdScanData[51] = 4850;
	ccdScanData[52] = 4852;
	ccdScanData[53] = 4847;
	ccdScanData[54] = 4857;
	ccdScanData[55] = 4850;
	ccdScanData[56] = 4860;
	ccdScanData[57] = 4860;
	ccdScanData[58] = 4862;
	ccdScanData[59] = 4865;
	ccdScanData[60] = 4865;
	ccdScanData[61] = 4850;
	ccdScanData[62] = 4851;
	ccdScanData[63] = 4858;
	ccdScanData[64] = 4856;
	ccdScanData[65] = 4852;
	ccdScanData[66] = 4853;
	ccdScanData[67] = 4855;
	ccdScanData[68] = 4851;
	ccdScanData[69] = 4843;
	ccdScanData[70] = 4858;
	ccdScanData[71] = 4851;
	ccdScanData[72] = 4855;
	ccdScanData[73] = 4852;
	ccdScanData[74] = 4859;
	ccdScanData[75] = 4859;
	ccdScanData[76] = 4857;
	ccdScanData[77] = 4857;
	ccdScanData[78] = 4854;
	ccdScanData[79] = 4852;
	ccdScanData[80] = 4847;
	ccdScanData[81] = 4857;
	ccdScanData[82] = 4860;
	ccdScanData[83] = 4858;
	ccdScanData[84] = 4850;
	ccdScanData[85] = 4854;
	ccdScanData[86] = 4853;
	ccdScanData[87] = 4852;
	ccdScanData[88] = 4864;
	ccdScanData[89] = 4857;
	ccdScanData[90] = 4843;
	ccdScanData[91] = 4850;
	ccdScanData[92] = 4858;
	ccdScanData[93] = 4852;
	ccdScanData[94] = 4847;
	ccdScanData[95] = 4856;
	ccdScanData[96] = 4855;
	ccdScanData[97] = 4858;
	ccdScanData[98] = 4854;
	ccdScanData[99] = 4860;
	ccdScanData[100] = 4857;
	ccdScanData[101] = 4849;
	ccdScanData[102] = 4847;
	ccdScanData[103] = 4847;
	ccdScanData[104] = 4850;
	ccdScanData[105] = 4850;
	ccdScanData[106] = 4864;
	ccdScanData[107] = 4846;
	ccdScanData[108] = 4855;
	ccdScanData[109] = 4847;
	ccdScanData[110] = 4859;
	ccdScanData[111] = 4852;
	ccdScanData[112] = 4854;
	ccdScanData[113] = 4854;
	ccdScanData[114] = 4865;
	ccdScanData[115] = 4856;
	ccdScanData[116] = 4847;
	ccdScanData[117] = 4851;
	ccdScanData[118] = 4858;
	ccdScanData[119] = 4852;
	ccdScanData[120] = 4864;
	ccdScanData[121] = 4852;
	ccdScanData[122] = 4845;
	ccdScanData[123] = 4847;
	ccdScanData[124] = 4856;
	ccdScanData[125] = 4850;
	ccdScanData[126] = 4853;
	ccdScanData[127] = 4864;
	ccdScanData[128] = 4860;
	ccdScanData[129] = 4856;
	ccdScanData[130] = 4851;
	ccdScanData[131] = 4857;
	ccdScanData[132] = 4852;
	ccdScanData[133] = 4854;
	ccdScanData[134] = 4872;
	ccdScanData[135] = 4865;
	ccdScanData[136] = 4847;
	ccdScanData[137] = 4847;
	ccdScanData[138] = 4858;
	ccdScanData[139] = 4854;
	ccdScanData[140] = 4862;
	ccdScanData[141] = 4860;
	ccdScanData[142] = 4853;
	ccdScanData[143] = 4851;
	ccdScanData[144] = 4865;
	ccdScanData[145] = 4857;
	ccdScanData[146] = 4859;
	ccdScanData[147] = 4860;
	ccdScanData[148] = 4862;
	ccdScanData[149] = 4868;
	ccdScanData[150] = 4872;
	ccdScanData[151] = 4870;
	ccdScanData[152] = 4880;
	ccdScanData[153] = 4873;
	ccdScanData[154] = 4876;
	ccdScanData[155] = 4870;
	ccdScanData[156] = 4877;
	ccdScanData[157] = 4873;
	ccdScanData[158] = 4875;
	ccdScanData[159] = 4870;
	ccdScanData[160] = 4884;
	ccdScanData[161] = 4871;
	ccdScanData[162] = 4877;
	ccdScanData[163] = 4876;
	ccdScanData[164] = 4883;
	ccdScanData[165] = 4880;
	ccdScanData[166] = 4880;
	ccdScanData[167] = 4884;
	ccdScanData[168] = 4884;
	ccdScanData[169] = 4865;
	ccdScanData[170] = 4880;
	ccdScanData[171] = 4881;
	ccdScanData[172] = 4882;
	ccdScanData[173] = 4881;
	ccdScanData[174] = 4880;
	ccdScanData[175] = 4876;
	ccdScanData[176] = 4871;
	ccdScanData[177] = 4882;
	ccdScanData[178] = 4888;
	ccdScanData[179] = 4880;
	ccdScanData[180] = 4881;
	ccdScanData[181] = 4874;
	ccdScanData[182] = 4877;
	ccdScanData[183] = 4937;
	ccdScanData[184] = 4872;
	ccdScanData[185] = 4867;
	ccdScanData[186] = 4880;
	ccdScanData[187] = 4861;
	ccdScanData[188] = 4855;
	ccdScanData[189] = 4865;
	ccdScanData[190] = 4871;
	ccdScanData[191] = 4859;
	ccdScanData[192] = 4874;
	ccdScanData[193] = 4867;
	ccdScanData[194] = 4880;
	ccdScanData[195] = 4860;
	ccdScanData[196] = 4860;
	ccdScanData[197] = 4856;
	ccdScanData[198] = 4875;
	ccdScanData[199] = 4863;
	ccdScanData[200] = 4859;
	ccdScanData[201] = 4862;
	ccdScanData[202] = 4870;
	ccdScanData[203] = 4865;
	ccdScanData[204] = 4859;
	ccdScanData[205] = 4861;
	ccdScanData[206] = 4862;
	ccdScanData[207] = 4858;
	ccdScanData[208] = 4853;
	ccdScanData[209] = 4862;
	ccdScanData[210] = 4847;
	ccdScanData[211] = 4858;
	ccdScanData[212] = 4866;
	ccdScanData[213] = 4860;
	ccdScanData[214] = 4866;
	ccdScanData[215] = 4859;
	ccdScanData[216] = 4868;
	ccdScanData[217] = 4864;
	ccdScanData[218] = 4864;
	ccdScanData[219] = 4862;
	ccdScanData[220] = 4866;
	ccdScanData[221] = 4858;
	ccdScanData[222] = 4868;
	ccdScanData[223] = 4860;
	ccdScanData[224] = 4867;
	ccdScanData[225] = 4863;
	ccdScanData[226] = 4872;
	ccdScanData[227] = 4859;
	ccdScanData[228] = 4855;
	ccdScanData[229] = 4863;
	ccdScanData[230] = 4863;
	ccdScanData[231] = 4865;
	ccdScanData[232] = 4873;
	ccdScanData[233] = 4872;
	ccdScanData[234] = 4867;
	ccdScanData[235] = 4859;
	ccdScanData[236] = 4880;
	ccdScanData[237] = 4868;
	ccdScanData[238] = 4871;
	ccdScanData[239] = 4859;
	ccdScanData[240] = 4855;
	ccdScanData[241] = 4855;
	ccdScanData[242] = 4870;
	ccdScanData[243] = 4871;
	ccdScanData[244] = 4869;
	ccdScanData[245] = 4867;
	ccdScanData[246] = 4873;
	ccdScanData[247] = 4867;
	ccdScanData[248] = 4874;
	ccdScanData[249] = 4864;
	ccdScanData[250] = 4868;
	ccdScanData[251] = 4855;
	ccdScanData[252] = 4875;
	ccdScanData[253] = 4867;
	ccdScanData[254] = 4874;
	ccdScanData[255] = 4864;
	ccdScanData[256] = 4870;
	ccdScanData[257] = 4868;
	ccdScanData[258] = 4871;
	ccdScanData[259] = 4865;
	ccdScanData[260] = 4867;
	ccdScanData[261] = 4863;
	ccdScanData[262] = 4855;
	ccdScanData[263] = 4865;
	ccdScanData[264] = 4872;
	ccdScanData[265] = 4866;
	ccdScanData[266] = 4873;
	ccdScanData[267] = 4872;
	ccdScanData[268] = 4862;
	ccdScanData[269] = 4862;
	ccdScanData[270] = 4869;
	ccdScanData[271] = 4865;
	ccdScanData[272] = 4877;
	ccdScanData[273] = 4872;
	ccdScanData[274] = 4863;
	ccdScanData[275] = 4863;
	ccdScanData[276] = 4870;
	ccdScanData[277] = 4868;
	ccdScanData[278] = 4878;
	ccdScanData[279] = 4873;
	ccdScanData[280] = 4875;
	ccdScanData[281] = 4859;
	ccdScanData[282] = 4871;
	ccdScanData[283] = 4868;
	ccdScanData[284] = 4876;
	ccdScanData[285] = 4868;
	ccdScanData[286] = 4877;
	ccdScanData[287] = 4859;
	ccdScanData[288] = 4862;
	ccdScanData[289] = 4868;
	ccdScanData[290] = 4886;
	ccdScanData[291] = 4880;
	ccdScanData[292] = 4880;
	ccdScanData[293] = 4873;
	ccdScanData[294] = 4874;
	ccdScanData[295] = 4878;
	ccdScanData[296] = 4880;
	ccdScanData[297] = 4882;
	ccdScanData[298] = 4877;
	ccdScanData[299] = 4878;
	ccdScanData[300] = 4881;
	ccdScanData[301] = 4872;
	ccdScanData[302] = 4880;
	ccdScanData[303] = 4874;
	ccdScanData[304] = 4871;
	ccdScanData[305] = 4869;
	ccdScanData[306] = 4879;
	ccdScanData[307] = 4873;
	ccdScanData[308] = 4883;
	ccdScanData[309] = 4875;
	ccdScanData[310] = 4877;
	ccdScanData[311] = 4870;
	ccdScanData[312] = 4871;
	ccdScanData[313] = 4877;
	ccdScanData[314] = 4880;
	ccdScanData[315] = 4875;
	ccdScanData[316] = 4884;
	ccdScanData[317] = 4871;
	ccdScanData[318] = 4876;
	ccdScanData[319] = 4872;
	ccdScanData[320] = 4877;
	ccdScanData[321] = 4875;
	ccdScanData[322] = 4877;
	ccdScanData[323] = 4867;
	ccdScanData[324] = 4876;
	ccdScanData[325] = 4871;
	ccdScanData[326] = 4877;
	ccdScanData[327] = 4877;
	ccdScanData[328] = 4873;
	ccdScanData[329] = 4859;
	ccdScanData[330] = 4880;
	ccdScanData[331] = 4872;
	ccdScanData[332] = 4871;
	ccdScanData[333] = 4869;
	ccdScanData[334] = 4877;
	ccdScanData[335] = 4876;
	ccdScanData[336] = 4874;
	ccdScanData[337] = 4863;
	ccdScanData[338] = 4875;
	ccdScanData[339] = 4880;
	ccdScanData[340] = 4877;
	ccdScanData[341] = 4873;
	ccdScanData[342] = 4867;
	ccdScanData[343] = 4871;
	ccdScanData[344] = 4863;
	ccdScanData[345] = 4870;
	ccdScanData[346] = 4879;
	ccdScanData[347] = 4865;
	ccdScanData[348] = 4873;
	ccdScanData[349] = 4867;
	ccdScanData[350] = 4863;
	ccdScanData[351] = 4867;
	ccdScanData[352] = 4868;
	ccdScanData[353] = 4865;
	ccdScanData[354] = 4874;
	ccdScanData[355] = 4873;
	ccdScanData[356] = 4880;
	ccdScanData[357] = 4859;
	ccdScanData[358] = 4862;
	ccdScanData[359] = 4855;
	ccdScanData[360] = 4867;
	ccdScanData[361] = 4869;
	ccdScanData[362] = 4875;
	ccdScanData[363] = 4863;
	ccdScanData[364] = 4863;
	ccdScanData[365] = 4854;
	ccdScanData[366] = 4868;
	ccdScanData[367] = 4868;
	ccdScanData[368] = 4871;
	ccdScanData[369] = 4868;
	ccdScanData[370] = 4854;
	ccdScanData[371] = 4858;
	ccdScanData[372] = 4863;
	ccdScanData[373] = 4864;
	ccdScanData[374] = 4872;
	ccdScanData[375] = 4865;
	ccdScanData[376] = 4866;
	ccdScanData[377] = 4858;
	ccdScanData[378] = 4863;
	ccdScanData[379] = 4855;
	ccdScanData[380] = 4868;
	ccdScanData[381] = 4864;
	ccdScanData[382] = 4859;
	ccdScanData[383] = 4858;
	ccdScanData[384] = 4863;
	ccdScanData[385] = 4860;
	ccdScanData[386] = 4868;
	ccdScanData[387] = 4869;
	ccdScanData[388] = 4876;
	ccdScanData[389] = 4866;
	ccdScanData[390] = 4872;
	ccdScanData[391] = 4866;
	ccdScanData[392] = 4866;
	ccdScanData[393] = 4855;
	ccdScanData[394] = 4864;
	ccdScanData[395] = 4872;
	ccdScanData[396] = 4874;
	ccdScanData[397] = 4873;
	ccdScanData[398] = 4880;
	ccdScanData[399] = 4862;
	ccdScanData[400] = 4870;
	ccdScanData[401] = 4869;
	ccdScanData[402] = 4874;
	ccdScanData[403] = 4863;
	ccdScanData[404] = 4882;
	ccdScanData[405] = 4863;
	ccdScanData[406] = 4886;
	ccdScanData[407] = 4871;
	ccdScanData[408] = 4882;
	ccdScanData[409] = 4881;
	ccdScanData[410] = 4882;
	ccdScanData[411] = 4875;
	ccdScanData[412] = 4892;
	ccdScanData[413] = 4886;
	ccdScanData[414] = 4888;
	ccdScanData[415] = 4891;
	ccdScanData[416] = 4889;
	ccdScanData[417] = 4879;
	ccdScanData[418] = 4891;
	ccdScanData[419] = 4890;
	ccdScanData[420] = 4897;
	ccdScanData[421] = 4904;
	ccdScanData[422] = 4901;
	ccdScanData[423] = 4899;
	ccdScanData[424] = 4895;
	ccdScanData[425] = 4902;
	ccdScanData[426] = 4914;
	ccdScanData[427] = 4908;
	ccdScanData[428] = 4907;
	ccdScanData[429] = 4909;
	ccdScanData[430] = 4922;
	ccdScanData[431] = 4919;
	ccdScanData[432] = 4926;
	ccdScanData[433] = 4923;
	ccdScanData[434] = 4936;
	ccdScanData[435] = 4929;
	ccdScanData[436] = 4929;
	ccdScanData[437] = 4936;
	ccdScanData[438] = 4930;
	ccdScanData[439] = 4938;
	ccdScanData[440] = 4936;
	ccdScanData[441] = 4937;
	ccdScanData[442] = 4941;
	ccdScanData[443] = 4941;
	ccdScanData[444] = 4950;
	ccdScanData[445] = 4944;
	ccdScanData[446] = 4951;
	ccdScanData[447] = 4948;
	ccdScanData[448] = 4961;
	ccdScanData[449] = 4961;
	ccdScanData[450] = 4959;
	ccdScanData[451] = 4962;
	ccdScanData[452] = 4966;
	ccdScanData[453] = 4962;
	ccdScanData[454] = 4973;
	ccdScanData[455] = 4966;
	ccdScanData[456] = 4971;
	ccdScanData[457] = 4975;
	ccdScanData[458] = 4985;
	ccdScanData[459] = 4986;
	ccdScanData[460] = 4988;
	ccdScanData[461] = 4975;
	ccdScanData[462] = 4986;
	ccdScanData[463] = 4990;
	ccdScanData[464] = 5002;
	ccdScanData[465] = 4990;
	ccdScanData[466] = 4998;
	ccdScanData[467] = 4997;
	ccdScanData[468] = 5001;
	ccdScanData[469] = 4997;
	ccdScanData[470] = 4991;
	ccdScanData[471] = 4991;
	ccdScanData[472] = 5008;
	ccdScanData[473] = 5012;
	ccdScanData[474] = 5010;
	ccdScanData[475] = 5001;
	ccdScanData[476] = 5007;
	ccdScanData[477] = 5007;
	ccdScanData[478] = 5010;
	ccdScanData[479] = 5011;
	ccdScanData[480] = 5023;
	ccdScanData[481] = 5023;
	ccdScanData[482] = 5028;
	ccdScanData[483] = 5019;
	ccdScanData[484] = 5025;
	ccdScanData[485] = 5026;
	ccdScanData[486] = 5034;
	ccdScanData[487] = 5040;
	ccdScanData[488] = 5033;
	ccdScanData[489] = 5023;
	ccdScanData[490] = 5023;
	ccdScanData[491] = 5018;
	ccdScanData[492] = 5023;
	ccdScanData[493] = 5020;
	ccdScanData[494] = 5030;
	ccdScanData[495] = 5030;
	ccdScanData[496] = 5035;
	ccdScanData[497] = 5029;
	ccdScanData[498] = 5036;
	ccdScanData[499] = 5023;
	ccdScanData[500] = 5026;
	ccdScanData[501] = 5020;
	ccdScanData[502] = 5030;
	ccdScanData[503] = 5029;
	ccdScanData[504] = 5033;
	ccdScanData[505] = 5023;
	ccdScanData[506] = 5036;
	ccdScanData[507] = 5027;
	ccdScanData[508] = 5032;
	ccdScanData[509] = 5025;
	ccdScanData[510] = 5033;
	ccdScanData[511] = 5032;
	if ( iPixels == 1024 ) {
		for ( int i = 0; i < 512; i++ )
			ccdScanData[i+512] = ccdScanData[i];
	}

	srand((unsigned)time(NULL));
	for ( int i = 0; i < iPixels; i++ )
		ccdScanData[i] += rand() % 1000;

	return TRUE;
}

//---------------------------------------------------------------------------
// ChangeCcdShutter
BOOL CSrHeadDesktop::ChangeCcdShutter(int iPos)
{
	TRACE(_T("CSrHeadDesktop::ChangeCcdShutter()\n"));

	return TRUE;
}

//---------------------------------------------------------------------------
// OpenUvShutter
BOOL CSrHeadDesktop::OpenUvShutter()
{
	TRACE(_T("CSrHeadDesktop::OpenUvShutter()\n"));

	// シャッターの動作完了報告はありませんので
	// 呼び元側で、タイマーディレイしてください
	return TRUE;
}

//---------------------------------------------------------------------------
// CloseUvShutter
BOOL CSrHeadDesktop::CloseUvShutter()
{
	TRACE(_T("CSrHeadDesktop::CloseUvShutter()\n"));

	// シャッターの動作完了報告はありませんので
	// 呼び元側で、タイマーディレイしてください
	return TRUE;
}

//---------------------------------------------------------------------------
// OpenTransShutter
BOOL CSrHeadDesktop::OpenTransShutter()
{
	TRACE(_T("CSrHeadDesktop::OpenTransShutter()\n"));

	// シャッターの動作完了報告はありませんので
	// 呼び元側で、タイマーディレイしてください
	return TRUE;
}

//---------------------------------------------------------------------------
// CloseTransShutter
BOOL CSrHeadDesktop::CloseTransShutter()
{
	TRACE(_T("CSrHeadDesktop::CloseTransShutter()\n"));

	// シャッターの動作完了報告はありませんので
	// 呼び元側で、タイマーディレイしてください
	return TRUE;
}
