/*
  ==============================================================================

    Globals.cpp
    Created: 7 Nov 2023 5:32:04pm
    Author:  Eugene Clewlow

  ==============================================================================
*/

#include "waves/Globals.h"

using namespace waves;

Context context;
UserSettings user_settings;
// SNAPSHOT settings_;
int8_t engine;
int8_t switch_1_test;
int8_t switch_2_test;

// uint8_t effect = EFFECT_TYPE_FM;
// uint8_t sub_osc_shape = SUBOSC_WAVE_RAMP;

Storage storage;

MainMenu mainMenu;
ModeMenu modeMenu;
FxMenu fxMenu;
Oscilloscope oscilloscope;
ControlStatusMenu controlStatusMenu;
DeviceConfigurationMenu deviceConfigurationMenu;
InfoMenu infoMenu;
DisplayConfigurationMenu displayConfigurationMenu;
OscilloscopeConfigurationMenu oscilloscopeConfigurationMenu;
SubOscillatorMenu subOscillatorMenu;
EnterNameMenu enterNameMenu;
WavetableModeMenu wavetableModeMenu;
ABModeMenu abModeMenu;
WaveEditor waveEditor;
LoadWaveMenu loadWaveMenu;
SaveWaveMenu saveWaveMenu;
Popup popup;
MatrixMode matrixMode;
DrumMode drumMode;
IOConfigurationMenu ioConfigurationMenu;
CalibrationMenu calibrationMenu;
QuantizerMenu quantizerMenu;
ManageMenu manageMenu;
SnapshotMenu snapshotMenu;
MemoryClearMenu memoryClearMenu;

ABEngine abEngine;
WavetableEngine wavetableEngine;
MatrixEngine matrixEngine;
DrumEngine drumEngine;
Suboscillator suboscillator;

EffectManager effect_manager;
FM fm;
PhaseDistortion phase_distortion;
RingModulator ring_modulator;
Wavefolder wavefolder;
Wavewrapper wavewrapper;
Bitcrush bitcrush;
Drive drive;
Bypass bypass;

Quantizer quantizer;

// Adc adc;
// SystemClock system_clock;
Flash flash;

int16_t BUF1[2048];
int16_t BUF2[2048];
int16_t BUF3[2048];
int16_t BUF4[2048];
int16_t BUF5[2048];

EEPROM_PIN eeprom[11] = {
  { GPIOA, GPIO_Pin_4  },
  { GPIOA, GPIO_Pin_3  },
  { GPIOA, GPIO_Pin_2  },
  { GPIOA, GPIO_Pin_5  },
  { GPIOA, GPIO_Pin_7  },
  { GPIOA, GPIO_Pin_6  },
  { GPIOA, GPIO_Pin_15 },
  { GPIOB, GPIO_Pin_4  },
  { GPIOC, GPIO_Pin_10 },
  { GPIOC, GPIO_Pin_12 },
  { GPIOC, GPIO_Pin_11 },
};

// int16_t dataBuffer[2048];
// int16_t double_waveframe_buffer_1[4096];
// int16_t double_waveframe_buffer_2[4096];
// int16_t double_waveframe_buffer_3[4096];
// int16_t double_waveframe_buffer_4[4096];
int16_t pump_buffer[6144];


int16_t triple_waveframe_buffer_1[6144];
int16_t triple_waveframe_buffer_2[6144];
int16_t triple_waveframe_buffer_3[6144];
int16_t triple_waveframe_buffer_4[6144];
int16_t triple_waveframe_buffer_5[6144];
int16_t triple_waveframe_buffer_6[6144];
int16_t triple_waveframe_buffer_7[6144];
int16_t triple_waveframe_buffer_8[6144];

int16_t * front_buffer = triple_waveframe_buffer_1;
int16_t * back_buffer = triple_waveframe_buffer_2;

