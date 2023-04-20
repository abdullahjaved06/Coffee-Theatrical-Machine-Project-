/* 
 Compiled by: Devomech
 email      :
 */

//*************************************************************************************************************************

// 1000 motor steps = 11 mm at 800 pulses/rev
// 90.9090909091 steps per mm at 800 pulses/rev

#define X_factor 200
#define Y_factor 400
#define EXTEND 5015

#include <AccelStepper.h>
#include <MultiStepper.h>

bool all_done = 0;
// Mode Selection:
// Mode 1 -> Animation Mode
// Mode 2 -> Correction Mode
#define Mode1 43
#define Mode2 31
#define GO 40

// Limit Switches -- Ball Screw
#define ball_screw_NO 46

// Limit Switches -- X Motor
#define X_NO 42

// Limit Switches -- Y Motor
#define Y_NO 44

// Timer variable for mode selection
unsigned int press_time = 0;
unsigned int release_time = 0;

// Limit Switches Self-Correction Mode
bool sel = 0;
bool self_corr_done = 0;

// Solenoid Actuator
const byte solenoid_sig = 28;

// Water pipes
const byte flow_water = 30;

// ButterFly
const int ATtiny_ctrl = 7;

// Ball screw motor
bool done = 0;
const byte stepPin = 17;
const byte dirPin = 19;

// Empty cups part
const byte LA_pulse = 12;
float t_pulse;
long pos[2];
const byte IN3 = A3;
const byte IN4 = A1;
float DIRECTION = -1;

bool flag_down_movement, flag_up_movement, flag_left_movement, flag_fix_movement, FWRD_movement, BKWD_movement, flag_LA_contract, complete, LA_mov;

// XY motion part
unsigned short i = 0;
unsigned long del = 12000;
long positions[2];
const byte stepper_motor_Y_direction_pin = 9; //33; //6;
const byte stepper_motor_Y_step_pin = 11; //29;       //7;                                                                                                                                                                                   
const byte stepper_motor_X_direction_pin = 15; //4;
const byte stepper_motor_X_step_pin = 13;       //5;
const byte movement=2;
const byte powderMotor_dir = 23;
const byte powderMotor_step = 25;
const byte IN1 = 45;
const byte IN2 = 47;

bool movement_var;
bool flag_path = 0;
bool flag_right = 0;
bool flag_left = 0;

bool assertion = 1;

float var_x=0;
float var_y=0;
//===============================> change the rotation value according to your length <=====================================

float X_right[] = {
0,
0,
0.842997043,
3.434010283,
7.412418731,
10.90616392,
15.63337822,
22.63387649,
26.31466075,
29.84728313,
34.79213898,
39.50144958,
43.3647388
};

float Y_right[] = {
0,
0,
0.005661391,
0.02306209,
0.049780244,
0.073243501,
0.104990477,
0.15200435,
0.176723722,
0.20044807,
0.233656681,
0.26528342,
0.291228457
};

float X_left[] = {
0,
0,
-2.417559124,
-6.441284714,
-9.969468044,
-14.35236511,
-17.77821395,
-23.9840557,
-28.09642466,
//This-33.26501739
//-40.5899407,
//-45.68510099,
//-51.20043716
};

float Y_left[] ={
0,
0,
-0.016235818,
-0.043258312,
-0.066952849,
-0.096387463,
-0.119394743,
-0.161071869,
-0.188689674,
//This-0.223400855
//-0.272593498,
//-0.306811522,
//-0.343851358
};

