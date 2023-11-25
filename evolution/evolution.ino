// Copyright 2023 Sean Luke
// (sean@cs.gmu.edu)
//
// Released under the Apache 2.0 License
//
// WARNING: Mozzi itself is released under a broken non-open-source license, namely the 
// Creative Commons Attribution-NonCommercial-ShareAlike 4.0 International License.
// This license is not compatible with the LGPL (used by the Arduino itself!) and is
// also viral, AND is non-commercial only.  What a mess.  I am pushing them to change 
// their license to something reasonable like Apache or GPL but in the meantime I don't 
// have much choice but to turn my head and ignore the broken license.  So I'm releasing
// under Apache for the time being.


/// EVOLUTION
///
/// Evolution is an evolving drone synthesizer using 9-partial additive synthesis.  
/// Evolution is meant to run on the AE Modular GRAINS, but it could be adapted to any Arduino.
///
/// All Evolution does is randomly (and usually slowly) change the amplitudes of 8 of 
/// the partials, leaving the fundamental alone.  You have 8 choices of frequency 
/// combinations.  The frequencies of the partials can be found 
/// in partialFrequencies[] if you wanted to modify them.
///
/// SET GRAINS TO MOZZI MODE.  Sorry, no Grains mode.
///
/// You will need to install the Mozzi Library.  You can do this from the Library Manager
/// in your Arduino IDE.
///
/// NOTE: Probably due to the voltage divider on all of its analog inputs, GRAINS is limited
/// to about a 45 note range.  


/// CONFIGURATION
///
/// IN 1            [Unused]
/// IN 2            [Unused]
/// IN 3            Pitch CV	[This isn't IN2, like Pitch Tune, because Pitch Tune must be +/-]
/// AUDIO IN (A)    [Unused]
/// AUDIO OUT       Out
/// DIGITAL OUT (D) Clock
///
/// POT 1           Choice of Partials [Set the switch to MAN]
///
/// POT 2           Pitch Tune  [Set the switch to MAN]
///
/// POT 3           Evolution Rate.  If set to far left, only evolves when clocked


// ADJUSTING TRACKING

// Grains can only do about 3.75 octaves due to its circuit design.  I have positioned 
// it to start at the C two octaves below middle C.  For the same reason, I have to 
// "stretch" Grains from 3.75V to 5V, so to speak, in software.  I have tuned this 
// stretch for my own Grains, but if Evolution is tracking poorly in your Grains, get with 
// me and we might be able to customize it for you.
//
// In short, you can play with the function that has "pow(...)" in it below to get the
// tracking you need.  65.41 is the frequency two octaves below middle C.  45.2 is the
// stretch factor appropriate for my grains -- higher is more stretched.  60 is a full
// 5V, 5-octave (60 note) stretch.  You might need to stretch or decrease by just a tad,
// like .1 or .2.  I don't know how consistent Grains' internal resistors are from unit
// to unit.
//
// Once you have a desired base frequency and stretch value, you could pump out all the
// values from 0 to 1023 and put them into the "pitch[]" table.  I use some lisp code
// as below but you can choose whatever language suits your fancy.  :-)  Then use the
// FREQUENCIES macro instead of the pow() function, it's significantly faster.





#define CONTROL_RATE 100			// More than this and we're gonna get clicks probably

#include <MozziGuts.h>
#include <Oscil.h>
#include <mozzi_rand.h>
#include <tables/sin256_int8.h>

/// OSCILLATORS

Oscil<SIN256_NUM_CELLS, AUDIO_RATE> tonewheel1(SIN256_DATA);
Oscil<SIN256_NUM_CELLS, AUDIO_RATE> tonewheel2(SIN256_DATA);
Oscil<SIN256_NUM_CELLS, AUDIO_RATE> tonewheel3(SIN256_DATA);
Oscil<SIN256_NUM_CELLS, AUDIO_RATE> tonewheel4(SIN256_DATA);
Oscil<SIN256_NUM_CELLS, AUDIO_RATE> tonewheel5(SIN256_DATA);
Oscil<SIN256_NUM_CELLS, AUDIO_RATE> tonewheel6(SIN256_DATA);
Oscil<SIN256_NUM_CELLS, AUDIO_RATE> tonewheel7(SIN256_DATA);
Oscil<SIN256_NUM_CELLS, AUDIO_RATE> tonewheel8(SIN256_DATA);
Oscil<SIN256_NUM_CELLS, AUDIO_RATE> tonewheel9(SIN256_DATA);