int16_t * front_buffer_1 = triple_waveframe_buffer_1;
int16_t * front_buffer_2 = triple_waveframe_buffer_2;
int16_t * front_buffer_3 = triple_waveframe_buffer_3;
int16_t * front_buffer_4 = triple_waveframe_buffer_4;
int16_t * back_buffer_1 = triple_waveframe_buffer_5;
int16_t * back_buffer_2 = triple_waveframe_buffer_6;
int16_t * back_buffer_3 = triple_waveframe_buffer_7;
int16_t * back_buffer_4 = triple_waveframe_buffer_8;

int16_t * matrix_front_buffer_1 = triple_waveframe_buffer_1;
int16_t * matrix_front_buffer_2 = triple_waveframe_buffer_2;
int16_t * matrix_front_buffer_3 = triple_waveframe_buffer_3;
int16_t * matrix_back_buffer_1 = triple_waveframe_buffer_4;
int16_t * matrix_back_buffer_2 = triple_waveframe_buffer_5;
int16_t * matrix_back_buffer_3 = triple_waveframe_buffer_6;

// int16_t * front_buffer;
// int16_t * back_buffer;

// int16_t * front_buffer_1;
// int16_t * front_buffer_2;
// int16_t * back_buffer_1;
// int16_t * back_buffer_2;

// int16_t * matrix_front_buffer_1;
// int16_t * matrix_front_buffer_2;
// int16_t * matrix_front_buffer_3;
// int16_t * matrix_back_buffer_1;
// int16_t * matrix_back_buffer_2;
// int16_t * matrix_back_buffer_3;

float loading = 0;

uint8_t lcd_buffer[8][128];

uint32_t _EREG_ = 0;

FlagStateTypeDef GetFlag(uint32_t* __registry, uint8_t __flag)
{
  return (*__registry & (1 << __flag)) ? FLAG_SET : FLAG_CLEAR;
}

void SetFlag(uint32_t* __registry, uint8_t __flag, FlagStateTypeDef __state)
{
  if (__state == FLAG_SET)
  {
    *__registry |= (1 << __flag);
  }
  else if (__state == FLAG_CLEAR)
  {
    *__registry &= ~(1 << __flag);
  }
  else
  {
    // Error_Handler("Flag Error!");
  }
}


