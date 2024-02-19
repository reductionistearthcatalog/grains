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


/// BOOKER
///
/// Booker is a Hammond Organ simulator, complete with Leslie.  It is meant to run on the 
/// AE Modular GRAINS, but it could be  adapted to any Arduino.  
///
/// SET GRAINS TO MOZZI MODE.  Sorry, no Grains mode.
///
/// You will need to install the Mozzi Library.  You can do this from the Library Manager
/// in your Arduino IDE.
///
/// ALIASING: At extremely high frequencies, a few of Booker's drawbars will go over Nyquist and
/// you may hear some antialiasing effects.  I can fix this but it puts me over the
/// computational budget of the device, so it's not going to happen.
///
/// THE LESLIE.  Booker comes with a Leslie, which is on by default:

#define LESLIE_ON               // To turn this off, put a // before the #define, as in //#define

// Leslie changes both the amplitude and pitch at a certain rate.
// You can adjust them here

#define LESLIE_FREQUENCY 5.66                                   // This is the 450 speed.  The classic slower speed is 0.66, but it's too slow
#define LESLIE_VOLUME 6                                 // Values are 0 (min but not gone), 1, 2, 3, 4, 5, 6 (max).
#define LESLIE_PITCH 60                                 // Values are 1.0 (lots) ... 128.0 or more (little).  Can be floating point.

/// ADJUSTING TUNING AND TRACKING
///
/// Grains's Inputs track 1.3V/octave, not 1V/octave: we'll need to scale them to track properly.  
/// To do this, you can adjust the Pitch CV Scaling on Pot 1.  This GRAINS program is set up to play 
/// the C three octaves below Middle C when it receives 0V.  You should be able to use Pot 1 to scale 
/// the pitch such that high Cs play in tune as well.  Once you have things tracking well, you can 
/// then use the Pitch Tune (Audio In) to tune 0V to some other note.  Note that as GRAINS resistors 
/// warm up, the scaling will change and you will need to adjust the tracking again, at least until 
/// they are fully warmed up.
///
/// By default the note corresponding to 0V is C0, three octaves below middle C, that is MIDI note 24, 
/// or 32.7 Hz.  You can customize the tuning for this Grains program but only UP.  This can be done 
/// in two ways.  First, you can add pitch to the tuning with a CV value to Audio In.  Second, you 
/// can transpose the pitch up by changing the TRANSPOSE_OCTAVES and/or TRANSPOSE_SEMITONES #defines 
/// in the code to positive integers.

/// CONFIGURATION
///
/// IN 1            Pitch CV
/// IN 2            Volume CV
/// IN 3            [Unused]
/// AUDIO IN (A)    Pitch Tune
/// AUDIO OUT       Out
/// DIGITAL OUT (D) [Unused]
///
/// POT 1           Pitch Scaling  [Set the switch to In1]
///
/// POT 2           Volume
///
/// POT 3           Choice of Organ [Set the switch to MAN]

/// Lastly, here is our present drawbar selection.  Keep this to no more than 31 but feel free
/// to change the selections.  There is a big list of 99 selections below you could swap in.

#define NUM_DRAWBAR_SELECTIONS 16
int16_t drawbars[NUM_DRAWBAR_SELECTIONS][9] = 
    {
    { 8, 8, 8, 8, 8, 8, 8, 8, 8 },    // 888888888 Full Organ
    { 8, 8, 5, 3, 2, 4, 5, 8, 8 },    // 885324588 Blues
    { 8, 8, 8, 8, 0, 0, 0, 0, 0 },    // 888800000 Booker T. Jones 1
    { 8, 8, 8, 6, 3, 0, 0, 0, 0 },    // 888630000 Booker T. Jones 2
    { 8, 7, 8, 0, 0, 0, 4, 5, 6 },    // 878000456 Bright Comping
    { 8, 4, 3, 0, 0, 0, 0, 0, 0 },    // 843000000 Dark Comping
    { 8, 0, 8, 8, 0, 8, 0, 0, 8 },    // 808808008 Gospel 1
    { 8, 8, 8, 0, 0, 0, 0, 0, 8 },    // 888000008 Gospel 2
    { 8, 6, 8, 6, 6, 6, 5, 6, 8 },    // 868666568 Greg Allman 1
    { 8, 8, 8, 6, 0, 0, 0, 0, 0 },    // 888600000 Greg Allman 2
    { 8, 8, 6, 8, 0, 0, 3, 0, 0 },    // 886800300 Paul Shaffer 1
    { 8, 8, 8, 7, 6, 8, 8, 8, 8 },    // 888768888 Paul Shaffer 2
    { 8, 8, 8, 8, 7, 8, 6, 7, 8 },    // 888878678 Paul Shaffer 3
    { 8, 0, 8, 0, 0, 0, 0, 0, 8 },    // 808000008 Reggae
    { 0, 8, 0, 0, 0, 0, 0, 0, 0 },    // 080000000 Sine
    { 8, 7, 6, 5, 4, 3, 2, 1, 1 },    // 876543211 Strings
    };