float X_Axis[]={
0,
0,
0.094650861,
0.177654939,
0.311625231,
0.473824542,
0.664780151,
0.808072351,
1.087631011,
1.279362758,
1.595337118,
1.932666644,
2.223203876,
2.527056583,
2.942373515,
3.255793391,
3.479994511,
3.792738773,
4.079058468,
4.273115937,
4.365245092,
4.520025596,
4.658641488,
4.856837566,
4.934289398,
5.013354259,
5.099563786,
5.256658944,
5.31957159,
5.384991107,
5.459616332,
5.534415532,
5.5687212,
5.608052303,
5.635553792,
5.691712684,
5.718740956,
5.728789186,
5.734766803,
5.736602631,
5.72877007,
5.706218136,
5.683408347,
5.652685857,
5.617494294,
5.589864296,
5.524791453,
5.455985803,
5.413175039,
5.36277438,
5.289439191,
5.231194388,
5.14791998,
5.029237693,
4.871105594,
4.740234708,
4.606279902,
4.369499298,
4.198727651,
3.950390859,
3.632883747,
3.469566285,
3.180830191,
3.049951918,
2.78916331,
2.606456866,
2.334493992,
2.064628857,
1.484264942,
1.170931143,
0.702012521,
0.349997391,
-0.093882619,
-0.507851902,
-1.119921818,
-1.596147681,
-2.076028567,
-2.595692118,
-2.934046214,
-3.501944138,
-4.065397843,
-4.547320252,
-5.004452476,
-5.589697788,
-5.984397285,
-6.619501606,
-7.387439316,
-8.164081571,
-8.813740745,
-9.431940391,
-10.14768494,
-10.71369333,
-11.2846604,
-11.86282173,
-12.59744044,
-13.34383887,
-14.530582,
-14.97901888,
-15.7666522,
-16.63550418,
-17.5689694,
-17.95643084,
-19.00474699,
-19.95998295,
-20.73816271,
-21.10459615,
-21.63942863,
-22.93218139,
-24.16227259,
-25.37512207,
-26.70613688,
-27.50117329,
-28.27464155,
-29.62765549,
-30.58051134,
-31.30066564,
-32.87984359,
-33.44323428,
-34.51787151,
-35.17987375,
-36.01394701,
-36.82591062,
-37.54632496,
-38.34818175,
-39.26958633,
-40.7681576,
-42.07646256,
-43.62803539,
-44.92883996,
-45.86331788,
-47.10403846,
-48.05538027,
-49.78249167,
-50.96067761,
-52.10900691,
-53.09448562,
-54.29826391,
-55.37944751,
-56.46455124,
-57.23929564,
-58.31080282,
-59.00832563,
-60.51275511,
-61.38059582,
-62.73228746,
-63.84764182,
-65.57132135,
-67.05600503,
-68.57470966,
-70.08644149,
-71.42983931,
-72.42550285,
-73.88584433,
-75.36646248,
-76.95061075,
-78.78745804,
-79.69902651,
-81.37054339,
-82.32154889,
-84.29945643,
-85.14315702,
-86.11965284,
-87.11450753,
-88.56096591,
-89.35218826,
-90.37871228,
-91.14813705,
-92.21443556,
-93.15196849,
-94.54172091,
-95.28430047,
-96.49583004,
-97.35567031,
-98.37161478,
-99.57097888,
-100.8826744,
-101.8410195,
-103.4875869,
-104.5743188,
-105.8381334,
-106.9150016,
-108.2000303,
-109.1115093,
-109.9715651,
-110.7193727,
-111.7277458,
-112.598684,
-113.3939403,
-114.1986388,
-115.6510464,
-116.6587244,
-117.8066339,
-118.4914108,
-119.5855542,
-120.1360481,
-120.8444001,
-121.9162741,
-123.1573067,
-123.7916177,
-124.9550825,
-125.6422041,
-126.1937415,
-126.7907734,
-127.3308085,
-127.8021393,
-128.2652142,
-128.7291226,
-129.4656576,
-130.174756,
-130.9096341,
-131.3570039,
-131.9069492,
-132.6206199,
-133.2501592,
-133.6765313,
-134.2347483,
-135.0552016,
-135.7341767,
-136.6225297,
-137.0987066,
-137.7899452,
-138.5306226,
-139.0566509,
-139.6730101,
-140.0824284,
-140.5519595,
-141.4318574,
-142.299988,
-142.8128819,
-143.9846419,
-145.2139529,
-146.1345564,
-147.2204263,
-148.4317669,
-149.7527338,
-150.5830441,
-151.822177,
-152.8241835,
-153.8183371,
-155.0548665,
-155.8064656,
-157.1365088,
-158.0594802,
-158.9075492,
-160.2087496,
-161.6863521,
-163.1980017,
-164.0965364,
-165.6044581,
-167.2323635,
-168.4976816,
-169.4032095,
-170.3610365,
-171.2258642,
-172.2350914,
-173.4806686,
-174.3049118,
-176.2047797,
-177.3109369,
-178.553516,
-179.9112532,
-180.8700045,
-181.6955115,
-182.5999388,
-183.4833823,
-184.7643685,
-187.0688821,
-189.0268757,
-191.4285231,
-193.1696916,
-194.9867473,
-196.3101266,
-197.567223,
-199.0361672,
-200.2050947,
-201.9847006,
-203.6119901,
-205.6849961,
-207.6540986,
-210.3830499,
-213.3027143,
-215.9561887,
-217.6410018,
-219.043111,
-220.8504299,
-222.958414,
-223.9299112,
-225.1691809,
-227.8166693,
-230.7422461,
-233.2089776,
-236.0090456,
-238.8402155,
-241.5599359,
-243.8705322,
-247.0779388,
-248.573078,
-250.148684,
-252.3645455,
-254.7678178,
-257.905701,
-260.7028017,
-262.1827524,
-263.6163058,
-265.6281913,
-267.0057275,
-268.1856518,
-269.9437998,
-271.8796024,
-273.9415474,
-275.5393291,
-276.9873072,
-278.7207947,
-280.2407877,
-282.3834925,
-283.9426901,
-286.2036389,
-287.527398,
-288.6204001,
-290.2338904,
-292.9320097,
-294.5999384,
-296.3327748,
-298.0251446,
-299.6082429,
-301.019244,
-302.0299889,
-303.1170649,
-304.0842911,
-306.1798534,
-307.9291731,
-309.3424079,
-310.9177443,
-311.8492998,
-312.9260413,
-314.1335456,
-315.1385172,
-317.4188262,
-319.6860068,
-322.1244024,
-324.7377146,
-326.1880973,
-327.3085943,
-330.0319553,
-331.7758403,
-334.0946531,
-336.4152927,
-338.6537818,
-340.221164,
-342.0811558,
-344.9161,
-346.2621154,
-347.958895,
-350.6048262,
-352.6370869,
-355.1075211,
-357.2370685,
-360.3506569,
-362.2706462,
-364.6333774,
-366.8113507,
-369.5944583,
-372.435975,
-375.3549716,
-377.2254574,
-379.1365781,
-383.0791562,
-385.8394475,
-388.2646639,
-392.1785042,       
-394.7154559,
-396.8563225,
-399.3282806,
-402.3104268,
-405.7948023,
-408.6055054,
-410.6202216,
-412.4729134,
-415.4863682,
-417.856711,
-419.5227427,
-422.0603264,
-424.0658879,
-426.1853704,
-428.0329488,
-429.9830016,
-431.8672147,
-434.2083495,
-436.1376444,
-439.3711004,
-441.4393814,
-442.7836633,
-445.3830101,
-447.7473744,
-450.6261036,
-453.2869376,
-456.1173805,
-458.698373,
-461.7463061,
-464.4639456,
-468.4684264,
-471.6452256,
-474.0510944,
-476.4549726,
-478.8017461,
-482.5945533,
-484.8592073,
-487.9938904,
-490.4078358,
-492.1272798,
-495.4599189,
-497.580594,
-499.3312224,
-501.9064171,
-504.4569731,
-506.3624803,
-509.2006145,
-512.023249,
-515.1390847,
-517.8786551,
-520.3630761,
-523.2857074,
-525.8129339,
-528.8769586,
-532.9544726,
-535.101711,
-536.5088481,
-538.6249451,
-540.1452839,
-542.7229898,
-544.463067,
-547.2618591,
-549.9113999,
-552.0474751,
-553.9625954,
-555.3322291,
-556.2220625,
-558.0495915,
-559.9829665,
-561.9741388,
-563.8767191,
-565.6737513,
-567.1811723,
-568.3924129,
-569.7253701,
-570.9423433,
-573.0819927,
-574.8445527,
-576.5514943,
-577.8356415,
-579.4629106,
-581.0991215,
-582.753588,
-584.3287902,
-585.5427267,
-586.8015984,
-588.2897854,
-589.339724,
-590.7020311,
-591.5868876,
-593.4903894,
-594.9099412,
-596.3428659,
-597.9560378,
-598.901732,
-599.4212252,
-601.0530589,
-602.4825537,
-603.5606237,
-604.649818,
-605.8496146,
-606.8967892,
-607.5603521,
-608.3034782,
-609.3811459,
-610.2900856,
-611.1604667,
-611.7763316,
-612.2382046,
-612.9950218,
-613.4003547,
-613.8583054,
-614.8431112,
-614.9390091,
-615.8161493,
-615.9174238,
-616.926991,
-616.9566402,
-616.97855,
-617.9704533,
-618.9743341,
-619.8706317,
-620.8380693,
-621.952793,
-622.9323524,
-623.8644859,
-624.8915281,
-625.7588899,
-626.6521967,
-627.9231829,
-628.9855512,
-629.9602652,
-630.9644485,
-631.9411393,
-632.6326268,
-633.41052,
-633.8858928,
-634.6637756,
-635.355227,
-636.0682863,
-636.7597377,
-637.4295812,
-638.3371112,
-638.9853469,
-639.8064454,
-640.4978968,
-641.1836104,
-641.7511524,
-642.6778562,
-643.1988788,
-643.9551538,
-645.1003701,
-646.3104101,
-647.1315086,
-647.8020422,
-648.276725,
-648.9532233,
-649.6812356,
-650.675197,
-651.6691584,
-652.8359826,
-653.5325452,
-654.434964,
-655.6017882,
-656.6314987,
-657.3520246,
-658.5158072
};
float Y_Axis[]={0,
 0,
-1.920936247,
-3.49059863,
-5.863823346,
-8.547914326,
-11.52156076,
-13.65581545,
-17.65470444,
-20.30928464,
-24.5842916,
-29.06758515,
-32.9063679,
-36.93471939,
-42.52015891,
-46.84231027,
-50.01725115,
-54.60224889,
-59.01083172,
-62.14942996,
-63.6919173,
-66.37392085,
-68.88982488,
-72.72449839,
-74.31786385,
-76.01241885,
-77.95278504,
-81.81033377,
-83.50926001,
-85.40270729,
-87.77032842,
-90.46150973,
-91.85075829,
-93.62047281,
-95.0168503,
-98.62175777,
-101.2482069,
-102.7536835,
-104.2545611,
-105.8842002,
-108.5751052,
-111.3389258,
-113.1371275,
-115.0080015,
-116.7523543,
-117.9397577,
-120.3385845,
-122.4811091,
-123.6750554,
-124.9772192,
-126.7137879,
-127.986099,
-129.6740228,
-131.8679038,
-134.4963678,
-136.4764992,
-138.3585846,
-141.4014755,
-143.4150916,
-146.1279008,
-149.2969654,
-150.8190635,
-153.3602698,
-154.4565317,
-156.5504255,
-157.9526102,
-159.952049,
-161.8431947,
-165.6417554,
-167.5609804,
-170.2860038,
-172.2289961,
-174.5682623,
-176.6500656,
-179.5717787,
-181.7299962,
-183.813603,
-185.9763043,
-187.3364173,
-189.5411153,
-191.6394601,
-193.369543,
-194.9595364,
-196.9275323,
-198.214584,
-200.2220877,
-202.5524442,
-204.8101721,
-206.6282634,
-208.3028046,
-210.178222,
-211.6159324,
-213.0278205,
-214.4200772,
-216.1376444,
-217.8270333,
-220.4051839,
-221.3468683,
-222.9600255,
-224.6819111,
-226.4680517,
-227.1908946,
-229.0946542,
-230.7662873,
-232.0858359,
-232.6945336,
-233.5688238,
-235.615162,
-237.478411,
-239.2398029,
-241.0908501,
-242.1574048,
-243.1679713,
-244.8737525,
-246.0293003,
-246.8783583,
-248.6695064,
-249.2856942,
-250.4287574,
-251.1122537,
-251.9514627,
-252.7453409,
-253.4309694,
-254.1737206,
-255.0011518,
-256.2886851,
-257.3551672,
-258.5520103,
-259.4996822,
-260.1495601,
-260.9730105,
-261.574294,
-262.5998529,
-263.2510006,
-263.8480447,
-264.3309003,
-264.883823,
-265.3458758,
-265.7766897,
-266.064062,
-266.433647,
-266.6567808,
-267.0908772,
-267.3117662,
-267.6122541,
-267.8197831,
-268.0674854,
-268.2085734,
-268.2820609,
-268.2820609,
-268.219317,
-268.1340034,
-267.9479137,
-267.6829561,
-267.3117662,
-266.7649437,
-266.4460431,
-265.7766897,
-265.3458758,
-264.3309003,
-263.8480447,
-263.2510006,
-262.5998529,
-261.574294,
-260.9730105,
-260.1495601,
-259.4996821,
-258.5520103,
-257.6728411,
-256.2886851,
-255.5085861,
-254.1737206,
-253.1788126,
-251.9514627,
-250.4287574,
-248.6695064,
-247.3203906,
-244.8737525,
-243.1679713,
-241.0908501,
-239.2398029,
-236.9313841,
-235.2275922,
-233.5688238,
-232.0858359,
-230.0255431,
-228.1897643,
-226.4680517,
-224.6819111,
-221.3468683,
-218.9497346,
-216.1376444,
-214.4200772,
-211.6159324,
-210.178222,
-208.3028046,
-205.4130528,
-201.9956097,
-200.2220877,
-196.9275323,
-194.9595364,
-193.369543,
-191.6394601,
-190.067751,
-188.6916321,
-187.3364173,
-185.9763043,
-183.813603,
-181.7299962,
-179.5717787,
-178.2597643,
-176.6500656,
-174.5682623,
-172.7405237,
-171.5081933,
-169.9025901,
-167.5609804,
-165.6417554,
-163.159312,
-161.8431947,
-159.952049,
-157.9526102,
-156.5504255,
-154.9271143,
-153.8609978,
-152.6506735,
-150.4190561,
-148.2654194,
-147.0160878,
-144.2274427,
-141.4014755,
-139.3529183,
-137.0118373,
-134.4963678,
-131.8679038,
-130.2761461,
-127.986099,
-126.2081852,
-124.5083234,
-122.4811091,
-121.2949266,
-119.2785141,
-117.9397577,
-116.7523543,
-115.0080015,
-113.1371275,
-111.3389258,
-110.3232265,
-108.7044021,
-107.072815,
-105.8842002,
-105.0746669,
-104.2545611,
-103.5451906,
-102.7536835,
-101.8291337,
-101.2482069,
-99.99938093,
-99.32825747,
-98.62175777,
-97.9053812,
-97.43340241,
-97.04891361,
-96.65046206,
-96.28385768,
-95.79118074,
-95.0168503,
-94.46781579,
-93.92596722,
-93.62047281,
-93.37735152,
-93.24792847,
-93.16145057,
-93.10444342,
-93.09238031,
-93.12971613,
-93.22177249,
-93.41732154,
-93.6824806,
-94.17483132,
-94.8576477,
-95.61378227,
-96.15901918,
-96.65046209,
-97.33341316,
-98.19865931,
-98.62175778,
-99.18335813,
-100.4634922,
-102.0010815,
-103.3933386,
-105.0746669,
-106.8785247,
-108.7044021,
-110.3232265,
-112.6667282,
-113.7952493,
-115.0080015,
-116.7523543,
-118.6924081,
-121.2949266,
-123.6750554,
-124.9553211,
-126.2081852,
-127.9860989,
-129.2156599,
-130.2761461,
-131.8679038,
-133.6351034,
-135.5323943,
-137.0118373,
-138.3585846,
-139.9773015,
-141.4014755,
-143.4150916,
-144.883616,
-147.0160878,
-148.2654194,
-149.2969654,
-150.8190635,
-153.3602698,
-154.9271143,
-156.5504255,
-158.130453,
-159.6028614,
-160.9100622,
-161.8431947,
-162.8435294,
-163.7305458,
-165.6417554,
-167.2251967,
-168.4958295,
-169.9025901,
-170.7294683,
-171.6804153,
-172.7405237,
-173.6175656,
-175.589197,
-177.5230032,
-179.5717786,
-181.7299962,
-182.9104064,
-183.813603,
-185.9763043,
-187.3364173,
-189.1142507,
-190.8576954,
-192.5048042,
-193.6375518,
-194.9595364,
-196.9275323,
-197.8419068,
-198.9760712,
-200.7033152,
-201.9956097,
-203.5262491,
-204.8101721,
-206.6282634,
-207.7144936,
-209.0148556,
-210.178222,
-211.6159324,
-213.0278205,
-214.4200772,
-215.2816715,
-216.1376444,
-217.8270333,
-218.9497346,
-219.896236,
-221.3468683,
-222.2377393,
-222.9600255,
-223.7611481,
-224.6819111,
-225.6963631,
-226.4680517,
-226.9963453,
-227.4642846,
-228.1897643,
-228.7302445,
-229.0946542,
-229.6257621,
-230.0255431,
-230.4294145,
-230.7662873,
-231.1069103,
-231.4218382,
-231.7942423,
-232.0858359,
-232.5445849,
-232.8189849,
-232.9896041,
-233.3027156,
-233.5688238,
-233.8696102,
-234.1257853,
-234.3762878,
-234.585775,
-234.8109139,
-234.9922074,
-235.2275922,
-235.3887843,
-235.4966331,
-235.5927159,
-235.6757253,
-235.7884537,
-235.8437497,
-235.9062502,
-235.9438037,
-235.965216,
-235.9947164,
-236.0056499,
-236.0103102,
-236.0103102,
-236.002649,
-235.9921943,
-235.9695174,
-235.9390597,
-235.8969776,
-235.8532044,
-235.8084424,
-235.7500839,
-235.6950286,
-235.6230444,
-235.519292,
-235.4614418,
-235.4224586,
-235.362373,
-235.3182076,
-235.2416216,
-235.1888382,
-235.1023599,
-235.0189759,
-234.9508755,
-234.8892905,
-234.8450112,
-234.8161625,
-234.7567698,
-234.6938193,
-234.6289788,
-234.5671257,
-234.5088889,
-234.4602378,
-234.4213126,
-234.3786783,
-234.3399648,
-234.2724598,
-234.2174612,
-234.1647896,
-234.1255839,
-234.0764701,
-234.0277835,
-233.9793261,
-233.9339673,
-233.8995611,
-233.8644136,
-233.823596,
-233.7952963,
-233.7592164,
-233.7361808,
-233.6877344,
-233.6526259,
-233.618106,
-233.5803874,
-233.5588549,
-233.5472128,
-233.5115189,
-233.4813683,
-233.4593376,
-233.4377106,
-233.4146384,
-233.3951596,
-233.3831404,
-233.3699834,
-233.3514813,
-233.336417,
-233.3224637,
-233.3128734,
-233.3058365,
-233.294597,
-233.2887273,
-233.2822226,
-233.2686948,
-233.2674114,
-233.2559537,
-233.2546637,
-233.2421792,
-233.2418229,
-233.24156,
-233.2300001,
-233.2189879,
-233.2097478,
-233.2004087,
-233.1904748,
-233.1824855,
-233.1755336,
-233.1686169,
-233.1633888,
-233.1585972,
-233.1528279,
-233.1489606,
-233.1461858,
-233.144109,
-233.1428572,
-233.1424328,
-233.142363,
-233.142363,
-233.142363,
-233.142363,
-233.142363,
-233.142363,
-233.142363,
-233.142363,
-233.142363,
-233.142363,
-233.142363,
-233.142363,
-233.142363,
-233.142363,
-233.142363,
-233.142363,
-233.142363,
-233.142363,
-233.142363,
-233.142363,
-233.142363,
-233.142363,
-233.142363,
-233.142363,
-233.142363,
-233.142363,
-233.142363,
-233.142363,
-233.142363,
-233.142363,
-233.142363,
-233.142363};