int16_t ROM[2048]={
6965,3795,289,-822,5137,6893,-4524,-11274,-13643,-6926,5366,4360,-3305,-15965,-20627,-7834,
2179,8556,4190,-10223,-20037,-23181,-11992,166,4342,1491,-7443,-1900,6332,1449,-7387,-18170,
-18635,-3790,5066,5010,498,-6045,1875,9318,4943,-4613,-16941,-12019,7956,11957,4999,-3669,
-10665,-1337,7379,7789,7110,1353,-1599,-683,2511,5063,741,-3735,-4555,-3251,2575,9935,
16479,19123,15977,9174,2787,1408,-2573,-8392,-9015,-4533,6342,16233,16103,12263,10771,13527,
22505,20576,9547,721,-5839,-3802,804,1208,1887,1778,5772,13930,20421,23660,18945,14610,
17294,16857,11489,7021,3018,2985,3858,1107,-895,1300,3750,6209,10077,14281,15974,16564,
18370,20392,21896,21907,19361,16958,14693,11617,7692,1890,-3650,-6997,-7343,-3982,-652,2173,
8853,16782,23461,27613,28319,27510,26680,23930,19256,13948,8334,2118,-3089,-6236,-7496,-6666,
-5437,-4033,-786,3808,8335,12794,16520,20161,24188,25801,24397,21842,18429,14969,11064,5290,
-25,-3627,-5663,-6445,-6474,-6178,-5756,-4784,-2822,244,4905,9287,12962,16532,19607,22029,
23081,21392,18373,14808,10733,6152,688,-5186,-8987,-11022,-11929,-11946,-10882,-8184,-3358,2262,
8009,12992,16901,19698,21526,22796,22273,19377,14919,9370,4629,746,-3204,-6329,-9058,-10046,
-8560,-6887,-5247,-3279,-742,3755,8724,12181,14616,16619,17970,18728,17997,15848,12677,9140,
5606,2265,-876,-3783,-6598,-7582,-7122,-6052,-4551,-2895,-254,4175,8459,11976,14277,15658,
17045,18168,18150,17012,14694,12009,9159,6274,2963,-656,-4084,-6531,-7716,-7544,-6622,-5001,
-2103,2230,7268,12393,16518,19715,22360,24110,24769,23884,21374,17888,13842,9499,4909,171,
-3913,-6677,-7662,-7135,-5613,-3274,-114,3943,8917,13792,18103,21390,23923,25929,27132,27129,
25681,22792,19447,15895,12421,8904,5140,1479,-897,-1899,-1664,-583,1202,4076,8130,12595,
16980,20543,23729,26558,28798,30290,30155,28206,25606,22673,19865,16827,13058,9469,7004,5949,
5988,6409,7210,8713,11098,14392,17765,20656,23006,25004,26832,28277,28332,27142,25467,23831,
22466,20950,18812,16828,15124,14083,13488,13018,12778,13083,14041,15648,17098,18162,18801,19629,
20894,22188,22751,22529,21819,21398,21396,21151,20263,19154,18184,17882,17990,17857,17537,17356,
17219,17365,17494,17337,17147,16939,16736,16667,16514,16141,15783,15661,15957,16542,17054,17390,
17746,18406,19264,19777,19793,19382,18942,18837,18779,18547,18010,17137,16366,15871,15605,15434,
15137,14926,15206,15737,16324,16765,16902,17155,17697,18295,18915,19234,19191,19337,19803,20229,
20230,19646,19033,18799,18790,18602,17989,17289,16946,16899,17070,17013,16695,16552,16957,17814,
18568,18874,19022,19452,20417,21549,22358,22671,22772,22886,23186,23467,23326,22763,22335,22195,
22185,21804,20795,19710,19133,18936,18878,18647,18537,19112,20183,21230,21976,22355,22921,24042,
25245,26241,26858,26925,26944,26944,26808,26523,25860,25085,24574,24408,24471,24262,23826,23492,
23436,23760,23981,23841,23830,23991,24371,24733,24516,24153,24105,24143,24506,24853,25074,25515,
25830,26016,26447,26753,27074,27356,27465,27794,28074,27935,27573,27084,26853,26788,26571,25998,
25118,24506,24272,24091,24066,23878,23722,24076,24537,24975,25660,26142,26568,27268,28005,28821,
29483,29628,29703,29895,30002,29695,29019,28522,28557,28892,29055,28706,28312,28057,27960,27987,
27692,27193,26799,26471,26543,26671,26424,26023,25766,25939,26421,26789,26979,27248,27995,29019,
29773,30142,30233,30302,30687,30985,30921,30644,30125,29717,29485,29057,28517,27757,27007,26609,
26200,25748,25226,24714,24868,25294,25510,25460,25030,24971,25534,26013,26376,26370,26198,26377,
26507,26591,26761,26772,26910,26996,26894,27002,26999,26888,26984,27022,27238,27425,27175,26931,
26736,26603,26398,25698,25075,24686,24567,24860,24792,24646,24856,24851,25004,25290,25447,26030,
26586,26705,26862,26967,26982,26829,26428,26265,26562,26795,26536,25970,25773,26058,26362,26393,
26109,26124,26525,26564,26567,26598,26437,26598,26629,26510,26839,26940,26710,26496,26204,25916,
25522,24949,24592,24907,25613,25677,25059,24319,23883,24244,24603,24303,24096,23992,23867,23784,
23195,22565,22475,22349,22346,22472,22392,22475,22551,22541,22836,23085,23189,23230,23445,24024,
24367,24210,23479,22595,22448,22359,22042,21864,21534,21624,21921,21399,20919,20810,20843,21479,
22049,22232,22574,22625,22461,22509,22618,22842,22944,22853,22913,23076,23448,23549,23306,23421,
23523,23546,23581,23156,23233,23571,23384,23269,22998,22793,23057,22838,22417,22180,21619,20947,
20452,20256,20684,21316,21636,21634,21708,22025,22281,22555,23004,23373,24047,24629,24593,24649,
24619,24380,24465,24270,24070,24172,24026,24031,24219,24263,24214,23691,23077,22811,22855,22887,
22522,22135,21966,21711,21424,20774,20130,20159,20250,20218,20288,20092,20191,20749,21245,21945,
22492,22475,22409,22403,22700,23089,23130,22954,22642,22504,22347,21715,21343,21229,21151,21187,
20529,19567,19109,18594,18230,17902,17236,16822,16547,16241,16187,15909,15435,14785,13981,13624,
13522,13437,13525,13476,13687,14016,13893,13869,14056,14647,15671,16019,16013,16014,15720,15693,
15551,15203,15369,15290,14910,14618,14356,14466,14577,14391,14157,13790,13546,13098,12370,12025,
11785,11478,11226,10638,10371,10692,10722,10513,10022,9251,8986,8808,8756,9424,10168,10664,
10610,10008,9836,10151,10539,10679,10815,11331,11469,11087,10498,9811,9976,10273,9758,9455,
9425,9322,9447,9004,8195,7924,7584,7115,7107,7320,7542,7398,6732,6047,5849,6017,
5807,5663,6148,6392,6434,5928,4874,4943,5586,5897,6412,6595,6731,6976,6500,5750,
5301,5020,4838,4612,4460,4451,4476,4229,3651,3250,3008,2559,2161,1940,1931,2189,
1879,869,446,372,498,991,756,560,697,87,-432,-818,-1009,-579,-317,-260,
-313,-309,-130,-226,-114,72,362,784,592,375,331,74,-213,-715,-1079,-741,
-129,17,275,588,440,395,-148,-737,-304,8,184,463,639,740,475,30,
-486,-275,390,258,155,-113,-758,-1048,-1713,-2394,-2237,-1891,-1633,-1434,-1566,-2098,
-2898,-4250,-5633,-6652,-7459,-7965,-8180,-8001,-7894,-8408,-9551,-11128,-11986,-12204,-12219,-11837,
-11597,-11243,-10767,-11171,-12100,-12713,-13137,-12872,-12104,-11611,-10528,-9508,-9114,-9090,-9760,-10460,
-10867,-11082,-10356,-5901,4755,9162,3823,5818,4502,2271,3319,416,2226,942,2762,1908,
1585,-346,-893,-1409,-2179,-1892,-2247,-1320,-888,-1149,-2722,-3344,-5115,-5276,-5993,-5088,
-4509,-3080,-3014,-3008,-4102,-4993,-6549,-6649,-6809,-5300,-4177,-2974,-3031,-3002,-3890,-3890,
-4182,-3814,-3387,-2705,-2664,-2606,-3169,-3456,-4730,-5609,-6207,-5708,-4637,-3636,-3715,-3715,
-4764,-5825,-7241,-8348,-8920,-8768,-8508,-7914,-7932,-8419,-9067,-9823,-10300,-10271,-10966,-10889,
-10980,-10987,-10743,-11179,-11854,-12103,-12340,-11806,-11322,-10997,-10783,-10714,-10873,-11331,-12120,-12840,
-13427,-13507,-13204,-12622,-12139,-11655,-11784,-12053,-12579,-13642,-14391,-14997,-15295,-14984,-15178,-15448,
-15922,-16514,-16705,-17331,-18299,-19026,-19541,-19229,-19028,-19661,-20539,-21470,-22298,-22747,-23123,-23117,
-22720,-22454,-22727,-23377,-24018,-24797,-25538,-26019,-26333,-26144,-25676,-25658,-25547,-25099,-24960,-24966,
-25410,-26185,-26136,-25704,-25561,-25458,-25558,-25503,-25505,-25584,-25379,-25040,-24393,-24323,-24698,-24385,
-24256,-24194,-24098,-24466,-24100,-23549,-23676,-23769,-23953,-23870,-23154,-22681,-22522,-22517,-22948,-23500,
-24040,-24517,-24541,-24488,-24349,-24102,-23892,-23378,-23502,-24325,-24877,-25536,-25743,-25855,-26566,-26633,
-26392,-26436,-26675,-27742,-28677,-28749,-28617,-28283,-28311,-28946,-29419,-30032,-30673,-30929,-31251,-31237,
-31035,-31031,-30751,-30796,-31168,-31443,-31898,-32004,-32116,-32664,-32768,-32628,-32216,-31753,-32208,-32727,
-32676,-32413,-31667,-31276,-31400,-31278,-31167,-31099,-30977,-30891,-30568,-30221,-29794,-29346,-29183,-28986,
-29051,-29073,-28236,-27427,-27116,-27094,-27457,-27293,-26640,-26374,-26069,-25593,-24959,-24252,-24053,-24183,
-24314,-24490,-24592,-24816,-24771,-24433,-24139,-23577,-23188,-22973,-22671,-23114,-23578,-23517,-23327,-22661,
-22224,-22507,-22622,-22687,-22702,-22578,-22611,-22620,-22655,-22748,-22701,-22644,-22357,-22309,-22719,-22678,
-22509,-22380,-22119,-22453,-22413,-21738,-21598,-21632,-21927,-22401,-22096,-21980,-22300,-22444,-22657,-22449,
-21950,-21746,-21460,-21288,-21262,-21199,-21218,-21045,-20897,-20924,-20699,-20394,-19824,-19358,-19646,-19708,
-19196,-18559,-17659,-17557,-17933,-17486,-17022,-16625,-16163,-16050,-15294,-14177,-13624,-13326,-13212,-12885,
-12207,-11605,-10975,-10662,-10516,-10304,-10074,-9228,-8398,-8315,-8269,-8118,-7388,-6216,-5932,-6194,
-6280,-6323,-6065,-6004,-6343,-6185,-5702,-5434,-5354,-5632,-5901,-5932,-5821,-5702,-5579,-5316,
-5450,-5925,-6005,-6033,-5786,-5490,-5823,-5773,-5293,-5194,-5295,-5858,-6304,-6059,-5862,-5868,
-5907,-5885,-5544,-5415,-5518,-5637,-5793,-5791,-5797,-5689,-5153,-4892,-4986,-5260,-5643,-5268,
-4565,-4326,-4077,-4116,-4100,-3760,-3929,-4098,-3847,-3449,-2721,-2324,-2253,-2081,-1959,-1628,
-1267,-1061,-738,-711,-599,-10,426,684,507,-72,151,887,1495,2034,2172,2414,
2968,2898,2469,2040,1903,2494,3005,3150,3131,2826,2853,3036,3053,3082,2731,2400,
2414,2370,2373,2060,1569,1578,1732,1782,1570,817,313,352,408,240,-434,-1296,
-1702,-1745,-1736,-2202,-3033,-3661,-4077,-4019,-3757,-3965,-4211,-4542,-4927,-4997,-5345,-5890,
-6121,-6176,-5952,-5939,-6349,-6725,-6957,-6986,-7019,-7181,-7296,-7441,-7401,-7294,-7271,-7148,
-7373,-7893,-8079,-8241,-8176,-7958,-8340,-8695,-8709,-8838,-8750,-8891,-9239,-9114,-8889,-8762,
-8787,-9063,-9171,-9237,-9226,-9173,-9371,-9545,-9872,-10206,-9967,-9906,-10143,-10434,-10978,-11135,
-11090,-11657,-12148,-12257,-12147,-11802,-11977,-12722,-13219,-13470,-13483,-13486,-13883,-14400,-14911,-15248,
-15282,-15441,-15747,-16361,-16907,-16859,-16747,-16712,-16891,-17438,-17448,-17299,-17759,-18421,-19255,-19747,
-19436,-19414,-20048,-20911,-21629,-21673,-21389,-21533,-22010,-22485,-22554,-22296,-22248,-22535,-22953,-23286,
-23240,-23070,-23067,-23482,-24225,-24695,-24815,-24718,-24712,-25202,-25530,-25396,-25130,-24946,-25179,-25698,
-25909,-25756,-25488,-25418,-25482,-25670,-25928,-26040,-26227,-26447,-26420,-26565,-26660,-26500,-26570,-26741,
-27009,-27364,-27141,-26717,-26601,-26627,-26690,-26593,-26348,-26355,-26487,-26664,-26757,-26860,-27031,-26907,
-26851,-27135,-27354,-27670,-27648,-27259,-27207,-27072,-26722,-26493,-26276,-26541,-27157,-27434,-27358,-27106,
-26864,-27046,-27329,-27395,-27282,-27072,-27081,-27267,-27257,-26908,-26182,-25699,-25901,-26132,-26082,-25661,
-24916,-24853,-25200,-25199,-24880,-24113,-23532,-23682,-23913,-23880,-23305,-22554,-22309,-22316,-22134,-21625,
-20893,-20477,-20543,-20558,-20278,-19815,-19290,-19084,-19155,-19096,-18671,-18094,-17729,-17781,-18039,-17989,
-17308,-16607,-16439,-16639,-16835,-16388,-15384,-14850,-15024,-15403,-15544,-15205,-14836,-14967,-15174,-15106,
-14983,-14839,-14839,-15128,-15217,-15166,-14917,-14321,-13876,-13770,-13779,-13623,-13316,-13159,-13309,-13635,
-13733,-13500,-13119,-12768,-12463,-12271,-12289,-12233,-12152,-12126,-11881,-11797,-11599,-11056,-10681,-10358,
-10322,-10736,-10851,-10697,-10357,-9841,-9652,-9498,-9241,-9044,-8909,-9108,-9279,-9044,-8450,-7913,
-7739,-7811,-7701,-7087,-6466,-6263,-6235,-6295,-6059,-5512,-5098,-5004,-5261,-5516,-5409,-4840,
-4227,-4379,-4893,-4896,-4244,-3112,-2477,-3006,-3358,-2938,-2172,-1497,-1812,-2613,-2625,-2123,
-1684,-1683,-2276,-2402,-1773,-1140,-732,-976,-1474,-1352,-699,185,578,112,-493,-740,
-401,-134,-500,-840,-619,-32,476,127,-1010,-1574,-1439,-1007,-552,-955,-1320,-972,
-654,-370,-486,-1021,-1081,-1149,-1389,-1313,-1276,-1416,-1586,-1831,-1869,-1905,-2331,-2977,
-3006,-2236,-1630,-1567,-1845,-2123,-1909,-1846,-2573,-3223,-3282,-3396,-3357,-3639,-4146,-3843,
-3782,-4317,-4485,-4463,-3995,-3427,-3696,-4078,-3754,-3337,-3213,-3510,-4052,-4185,-4050,-4381,
-5145,-5736,-5801,-5497,-5142,-5374,-5729,-5824,-5704,-5265,-5063,-5280,-5678,-6214,-6460,-6294,
-6225,-6751,-7367,-7173,-6537,-6591,-7806,-9862,-10355,-8393,-6807,-6629,-7765,-9651,-9633,-8711,
-8493,-8303,-8695,-9065,-9019,-9540,-10251,-10745,-11291,-11656,-11635,-11483,-11869,-12792,-13258,-12762,
-11391,-10884,-12514,-13883,-13271,-11933,-10980,-11697,-13736,-14334,-14035,-14477,-14605,-15049,-15164,-14196,
-14230,-14756,-15613,-16678,-16303,-15529,-14756,-14530,-15221,-15118,-14386,-14300,-14875,-16516,-18133,-17913,
-17444,-17094,-16891,-17780,-18425,-18593,-18571,-17826,-17104,-16720,-16648,-16967,-17520,-18876,-20186,-20258,
-19389,-17850,-16756,-17537,-18545,-19629,-21115,-21358,-20677,-19449,-18372,-18383,-19131,-19719,-19793,-19400,
-18830,-18506,-18903,-20675,-21735,-21012,-19563,-17598,-17143,-18682,-20665,-23161,-24206,-22753,-19242,-16812,
-17803,-19409,-20219,-17501,-14187,-15953,-18824,-19388,-18661,-16072,-16324,-19843,-19832,-17478,-12464,-6922,
-6890,-12614,-22058,-21476,-10690,-5588,-5663,-11316,-11653,824,4990,-437,-1865,655,5470,6965,
};