/** Here is a collection of Drawbar Selections for you.
    You can find more online.
        
    { 0, 0, 7, 7, 4, 0, 0, 3, 4 },    // 007740034 Alone in the City
    { 8, 8, 7, 7, 2, 4, 1, 1, 0 },    // 887724110 America (Gospel) (U)
    { 0, 0, 6, 6, 0, 6, 0, 0, 0 },    // 006606000 America (Gospel) (L)
    { 8, 8, 5, 3, 2, 4, 5, 8, 8 },    // 885324588 Blues
    { 8, 8, 8, 8, 0, 0, 0, 0, 0 },    // 888800000 Booker T. Jones 1
    { 8, 8, 8, 6, 3, 0, 0, 0, 0 },    // 888630000 Booker T. Jones 2
    { 8, 8, 8, 8, 0, 8, 0, 0, 8 },    // 888808008 Born to B3 (Gospel) (U)
    { 0, 0, 7, 7, 2, 5, 4, 0, 0 },    // 007725400 Born to B3 (Gospel) (L)
    { 8, 8, 8, 1, 1, 0, 0, 0, 0 },    // 888110000 Brian Auger 1
    { 8, 8, 8, 8, 0, 5, 0, 0, 0 },    // 888805000 Brian Auger 2
    { 8, 7, 8, 0, 0, 0, 4, 5, 6 },    // 878000456 Bright Comping
    { 8, 0, 0, 0, 0, 0, 8, 8, 8 },    // 800000888 Brother Jack
    { 8, 4, 3, 0, 0, 0, 0, 0, 0 },    // 843000000 Dark Comping
    { 8, 8, 8, 8, 8, 8, 8, 8, 8 },    // 888888888 Dark Solo A (U)
    { 6, 6, 2, 0, 0, 0, 0, 0, 0 },    // 662000000 Dark Solo A (L)
    { 8, 2, 8, 2, 0, 0, 0, 0, 2 },    // 828200002 Dark Solo B (U)
    { 6, 0, 6, 0, 0, 0, 0, 0, 0 },    // 606000000 Dark Solo B (L)
    { 8, 8, 8, 0, 0, 0, 8, 8, 8 },    // 888000888 Fat
    { 0, 8, 0, 0, 8, 0, 8, 8, 3 },    // 080080883 Fifth Organ (Gospel) (U)
    { 0, 0, 8, 8, 0, 6, 0, 0, 0 },    // 008806000 Fifth Organ (Gospel) (L)
    { 0, 0, 6, 8, 0, 2, 0, 0, 0 },    // 006802000 Flutes
    { 8, 8, 8, 6, 6, 6, 8, 8, 8 },    // 888666888 Full and High
    { 8, 6, 8, 8, 6, 8, 0, 6, 8 },    // 868868068 Full and Sweet
    { 8, 8, 8, 8, 8, 8, 8, 8, 8 },    // 888888888 Full Organ
    { 6, 8, 8, 6, 0, 0, 0, 0, 4 },    // 688600004 Funky Comping
    { 8, 8, 8, 8, 0, 0, 0, 0, 0 },    // 888800000 Gimme Some Loving
    { 8, 0, 8, 8, 0, 8, 0, 0, 8 },    // 808808008 Gospel 1
    { 8, 8, 8, 0, 0, 0, 0, 0, 8 },    // 888000008 Gospel 2
    { 8, 6, 8, 6, 6, 6, 5, 6, 8 },    // 868666568 Greg Allman 1
    { 8, 8, 8, 6, 0, 0, 0, 0, 0 },    // 888600000 Greg Allman 2
    { 8, 8, 6, 0, 0, 0, 0, 4, 0 },    // 886000040 Greg Allman 3
    { 8, 8, 8, 8, 0, 0, 0, 8, 8 },    // 888800088 Greg Rolie 1
    { 8, 8, 6, 4, 0, 0, 0, 0, 0 },    // 886400000 Greg Rolie 2
    { 8, 8, 8, 8, 8, 6, 6, 6, 6 },    // 888886666 Greg Rolie 4
    { 8, 8, 8, 4, 2, 0, 0, 8, 0 },    // 888420080 Groove Holmes (Gospel) (U)
    { 0, 0, 0, 5, 0, 5, 0, 0, 0 },    // 000505000 Groove Holmes (Gospel) (L)
    { 8, 8, 0, 0, 0, 0, 0, 0, 0 },    // 880000000 House Bass (Gospel) (U)
    { 0, 0, 8, 0, 8, 0, 0, 0, 0 },    // 008080000 House Bass (Gospel) (L)
    { 8, 6, 8, 6, 0, 0, 0, 0, 6 },    // 868600006 Jimmy McGriff 1
    { 8, 8, 3, 2, 0, 0, 1, 2, 5 },    // 883200125 Jimmy McGriff 2 (Gospel) (U)
    { 4, 4, 8, 6, 5, 0, 0, 0, 0 },    // 448650000 Jimmy McGriff 2 (Gospel) (L)
    { 8, 8, 8, 8, 8, 8, 8, 8, 8 },    // 888888888 Jimmy Smith 1 (U)
    { 0, 0, 7, 5, 0, 0, 0, 0, 0 },    // 007500000 Jimmy Smith 1 (L)
    { 8, 8, 8, 0, 0, 0, 0, 0, 0 },    // 888000000 Jimmy Smith 2 (U)
    { 8, 3, 8, 0, 0, 0, 0, 0, 0 },    // 838000000 Jimmy Smith 2 (L)
    { 8, 8, 8, 0, 0, 0, 0, 0, 0 },    // 888000000 Jimmy Smith 3 (U)
    { 8, 0, 8, 0, 0, 0, 0, 0, 0 },    // 808000000 Jimmy Smith 3 (L)
    { 8, 8, 8, 4, 0, 0, 0, 8, 0 },    // 888400080 Joey DeFrancesco
    { 8, 8, 4, 4, 0, 0, 0, 0, 0 },    // 884400000 Jon Lord
    { 8, 8, 0, 0, 6, 0, 0, 0, 0 },    // 880060000 Latin (Gospel) (U)
    { 0, 0, 6, 6, 7, 6, 0, 0, 0 },    // 006676000 Latin (Gospel) (L)
    { 8, 0, 0, 8, 0, 8, 0, 0, 0 },    // 800808000 Matthew Fisher
    { 8, 6, 8, 8, 0, 0, 0, 0, 4 },    // 868800004 Melvin Crispel
    { 8, 0, 3, 6, 0, 0, 0, 0, 0 },    // 803600000 Mellow
    { 0, 0, 7, 8, 0, 0, 4, 5, 3 },    // 007800453 Meditation Time (Gospel) (U)
    { 0, 0, 6, 7, 0, 0, 5, 4, 0 },    // 006700540 Meditation Time (Gospel) (L)
    { 8, 8, 6, 8, 0, 0, 3, 0, 0 },    // 886800300 Paul Shaffer 1
    { 8, 8, 8, 7, 6, 8, 8, 8, 8 },    // 888768888 Paul Shaffer 2
    { 8, 8, 8, 8, 7, 8, 6, 7, 8 },    // 888878678 Paul Shaffer 3
    { 8, 5, 0, 0, 0, 5, 0, 0, 0 },    // 850005000 Pink Floyd
    { 8, 8, 8, 8, 0, 0, 0, 0, 0 },    // 888800000 Power Chords
    { 8, 8, 8, 8, 0, 0, 0, 0, 0 },    // 888800000 Progessive (Gospel) (U)
    { 0, 0, 8, 8, 8, 4, 0, 0, 0 },    // 008884000 Progessive (Gospel) (L)
    { 0, 0, 6, 8, 7, 6, 4, 0, 0 },    // 006876400 Ray Charles
    { 8, 0, 8, 0, 0, 0, 0, 0, 8 },    // 808000008 Reggae
    { 8, 8, 8, 8, 0, 0, 0, 0, 0 },    // 888800000 Rock, R&B (U)
    { 8, 4, 8, 0, 0, 0, 0, 0, 0 },    // 848000000 Rock, R&B (L)
    { 8, 0, 0, 3, 8, 8, 8, 8, 8 },    // 800388888 Screaming (Gospel) (U)
    { 0, 0, 7, 0, 3, 3, 3, 3, 3 },    // 007033333 Screaming (Gospel) (L)
    { 0, 0, 8, 8, 8, 8, 8, 0, 0 },    // 008888800 Shirley Scott
    { 8, 3, 0, 0, 0, 0, 3, 7, 8 },    // 830000378 Simmering
    { 0, 8, 0, 0, 0, 0, 0, 0, 0 },    // 080000000 Sine
    { 8, 7, 6, 5, 5, 6, 7, 8, 8 },    // 876556788 Shouting 1
    { 6, 6, 8, 8, 4, 8, 5, 8, 8 },    // 668848588 Shouting 2
    { 8, 7, 8, 6, 4, 5, 4, 6, 6 },    // 878645466 Shouting 3 (Gospel) (U)
    { 8, 8, 8, 8, 0, 0, 0, 0, 0 },    // 888800000 Shouting 3 (Gospel) (L)
    { 0, 0, 8, 4, 0, 0, 0, 0, 0 },    // 008400000 Slow Balllad
    { 0, 6, 8, 8, 4, 0, 0, 0, 3 },    // 068840003 Slowly
    { 8, 8, 8, 7, 0, 0, 0, 0, 0 },    // 888700000 Soft Backing (Gospel) (U)
    { 5, 5, 5, 4, 0, 0, 0, 0, 0 },    // 555400000 Soft Backing (Gospel) (L)
    { 8, 0, 8, 4, 0, 0, 0, 0, 8 },    // 808400008 Soft Chords
    { 6, 7, 8, 4, 0, 4, 2, 3, 1 },    // 678404231 Speaker Talking (Gospel) (U)
    { 0, 0, 6, 6, 0, 2, 0, 2, 4 },    // 006602024 Speaker Talking (Gospel) (L)
    { 8, 8, 8, 6, 4, 3, 2, 0, 0 },    // 888643200 Steppenwolf
    { 8, 8, 8, 8, 7, 6, 7, 8, 8 },    // 888876788 Steve Winwood
    { 8, 7, 6, 5, 4, 3, 2, 1, 1 },    // 876543211 Strings
    { 0, 0, 8, 0, 0, 0, 0, 0, 0 },    // 008000000 Sweet
    { 7, 8, 7, 7, 4, 6, 0, 4, 6 },    // 787746046 Testimony Service  (Gospel) (U)
    { 0, 0, 8, 8, 0, 0, 6, 7, 3 },    // 008800673 Testimony Service  (Gospel) (L)
    { 8, 7, 8, 6, 5, 6, 4, 6, 7 },    // 878656467 Theatre Organ (Gospel) (U)
    { 0, 0, 8, 8, 4, 4, 0, 0, 0 },    // 008844000 Theatre Organ (Gospel) (L)
    { 8, 8, 8, 8, 0, 0, 0, 0, 0 },    // 888800000 Tom Coster
    { 8, 0, 0, 0, 0, 0, 0, 0, 8 },    // 800000008 Whistle 1
    { 8, 8, 8, 0, 0, 0, 0, 0, 8 },    // 888000008 Whistle 2
    { 6, 8, 8, 6, 0, 0, 0, 0, 0 },    // 688600000 Whiter Shade Of Pale 1 (U)
    { 8, 8, 0, 0, 7, 0, 7, 7, 0 },    // 880070770 Whiter Shade Of Pale 1 (L)
    { 8, 8, 8, 8, 0, 8, 0, 0, 6 },    // 888808006 Whiter Shade Of Pale 2 (U)
    { 0, 0, 4, 4, 4, 0, 0, 0, 0 },    // 004440000 Whiter Shade Of Pale 2 (L)
    { 8, 6, 6, 8, 0, 0, 0, 0, 0 },    // 866800000 Wide Leslie
*/    