AccelStepper stepper_X_Axis(AccelStepper::DRIVER, stepper_motor_X_step_pin, stepper_motor_X_direction_pin);
AccelStepper stepper_Y_Axis(AccelStepper::DRIVER, stepper_motor_Y_step_pin, stepper_motor_Y_direction_pin);
MultiStepper steppers;

void setup()
{  
  Serial.begin(9600);
  pinMode(movement, INPUT_PULLUP);
  pinMode(IN1, OUTPUT);
  pinMode(IN2, OUTPUT);
  pinMode(IN3, OUTPUT);
  pinMode(IN4, OUTPUT);
  pinMode(stepPin, OUTPUT); 
  pinMode(dirPin, OUTPUT);
  pinMode(ATtiny_ctrl, OUTPUT);
  pinMode(flow_water, OUTPUT);
  pinMode(solenoid_sig, OUTPUT);
  pinMode(powderMotor_dir, OUTPUT);
  pinMode(powderMotor_step, OUTPUT);
  pinMode(ball_screw_NO, INPUT_PULLUP);
  pinMode(X_NO, INPUT_PULLUP);
  pinMode(Y_NO, INPUT_PULLUP);
  pinMode(GO, INPUT_PULLUP);
  pinMode(Mode1, INPUT_PULLUP);
  pinMode(Mode2, INPUT_PULLUP);
  
  digitalWrite(IN1, LOW);
  digitalWrite(IN2, LOW);
  digitalWrite(IN3, LOW);
  digitalWrite(IN4, LOW);
  digitalWrite(stepPin, LOW);
  digitalWrite(dirPin, LOW);
  digitalWrite(ATtiny_ctrl, LOW);
  digitalWrite(powderMotor_dir, LOW);
  digitalWrite(powderMotor_step, LOW);
  digitalWrite(flow_water, HIGH);
  digitalWrite(solenoid_sig, HIGH);
  
  stepper_X_Axis.setMaxSpeed(8000);
  stepper_Y_Axis.setMaxSpeed(10000);

  steppers.addStepper(stepper_X_Axis);
  steppers.addStepper(stepper_Y_Axis);

  flag_path = 0;
  flag_left = 0;
  flag_right = 0;
  done = 0;

  assertion = (sizeof(X_Axis) == sizeof(Y_Axis)) && (sizeof(X_right) == sizeof(Y_right)) && (sizeof(X_left) == sizeof(Y_left));
  if(assertion == 0)
  {
    Serial.print("Assertion Failed!");
    while(1);
  }
}

