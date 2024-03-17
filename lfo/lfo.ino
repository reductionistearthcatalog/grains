// Copyright 2024 Sean Luke
// (sean@cs.gmu.edu)
//
// Released under the Apache 2.0 License


/// LFO
///
/// LFO is a warpable LFO.  You have a choice of Tri, Sine, or Pulse, all with adjustable GAIN
/// and adjustable SHAPE. When the LFO completes its period, a TRIGGER is emitted on
/// digital out -- you could use this to sync things to the LFO for example.
///
/// SET GRAINS TO MOZZI MODE.  Sorry, no Grains mode.
///
/// You can adjust the SHAPE of the wave.  The shape of a Pulse wave is its Pulse Width,
/// hence PWM.  The shape of the Triangle wave goes from Sawtooth to Triangle to Ramp.
/// The shape of the Sine wave goes from being squished almost into a Sawtooth, to Cosine,
/// to being squished almost into a Ramp.
///
/// The LFO has adjustable RATE of course.  There are three ranges in which you can adjust
/// the rate: Short, Medium, and Long.  Within a range you can adjust the rate with the RATE
/// knob or CV.
///
/// Gain is provided on Audio IN.  By default, of course, that is 0.  So by default we disable
/// gain entirely.  If you'd like to control the LFO gain via CV on Audio In, then uncomment
/// (remove the // before) the following #define:

// #define GAIN

/// Pot 3 controls both the TYPE of the LFO wave (Tri, Sine, Pulse) and the RANGE (Short, Medium, Long).
/// Thus there are 9 options in total.  You will probably find MEDIUM to be the most useful range.
/// 
/// The LFO can also be RESET with a trigger.  
///
/// Note that the LFO can only reach about, oh, 4V due to limitations in GRAINS.
///
/// Looking for a RANDOM, NOISY, or SAMPLE AND HOLD LFO?  Try my GRAINS firmware STOCHASTIC.
///
/// Looking for multiple complex pulse wave LFOs?  Try my GRAINS firmware PULSES.


/// GRAINS BUG
/// 
/// There is a bug in GRAINS that affects Pots (Dials) 1 and 2.  If you set the 
/// switch to "Man", then the range of the Pot is correct.  But if you set the switch 
/// to "In 1" (or "In 2"), then the range of the Dial is maximum (1.0 probability for 
/// our purposes) at about the 2 o'clock position on the Dial.  Beyond that it stays at 1.0
///
///


/// CONFIGURATION 
///
/// IN 1            Shape / PWM CV
/// IN 2            Rate CV
/// IN 3            Reset
/// AUDIO IN (A)    Gain
/// AUDIO OUT       Output
/// DIGITAL OUT (D) Period Completion Trigger
///
/// POT 1           Shape / PWM
///					[If not using the CV, set switch to MAN, else set to IN2 and put pot at roughly 2'oclock]
///                 
///
/// POT 2          	Rate
///					[If not using the CV, set switch to MAN, else set to IN2 and put pot at roughly 2'oclock]
///                 
///
/// POT 3           Type (Short Saw/Tri/Ramp, Short Sine, Short Pulse, Medium Saw/Tri/Ramp, Medium Sine, Medium Pulse, Long Saw/Tri/Ramp, Long Sine, Long Pulse)
        


#define CONTROL_RATE 128

#include <MozziGuts.h>

#define CV_POT_IN1    A2    
#define CV_POT_IN2    A1   
#define CV_POT3       A0 
#define CV_IN3        A3
#define CV_AUDIO_IN   A4 
#define CV_AUDIO_OUT  9 
#define CV_GATE_OUT   8  
#define RANDOM_PIN    A5


uint8_t type;			// saw/tri/ramp, sine, pulse
uint8_t length;			// 0, 1, 2
uint8_t rate;			// 0...255
uint8_t gain;			// 0...127
uint16_t warp;			// 0...255
uint16_t position = 0;		// 0...1023

uint8_t count0;			// 0...255
uint8_t count1;			// 0...127
uint8_t count2;			// 0...127


// Generated in Mathematica with
// Tri[x_] := If[x < 0.5, x*4 - 1, 1 - (x - 0.5)*4]
// Table[Floor[Tri[x/1024] * 200] - 44, {x, 0, 1023}]