Oscil<SIN256_NUM_CELLS, AUDIO_RATE> oscils[9] = 
	{ 
	tonewheel1, tonewheel2, tonewheel3,
	tonewheel4, tonewheel5, tonewheel6, 
	tonewheel7, tonewheel8, tonewheel9 
	};

// This has custom tracking for my Grains module.  To change the base note, you need to specify
// its frequency (I have 65.41, C two octaves below middle C).  And to "stretch" it, you need to
// change 44.5 to something else -- bigger is more stretched, and 60 would be perfectly 5V.  :-(
/*
 */
// I can't explain why it's 75% (45.0 vs 60.0)

// A0 and up. Thus C will be centered on the pitch knob.
PROGMEM const float pitches[1024+512] = {
34.65, 34.741882, 34.834003, 34.926373, 35.018986, 35.111843, 35.20495, 35.298298, 35.3919, 35.485744, 35.57984, 35.674183, 35.768784, 35.863625, 35.958725, 36.054077, 
36.149677, 36.245533, 36.341644, 36.43801, 36.53463, 36.631508, 36.728645, 36.826035, 36.923683, 37.021595, 37.119762, 37.21819, 37.31688, 37.415833, 37.515045, 37.61452, 
37.714264, 37.814266, 37.914536, 38.015076, 38.11588, 38.216946, 38.318287, 38.41989, 38.521767, 38.623913, 38.72633, 38.82902, 38.93198, 39.035213, 39.13872, 39.242504, 
39.34656, 39.450897, 39.555508, 39.660393, 39.765556, 39.871002, 39.976727, 40.082733, 40.18902, 40.295586, 40.402435, 40.50957, 40.616985, 40.72469, 40.832676, 40.94095, 
41.04951, 41.15836, 41.267498, 41.376926, 41.486645, 41.59665, 41.70695, 41.817543, 41.92843, 42.039608, 42.151085, 42.262856, 42.374916, 42.48728, 42.599945, 42.712902, 
42.826164, 42.939724, 43.053585, 43.167747, 43.28221, 43.396984, 43.512054, 43.627434, 43.743122, 43.85911, 43.97541, 44.092022, 44.208935, 44.326164, 44.4437, 44.56155, 
44.67971, 44.798183, 44.916977, 45.03608, 45.1555, 45.275234, 45.39529, 45.515663, 45.636353, 45.757362, 45.8787, 46.000355, 46.12233, 46.24463, 46.367256, 46.490204, 
46.61348, 46.737083, 46.861015, 46.98527, 47.109863, 47.23478, 47.36003, 47.48561, 47.61153, 47.737778, 47.864365, 47.99128, 48.118534, 48.24613, 48.37406, 48.502335, 
48.630947, 48.7599, 48.889194, 49.01883, 49.14881, 49.279137, 49.40981, 49.540825, 49.67219, 49.803905, 49.935966, 50.06838, 50.201145, 50.334263, 50.467728, 50.601547, 
50.735725, 50.870262, 51.005154, 51.140396, 51.276005, 51.411972, 51.548298, 51.68499, 51.822037, 51.959454, 52.097233, 52.235374, 52.373886, 52.51276, 52.652008, 52.791622, 
52.93161, 53.071968, 53.212692, 53.353794, 53.49527, 53.637123, 53.77935, 53.921955, 54.064934, 54.208294, 54.352036, 54.496162, 54.640667, 54.78555, 54.930824, 55.07648, 
55.222527, 55.368958, 55.515774, 55.662987, 55.810585, 55.958572, 56.106956, 56.255733, 56.404903, 56.55447, 56.704433, 56.854794, 57.00555, 57.15671, 57.30827, 57.460236, 
57.6126, 57.765366, 57.918537, 58.072117, 58.22611, 58.3805, 58.53531, 58.690517, 58.846146, 59.00219, 59.158646, 59.315506, 59.472794, 59.630493, 59.788616, 59.94715, 
60.106113, 60.26549, 60.425293, 60.58552, 60.746174, 60.907253, 61.068752, 61.23069, 61.39305, 61.555847, 61.719067, 61.882725, 62.046818, 62.211346, 62.37631, 62.54171, 
62.70755, 62.87383, 63.040546, 63.207706, 63.375317, 63.54336, 63.711857, 63.8808, 64.050186, 64.220024, 64.39031, 64.56106, 64.73225, 64.9039, 65.076, 65.24856, 
65.42158, 65.595055, 65.76899, 65.94338, 66.11824, 66.293564, 66.46935, 66.64561, 66.82233, 66.99952, 67.17718, 67.3553, 67.533905, 67.71298, 67.89254, 68.07256, 
68.25307, 68.43405, 68.61551, 68.797455, 68.97989, 69.162796, 69.34619, 69.530075, 69.71444, 69.89931, 70.084656, 70.27049, 70.456825, 70.64365, 70.83098, 71.0188, 
71.20711, 71.39593, 71.58524, 71.77506, 71.96539, 72.15621, 72.34755, 72.53938, 72.731735, 72.9246, 73.117966, 73.31185, 73.50625, 73.70116, 73.89658, 74.092545, 
74.289, 74.486, 74.68351, 74.88154, 75.0801, 75.27919, 75.4788, 75.67895, 75.879616, 76.080826, 76.28256, 76.48484, 76.68765, 76.891, 77.09489, 77.29932, 
77.50429, 77.7098, 77.91586, 78.12247, 78.32961, 78.53732, 78.745575, 78.954384, 79.16374, 79.37366, 79.58413, 79.79516, 80.006744, 80.2189, 80.43161, 80.64489, 
80.858734, 81.073135, 81.28812, 81.50366, 81.71978, 81.93648, 82.15374, 82.37158, 82.59001, 82.809006, 83.02859, 83.24875, 83.4695, 83.690834, 83.91275, 84.135254, 
84.35835, 84.58204, 84.80633, 85.031204, 85.256676, 85.48275, 85.70941, 85.93669, 86.164566, 86.393036, 86.62212, 86.851814, 87.082115, 87.31302, 87.54455, 87.77669, 
88.00945, 88.24281, 88.4768, 88.71141, 88.94664, 89.1825, 89.41898, 89.65609, 89.89382, 90.1322, 90.371185, 90.610825, 90.8511, 91.091995, 91.33354, 91.57573, 
91.81855, 92.062035, 92.306145, 92.55091, 92.79632, 93.04238, 93.2891, 93.53647, 93.78449, 94.03318, 94.282524, 94.532524, 94.783195, 95.03453, 95.28653, 95.53919, 
95.79253, 96.04655, 96.30122, 96.55657, 96.812614, 97.06932, 97.32671, 97.58479, 97.84355, 98.103004, 98.363144, 98.62396, 98.88548, 99.14769, 99.4106, 99.6742, 
99.9385, 100.2035, 100.46921, 100.73561, 101.00274, 101.27055, 101.539085, 101.808334, 102.07829, 102.34897, 102.62037, 102.89248, 103.16531, 103.43887, 103.71316, 103.988174, 
104.263916, 104.540375, 104.81759, 105.09553, 105.37421, 105.653625, 105.93378, 106.214676, 106.49632, 106.77871, 107.06185, 107.34574, 107.63038, 107.91579, 108.20194, 108.48885, 
108.77653, 109.064964, 109.35417, 109.64414, 109.934875, 110.22639, 110.51867, 110.81172, 111.10556, 111.40018, 111.695564, 111.991745, 112.288704, 112.58646, 112.885, 113.184326, 
113.48446, 113.78537, 114.08709, 114.38961, 114.69293, 114.99706, 115.301994, 115.60773, 115.91429, 116.22165, 116.52983, 116.83883, 117.14864, 117.45928, 117.770744, 118.08302, 
118.39614, 118.71009, 119.02487, 119.34048, 119.65694, 119.97421, 120.29235, 120.61132, 120.93113, 121.25181, 121.573326, 121.89568, 122.218925, 122.543, 122.86795, 123.19374, 
123.52041, 123.847946, 124.176346, 124.505615, 124.83577, 125.16679, 125.498695, 125.83147, 126.16512, 126.49968, 126.835106, 127.171425, 127.508644, 127.84675, 128.18575, 128.52567, 
128.86647, 129.20818, 129.5508, 129.8943, 130.23875, 130.58409, 130.93036, 131.27754, 131.62564, 131.97467, 132.32462, 132.6755, 133.02731, 133.38005, 133.73373, 134.08835, 
134.44391, 134.8004, 135.15784, 135.51624, 135.87556, 136.23587, 136.59712, 136.95934, 137.3225, 137.68661, 138.05173, 138.41779, 138.78484, 139.15285, 139.52182, 139.89178, 
140.26273, 140.63467, 141.00757, 141.38147, 141.75636, 142.13225, 142.50916, 142.88704, 143.26591, 143.6458, 144.02669, 144.4086, 144.79155, 145.17548, 145.56044, 145.9464, 
146.33339, 146.72144, 147.11049, 147.50058, 147.89168, 148.28384, 148.67702, 149.0713, 149.46658, 149.8629, 150.26028, 150.65872, 151.05823, 151.45879, 151.8604, 152.26306, 
152.66681, 153.07162, 153.47754, 153.8845, 154.29254, 154.70168, 155.1119, 155.5232, 155.9356, 156.34908, 156.76366, 157.17934, 157.59612, 158.01402, 158.43303, 158.85312, 
159.27435, 159.69667, 160.12016, 160.54474, 160.97044, 161.39728, 161.82524, 162.25435, 162.6846, 163.11598, 163.5485, 163.98218, 164.41699, 164.853, 165.29012, 165.7284, 
166.16786, 166.60846, 167.05025, 167.49324, 167.93736, 168.38268, 168.82916, 169.27682, 169.72571, 170.17577, 170.62701, 171.07944, 171.53308, 171.98793, 172.444, 172.90125, 
173.35971, 173.8194, 174.28032, 174.74246, 175.20581, 175.6704, 176.13622, 176.60324, 177.07155, 177.54109, 178.01187, 178.48387, 178.95717, 179.43169, 179.9075, 180.38455, 
180.86285, 181.34244, 181.82329, 182.30545, 182.78886, 183.27354, 183.75952, 184.24678, 184.73532, 185.2252, 185.71634, 186.2088, 186.70255, 187.19762, 187.69403, 188.19171, 
188.69072, 189.19107, 189.69273, 190.19572, 190.70009, 191.20575, 191.71275, 192.2211, 192.7308, 193.2419, 193.75429, 194.26805, 194.78319, 195.29967, 195.81754, 196.3368, 
196.8574, 197.3794, 197.90279, 198.42754, 198.95372, 199.48128, 200.01022, 200.54059, 201.07234, 201.6055, 202.14012, 202.67612, 203.21355, 203.7524, 204.29265, 204.8344, 
205.37753, 205.92213, 206.46814, 207.01563, 207.56456, 208.11496, 208.66681, 209.22012, 209.77489, 210.33113, 210.8889, 211.44807, 212.00877, 212.57094, 213.13458, 213.69974, 
214.26643, 214.83458, 215.40424, 215.97542, 216.5481, 217.12234, 217.69806, 218.27531, 218.8541, 219.43442, 220.01628, 220.59972, 221.18468, 221.77116, 222.3592, 222.94884, 
223.54004, 224.13278, 224.72711, 225.32298, 225.92047, 226.51953, 227.1202, 227.72244, 228.32626, 228.93172, 229.53874, 230.14743, 230.7577, 231.36958, 231.9831, 232.59822, 
233.21498, 233.83342, 234.45345, 235.07513, 235.69847, 236.32344, 236.95013, 237.57843, 238.2084, 238.84004, 239.47336, 240.10835, 240.74507, 241.38342, 242.0235, 242.66524, 
243.3087, 243.9539, 244.60077, 245.24936, 245.89966, 246.55171, 247.20552, 247.86101, 248.51823, 249.17722, 249.83794, 250.50041, 251.16467, 251.83067, 252.49843, 253.16797, 
253.83928, 254.5124, 255.18727, 255.86394, 256.5424, 257.22263, 257.9047, 258.58862, 259.2743, 259.9618, 260.6511, 261.34225, 262.03528, 262.7301, 263.42676, 264.12527, 
264.82562, 265.52783, 266.23196, 266.9379, 267.64572, 268.35544, 269.06702, 269.78052, 270.49585, 271.2131, 271.93228, 272.65335, 273.3763, 274.10126, 274.82806, 275.55682, 
276.28748, 277.02008, 277.75467, 278.49118, 279.2296, 279.97006, 280.7124, 281.4568, 282.20313, 282.95142, 283.70172, 284.45398, 285.20822, 285.96457, 286.72284, 287.4831, 
288.24542, 289.0097, 289.77606, 290.5445, 291.3149, 292.08737, 292.86188, 293.63843, 294.41708, 295.19778, 295.98053, 296.76535, 297.55225, 298.34125, 299.1324, 299.9256, 
300.7209, 301.51825, 302.31778, 303.11948, 303.92322, 304.72913, 305.53714, 306.34732, 307.15964, 307.97415, 308.7908, 309.6096, 310.43054, 311.2537, 312.07907, 312.9066, 
313.73627, 314.5682, 315.40234, 316.23868, 317.07724, 317.918, 318.76102, 319.60626, 320.45374, 321.3035, 322.15546, 323.0097, 323.8662, 324.72498, 325.58603, 326.4494, 
327.31503, 328.18295, 329.0532, 329.9257, 330.8006, 331.67773, 332.55722, 333.43906, 334.3232, 335.20975, 336.0986, 336.9898, 337.8834, 338.77933, 339.6776, 340.57837, 
341.48148, 342.38696, 343.29483, 344.2051, 345.1179, 346.033, 346.95056, 347.8705, 348.79297, 349.71783, 350.6452, 351.57498, 352.5072, 353.44193, 354.37912, 355.31885, 
356.26102, 357.20572, 358.15286, 359.10257, 360.05478, 361.00955, 361.9668, 362.92664, 363.88895, 364.85385, 365.82138, 366.79138, 367.76398, 368.73914, 369.71692, 370.69727, 
371.68027, 372.66583, 373.654, 374.64478, 375.6382, 376.6343, 377.63303, 378.63434, 379.63834, 380.645, 381.65433, 382.6664, 383.6811, 384.69846, 385.71854, 386.7413, 
387.76688, 388.79507, 389.82602, 390.85968, 391.8961, 392.93524, 393.97726, 395.02194, 396.0694, 397.1196, 398.17264, 399.2285, 400.2871, 401.3485, 402.4127, 403.47977, 
404.54965, 405.6224, 406.698, 407.77637, 408.85767, 409.9418, 411.02887, 412.11877, 413.21155, 414.30722, 415.40582, 416.50732, 417.6118, 418.71918, 419.82944, 420.94266, 
422.05884, 423.17807, 424.30017, 425.42523, 426.55334, 427.6844, 428.81842, 429.95557, 431.09564, 432.23877, 433.3849, 434.53406, 435.68637, 436.84164, 437.99997, 439.16138, 
440.32584, 441.49344, 442.66418, 443.83795, 445.01483, 446.1949, 447.37802, 448.56436, 449.75378, 450.94635, 452.14212, 453.341, 454.54318, 455.74844, 456.9569, 458.1686, 
459.38348, 460.6016, 461.82303, 463.0476, 464.27545, 465.5065, 466.74084, 467.97855, 469.21945, 470.46365, 471.71115, 472.96194, 474.21606, 475.47357, 476.73438, 477.99847, 
479.26596, 480.53677, 481.81107, 483.08865, 484.3696, 485.654, 486.94174, 488.23294, 489.52762, 490.82565, 492.12714, 493.43207, 494.74048, 496.05243, 497.36777, 498.68658, 
500.0089, 501.33478, 502.6641, 503.99707, 505.3335, 506.67343, 508.01694, 509.364, 510.71475, 512.069, 513.42676, 514.7882, 516.1532, 517.52185, 518.89417, 520.27014, 
521.64966, 523.0329, 524.4198, 525.8104, 527.2047, 528.6026, 530.0043, 531.40967, 532.8187, 534.2317, 535.64825, 537.0686, 538.49274, 539.9206, 541.35236, 542.78784, 
544.22705, 545.67017, 547.11707, 548.5678, 550.02246, 551.48096, 552.9433, 554.40955, 555.87964, 557.35364, 558.83154, 560.31335, 561.7991, 563.28876, 564.78235, 566.27997, 
567.7815, 569.28723, 570.7968, 572.3103, 573.8279, 575.3494, 576.87506, 578.4047, 579.9385, 581.4762, 583.018, 584.56396, 586.11426, 587.66833, 589.2267, 590.78906, 
592.35565, 593.92633, 595.50116, 597.08026, 598.6635, 600.2509, 601.8425, 603.4386, 605.0387, 606.643, 608.2516, 609.86444, 611.48157, 613.103, 614.72876, 616.3587, 
617.9931, 619.63184, 621.275, 622.9224, 624.5742, 626.2303, 627.8908, 629.5558, 631.2251, 632.89886, 634.5771, 636.25977, 637.94684, 639.6387, 641.3347, 643.03534, 
644.7404, 646.45, 648.1642, 649.8828, 651.60614, 653.33386, 655.0663, 656.8033, 658.5451, 660.29126, 662.0421, 663.79767, 665.55774, 667.3226, 669.09204, 670.8663, 
672.64514, 674.4287, 676.2171, 678.0104, 679.8082, 681.6108, 683.41815, 685.23035, 687.04724, 688.8691, 690.6957, 692.52716, 694.36346, 696.20465, 698.051, 699.902, 
701.7578, 703.61865, 705.4844, 707.35504, 709.23065, 711.1113, 712.9969, 714.88745, 716.78314, 718.68396, 720.58966, 722.50037, 724.4162, 726.33704, 728.263, 730.1941, 
732.1303, 734.07166, 736.0181, 737.9697, 739.9268, 741.88885, 743.856, 745.8284, 747.8061, 749.789, 751.77716, 753.77057, 755.76935, 757.7734, 759.78265, 761.79755, 
763.81757, 765.8429, 767.87366, 769.9097, 771.9513, 773.99817, 776.05054, 778.10834, 780.1716, 782.24036, 784.31476, 786.3945, 788.47974, 790.5705, 792.66675, 794.7686, 
796.87604, 798.989, 801.10767, 803.2319, 805.36176, 807.49756, 809.63873, 811.7856, 813.9381, 816.0964, 818.2604, 820.4301, 822.6056, 824.7868, 826.9738, 829.1666, 
831.36554, 833.57, 835.78033, 837.9965, 840.2185, 842.4465, 844.68036, 846.9201, 849.1658, 851.4175, 853.6751, 855.939, 858.2086, 860.4843, 862.766, 865.0537, 
867.34753, 869.6474, 871.95337, 874.26544, 876.5837, 878.9081, 881.23883, 883.57556, 885.9185, 888.2676, 890.623, 892.98456, 895.3524, 897.72656, 900.10693, 902.4937, 
904.8868, 907.28656, 909.69226, 912.1045, 914.523, 916.948, 919.37933, 921.8172, 924.26154, 926.71234, 929.1696, 931.6334, 934.10406, 936.581, 939.06445, 941.5545, 
944.05115, 946.5544, 949.06433, 951.5809, 954.1041, 956.63403, 959.17096, 961.71436, 964.2644, 966.8213, 969.38495, 971.9554, 974.5326, 977.1167, 979.7077, 982.3055, 
984.9102, 987.52216, 990.1407, 992.7662, 995.39856, 998.03796, 1000.6844, 1003.3378, 1005.99835, 1008.66583, 1011.34045, 1014.02216, 1016.7113, 1019.4072, 1022.1103, 1024.8206, 
1027.538, 1030.2626, 1032.9945, 1035.7336, 1038.48, 1041.2336, 1043.9946, 1046.7632, 1049.5388, 1052.3218, 1055.1122, 1057.91, 1060.7151, 1063.5277, 1066.3478, 1069.1753, 
1072.0104, 1074.853, 1077.7034, 1080.561, 1083.4263, 1086.2992, 1089.1796, 1092.0677, 1094.9635, 1097.867, 1100.7781, 1103.6969, 1106.6234, 1109.5582, 1112.5002, 1115.4502, 
1118.4081, 1121.3737, 1124.347, 1127.3284, 1130.3176, 1133.3148, 1136.32, 1139.333, 1142.3545, 1145.3835, 1148.4207, 1151.466, 1154.5192, 1157.5806, 1160.65, 1163.7275, 
1166.8132, 1169.9073, 1173.0094, 1176.1201, 1179.2388, 1182.3657, 1185.5009, 1188.6444, 1191.7961, 1194.9564, 1198.125, 1201.302, 1204.4874, 1207.6813, 1210.8839, 1214.0947, 
1217.3141, 1220.5419, 1223.7783, 1227.0233, 1230.2769, 1233.5392, 1236.8099, 1240.0896, 1243.3778, 1246.6752, 1249.981, 1253.2954, 1256.6187, 1259.9507, 1263.2917, 1266.6414, 
1270.0001, 1273.3677, 1276.7441, 1280.1296, 1283.5244, 1286.9279, 1290.3403, 1293.7618, 1297.1924, 1300.6321, 1304.0808, 1307.5387, 1311.0059, 1314.482, 1317.9677, 1321.4629, 
1324.9668, 1328.4801, 1332.0028, 1335.5348, 1339.0762, 1342.6268, 1346.187, 1349.7565, 1353.3357, 1356.9242, 1360.5226, 1364.1302, 1367.7474, 1371.3741, 1375.0105, 1378.6565, 
1382.3121, 1385.9775, 1389.6526, 1393.3375, 1397.0321, 1400.7369, 1404.4512, 1408.1753, 1411.9092, 1415.6531, 1419.4067, 1423.1705, 1426.9443, 1430.7279, 1434.5217, 1438.3254, 
1442.1399, 1445.964, 1449.7981, 1453.6425, 1457.4968, 1461.3617, 1465.2367, 1469.122, 1473.0173, 1476.9232, 1480.8395, 1484.7666, 1488.7037, 1492.6511, 1496.6091, 1500.5775, 
1504.5565, 1508.5461, 1512.5461, 1516.5569, 1520.5781, 1524.6101, 1528.6533, 1532.7068, 1536.771, 1540.8458, 1544.9316, 1549.0282, 1553.1356, 1557.2539, 1561.3832, 1565.5234, 
1569.6747, 1573.8373, 1578.0105, 1582.1948, 1586.3901, 1590.5967, 1594.8143, 1599.0432, 1603.2832, 1607.5345, 1611.7971, 1616.071, 1620.3568, 1624.6533, 1628.9613, 1633.2806, 
1637.6115, 1641.9539, 1646.3077, 1650.6731, 1655.05, 1659.4386, 1663.8387, 1668.2513, 1672.6748, 1677.1101, 1681.5571, 1686.0161, 1690.4868, 1694.9692, 1699.4637, 1703.97, 
1708.4883, 1713.0186, 1717.5614, 1722.1157, 1726.6821, 1731.2606, 1735.8512, 1740.4541, 1745.0691, 1749.6963, 1754.3359, 1758.9878, 1763.6525, 1768.329, 1773.0181, 1777.7194, 
1782.4332, 1787.1595, 1791.8984, 1796.6499, 1801.4138, 1806.1906, 1810.98, 1815.7825, 1820.5973, 1825.4249, 1830.2651, 1835.1184, 1839.9844, 1844.8633, 1849.7552, 1854.66, 
1859.5779, 1864.5088, 1869.4534, 1874.4105, 1879.3807, 1884.3641, 1889.3608, 1894.3706, 1899.3937, 1904.4303, 1909.4801, 1914.5433, 1919.6199, 1924.7107, 1929.8142, 1934.9314, 
1940.0621, 1945.2064, 1950.3644, 1955.536, 1960.7213, 1965.9204, 1971.1333, 1976.36, 1981.6013, 1986.8557, 1992.124, 1997.4065, 2002.7028, 2008.0132, 2013.3378, 2018.6764, 
};