PROGMEM const float frequencies[1024+512] = {
    32.7, 32.81097, 32.922318, 33.034042, 33.146145, 33.25863, 33.37149, 33.48474, 33.59837, 33.71239, 33.826794, 33.94159, 34.056774, 34.172344, 34.28831, 34.40467, 
    34.521423, 34.638577, 34.756123, 34.87407, 34.992416, 35.111168, 35.23032, 35.349876, 35.469837, 35.590206, 35.710987, 35.832172, 35.95377, 36.075783, 36.198208, 36.32105, 
    36.444305, 36.56798, 36.692078, 36.816593, 36.941532, 37.066895, 37.192684, 37.3189, 37.445545, 37.572617, 37.700123, 37.82806, 37.956432, 38.08524, 38.214485, 38.34417, 
    38.47429, 38.60486, 38.735863, 38.867317, 38.999214, 39.131565, 39.26436, 39.397606, 39.531303, 39.665455, 39.80006, 39.935123, 40.070644, 40.20663, 40.34307, 40.47998, 
    40.617348, 40.755188, 40.893494, 41.03227, 41.171513, 41.31123, 41.451424, 41.59209, 41.733234, 41.874863, 42.016968, 42.159554, 42.302628, 42.446182, 42.59022, 42.73476, 
    42.87978, 43.025295, 43.171303, 43.317806, 43.46481, 43.612312, 43.760315, 43.908817, 44.057823, 44.207336, 44.357357, 44.507885, 44.658924, 44.81048, 44.962543, 45.115128, 
    45.26823, 45.421852, 45.575993, 45.73066, 45.885845, 46.04156, 46.197807, 46.354584, 46.511887, 46.66973, 46.828106, 46.987022, 47.146477, 47.30647, 47.467007, 47.62809, 
    47.78972, 47.951893, 48.11462, 48.277905, 48.44174, 48.60613, 48.771072, 48.93658, 49.102654, 49.269283, 49.43648, 49.604248, 49.772583, 49.94149, 50.11097, 50.281025, 
    50.451656, 50.622868, 50.79466, 50.967033, 51.13999, 51.31354, 51.487675, 51.6624, 51.83772, 52.013634, 52.190147, 52.36726, 52.544968, 52.723286, 52.902206, 53.08173, 
    53.261868, 53.44261, 53.623974, 53.80595, 53.988544, 54.171757, 54.355595, 54.540054, 54.725136, 54.91085, 55.097195, 55.28417, 55.47178, 55.660027, 55.848915, 56.038437, 
    56.228607, 56.419422, 56.61089, 56.803, 56.99576, 57.189182, 57.38326, 57.57799, 57.773384, 57.96944, 58.166164, 58.363556, 58.561615, 58.76035, 58.95976, 59.15984, 
    59.360603, 59.562046, 59.764175, 59.966988, 60.17049, 60.374683, 60.579563, 60.785145, 60.991425, 61.198402, 61.406086, 61.614468, 61.82356, 62.033363, 62.243877, 62.455105, 
    62.667053, 62.879715, 63.093098, 63.307213, 63.52205, 63.737614, 63.95391, 64.170944, 64.38871, 64.607216, 64.82647, 65.04646, 65.2672, 65.488686, 65.71092, 65.93392, 
    66.15767, 66.38218, 66.60745, 66.83349, 67.06029, 67.287865, 67.51621, 67.74534, 67.975235, 68.20591, 68.43737, 68.66962, 68.90265, 69.136475, 69.37109, 69.606514, 
    69.84273, 70.07974, 70.31756, 70.55618, 70.79562, 71.03587, 71.27694, 71.518814, 71.76152, 72.00505, 72.249405, 72.49458, 72.7406, 72.98745, 73.23514, 73.483665, 
    73.73304, 73.98325, 74.23433, 74.48624, 74.73901, 74.992645, 75.24714, 75.502495, 75.75872, 76.01581, 76.27377, 76.53261, 76.79233, 77.052925, 77.314415, 77.57678, 
    77.84005, 78.1042, 78.36925, 78.63521, 78.902054, 79.169815, 79.438484, 79.70806, 79.97856, 80.24997, 80.52231, 80.795555, 81.06975, 81.34486, 81.62091, 81.897896, 
    82.17582, 82.45468, 82.734505, 83.01527, 83.29699, 83.57966, 83.8633, 84.14788, 84.43345, 84.71997, 85.00748, 85.29596, 85.58542, 85.875854, 86.167274, 86.459694, 
    86.7531, 87.0475, 87.3429, 87.63931, 87.936714, 88.23513, 88.53457, 88.835014, 89.136475, 89.43897, 89.742485, 90.04703, 90.352615, 90.65923, 90.96689, 91.27559, 
    91.585335, 91.89614, 92.208, 92.52091, 92.83488, 93.14992, 93.466034, 93.78321, 94.10147, 94.420815, 94.741234, 95.062744, 95.38535, 95.709045, 96.03384, 96.35974, 
    96.68674, 97.014854, 97.34407, 97.674416, 98.00588, 98.33848, 98.672195, 99.007034, 99.343025, 99.68015, 100.018425, 100.35785, 100.69841, 101.04014, 101.383026, 101.72707, 
    102.072296, 102.418686, 102.76624, 103.11499, 103.46492, 103.81603, 104.168335, 104.521835, 104.87654, 105.232445, 105.58956, 105.94788, 106.30742, 106.66818, 107.03017, 107.39338, 
    107.75783, 108.12351, 108.49043, 108.858604, 109.22803, 109.598694, 109.97062, 110.34382, 110.71828, 111.094, 111.47101, 111.8493, 112.22886, 112.60971, 112.99186, 113.375305, 
    113.760056, 114.1461, 114.53347, 114.92215, 115.31214, 115.70346, 116.0961, 116.49008, 116.8854, 117.28206, 117.68006, 118.079414, 118.480125, 118.882195, 119.28563, 119.69043, 
    120.09661, 120.504166, 120.9131, 121.323425, 121.73515, 122.14826, 122.56278, 122.97871, 123.39604, 123.8148, 124.23497, 124.65657, 125.0796, 125.50407, 125.92997, 126.35732, 
    126.78612, 127.21638, 127.64809, 128.08127, 128.51593, 128.95204, 129.38965, 129.82875, 130.26933, 130.71141, 131.15498, 131.60007, 132.04666, 132.49478, 132.9444, 133.39555, 
    133.84824, 134.30246, 134.75822, 135.21555, 135.6744, 136.13481, 136.59679, 137.06035, 137.52547, 137.99217, 138.46046, 138.93033, 139.40181, 139.87486, 140.34953, 140.82582, 
    141.30373, 141.78326, 142.2644, 142.7472, 143.2316, 143.71767, 144.2054, 144.69475, 145.18579, 145.67848, 146.17285, 146.6689, 147.16663, 147.66605, 148.16716, 148.66997, 
    149.1745, 149.68073, 150.18869, 150.69836, 151.20976, 151.7229, 152.23778, 152.75441, 153.27278, 153.79292, 154.31483, 154.83852, 155.36397, 155.8912, 156.42023, 156.95105, 
    157.48367, 158.01811, 158.55435, 159.0924, 159.6323, 160.17403, 160.71759, 161.26299, 161.81024, 162.35936, 162.91034, 163.46318, 164.0179, 164.57451, 165.133, 165.69339, 
    166.25568, 166.81989, 167.38599, 167.95403, 168.52399, 169.09589, 169.66972, 170.24551, 170.82324, 171.40294, 171.9846, 172.56825, 173.15387, 173.74149, 174.33109, 174.92268, 
    175.5163, 176.11192, 176.70956, 177.30923, 177.91096, 178.51471, 179.1205, 179.72836, 180.33829, 180.95027, 181.56433, 182.18048, 182.79874, 183.41907, 184.0415, 184.66606, 
    185.29276, 185.92155, 186.55247, 187.18556, 187.82079, 188.45816, 189.09772, 189.73943, 190.38332, 191.0294, 191.67766, 192.32814, 192.9808, 193.6357, 194.29282, 194.95216, 
    195.61374, 196.27757, 196.94365, 197.61198, 198.2826, 198.95547, 199.63065, 200.3081, 200.98785, 201.66992, 202.35431, 203.04102, 203.73004, 204.42142, 205.11514, 205.8112, 
    206.50964, 207.21043, 207.91362, 208.6192, 209.32715, 210.0375, 210.75029, 211.46548, 212.1831, 212.90317, 213.62566, 214.35062, 215.07803, 215.8079, 216.54027, 217.2751, 
    218.01244, 218.75227, 219.49463, 220.2395, 220.9869, 221.73682, 222.4893, 223.24432, 224.00192, 224.7621, 225.52484, 226.29016, 227.0581, 227.82864, 228.60178, 229.37756, 
    230.15598, 230.93703, 231.72072, 232.50706, 233.2961, 234.08781, 234.88219, 235.67928, 236.47908, 237.28157, 238.0868, 238.89478, 239.70547, 240.51894, 241.33514, 242.15413, 
    242.97589, 243.80046, 244.62779, 245.45796, 246.29094, 247.12674, 247.96538, 248.80687, 249.65121, 250.49841, 251.3485, 252.20146, 253.05731, 253.91609, 254.77777, 255.64238, 
    256.50992, 257.3804, 258.2538, 259.13025, 260.0096, 260.89197, 261.7773, 262.66568, 263.55707, 264.45145, 265.34888, 266.24933, 267.1529, 268.05948, 268.96915, 269.88193, 
    270.7978, 271.71674, 272.63882, 273.56406, 274.4924, 275.42392, 276.3586, 277.29642, 278.23743, 279.18167, 280.1291, 281.0797, 282.03357, 282.9907, 283.95102, 284.91464, 
    285.8815, 286.85165, 287.82513, 288.80188, 289.78192, 290.76532, 291.75204, 292.74213, 293.7356, 294.7324, 295.7326, 296.73615, 297.74316, 298.75357, 299.76743, 300.78467, 
    301.8054, 302.82962, 303.8573, 304.88843, 305.9231, 306.96127, 308.00296, 309.0482, 310.097, 311.1493, 312.20517, 313.2647, 314.32776, 315.39444, 316.46475, 317.53873, 
    318.61627, 319.69754, 320.78244, 321.87106, 322.96335, 324.05936, 325.15906, 326.26248, 327.3697, 328.48065, 329.59537, 330.71384, 331.83618, 332.96228, 334.0922, 335.22595, 
    336.36356, 337.50504, 338.65036, 339.79962, 340.95273, 342.10977, 343.27072, 344.43567, 345.60455, 346.77734, 347.95416, 349.13498, 350.31976, 351.5086, 352.70148, 353.89838, 
    355.09937, 356.3044, 357.51355, 358.7268, 359.94415, 361.16565, 362.3913, 363.6211, 364.85507, 366.0932, 367.33557, 368.58215, 369.83298, 371.088, 372.34732, 373.6109, 
    374.87878, 376.15094, 377.42746, 378.70825, 379.99344, 381.28296, 382.57687, 383.87518, 385.17786, 386.485, 387.79654, 389.11258, 390.43304, 391.758, 393.08746, 394.42142, 
    395.75992, 397.10294, 398.45056, 399.8027, 401.1595, 402.52084, 403.8868, 405.25742, 406.6327, 408.01263, 409.39722, 410.78656, 412.1806, 413.57935, 414.98285, 416.3911, 
    417.80417, 419.22202, 420.64468, 422.07214, 423.5045, 424.94168, 426.38376, 427.83072, 429.28256, 430.73938, 432.20108, 433.66782, 435.13947, 436.61615, 438.09784, 439.58453, 
    441.07632, 442.57312, 444.075, 445.582, 447.09412, 448.61136, 450.13376, 451.66132, 453.19406, 454.73203, 456.27515, 457.82355, 459.3772, 460.93616, 462.50037, 464.0699, 
    465.6447, 467.2249, 468.8105, 470.4014, 471.99774, 473.5995, 475.2067, 476.81934, 478.43744, 480.06107, 481.6902, 483.3248, 484.96503, 486.61075, 488.26212, 489.91907, 
    491.58163, 493.24985, 494.9237, 496.60327, 498.28854, 499.9795, 501.6762, 503.37866, 505.0869, 506.80096, 508.5208, 510.24652, 511.97806, 513.7155, 515.4588, 517.20807, 
    518.96326, 520.72437, 522.49146, 524.2646, 526.0437, 527.82886, 529.6201, 531.4174, 533.2208, 535.0303, 536.84595, 538.6678, 540.4958, 542.32996, 544.1704, 546.01715, 
    547.87, 549.7293, 551.59485, 553.4667, 555.3449, 557.2295, 559.1205, 561.01794, 562.9218, 564.8321, 566.7489, 568.6721, 570.602, 572.5383, 574.4813, 576.43085, 
    578.387, 580.3498, 582.3193, 584.2954, 586.2782, 588.26776, 590.2641, 592.2672, 594.2771, 596.2938, 598.3174, 600.3478, 602.3851, 604.4293, 606.4805, 608.5387, 
    610.60376, 612.6759, 614.75507, 616.84125, 618.9346, 621.0349, 623.14246, 625.2571, 627.3789, 629.508, 631.6443, 633.7878, 635.93866, 638.09674, 640.26215, 642.4349, 
    644.61505, 646.8026, 648.99756, 651.19995, 653.40985, 655.62726, 657.8521, 660.0846, 662.32465, 664.57227, 666.8275, 669.09045, 671.361, 673.63934, 675.9254, 678.21924, 
    680.5208, 682.8302, 685.1474, 687.4725, 689.8054, 692.14636, 694.4952, 696.85205, 699.2168, 701.58966, 703.9705, 706.3595, 708.7566, 711.16174, 713.5752, 715.9967, 
    718.42645, 720.86456, 723.31085, 725.7654, 728.22833, 730.69965, 733.1793, 735.6674, 738.16394, 740.66895, 743.18243, 745.7045, 748.23505, 750.7743, 753.322, 755.87854, 
    758.44366, 761.01746, 763.6, 766.19135, 768.79144, 771.4004, 774.0182, 776.64484, 779.28046, 781.925, 784.5785, 787.241, 789.9126, 792.5932, 795.2829, 797.98175, 
    800.6897, 803.4069, 806.13336, 808.869, 811.61395, 814.3682, 817.13184, 819.90485, 822.68726, 825.47906, 828.28033, 831.0912, 833.9115, 836.74146, 839.581, 842.4302, 
    845.289, 848.15753, 851.0358, 853.9238, 856.8217, 859.7294, 862.647, 865.57434, 868.5118, 871.4591, 874.4165, 877.38385, 880.3613, 883.3488, 886.34656, 889.35443, 
    892.3725, 895.4008, 898.4394, 901.4883, 904.54755, 907.61725, 910.69727, 913.7878, 916.8888, 920.0003, 923.1224, 926.25507, 929.3983, 932.5523, 935.717, 938.8924, 
    942.0786, 945.27563, 948.48346, 951.7022, 954.9319, 958.1725, 961.42413, 964.68677, 967.96045, 971.2453, 974.54126, 977.84845, 981.1668, 984.4965, 987.8374, 991.1897, 
    994.55334, 997.92847, 1001.31494, 1004.713, 1008.12256, 1011.5437, 1014.97644, 1018.42084, 1021.8769, 1025.3447, 1028.8242, 1032.3157, 1035.8188, 1039.334, 1042.861, 1046.4, 
    1049.951, 1053.5142, 1057.0894, 1060.6766, 1064.2761, 1067.8877, 1071.5117, 1075.1478, 1078.7965, 1082.4574, 1086.1309, 1089.8168, 1093.515, 1097.226, 1100.9495, 1104.6855, 
    1108.4344, 1112.1959, 1115.9702, 1119.7573, 1123.5574, 1127.3702, 1131.196, 1135.0348, 1138.8866, 1142.7516, 1146.6295, 1150.5206, 1154.425, 1158.3427, 1162.2736, 1166.2178, 
    1170.1754, 1174.1465, 1178.131, 1182.129, 1186.1406, 1190.1659, 1194.2048, 1198.2574, 1202.3237, 1206.4039, 1210.4979, 1214.6058, 1218.7277, 1222.8635, 1227.0134, 1231.1772, 
    1235.3555, 1239.5476, 1243.7542, 1247.9749, 1252.2101, 1256.4595, 1260.7234, 1265.0017, 1269.2946, 1273.6019, 1277.924, 1282.2606, 1286.6122, 1290.9783, 1295.3594, 1299.7551, 
    1304.166, 1308.5918, 1313.0326, 1317.4884, 1321.9594, 1326.4456, 1330.9469, 1335.4635, 1339.9956, 1344.543, 1349.1057, 1353.6841, 1358.2778, 1362.8871, 1367.5123, 1372.153, 
    1376.8094, 1381.4817, 1386.1698, 1390.8739, 1395.594, 1400.3301, 1405.0822, 1409.8503, 1414.6348, 1419.4354, 1424.2523, 1429.0856, 1433.9353, 1438.8014, 1443.6841, 1448.5834, 
    1453.4993, 1458.4318, 1463.3811, 1468.347, 1473.33, 1478.3298, 1483.3467, 1488.3804, 1493.4314, 1498.4994, 1503.5847, 1508.6873, 1513.807, 1518.9442, 1524.0989, 1529.271, 
    1534.4606, 1539.6678, 1544.893, 1550.1356, 1555.3961, 1560.6743, 1565.9706, 1571.2849, 1576.6171, 1581.9674, 1587.3359, 1592.7227, 1598.1277, 1603.551, 1608.9928, 1614.453, 
    1619.9318, 1625.4291, 1630.9451, 1636.4797, 1642.0333, 1647.6056, 1653.1968, 1658.807, 1664.4363, 1670.0847, 1675.7523, 1681.439, 1687.1451, 1692.8706, 1698.6154, 1704.3798, 
    1710.1636, 1715.9672, 1721.7904, 1727.6334, 1733.4962, 1739.379, 1745.2817, 1751.2043, 1757.1472, 1763.1102, 1769.0934, 1775.0969, 1781.1208, 1787.1653, 1793.23, 1799.3154, 
    1805.4215, 1811.5485, 1817.696, 1823.8644, 1830.0538, 1836.2643, 1842.4957, 1848.7483, 1855.0221, 1861.3173, 1867.6338, 1873.9717, 1880.3312, 1886.7123, 1893.1149, 1899.5393, 
    1905.9855, 1912.4536, 1918.9436, 1925.4557, 1931.9899, 1938.546, 1945.1246, 1951.7256, 1958.3489, 1964.9948, 1971.663, 1978.3539, 1985.0676, 1991.8041, 1998.5634, 2005.3457, 
    2012.1509, 2018.9791, 2025.8308, 2032.7056, 2039.6036, 2046.5251, 2053.4702, 2060.4387, 2067.431, 2074.447, 2081.4868, 2088.5503, 2095.638, 2102.7495, 2109.8855, 2117.0454, 
    2124.2297, 2131.4385, 2138.6716, 2145.9292, 2153.2117, 2160.5188, 2167.8508, 2175.2075, 2182.589, 2189.9958, 2197.4277, 2204.8848, 2212.3672, 2219.875, 2227.4084, 2234.9673, 
    2242.5518, 2250.1619, 2257.7979, 2265.46, 2273.148, 2280.862, 2288.602, 2296.3687, 2304.1616, 2311.981, 2319.8267, 2327.6992, 2335.5984, 2343.5244, 2351.4773, 2359.4573, 
    2367.464, 2375.4985, 2383.5596, 2391.6484, 2399.7646, 2407.9084, 2416.0798, 2424.279, 2432.5059, 2440.7607, 2449.0435, 2457.3545, 2465.6936, 2474.0613, 2482.457, 2490.8816, 
    2499.3345, 2507.816, 2516.3267, 2524.8657, 2533.434, 2542.0315, 2550.658, 2559.314, 2567.999, 2576.7139, 2585.4578, 2594.232, 2603.0354, 2611.8691, 2620.7327, 2629.6262, 
    2638.5498, 2647.5042, 2656.4885, 2665.5037, 2674.549, 2683.6255, 2692.7322, 2701.8704, 2711.039, 2720.2393, 2729.4707, 2738.7334, 2748.0273, 2757.3528, 2766.7102, 2776.099, 
    2785.52, 2794.973, 2804.458, 2813.9749, 2823.5242, 2833.1062, 2842.7205, 2852.3672, 2862.047, 2871.7595, 2881.505, 2891.2837, 2901.0955, 2910.9404, 2920.8188, 2930.7307, 
    2940.6765, 2950.656, 2960.6692, 2970.716, 2980.7974, 2990.913, 3001.0627, 3011.247, 3021.466, 3031.7195, 3042.0078, 3052.3313, 3062.6895, 3073.0828, 3083.5117, 3093.9756, 
    3104.4753, 3115.0103, 3125.5813, 3136.1882, 3146.8313, 3157.5103, 3168.225, 3178.9768, 3189.765, 3200.5896, 3211.4512, 3222.349, 3233.2844, 3244.2568, 3255.2664, 3266.3135, 
    3277.398, 3288.5198, 3299.6797, 3310.8774, 3322.113, 3333.3867, 3344.6987, 3356.0493, 3367.4382, 3378.866, 3390.3323, 3401.8374, 3413.3818, 3424.9653, 3436.5881, 3448.2505, 
    3459.9524, 3471.6938, 3483.4753, 3495.2969, 3507.1582, 3519.0598, 3531.0022, 3542.9849, 3555.008, 3567.0723, 3579.1775, 3591.3235, 3603.5107, 3615.7395, 3628.0098, 3640.3218, 
    3652.6753, 3665.071, 3677.5088, 3689.9885, 3702.5107, 3715.0752, 3727.6826, 3740.3328, 3753.026, 3765.762, 3778.5413, 3791.364, 3804.2302, 3817.1401, 3830.0938, 3843.0916, 
    3856.1333, 3869.2192, 3882.3496, 3895.525, 3908.7444, 3922.009, 3935.3186, 3948.6733, 3962.0735, 3975.519, 3989.0103, 4002.547, 4016.1301, 4029.759, 4043.4343, 4057.1558, 
    4070.924, 4084.7388, 4098.6006, 4112.51, 4126.4653, 4140.4688, 4154.52, 4168.6187, 4182.765, 4196.9595, 4211.202, 4225.493, 4239.833, 4254.2207, 4268.6577, 4283.1436, 
    4297.6787, 4312.263, 4326.8975, 4341.5806, 4356.314, 4371.097, 4385.931, 4400.815, 4415.7495, 4430.735, 4445.7705, 4460.858, 4475.9956, 4491.185, 4506.4263, 4521.719, 
    4537.0645, 4552.461, 4567.91, 4583.411, 4598.9653, 4614.5728, 4630.232, 4645.9453, 4661.7114, 4677.5313, 4693.405, 4709.332, 4725.3135, 4741.349, 4757.439, 4773.584, 
    4789.783, 4806.038, 4822.3477, 4838.7124, 4855.133, 4871.609, 4888.141, 4904.729, 4921.3735, 4938.0747, 4954.8325, 4971.647, 4988.5186, 5005.4473, 5022.4336, 5039.4775, 
    5056.5796, 5073.7393, 5090.957, 5108.2334, 5125.569, 5142.963, 5160.4155, 5177.9277, 5195.4995, 5213.131, 5230.822, 5248.5728, 5266.3843, 5284.256, 5302.1885, 5320.1816, 
    5338.2363, 5356.3516, 5374.529, 5392.7676, 5411.0684, 5429.431, 5447.8564, 5466.3438, 5484.894, 5503.5073, 5522.184, 5540.924, 5559.7275, 5578.5947, 5597.526, 5616.5215, 
    5635.5815, 5654.706, 5673.8955, 5693.151, 5712.4707, 5731.856, 5751.3076, 5770.825, 5790.4087, 5810.0586, 5829.7754, 5849.5596, 5869.41, 5889.328, 5909.314, 5929.368, 
    };