void loop()    
{
  //delay(6000);
  digitalWrite(dirPin, HIGH);
  if (done == 0)
  {
    //for(long ii=0;ii<17640; ii++)
//    for(;;)
//    {
//      digitalWrite(stepPin,HIGH); 
//      delayMicroseconds(550);    // by changing this time delay between the steps we can change the rotation speed
//      digitalWrite(stepPin,LOW); 
//      delayMicroseconds(550);
//      if(digitalRead(ball_screw_NO) == LOW)
//      {
//        break;
//      }
//    }
    Serial.println("Done screw position");
  }
  Serial.println("Waiting for Mode selection ...");
  while(1)
  {
    if(digitalRead(Mode1) == LOW)
    {
      Serial.println("Entering Animation Mode");
      break;
    }
    else if(digitalRead(Mode2) == LOW)
    {
      Serial.println("Entering Self Correction Mode");
      self_correction();
      delay(100);
      Serial.println("Finished Self Correction");
      break;
    }
    else if((digitalRead(Mode1) == LOW && digitalRead(Mode2) == LOW) || (digitalRead(Mode1) == HIGH && digitalRead(Mode2) == HIGH))
    {
      continue;
    }
  }

  while(digitalRead(movement) == HIGH);
  
  delay(3000);
  if(digitalRead(movement) == LOW && done == 0)
  {
    digitalWrite(solenoid_sig, LOW);
    delay(500);
    digitalWrite(solenoid_sig, HIGH);
  }
  done = 1;
  self_corr_done = 1;
  for(byte iter=0; iter<2; iter++)
  {
    Serial.print("Animation Mode ON");
    var_x = 0;
    var_y = 0;
    movement_var = digitalRead(movement);
    while(movement_var == 1);
    while(movement_var == 0 && (flag_path == 0 || flag_left == 0 || flag_right == 0))
    {  
      movement_var = digitalRead(movement);
  
      
      // FOR RIGHT MOVEMENT
      if(movement_var == 0 && flag_right == 0 && flag_left == 0 && flag_path == 0)
      {
        for(i=1;i<13;i++)
        {
          var_x = (X_right[i]-X_right[i-1]);
          var_y = (Y_right[i]-Y_right[i-1]);
          movement_var = digitalRead(movement);
          //positions[0] = -var_x*25;
          //positions[1] = -var_y*25;
          positions[0] = -var_x*X_factor;
          positions[1] = -var_y*Y_factor;
          steppers.moveTo(positions);
          steppers.runSpeedToPosition();
    
          positions[0]=0;
          positions[1]=0;
        
          stepper_Y_Axis.setCurrentPosition(0);
          stepper_X_Axis.setCurrentPosition(0);
  
          if(i==12)
          {
            positions[0]=0;
            positions[1]=0;
            var_x=0;
            var_y=0;
    
            stepper_Y_Axis.setCurrentPosition(0);
            stepper_X_Axis.setCurrentPosition(0);
  
            // Send a 40ms pulse to Integrated powder motor to rotate by 30 deg.
            digitalWrite(powderMotor_dir, HIGH);
            for(int o=0;o<900;o++)
            {
              digitalWrite(powderMotor_step, HIGH);
              delayMicroseconds(700);
              digitalWrite(powderMotor_step, LOW);
              delayMicroseconds(700);
            }

            delay(1000);
            
            Serial.print("  i break ");
            break;
          }
          
          if(movement_var==1)
          {
            Serial.println("Break");
            stepper_Y_Axis.stop();
            stepper_X_Axis.stop();
            //flag_right=1;
            break;
          }
        }

        delay(2000);
      
        if(i==12 && movement_var == 0)
        {
           for(i=11;i>=1;i--)
           {
             var_x=(X_right[i]-X_right[i+1]);
             var_y=(Y_right[i]-Y_right[i+1]);
             movement_var = digitalRead(movement);
             //positions[0] = -var_x*25;
             //positions[1] = -var_y*25;
             positions[0] = -var_x*X_factor;
             positions[1] = -var_y*Y_factor;
             steppers.moveTo(positions);
             steppers.runSpeedToPosition();
             positions[0]=0;
             positions[1]=0;
           
             stepper_Y_Axis.setCurrentPosition(0);
             stepper_X_Axis.setCurrentPosition(0); 
  
             if (movement_var==1)
              {
                 Serial.println("Break");               
                 stepper_Y_Axis.stop();
                 stepper_X_Axis.stop();
                 //flag_right = 1;
                 break;
              }
           }
           flag_right = 1;
        }
      }
  
  
      // FOR LEFT MOVEMENT
      if(movement_var == 0 && flag_right == 1 && flag_left == 0 && flag_path == 0)
      {
        for(i=1;i<9;i++)
        {
          var_x = (X_left[i]-X_left[i-1]);
          var_y = (Y_left[i]-Y_left[i-1]);
          movement_var = digitalRead(movement);
          //positions[0] = -var_x*25;
          //positions[1] = -var_y*25;
          positions[0] = -var_x*X_factor;
          positions[1] = -var_y*Y_factor;
          steppers.moveTo(positions);
          steppers.runSpeedToPosition();
    
          positions[0]=0;
          positions[1]=0;
        
          stepper_Y_Axis.setCurrentPosition(0);
          stepper_X_Axis.setCurrentPosition(0);
  
          if(i==8)
          {
            positions[0]=0;
            positions[1]=0;
            var_x=0;
            var_y=0;
    
            stepper_Y_Axis.setCurrentPosition(0);
            stepper_X_Axis.setCurrentPosition(0);
  
            // Send a 40ms pulse to Integrated powder motor to rotate by 30 deg.
            digitalWrite(powderMotor_dir, HIGH);
            for(int o=0;o<900;o++)
            {
              digitalWrite(powderMotor_step, HIGH);
              delayMicroseconds(700);
              digitalWrite(powderMotor_step, LOW);
              delayMicroseconds(700);
            }
  
            delay(1000);
            
            Serial.print("  i break ");
            break;
          }
          
          if(movement_var==1)
          {
            Serial.println("Break");
            stepper_Y_Axis.stop();
            stepper_X_Axis.stop();
            //flag_left=1;
            break;
          }
        }
      
        if(i==8 && movement_var == 0)
        {
           for(i=7;i>=1;i--)
           {
             var_x=(X_left[i]-X_left[i+1]);
             var_y=(Y_left[i]-Y_left[i+1]);
  //           Serial.println(var_x);
  //           Serial.println(var_y);
  //           Serial.println();
             movement_var = digitalRead(movement);
             //positions[0] = -var_x*25;
             //positions[1] = -var_y*25;
             positions[0] = -var_x*X_factor;
             positions[1] = -var_y*Y_factor;
             steppers.moveTo(positions);
             steppers.runSpeedToPosition();
             positions[0]=0;
             positions[1]=0;
           
             stepper_Y_Axis.setCurrentPosition(0);
             stepper_X_Axis.setCurrentPosition(0);
  
             if (movement_var==1)
              {
                 Serial.println("Break");               
                 stepper_Y_Axis.stop();
                 stepper_X_Axis.stop();
                 //flag_left = 1;
                 break;
              }
           }
           flag_left = 1;
        }
      }
  
  
      
  
      // FOR THE SPIRAL TRAJECTORY
      if(movement_var == 0 && flag_path == 0 && flag_right == 1 && flag_left == 1)
      {
        for(i=1;i<537;i++)
        {
          var_x = (X_Axis[i]-X_Axis[i-1]);
          var_y = (Y_Axis[i]-Y_Axis[i-1]);
          movement_var = digitalRead(movement);
          //positions[0] = -var_x*25;
          //positions[1] = -var_y*25;
          positions[0] = -var_x*X_factor;
          positions[1] = -var_y*Y_factor;
          steppers.moveTo(positions);
          steppers.runSpeedToPosition();
    
          positions[0]=0;
          positions[1]=0;
        
          stepper_Y_Axis.setCurrentPosition(0);
          stepper_X_Axis.setCurrentPosition(0);
  
          
          if(i == 269)
          {
            Serial.println("Timer start");
            delay(del);
            Serial.println("Timer stop");
            digitalWrite(IN1,LOW);
            digitalWrite(IN2,LOW);
            delay(100);
  
            digitalWrite(IN1,LOW);
            digitalWrite(IN2,HIGH);
            delay(1200);
  
            digitalWrite(IN1,LOW);
            digitalWrite(IN2,LOW);
            delay(750);
  
            digitalWrite(IN1,HIGH);
            digitalWrite(IN2,LOW);
            delay(1200);
  
            digitalWrite(IN1,LOW);
            digitalWrite(IN2,LOW);
  
            delay(3000);
          }
        
          if(i==536)
          {
            positions[0]=0;
            positions[1]=0;
            var_x=0;
            var_y=0;
    
            stepper_Y_Axis.setCurrentPosition(0);
            stepper_X_Axis.setCurrentPosition(0);    
            
            Serial.print("  i break ");
            break;
          }
          
          if(movement_var==1)
          {
            Serial.println("Break");
            stepper_Y_Axis.stop();
            stepper_X_Axis.stop();
            //flag_path=1;
            break;
          }
        }
      
        if(i==536 && movement_var == 0)
        {
          
          flag_down_movement = LOW;
          flag_up_movement = LOW;
          flag_left_movement = LOW;
          flag_fix_movement = LOW;
          flag_LA_contract = LOW;
          LA_mov = LOW;
          complete = LOW;
      
          forward_mov();
      
          flag_down_movement = LOW;
          flag_up_movement = LOW;
          flag_left_movement = LOW;
          flag_fix_movement = LOW;
          flag_LA_contract = LOW;
          LA_mov = LOW;
          complete = LOW;
      
          backward_mov();
      
          flag_down_movement = LOW;
          flag_up_movement = LOW;
          flag_left_movement = LOW;
          flag_fix_movement = LOW;
          flag_LA_contract = LOW;
          LA_mov = LOW;
  
          for(i=535;i>=1;i--)
           {
             var_x=(X_Axis[i]-X_Axis[i+1]);
             var_y=(Y_Axis[i]-Y_Axis[i+1]);
             movement_var = digitalRead(movement);
             //positions[0] = -var_x*25;
             //positions[1] = -var_y*25;
             positions[0] = -var_x*X_factor;
             positions[1] = -var_y*Y_factor;
             steppers.moveTo(positions);
             steppers.runSpeedToPosition();
             positions[0]=0;
             positions[1]=0;
           
             stepper_Y_Axis.setCurrentPosition(0);
             stepper_X_Axis.setCurrentPosition(0);
  
             if (iter == 0 && i == 60)
             {
              digitalWrite(ATtiny_ctrl,HIGH);
             }

             if (iter == 1 && i == 1)
             {
              digitalWrite(ATtiny_ctrl,LOW);
             }
             
             if (movement_var==1)
              {
                 Serial.println("Break");               
                 stepper_Y_Axis.stop();
                 stepper_X_Axis.stop();
                 //flag_path=1;
                 break;
              }
           }
           flag_path=1;
  
       // Stop the movement when the button is pressed, regardless of the path being followed.
          if (movement_var==1)
          {
            Serial.println("break while");
            stepper_Y_Axis.stop();
            stepper_X_Axis.stop();
            flag_path=1;
            flag_right=1;
            flag_left=1;
            break;
          }
        }
      }
    }
    
    flag_path = 0;
    flag_left = 0;
    flag_right = 0;
    done = 0;

//    if(iter == 1)
//    {
//      all_done = 1;
//    }
//  }
//  if(all_done == 1)
//  {
//    while(1);
//  }
}

void forward_mov()
{ 
    digitalWrite(IN3,LOW);
    digitalWrite(IN4,LOW);
    
    movement_var = digitalRead(movement);
    while(movement_var == LOW)
    {
      
        Serial.print("Starting...\n");
        movement_var = digitalRead(movement);
        delay(1000);
  
        pos[0]=0;
        pos[1]=0;
  
        stepper_Y_Axis.setCurrentPosition(0);
        stepper_X_Axis.setCurrentPosition(0);

        DIRECTION = -1;
  
        Serial.print("MOVING DOWN\n");
        
        var_x = 0; var_y = DIRECTION*19;
        pos[0] = -var_x*X_factor;
        pos[1] = -var_y*Y_factor;
    
        steppers.moveTo(pos);
        steppers.runSpeedToPosition();
    
        pos[0]=0;
        pos[1]=0;

        Serial.println("Delay Start");
        //delay(del);
        Serial.println("Delay end");
   
        //LA Contract
        movement_var = digitalRead(movement);
        if(movement_var != 0)
        {
          break;
        }
        digitalWrite(IN3, LOW);
        digitalWrite(IN4, HIGH);
        delay(7000);
        digitalWrite(IN3, LOW);
        digitalWrite(IN4, LOW);

        digitalWrite(dirPin, HIGH);
        for(long ii=0;ii<3000; ii++)
        {
          digitalWrite(stepPin,HIGH);
          delayMicroseconds(300);    // by changing this time delay between the steps we can change the rotation speed
          digitalWrite(stepPin,LOW);
          delayMicroseconds(300);
        }

        delay(2101);
        digitalWrite(flow_water, LOW);
        delay(8000);
        digitalWrite(flow_water, HIGH);
        
    
        movement_var = digitalRead(movement);
        if(movement_var != 0)
        {
          break;
        }
        delay(2000);
  
        pos[0]=0;
        pos[1]=0;
        
        stepper_Y_Axis.setCurrentPosition(0);
        stepper_X_Axis.setCurrentPosition(0);
  
        DIRECTION = 1;
  
        Serial.print("MOVING UP\n");
        
        var_x = 0; var_y = DIRECTION*65;
        pos[0] = -var_x*X_factor;
        pos[1] = -var_y*Y_factor;
    
        steppers.moveTo(pos);
        steppers.runSpeedToPosition();
  
        pos[0]=0;
        pos[1]=0;  
  
        movement_var = digitalRead(movement);
        if(movement_var != 0)
        {
          break;
        }
        delay(2000);
  
        pos[0]=0;
        pos[1]=0;
        
        stepper_Y_Axis.setCurrentPosition(0);
        stepper_X_Axis.setCurrentPosition(0);
  
        DIRECTION = 1;
        
        Serial.print("MOVING LEFT\n");
        
        var_x = DIRECTION*428; var_y = 0;
        pos[0] = -var_x*X_factor;
        pos[1] = -var_y*Y_factor;
  
        steppers.moveTo(pos);
        steppers.runSpeedToPosition();

        pos[0]=0;
        pos[1]=0;

        stepper_Y_Axis.setCurrentPosition(0);
        stepper_X_Axis.setCurrentPosition(0);

        
//        digitalWrite(dirPin, HIGH);
//        for(long ii=0;ii<39000; ii++)
//        {
//          digitalWrite(stepPin,HIGH); 
//          delayMicroseconds(300);    // by changing this time delay between the steps we can change the rotation speed
//          digitalWrite(stepPin,LOW); 
//          delayMicroseconds(300);
//        }
      
        movement_var = digitalRead(movement);
        if(movement_var != 0)
        {
          break;
        }
        delay(2000);
  
        pos[0]=0;
        pos[1]=0;
        
        stepper_Y_Axis.setCurrentPosition(0);
        stepper_X_Axis.setCurrentPosition(0);

        DIRECTION = -1;
        
        Serial.print("MOVING TO FIX\n");
        delay(100);
        delay(del);
        var_x = 0; var_y = DIRECTION*36;
        pos[0] = -var_x*X_factor;
        pos[1] = -var_y*Y_factor;
    
        steppers.moveTo(pos);
        steppers.runSpeedToPosition();
    
        pos[0]=0;
        pos[1]=0;

        break;
    }

    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
    delay(780);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, LOW);
    delay(3000);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
    delay(1000);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, LOW);
  }