#define FREQUENCY(pitch) pgm_read_float_near(&pitches[pitch]);

const float partialFrequencies[8][9] = 
	{ 
	{ 1, 2, 3, 4, 5, 6, 7, 8, 9 },
	{ 1, 2, 4, 6, 8, 10, 12, 14, 16 },
	{ 1, 3, 5, 7, 9, 11, 13, 15, 17 },
	{ 1, 4, 7, 10, 13, 16, 19, 22, 25 },
	{ 1, 2, 3, 4, 1.05, 2.1, 3.15, 4.2, 5 },
	{ 1, 2, 3, 1.05, 2.1, 3.15, 1.1, 2.2, 3.3 },
	{ 1, 2, 3, 4, 5, 6, 1.1, 2.2, 3.3 },
	{ 1, 1.01, 1.02, 1.03, 1.04, .99, .98, .97, .96 },
	};
	
uint8_t cutdowns[8][9];

#define CV_POT_IN1    A2    // Partials Selection
#define CV_POT_IN2    A1    // Pitch Tune
#define CV_POT3       A0    // Evolution Rate
#define CV_IN3        A3    // Pitch
#define CV_AUDIO_IN   A4    // [Unused]
#define CV_AUDIO_OUT  9     // Out
#define CV_GATE_OUT   8     // Clock
#define RANDOM_PIN    A5