const PROGMEM int16_t TRIANGLE[1024] = 
{-244, -244, -243, -242, -241, -241, -240, -239, -238, -237, -237,
-236, -235, -234, -234, -233, -232, -231, -230, -230, -229, -228,
-227, -227, -226, -225, -224, -223, -223, -222, -221, -220, -219,
-219, -218, -217, -216, -216, -215, -214, -213, -212, -212, -211,
-210, -209, -209, -208, -207, -206, -205, -205, -204, -203, -202,
-202, -201, -200, -199, -198, -198, -197, -196, -195, -194, -194,
-193, -192, -191, -191, -190, -189, -188, -187, -187, -186, -185,
-184, -184, -183, -182, -181, -180, -180, -179, -178, -177, -177,
-176, -175, -174, -173, -173, -172, -171, -170, -169, -169, -168,
-167, -166, -166, -165, -164, -163, -162, -162, -161, -160, -159,
-159, -158, -157, -156, -155, -155, -154, -153, -152, -152, -151,
-150, -149, -148, -148, -147, -146, -145, -144, -144, -143, -142,
-141, -141, -140, -139, -138, -137, -137, -136, -135, -134, -134,
-133, -132, -131, -130, -130, -129, -128, -127, -127, -126, -125,
-124, -123, -123, -122, -121, -120, -119, -119, -118, -117, -116,
-116, -115, -114, -113, -112, -112, -111, -110, -109, -109, -108,
-107, -106, -105, -105, -104, -103, -102, -102, -101, -100, -99, -98,
-98, -97, -96, -95, -94, -94, -93, -92, -91, -91, -90, -89, -88, -87,
-87, -86, -85, -84, -84, -83, -82, -81, -80, -80, -79, -78, -77, -77,
-76, -75, -74, -73, -73, -72, -71, -70, -69, -69, -68, -67, -66, -66,
-65, -64, -63, -62, -62, -61, -60, -59, -59, -58, -57, -56, -55, -55,
-54, -53, -52, -52, -51, -50, -49, -48, -48, -47, -46, -45, -44, -44,
-43, -42, -41, -41, -40, -39, -38, -37, -37, -36, -35, -34, -34, -33,
-32, -31, -30, -30, -29, -28, -27, -27, -26, -25, -24, -23, -23, -22,
-21, -20, -19, -19, -18, -17, -16, -16, -15, -14, -13, -12, -12, -11,
-10, -9, -9, -8, -7, -6, -5, -5, -4, -3, -2, -2, -1, 0, 1, 2, 2, 3,
4, 5, 6, 6, 7, 8, 9, 9, 10, 11, 12, 13, 13, 14, 15, 16, 16, 17, 18,
19, 20, 20, 21, 22, 23, 23, 24, 25, 26, 27, 27, 28, 29, 30, 31, 31,
32, 33, 34, 34, 35, 36, 37, 38, 38, 39, 40, 41, 41, 42, 43, 44, 45,
45, 46, 47, 48, 48, 49, 50, 51, 52, 52, 53, 54, 55, 56, 56, 57, 58,
59, 59, 60, 61, 62, 63, 63, 64, 65, 66, 66, 67, 68, 69, 70, 70, 71,
72, 73, 73, 74, 75, 76, 77, 77, 78, 79, 80, 81, 81, 82, 83, 84, 84,
85, 86, 87, 88, 88, 89, 90, 91, 91, 92, 93, 94, 95, 95, 96, 97, 98,
98, 99, 100, 101, 102, 102, 103, 104, 105, 106, 106, 107, 108, 109,
109, 110, 111, 112, 113, 113, 114, 115, 116, 116, 117, 118, 119, 120,
120, 121, 122, 123, 123, 124, 125, 126, 127, 127, 128, 129, 130, 131,
131, 132, 133, 134, 134, 135, 136, 137, 138, 138, 139, 140, 141, 141,
142, 143, 144, 145, 145, 146, 147, 148, 148, 149, 150, 151, 152, 152,
153, 154, 155, 156, 155, 154, 153, 152, 152, 151, 150, 149, 148, 148,
147, 146, 145, 145, 144, 143, 142, 141, 141, 140, 139, 138, 138, 137,
136, 135, 134, 134, 133, 132, 131, 131, 130, 129, 128, 127, 127, 126,
125, 124, 123, 123, 122, 121, 120, 120, 119, 118, 117, 116, 116, 115,
114, 113, 113, 112, 111, 110, 109, 109, 108, 107, 106, 106, 105, 104,
103, 102, 102, 101, 100, 99, 98, 98, 97, 96, 95, 95, 94, 93, 92, 91,
91, 90, 89, 88, 88, 87, 86, 85, 84, 84, 83, 82, 81, 81, 80, 79, 78,
77, 77, 76, 75, 74, 73, 73, 72, 71, 70, 70, 69, 68, 67, 66, 66, 65,
64, 63, 63, 62, 61, 60, 59, 59, 58, 57, 56, 56, 55, 54, 53, 52, 52,
51, 50, 49, 48, 48, 47, 46, 45, 45, 44, 43, 42, 41, 41, 40, 39, 38,
38, 37, 36, 35, 34, 34, 33, 32, 31, 31, 30, 29, 28, 27, 27, 26, 25,
24, 23, 23, 22, 21, 20, 20, 19, 18, 17, 16, 16, 15, 14, 13, 13, 12,
11, 10, 9, 9, 8, 7, 6, 6, 5, 4, 3, 2, 2, 1, 0, -1, -2, -2, -3, -4,
-5, -5, -6, -7, -8, -9, -9, -10, -11, -12, -12, -13, -14, -15, -16,
-16, -17, -18, -19, -19, -20, -21, -22, -23, -23, -24, -25, -26, -27,
-27, -28, -29, -30, -30, -31, -32, -33, -34, -34, -35, -36, -37, -37,
-38, -39, -40, -41, -41, -42, -43, -44, -44, -45, -46, -47, -48, -48,
-49, -50, -51, -52, -52, -53, -54, -55, -55, -56, -57, -58, -59, -59,
-60, -61, -62, -62, -63, -64, -65, -66, -66, -67, -68, -69, -69, -70,
-71, -72, -73, -73, -74, -75, -76, -77, -77, -78, -79, -80, -80, -81,
-82, -83, -84, -84, -85, -86, -87, -87, -88, -89, -90, -91, -91, -92,
-93, -94, -94, -95, -96, -97, -98, -98, -99, -100, -101, -102, -102,
-103, -104, -105, -105, -106, -107, -108, -109, -109, -110, -111,
-112, -112, -113, -114, -115, -116, -116, -117, -118, -119, -119,
-120, -121, -122, -123, -123, -124, -125, -126, -127, -127, -128,
-129, -130, -130, -131, -132, -133, -134, -134, -135, -136, -137,
-137, -138, -139, -140, -141, -141, -142, -143, -144, -144, -145,
-146, -147, -148, -148, -149, -150, -151, -152, -152, -153, -154,
-155, -155, -156, -157, -158, -159, -159, -160, -161, -162, -162,
-163, -164, -165, -166, -166, -167, -168, -169, -169, -170, -171,
-172, -173, -173, -174, -175, -176, -177, -177, -178, -179, -180,
-180, -181, -182, -183, -184, -184, -185, -186, -187, -187, -188,
-189, -190, -191, -191, -192, -193, -194, -194, -195, -196, -197,
-198, -198, -199, -200, -201, -202, -202, -203, -204, -205, -205,
-206, -207, -208, -209, -209, -210, -211, -212, -212, -213, -214,
-215, -216, -216, -217, -218, -219, -219, -220, -221, -222, -223,
-223, -224, -225, -226, -227, -227, -228, -229, -230, -230, -231,
-232, -233, -234, -234, -235, -236, -237, -237, -238, -239, -240,
-241, -241, -242, -243, -244};