#define FREQUENCY(pitch) pgm_read_float_near(&frequencies[pitch])
#define CONTROL_RATE 100                        // More than this and we're gonna get clicks probably

#include <MozziGuts.h>
#include <Oscil.h>
#include <tables/sin256_int8.h>
#include <tables/sin1024_int8.h>

/// OSCILLATORS

Oscil<SIN1024_NUM_CELLS, AUDIO_RATE> tonewheel1(SIN1024_DATA);
Oscil<SIN1024_NUM_CELLS, AUDIO_RATE> tonewheel2(SIN1024_DATA);
Oscil<SIN1024_NUM_CELLS, AUDIO_RATE> tonewheel3(SIN1024_DATA);
Oscil<SIN1024_NUM_CELLS, AUDIO_RATE> tonewheel4(SIN1024_DATA);
Oscil<SIN1024_NUM_CELLS, AUDIO_RATE> tonewheel5(SIN1024_DATA);
Oscil<SIN1024_NUM_CELLS, AUDIO_RATE> tonewheel6(SIN1024_DATA);
Oscil<SIN1024_NUM_CELLS, AUDIO_RATE> tonewheel7(SIN1024_DATA);
Oscil<SIN1024_NUM_CELLS, AUDIO_RATE> tonewheel8(SIN1024_DATA);
Oscil<SIN1024_NUM_CELLS, AUDIO_RATE> tonewheel9(SIN1024_DATA);