void backward_mov()
{ 
    Serial.print("Starting...\n");
  
    digitalWrite(IN3,LOW);
    digitalWrite(IN4,LOW);
    
    movement_var = digitalRead(movement);
    while(movement_var == LOW)
    {
      movement_var = digitalRead(movement);
      if(movement_var != 0)
      {
        break;
      }
      delay(2000);

      pos[0]=0;
      pos[1]=0;
      
      stepper_Y_Axis.setCurrentPosition(0);
      stepper_X_Axis.setCurrentPosition(0);

      DIRECTION = 1;

      Serial.print("MOVING TO FIX\n");
      
      var_x = 0; var_y = DIRECTION*36;
      pos[0] = -var_x*X_factor;
      pos[1] = -var_y*Y_factor;
  
      steppers.moveTo(pos);
      steppers.runSpeedToPosition();
  
      pos[0]=0;
      pos[1]=0;

      movement_var = digitalRead(movement);
      if(movement_var != 0)
      {
        break;
      }
      
      delay(2000);

      pos[0]=0;
      pos[1]=0;
      
      stepper_Y_Axis.setCurrentPosition(0);
      stepper_X_Axis.setCurrentPosition(0);

      DIRECTION = -1;
      
      Serial.print("MOVING LEFT\n");
      
      var_x = DIRECTION*428; var_y = 0;
      pos[0] = -var_x*X_factor;
      pos[1] = -var_y*Y_factor;

  
      steppers.moveTo(pos);
      steppers.runSpeedToPosition();
  
      pos[0]=0;
      pos[1]=0;
 

      movement_var = digitalRead(movement);
      if(movement_var != 0)
      {
        break;
      }
      delay(2000);

      pos[0]=0;
      pos[1]=0;
      
      stepper_Y_Axis.setCurrentPosition(0);
      stepper_X_Axis.setCurrentPosition(0);

      DIRECTION = -1;

      Serial.print("MOVING UP\n");
      
      var_x = 0; var_y = DIRECTION*65;
      pos[0] = -var_x*X_factor;
      pos[1] = -var_y*Y_factor;
  
      steppers.moveTo(pos);
      steppers.runSpeedToPosition();

      pos[0]=0;
      pos[1]=0;

      //LA extend
      movement_var = digitalRead(movement);
      if(movement_var != 0)
      {
        break;
      }
                
      digitalWrite(IN3, LOW);
      digitalWrite(IN4, HIGH);
      delay(EXTEND);//delay(5125);
      digitalWrite(IN3, LOW);
      digitalWrite(IN4, LOW);

      movement_var = digitalRead(movement);
      if(movement_var != 0)
      {
        break;
      }
      
      delay(2000);

      pos[0]=0;
      pos[1]=0;

      stepper_Y_Axis.setCurrentPosition(0);
      stepper_X_Axis.setCurrentPosition(0);

      DIRECTION = 1;

      Serial.print("MOVING DOWN\n");
      
      var_x = 0; var_y = DIRECTION*19;
      pos[0] = -var_x*X_factor;
      pos[1] = -var_y*Y_factor;
  
      steppers.moveTo(pos);
      steppers.runSpeedToPosition();
  
      pos[0]=0;
      pos[1]=0;

      stepper_Y_Axis.setCurrentPosition(0);
      stepper_X_Axis.setCurrentPosition(0);
      
      break;
    }
}