// Generated in Mathematica with
// Table[Floor[-Cos[x / 1024 * 2 Pi]* 200] - 44 , {x, 0, 1023}]

const PROGMEM int16_t COSINE[1024] = 
{-244, -244, -244, -244, -244, -244, -244, -244, -244, -244, -244,
-244, -244, -244, -244, -244, -244, -243, -243, -243, -243, -243,
-243, -243, -242, -242, -242, -242, -242, -241, -241, -241, -241,
-240, -240, -240, -240, -239, -239, -239, -239, -238, -238, -238,
-237, -237, -237, -236, -236, -236, -235, -235, -234, -234, -234,
-233, -233, -232, -232, -232, -231, -231, -230, -230, -229, -229,
-228, -228, -227, -227, -226, -226, -225, -225, -224, -224, -223,
-223, -222, -221, -221, -220, -220, -219, -219, -218, -217, -217,
-216, -215, -215, -214, -213, -213, -212, -211, -211, -210, -209,
-209, -208, -207, -207, -206, -205, -204, -204, -203, -202, -201,
-201, -200, -199, -198, -198, -197, -196, -195, -194, -194, -193,
-192, -191, -190, -189, -189, -188, -187, -186, -185, -184, -183,
-182, -182, -181, -180, -179, -178, -177, -176, -175, -174, -173,
-172, -171, -170, -169, -169, -168, -167, -166, -165, -164, -163,
-162, -161, -160, -159, -158, -157, -156, -155, -154, -153, -151,
-150, -149, -148, -147, -146, -145, -144, -143, -142, -141, -140,
-139, -138, -137, -136, -134, -133, -132, -131, -130, -129, -128,
-127, -126, -124, -123, -122, -121, -120, -119, -118, -116, -115,
-114, -113, -112, -111, -110, -108, -107, -106, -105, -104, -103,
-101, -100, -99, -98, -97, -95, -94, -93, -92, -91, -90, -88, -87,
-86, -85, -84, -82, -81, -80, -79, -77, -76, -75, -74, -73, -71, -70,
-69, -68, -67, -65, -64, -63, -62, -60, -59, -58, -57, -56, -54, -53,
-52, -51, -49, -48, -47, -46, -44, -43, -42, -41, -40, -38, -37, -36,
-35, -33, -32, -31, -30, -29, -27, -26, -25, -24, -22, -21, -20, -19,
-18, -16, -15, -14, -13, -12, -10, -9, -8, -7, -5, -4, -3, -2, -1, 1,
2, 3, 4, 5, 6, 8, 9, 10, 11, 12, 14, 15, 16, 17, 18, 19, 21, 22, 23,
24, 25, 26, 27, 29, 30, 31, 32, 33, 34, 35, 37, 38, 39, 40, 41, 42,
43, 44, 45, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60,
61, 62, 64, 65, 66, 67, 68, 69, 70, 71, 72, 73, 74, 75, 76, 77, 78,
79, 80, 80, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91, 92, 93, 93,
94, 95, 96, 97, 98, 99, 100, 100, 101, 102, 103, 104, 105, 105, 106,
107, 108, 109, 109, 110, 111, 112, 112, 113, 114, 115, 115, 116, 117,
118, 118, 119, 120, 120, 121, 122, 122, 123, 124, 124, 125, 126, 126,
127, 128, 128, 129, 130, 130, 131, 131, 132, 132, 133, 134, 134, 135,
135, 136, 136, 137, 137, 138, 138, 139, 139, 140, 140, 141, 141, 142,
142, 143, 143, 143, 144, 144, 145, 145, 145, 146, 146, 147, 147, 147,
148, 148, 148, 149, 149, 149, 150, 150, 150, 150, 151, 151, 151, 151,
152, 152, 152, 152, 153, 153, 153, 153, 153, 154, 154, 154, 154, 154,
154, 154, 155, 155, 155, 155, 155, 155, 155, 155, 155, 155, 155, 155,
155, 155, 155, 155, 156, 155, 155, 155, 155, 155, 155, 155, 155, 155,
155, 155, 155, 155, 155, 155, 155, 154, 154, 154, 154, 154, 154, 154,
153, 153, 153, 153, 153, 152, 152, 152, 152, 151, 151, 151, 151, 150,
150, 150, 150, 149, 149, 149, 148, 148, 148, 147, 147, 147, 146, 146,
145, 145, 145, 144, 144, 143, 143, 143, 142, 142, 141, 141, 140, 140,
139, 139, 138, 138, 137, 137, 136, 136, 135, 135, 134, 134, 133, 132,
132, 131, 131, 130, 130, 129, 128, 128, 127, 126, 126, 125, 124, 124,
123, 122, 122, 121, 120, 120, 119, 118, 118, 117, 116, 115, 115, 114,
113, 112, 112, 111, 110, 109, 109, 108, 107, 106, 105, 105, 104, 103,
102, 101, 100, 100, 99, 98, 97, 96, 95, 94, 93, 93, 92, 91, 90, 89,
88, 87, 86, 85, 84, 83, 82, 81, 80, 80, 79, 78, 77, 76, 75, 74, 73,
72, 71, 70, 69, 68, 67, 66, 65, 64, 62, 61, 60, 59, 58, 57, 56, 55,
54, 53, 52, 51, 50, 49, 48, 47, 45, 44, 43, 42, 41, 40, 39, 38, 37,
35, 34, 33, 32, 31, 30, 29, 27, 26, 25, 24, 23, 22, 21, 19, 18, 17,
16, 15, 14, 12, 11, 10, 9, 8, 6, 5, 4, 3, 2, 1, -1, -2, -3, -4, -5,
-7, -8, -9, -10, -12, -13, -14, -15, -16, -18, -19, -20, -21, -22,
-24, -25, -26, -27, -29, -30, -31, -32, -33, -35, -36, -37, -38, -40,
-41, -42, -43, -44, -46, -47, -48, -49, -51, -52, -53, -54, -56, -57,
-58, -59, -60, -62, -63, -64, -65, -67, -68, -69, -70, -71, -73, -74,
-75, -76, -77, -79, -80, -81, -82, -84, -85, -86, -87, -88, -90, -91,
-92, -93, -94, -95, -97, -98, -99, -100, -101, -103, -104, -105,
-106, -107, -108, -110, -111, -112, -113, -114, -115, -116, -118,
-119, -120, -121, -122, -123, -124, -126, -127, -128, -129, -130,
-131, -132, -133, -134, -136, -137, -138, -139, -140, -141, -142,
-143, -144, -145, -146, -147, -148, -149, -150, -151, -153, -154,
-155, -156, -157, -158, -159, -160, -161, -162, -163, -164, -165,
-166, -167, -168, -169, -169, -170, -171, -172, -173, -174, -175,
-176, -177, -178, -179, -180, -181, -182, -182, -183, -184, -185,
-186, -187, -188, -189, -189, -190, -191, -192, -193, -194, -194,
-195, -196, -197, -198, -198, -199, -200, -201, -201, -202, -203,
-204, -204, -205, -206, -207, -207, -208, -209, -209, -210, -211,
-211, -212, -213, -213, -214, -215, -215, -216, -217, -217, -218,
-219, -219, -220, -220, -221, -221, -222, -223, -223, -224, -224,
-225, -225, -226, -226, -227, -227, -228, -228, -229, -229, -230,
-230, -231, -231, -232, -232, -232, -233, -233, -234, -234, -234,
-235, -235, -236, -236, -236, -237, -237, -237, -238, -238, -238,
-239, -239, -239, -239, -240, -240, -240, -240, -241, -241, -241,
-241, -242, -242, -242, -242, -242, -243, -243, -243, -243, -243,
-243, -243, -244, -244, -244, -244, -244, -244, -244, -244, -244,
-244, -244, -244, -244, -244, -244, -244};