Oscil<SIN1024_NUM_CELLS, AUDIO_RATE> oscils[9] = 
    { 
    tonewheel1, tonewheel2, tonewheel3,
    tonewheel4, tonewheel5, tonewheel6, 
    tonewheel7, tonewheel8, tonewheel9 
    };

#ifdef LESLIE_ON
Oscil<SIN256_NUM_CELLS, CONTROL_RATE> leslie(SIN256_DATA);              // Leslie LFO
#endif
        
//float drawbarFrequencies[9] = { 0.5, 1.0, 1.5, 2.0, 3.0, 4.0, 5.0, 6.0, 8.0 };
uint8_t drawbarFrequencies[9] = { 1, 2, 3, 4, 6, 8, 10, 12, 16 };

// These are the amplitudes of each of the drawbar stop positions (0...8).
// Each additional stop increases by 3db.  I think I have this right?  Not sure.
// We are int8_t, not uin8_t, so we can multiply faster against oscils, which is
// also int8_t.
int8_t drawBarAmplitudes[9] = 
    {
    0, 7, 18, 32, 48, 65, 85, 105, 127
    };


#define CV_POT_IN1    A2    // Note in, Pitch Scaling
#define CV_POT_IN2    A1    // Gain
#define CV_POT3       A0    // Organ Selection
#define CV_IN3        A3    // [Unused]
#define CV_AUDIO_IN   A4    // Pitch Tune
#define CV_AUDIO_OUT  9     // Out
#define CV_GATE_OUT   8     // [Unused]