uint8_t targets[9];
uint8_t amplitudes[9];
uint8_t cutdownAmplitudes1[8][9];
uint8_t cutdownAmplitudes2[8][9];
uint8_t finalAmplitudes[9];

void setup()
{
//Serial.begin(9600);
  startMozzi();
  randSeed(RANDOM_PIN);
  pinMode(CV_GATE_OUT, INPUT_PULLUP);		// duh
  
  amplitudes[0] = 255;
  for(uint8_t i = 1; i < 9; i++)
  	{
  	targets[i] = rand(256);
  	amplitudes[i] = rand(256);
  	}
  	
  for(uint8_t i = 0; i < 8; i++)
  	{
  	for(uint8_t j = 0; j < 9; j++)
  		{
  		cutdownAmplitudes1[i][j] = (uint8_t)(255.0 / partialFrequencies[i][j]);			// dark
  		cutdownAmplitudes2[i][j] = (uint8_t)(255.0 / sqrt(partialFrequencies[i][j]));	// medium
  		}
  	}
}

uint8_t locked = true;		// initially TRUE so that if we have nothing plugged into CV_GATE_OUT, we never clock because we think we already have.  Arduino only has a pullup, not a pull-down.
uint8_t rate = 0;
uint8_t counter = 1;

void loop()
{
  audioHook();
}