const PROGMEM int16_t PULSE[1024] = 
{-244, -244, -244, -244, -244, -244, -244, -244, -244, -244, -244,
-244, -244, -244, -244, -244, -244, -244, -244, -244, -244, -244,
-244, -244, -244, -244, -244, -244, -244, -244, -244, -244, -244,
-244, -244, -244, -244, -244, -244, -244, -244, -244, -244, -244,
-244, -244, -244, -244, -244, -244, -244, -244, -244, -244, -244,
-244, -244, -244, -244, -244, -244, -244, -244, -244, -244, -244,
-244, -244, -244, -244, -244, -244, -244, -244, -244, -244, -244,
-244, -244, -244, -244, -244, -244, -244, -244, -244, -244, -244,
-244, -244, -244, -244, -244, -244, -244, -244, -244, -244, -244,
-244, -244, -244, -244, -244, -244, -244, -244, -244, -244, -244,
-244, -244, -244, -244, -244, -244, -244, -244, -244, -244, -244,
-244, -244, -244, -244, -244, -244, -244, -244, -244, -244, -244,
-244, -244, -244, -244, -244, -244, -244, -244, -244, -244, -244,
-244, -244, -244, -244, -244, -244, -244, -244, -244, -244, -244,
-244, -244, -244, -244, -244, -244, -244, -244, -244, -244, -244,
-244, -244, -244, -244, -244, -244, -244, -244, -244, -244, -244,
-244, -244, -244, -244, -244, -244, -244, -244, -244, -244, -244,
-244, -244, -244, -244, -244, -244, -244, -244, -244, -244, -244,
-244, -244, -244, -244, -244, -244, -244, -244, -244, -244, -244,
-244, -244, -244, -244, -244, -244, -244, -244, -244, -244, -244,
-244, -244, -244, -244, -244, -244, -244, -244, -244, -244, -244,
-244, -244, -244, -244, -244, -244, -244, -244, -244, -244, -244,
-244, -244, -244, -244, -244, -244, -244, -244, -244, -244, -244,
-244, -244, -244, -244, -244, -244, -244, -244, -244, -244, -244,
-244, -244, -244, -244, -244, -244, -244, -244, -244, -244, -244,
-244, -244, -244, -244, -244, -244, -244, -244, -244, -244, -244,
-244, -244, -244, -244, -244, -244, -244, -244, -244, -244, -244,
-244, -244, -244, -244, -244, -244, -244, -244, -244, -244, -244,
-244, -244, -244, -244, -244, -244, -244, -244, -244, -244, -244,
-244, -244, -244, -244, -244, -244, -244, -244, -244, -244, -244,
-244, -244, -244, -244, -244, -244, -244, -244, -244, -244, -244,
-244, -244, -244, -244, -244, -244, -244, -244, -244, -244, -244,
-244, -244, -244, -244, -244, -244, -244, -244, -244, -244, -244,
-244, -244, -244, -244, -244, -244, -244, -244, -244, -244, -244,
-244, -244, -244, -244, -244, -244, -244, -244, -244, -244, -244,
-244, -244, -244, -244, -244, -244, -244, -244, -244, -244, -244,
-244, -244, -244, -244, -244, -244, -244, -244, -244, -244, -244,
-244, -244, -244, -244, -244, -244, -244, -244, -244, -244, -244,
-244, -244, -244, -244, -244, -244, -244, -244, -244, -244, -244,
-244, -244, -244, -244, -244, -244, -244, -244, -244, -244, -244,
-244, -244, -244, -244, -244, -244, -244, -244, -244, -244, -244,
-244, -244, -244, -244, -244, -244, -244, -244, -244, -244, -244,
-244, -244, -244, -244, -244, -244, -244, -244, -244, -244, -244,
-244, -244, -244, -244, -244, -244, -244, -244, -244, -244, -244,
-244, -244, -244, -244, -244, -244, -244, -244, -244, -244, -244,
-244, -244, -244, -244, -244, -244, -244, -244, -244, -244, -244,
-244, -244, -244, -244, -244, -244, 156, 156, 156, 156, 156, 156,
156, 156, 156, 156, 156, 156, 156, 156, 156, 156, 156, 156, 156, 156,
156, 156, 156, 156, 156, 156, 156, 156, 156, 156, 156, 156, 156, 156,
156, 156, 156, 156, 156, 156, 156, 156, 156, 156, 156, 156, 156, 156,
156, 156, 156, 156, 156, 156, 156, 156, 156, 156, 156, 156, 156, 156,
156, 156, 156, 156, 156, 156, 156, 156, 156, 156, 156, 156, 156, 156,
156, 156, 156, 156, 156, 156, 156, 156, 156, 156, 156, 156, 156, 156,
156, 156, 156, 156, 156, 156, 156, 156, 156, 156, 156, 156, 156, 156,
156, 156, 156, 156, 156, 156, 156, 156, 156, 156, 156, 156, 156, 156,
156, 156, 156, 156, 156, 156, 156, 156, 156, 156, 156, 156, 156, 156,
156, 156, 156, 156, 156, 156, 156, 156, 156, 156, 156, 156, 156, 156,
156, 156, 156, 156, 156, 156, 156, 156, 156, 156, 156, 156, 156, 156,
156, 156, 156, 156, 156, 156, 156, 156, 156, 156, 156, 156, 156, 156,
156, 156, 156, 156, 156, 156, 156, 156, 156, 156, 156, 156, 156, 156,
156, 156, 156, 156, 156, 156, 156, 156, 156, 156, 156, 156, 156, 156,
156, 156, 156, 156, 156, 156, 156, 156, 156, 156, 156, 156, 156, 156,
156, 156, 156, 156, 156, 156, 156, 156, 156, 156, 156, 156, 156, 156,
156, 156, 156, 156, 156, 156, 156, 156, 156, 156, 156, 156, 156, 156,
156, 156, 156, 156, 156, 156, 156, 156, 156, 156, 156, 156, 156, 156,
156, 156, 156, 156, 156, 156, 156, 156, 156, 156, 156, 156, 156, 156,
156, 156, 156, 156, 156, 156, 156, 156, 156, 156, 156, 156, 156, 156,
156, 156, 156, 156, 156, 156, 156, 156, 156, 156, 156, 156, 156, 156,
156, 156, 156, 156, 156, 156, 156, 156, 156, 156, 156, 156, 156, 156,
156, 156, 156, 156, 156, 156, 156, 156, 156, 156, 156, 156, 156, 156,
156, 156, 156, 156, 156, 156, 156, 156, 156, 156, 156, 156, 156, 156,
156, 156, 156, 156, 156, 156, 156, 156, 156, 156, 156, 156, 156, 156,
156, 156, 156, 156, 156, 156, 156, 156, 156, 156, 156, 156, 156, 156,
156, 156, 156, 156, 156, 156, 156, 156, 156, 156, 156, 156, 156, 156,
156, 156, 156, 156, 156, 156, 156, 156, 156, 156, 156, 156, 156, 156,
156, 156, 156, 156, 156, 156, 156, 156, 156, 156, 156, 156, 156, 156,
156, 156, 156, 156, 156, 156, 156, 156, 156, 156, 156, 156, 156, 156,
156, 156, 156, 156, 156, 156, 156, 156, 156, 156, 156, 156, 156, 156,
156, 156, 156, 156, 156, 156, 156, 156, 156, 156, 156, 156, 156, 156,
156, 156, 156, 156, 156, 156, 156, 156, 156, 156, 156, 156, 156, 156,
156, 156, 156, 156, 156, 156, 156, 156, 156, 156, 156, 156, 156, 156,
156, 156, 156, 156, 156, 156, 156, 156, 156, 156, 156, 156, 156, 156,
156, 156, 156, 156, 156, 156, 156, 156, 156, 156, 156, 156, 156, 156,
156, 156};