#define MEDIAN_OF_THREE(a,b,c) (((a) <= (b)) ? (((b) <= (c)) ? (b) : (((a) < (c)) ? (c) : (a))) : (((a) <= (c)) ? (a) : (((b) < (c)) ? (c) : (b))))
uint16_t pitchCV;
uint16_t tuneCV;
uint16_t pA;
uint16_t pB;
void initializeFrequency(uint8_t pitch, uint8_t tune)
    {
    pitchCV = mozziAnalogRead(pitch);
    tuneCV = mozziAnalogRead(tune);
    }
        
#define TRANSPOSE_SEMITONES 0
#define TRANSPOSE_OCTAVES 0
#define LARGE_JUMP 32
#define SEMITONE 17
#define FREQ_COUNTER_MAX 4
uint8_t freqCounter = 0;
inline float getFrequency(uint8_t pitch, uint8_t tune)
    {
    tuneCV = (tuneCV * 15 + mozziAnalogRead(tune)) >> 4;
    uint16_t p = mozziAnalogRead(pitch);

    uint16_t diff = (p > pitchCV ? p - pitchCV : pitchCV - p);
    if (diff >= LARGE_JUMP)
        {
        pitchCV = p;            // jump right there
        freqCounter = FREQ_COUNTER_MAX;
        }
    else if (freqCounter > 0)
        {
        freqCounter--;
        pitchCV = (pitchCV + p) >> 1;
        pB = pA;
        pA = pitchCV;
        }
    else
        {
        uint16_t p1 = MEDIAN_OF_THREE(p, pA, pB);
        pB = pA;
        pA = p;
        pitchCV = (pitchCV * 7 + p1) >> 3;
        }
    return FREQUENCY(pitchCV + (tuneCV >> 1) + TRANSPOSE_SEMITONES * 17 + TRANSPOSE_OCTAVES * 205);
    }