void self_correction()
{
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, HIGH);
    delay(6000);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, LOW);

    Serial.println("Fully Retracted");

    delay(1000);
    
    pos[0]=0;
    pos[1]=0;

    stepper_Y_Axis.setCurrentPosition(0);
    stepper_X_Axis.setCurrentPosition(0);

    DIRECTION = 1;

    Serial.print("Vertical Offset\n");
    
    var_x = 0; var_y = DIRECTION*48.5;
    pos[0] = -var_x*X_factor;
    pos[1] = -var_y*Y_factor;

    steppers.moveTo(pos);
    while(digitalRead(Y_NO) == HIGH && stepper_Y_Axis.distanceToGo())
    {
      steppers.run();
      while(digitalRead(GO) == HIGH);
    }
    
    stepper_X_Axis.stop();
    stepper_Y_Axis.stop();
    
    pos[0]=0;
    pos[1]=0;
 
    delay(1000);

    pos[0]=0;
    pos[1]=0;
    
    stepper_Y_Axis.setCurrentPosition(0);
    stepper_X_Axis.setCurrentPosition(0);

    DIRECTION = 1;
    
    Serial.print("MOVING LEFT\n");
    
    var_x = DIRECTION*100000; var_y = 0;
    pos[0] = -var_x*X_factor;
    pos[1] = -var_y*Y_factor;

    steppers.moveTo(pos);
    while(digitalRead(X_NO) == HIGH && stepper_X_Axis.distanceToGo())
    {
      steppers.run();
      while(digitalRead(GO) == HIGH);
    }
    stepper_X_Axis.stop();
    stepper_Y_Axis.stop();
    Serial.println("Stopped Moving LEFT");
    pos[0]=0;
    pos[1]=0;

    stepper_Y_Axis.setCurrentPosition(0);
    stepper_X_Axis.setCurrentPosition(0);

    delay(1000);

    pos[0]=0;
    pos[1]=0;

    stepper_Y_Axis.setCurrentPosition(0);
    stepper_X_Axis.setCurrentPosition(0);

    DIRECTION = 1;

    Serial.print("MOVING UP\n");

    var_x = 0; var_y = DIRECTION*100000;
    pos[0] = -var_x*X_factor;
    pos[1] = -var_y*X_factor;

    steppers.moveTo(pos);
    while(digitalRead(Y_NO) == HIGH && stepper_Y_Axis.distanceToGo())
    {
      steppers.run();
      while(digitalRead(GO) == HIGH);
    }
    stepper_X_Axis.stop();
    stepper_Y_Axis.stop();
    Serial.println("Stopped Moving UP");
    pos[0]=0;
    pos[1]=0;

    delay(1000);

    pos[0]=0;
    pos[1]=0;

    stepper_Y_Axis.setCurrentPosition(0);
    stepper_X_Axis.setCurrentPosition(0);

    DIRECTION = -1;
    
    Serial.print("Final Offset\n");
    
    var_x = DIRECTION*35.2; var_y = 0;
    pos[0] = -var_x*X_factor;
    pos[1] = -var_y*Y_factor;

    steppers.moveTo(pos);
    while(stepper_X_Axis.distanceToGo())
    {
      steppers.run();
      while(digitalRead(GO) == HIGH);
    }

    delay(1000);
    
    pos[0]=0;
    pos[1]=0;

    stepper_Y_Axis.setCurrentPosition(0);
    stepper_X_Axis.setCurrentPosition(0);

    digitalWrite(IN3, HIGH);
    digitalWrite(IN4, LOW);
    delay(EXTEND);//delay(5125);
    digitalWrite(IN3, LOW);
    digitalWrite(IN4, LOW);
}