const uint8_t rates[256] = 
{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 1,
1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
1, 1, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3, 3, 3,
3, 3, 3, 3, 3, 3, 3, 3, 3, 3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4, 5, 5, 5,
5, 5, 5, 5, 5, 5, 6, 6, 6, 6, 6, 6, 6, 6, 7, 7, 7, 7, 7, 7, 8, 8, 8,
8, 8, 8, 9, 9, 9, 9, 9, 9, 10, 10, 10, 10, 11, 11, 11, 11, 11, 12,
12, 12, 12, 13, 13, 13, 13, 14, 14, 14, 15, 15, 15, 16, 16, 16, 16,
17, 17, 18, 18, 18, 19, 19, 19, 20, 20, 21, 21, 21, 22, 22, 23, 23,
24, 24, 25, 25, 26, 26, 27, 27, 28, 28, 29, 29, 30, 30, 31, 32, 32,
33, 34, 34, 35, 36, 36, 37, 38, 39, 39, 40, 41, 42, 42, 43, 44, 45,
46, 47, 48, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62,
64, 65, 66, 67, 69, 70, 71, 73, 74, 76, 77, 79, 80, 82, 83, 85, 87,
88, 90, 92, 94, 95, 97, 99, 101, 103, 105, 107, 109, 111, 113, 115,
118, 120, 122, 125, 127};