void setup()
    {
    startMozzi();
    // Fire up the leslie
#ifdef LESLIE_ON  
    leslie.setFreq((float)LESLIE_FREQUENCY);
#endif

// precompute the drawbars so we don't have to compute them during updateAudio()
    for(uint8_t i = 0; i < NUM_DRAWBAR_SELECTIONS; i++)
        {
        for(uint8_t j = 0; j < 9; j++)
            {
            drawbars[i][j] = drawBarAmplitudes[drawbars[i][j]];
            }
        }
    initializeFrequency(CV_POT_IN1, CV_AUDIO_IN);
    }

void loop()
    {
    audioHook();
    }
    

// Median-Of-Three Filter for Pitch, dunno if we need it
uint16_t inA = 0;
uint16_t inB = 0;
uint8_t organ = 0;      // Current index into the drawbars array
uint16_t gain;          // our current gain

#define COUNTER_MAX 4
uint16_t counter =0;
void updateControl()                          
    {
    float frequency = getFrequency(CV_POT_IN1, CV_AUDIO_IN);
        
    // leslie!
#ifdef LESLIE_ON
    int8_t les = leslie.next();
    frequency += les * (1.0 / LESLIE_PITCH);
#endif

    // set the drawbars
    for(uint8_t i = 0; i < 9; i++)
        {
        oscils[i].setFreq((frequency * drawbarFrequencies[i]));
        }

    // determine the gain
    gain = 1 + (mozziAnalogRead(CV_POT_IN2) >> 2)                       // Gain (Volume)
#ifdef LESLIE_ON
#if LESLIE_VOLUME==0
#else
        + ((les + 128) >> (8 - (LESLIE_VOLUME - 1)));         // note: >> 8 still goes between -1 and 0 depending on whether it's positive or negative initially
#endif
#endif
    ;

    // what organ are we playing?
    if (++counter == COUNTER_MAX)
        {
        counter = 0;
        organ = (organ + (mozziAnalogRead(CV_POT3) * NUM_DRAWBAR_SELECTIONS)) >> 10;  // Organ Selection
        }
    }                                             


// Scale from -32768...+32767 to -240 ... +240
inline int16_t scaleAudio(int16_t val)
  {
  if (val == 0) return 0;
  return ((val >> 4) * 15) >> 7;
  }

int updateAudio()                             
    { 
    int16_t* d = drawbars[organ];
    int32_t val = 0;
    for(uint8_t i = 0; i < 9; i++)
        {
        val += (oscils[i].next() * d[i]);
        }
    return scaleAudio((val >> 8) * gain);
    }