// Median-Of-Three Filter for Pitch, dunno if we need it
#define MEDIAN_OF_THREE(a,b,c) (((a) <= (b)) ? (((b) <= (c)) ? (b) : (((a) < (c)) ? (c) : (a))) : (((a) <= (c)) ? (a) : (((b) < (c)) ? (c) : (b))))
uint16_t inA = 0;
uint16_t inB = 0;
uint16_t pitchCV;
uint16_t tuneCV;
uint16_t pitch;
uint8_t gainDivide = 11;
uint8_t draw = 0;

// For the time being we're making sure that the amplitudes follow a sawtooth/square cut-down with frequency
void convertAmplitudes(uint8_t draw)
	{
	if (draw < 8)
		{
		gainDivide = 10;		// they're dark and quiet
		for(uint8_t i = 1; i < 9; i++)
			finalAmplitudes[i] = (amplitudes[i] * cutdownAmplitudes1[draw][i]) >> 8;
		}
	else if (draw < 16)
		{
		gainDivide = 10;		// they're medium
		for(uint8_t i = 1; i < 9; i++)
			finalAmplitudes[i] = (amplitudes[i] * cutdownAmplitudes2[draw][i]) >> 8;
		}
	else
		{
		gainDivide = 11;		// they're bright and loud
		for(uint8_t i = 1; i < 9; i++)
			finalAmplitudes[i] = amplitudes[i];
		}
	}