float invwarp;
float invwarprem;


void setup() 
    {
    Serial.begin(250000);
    startMozzi();
    digitalWrite(CV_GATE_OUT, 0);		// so we match the digitalState
    }

// this is gonna be costly, it has to go to 32 bit
// And it has a division...
// Maybe we should go floating point and precompute the m2/2 and m2/(m-p).... 
inline uint16_t computeWarp(uint16_t val)
	{
	// warp goes 0...1023
	// val goes 0...1023
	
	// WARP FUNCTION IS:	X -> val 0...m, p -> pivot 0...m, m -> max
	// Output is 0...m
	
	// F[x_, m_, p_] := If[x < p, x/p * (m/2),
  	//						(x - p) / (m - p) * (m/2) + (m/2)]
	
	const uint16_t m = 1023;
	const uint32_t m2 = 511;
	uint16_t x = val;
	if (x < warp)
		{
		uint16_t w = (uint16_t)((x * m2) * invwarp);	// / p;
		return w;
		}
	else
		{
		uint16_t w = (uint16_t)( m2 + ((x - warp) * m2) * invwarprem); // / (m - p);
		return w;
		}
	}

inline void reset()
	{
	count0 = 0; 
	count1 = 0;
	count2 = 0;
	position = 0;
	}
	
inline uint8_t countdown()
	{
	if (length == 0)
		{
		return 128 - (rate >> 1);
		}
	else if (length == 1)
		{
		if (count0 > 0)
			{
			count0--;
			return 0;
			}
		else
			{
			count0 = rates[rate];
			return 1;
			}
		}
	else // if (length == 1)
		{
		if (count1 > 0)
			{
			count1--;
			return 0;
			}
		else if (count0 > 0)
			{
			count0--;
			count1 = 127;
			return 0;
			}
		else
			{
			count0 = rates[rate];
			count1 = 127;
			return 1;
			}
		}
	}

static uint8_t types[9] = { 0, 1, 2, 0, 1, 2, 0, 1, 2 };
static uint8_t lengths[9] = { 0, 0, 0, 1, 1, 1, 2, 2, 2 };
void updateControl() 
    {
	rate = mozziAnalogRead(CV_POT_IN2) >> 2;		// 256
	 warp = mozziAnalogRead(CV_POT_IN1);		// 1024
	 invwarp = 1.0 / warp;
	 invwarprem= 1.0 / (1023 - warp);
	 gain = mozziAnalogRead(CV_AUDIO_IN) >> 3;		// 128
	uint8_t pot3 = (mozziAnalogRead(CV_POT3) * 9) >> 10;
	type = types[pot3];
	uint8_t oldLength = length;
	length = lengths[pot3];

	if (length != oldLength || mozziAnalogRead(CV_IN3) > 512) reset();		// also reset if we're making a big change to speed
    }


#define _COSINE(warped) ((int16_t)pgm_read_word_near(&COSINE[warped]))
#define _TRIANGLE(warped) ((int16_t)pgm_read_word_near(&TRIANGLE[warped]))
#define _PULSE(warped) ((int16_t)pgm_read_word_near(&PULSE[warped]))

inline int16_t adjust(int16_t val)
	{
#ifdef GAIN
	return (val * gain) >> 7;
#else
	return val;
#endif
	}

uint8_t triggerCountdown;
int updateAudio()    
    {
    // drop trigger
	  if (triggerCountdown > 0)
    	{
    	if (triggerCountdown == 1)
    		{
			digitalWrite(CV_GATE_OUT, 0);
    		}
    	triggerCountdown--;
    	}

    // So this is gonna be slow.  May be an issue in terms of total LFO rate...
    
    uint8_t increment = countdown();
    position += increment;
    if (position >= 1024) position -= 1024;
    uint16_t warped = computeWarp(position);

	// set trigger if need be
	if (position == 0)
		{
		digitalWrite(CV_GATE_OUT, 1);
		triggerCountdown = 255;
		}
	
    if (type==0)		// Sine
    	{
    	return adjust(_COSINE(warped));
    	}
    else if (type == 1)			// Saw/Tri/Ramp
    	{
    	return adjust(_TRIANGLE(warped));
    	}
    else //if (type == 2)			// Square
    	{
    	return adjust(_PULSE(warped));
    	}
    }

void loop() 
    {
    audioHook();
    }