int8_t drawCounter = -1;
void updateControl()                          
{
	uint16_t in = mozziAnalogRead(CV_IN3);
	// let's try a median of three filter to reduce some of the jumpiness
	uint16_t pitchCV = MEDIAN_OF_THREE(in, inA, inB);
	inA = inB;
	inB = in;
	tuneCV = mozziAnalogRead(CV_POT_IN2);
//	tuneCV = (tuneCV * 7 + mozziAnalogRead(CV_POT_IN2)) >> 3;
	pitch = (pitch * 7 + pitchCV + (tuneCV >> 1)) >> 3;
	
	
	// convert to frequency.  This oughta be a lookup table
	// INCREDIBLY, pow is almost cheap enough to use here
	//float frequency = pow(2.0, pitch * ( 46.9 / 1023.0 / 12.0)) * 34.65;
	float frequency = FREQUENCY(pitch);
	
 	uint8_t d = (mozziAnalogRead(CV_POT_IN1) * 24) >> 10;
 	if (d != draw)
 		{
 		if (drawCounter == -1) drawCounter = 10;
 		else if (drawCounter == 0) draw = d;
 		drawCounter--;
 		}
 	else drawCounter = -1;
 	
	// set the partials
	const float* freq = partialFrequencies[draw >= 16 ? draw - 16: draw >= 8 ? draw - 8 : draw];
	  for(uint8_t i = 0; i < 9; i++)
		{
		oscils[i].setFreq(frequency * freq[i]);
		}
		
	// determine the rate
	rate = (mozziAnalogRead(CV_POT3) >> 4);
	if (rate > 0)
		{
		if (counter > 64)
			{
			counter = 1;		// rate must be > 0 here remember
			// update the targets
			  for(uint8_t i = 1; i < 9; i++)
				{
				if (amplitudes[i] < targets[i])
					{
					if (targets[i] - amplitudes[i] >= 2) amplitudes[i] += 2;
					else targets[i] = rand(256);
					}
				else
					{
					if (amplitudes[i] - targets[i] >= 2) amplitudes[i] -= 2;
					else targets[i] = rand(256);
					}
				}
			convertAmplitudes(draw);
			}
		else counter += rate;
		}
	else
		{
		counter = 1;
		if (digitalRead(CV_GATE_OUT))
			{
			if (!locked)
				{
				locked = true;
				  for(uint8_t i = 1; i < 9; i++)
					{
					amplitudes[i] = rand(256);
					}
				convertAmplitudes(draw);
				}
			}
		else { locked = false; }
		}
	}                                             


int updateAudio()                             
{
  int32_t val = oscils[0].next() * 256;
  for(uint8_t i = 1; i < 9; i++)
  	{
	  val += (oscils[i].next() * finalAmplitudes[i]);
  	}
  return (int)(val >> gainDivide);		// seems to get low enough?
}