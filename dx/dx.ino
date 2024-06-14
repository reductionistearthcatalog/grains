// Copyright 2024 Sean Luke
// (sean@cs.gmu.edu)
//
// Released under the Apache 2.0 License


/// DX
///
/// DX is a 2- or 3-operator FM synthesizer.  DX is meant to run on the AE Modular GRAINS, 
/// but it could be adapted to any Arduino.
///
/// DX has four FM algorithms.  One algorithm has 2 operators but one operator has self-modulation.
/// The other three algorithms have three operators, but you will have less control from the pots.
///
/// SET GRAINS TO MOZZI MODE.  Sorry, no Grains mode.
///
/// You will need to install the Mozzi Library.  You can do this from the Library Manager
/// in your Arduino IDE.
///
/// HOW FM WORKS
/// 
/// All you need to know is that FM in DX is organized with several sine-wave oscillators called 
/// OPERATORS.  These operators output sound, but only some are used in the final output. If an 
/// operator's sound is used in the final output, it is called a CARRIER.  You can have multiple carriers.
/// Other operators make sound but that sound is used as a modulation source to change the frequency 
/// (really the phase) of ANOTHER operator in real time, warping it into another sound wave.  
/// These oscillators are called MODULATORS.  It is possible for a modulator
/// to modulate another modulator, or to modulate itself. [I'm abusing the terms modulator and 
/// carrier here to simplify things].
///
/// Each operator has a FREQUENCY.  One carrier plays the primary PITCH.  The other operator's
/// frequencies are determined RELATIVE to that carrier, and modulators commonly have relative pitches
/// that are always integer multiples or divisors of the carrier pitch (otherwise it doesn't sound tonal).
/// 
/// The connection between a modulator and the operator it is modulating has an associated value
/// called the INDEX OF MODULATION.  This is basically how much impact the modulator has on changing
/// the frequency of the downstream operator.  If you set it to zero, for example, it's as if they're
/// disconnected.
/// 
/// Changing both the frequency and index of modulation of a modulator can have a big impact on the
/// sound of the downstream operator. However commonly you'd only change the index of modulation in
/// real time (with an envelope, say).
///
/// The arrangement of operators -- which operators modulate which other ones, and which operators
/// are carriers -- is called an ALGORITHM.  This weird term stems from Yamaha.  DX offers four
/// algorithms.  The first algorithm has a single modulator modulating a single carrier, and the
/// modulator can also modulate itself.  The second algorithm has two modulators both modulating
/// a single carrier.  The third algorithm has one modulator modulating two carriers.  And the
/// fourth algorithm has a modulator modulating a second modulator, while that second modulator
/// modulates a carrier.
///
///
/// THE ALGORITHMS
///
/// You set the algorithm by changing the following define to a value 1...4
///
/// Confused?  Then just stick with the 2-modulator ALGORITHM 1, ignoring self modulation, to get your
/// feet wet.


#define ALGORITHM 1


/// ALGORITHM 1.  This is a two-operator algorithm, with a MODULATOR and a CARRIER.  The CARRIER makes
/// the final sound, and so you will specify the carrier's note pitch on POT1/IN1.  The 
/// modulator has a RELATIVE PITCH to the carrier, one of 0.5, 1.0, 2.0, 3.0, ... 15.0, selectable
/// on POT 3.  The modulator also has an INDEX OF MODULATION (the "amplitude" of the operator in Yamaha
/// speak) which determines how much affect the modulator has on the carrier.  This is set with POT2/IN2.
///
/// This is the basic setup.  However the modulator can also be optionally SELF MODULATED, that is, it
/// modulates itself.  You specify the INDEX OF SELF MODULATION with IN3.  Because this is often done with
/// an envelope, you'll need a way to temper its maximum value.  This is done by setting the INDEX OF SELF
/// MODULATION SCALING on AUDIO IN.   
///
///
/// ALGORITHM 2.  This is a three-operator algorithm, with TWO MODULATORS simultaneously modulating the same
/// CARRIER.  Again, the CARRIER makes the final sound, and so you will specify the carrier's note pitch on 
/// POT1/IN1.  Each modulator has a RELATIVE PITCH to the carrier, one of 0.5, 1.0, 2.0, 3.0, ... 15.0.  
/// Modulator 1's relative pitch is selectable on POT 3.  Modulator 2's relative pitch is selectable on.
/// AUDIO IN.  Again, each modulator also has an INDEX OF MODULATION (the "amplitude" of the operator in
/// Yamaha speak) which determines how much affect the modulator has on the carrier.  For Modulator 1, this
/// is set with POT2/IN2.  For Modulator 2, this is set with IN3. 
///
///
/// ALGORITHM 3.  This is another three-operator algorithm, with ONE MODULATOR simultaneously modulating
/// TWO CARRIERS.  Each CARRIER makes a final sound -- they are summed -- and so you will specify each
/// carrier's note pitch.  For Carrier 1, you specify the pitch on POT1/IN1.  Carrier 2's pitch is specified
/// RELATIVE to Carrier on AUDIO IN, smoothly going from Carrier 1's pitch to two octaves above.  The
/// modulator also has a RELATIVE PITCH to the carrier, one of 0.5, 1.0, 2.0, 3.0, ... 15.0, selectable 
/// on POT 3.  For each carrier, the modulator also has an INDEX OF MODULATION (the "amplitude" of the 
/// operator in Yamaha speak) which determines how much affect the modulator has on the carrier.  The
/// modulation of Carrier 1 is set with POT2/IN2.  The modulation of Carrier 2 is set with IN3.
///
///
/// ALGORITHM 4.  This is a final three-operator algorithm, with MODUATOR 2 modulating MODULATOR 1, while
/// MODULATOR 1 is modulating the CARRIER.  The CARRIER makes the final sound and so you will specify the
/// carrier's note pitch on POT1/IN1.  Each modulator has a RELATIVE PITCH to the carrier, one of 
/// 0.5, 1.0, 2.0, 3.0, ... 15.0.  For MODUATOR 1, this is selectable on POT 3.  For MODULATOR 2, this is
/// selectable on AUDIO IN.  Each modulator also has an INDEX OF MODULATION (the "amplitude" of the 
/// operator in Yamaha speak) which determines how much affect the modulator has on its downstream
/// modulator or carrier.  The index of modulation for MODULATOR 1 is set with POT2/IN2.  The index of
/// modulation for MODULATOR 2 is set with IN3.


/// INDEX OF MODULATION SCALING
//
/// The index of modulation of MODULATOR 1 can be scaled DOWN with Pot 2.  But the index of modulation
/// of MODULATOR 2 goes full blast (we don't have enough pots) in algorithms 2, 3, and 4.  You can
/// at least reduce it in the #define here, which is by default 8:

#define INDEX_OF_MODULATION_2_SCALING		8		// Set to 1 ... 8.  Scales modulation *down*.


/// ADJUSTING TUNING AND TRACKING
///
/// Grains's Inputs track 1.3V/octave, not 1V/octave: we'll need to scale them to track properly.  
/// To do this, you can adjust the Pitch Scaling on Pot 1.  This GRAINS program is set up to play 
/// the C three octaves below Middle C when it receives 0V.  You should be able to use Pot 1 to scale 
/// the pitch such that high Cs play in tune as well.  Note that as GRAINS resistors 
/// warm up, the scaling will change and you will need to adjust the tracking again, at least until 
/// they are fully warmed up.
///
/// By default the note corresponding to 0V is C0, three octaves below middle C, that is MIDI note 24, 
/// or 32.7 Hz.  You can customize the tuning for this Grains program.  This can be done 
/// in two ways.  First, you can add pitch to the tuning with a CV value to Audio In.  Second, you 
/// can transpose the pitch up by changing the TRANSPOSE_OCTAVES and/or TRANSPOSE_SEMITONES #defines 
/// in the code to positive integers.  You can also change TRANSPOSE_BITS: a "bit" is the minimum possible
/// change Grains can do, equal to 1/17 of a semitone.
///
/// IMPORTANT NOTE: unlike other grains oscillators in my repository, DX does not have a separate
/// Pitch Tune option on Audio In, because it needs to use Audio In for other purposes.  You can only
/// use TRANSPOSE_OCTAVES, TRANSPOSE_SEMITONES, or TRANPOSE_BITS.


#define TRANSPOSE_BITS (-6)
#define TRANSPOSE_SEMITONES 0
#define TRANSPOSE_OCTAVES 2


/// 2-OPERATOR CONFIGURATION
///
/// IN 1            Pitch CV
/// IN 2            Index of Modulation CV
/// IN 3            Index of Self Modulation CV
/// AUDIO IN (A)    Index of Self Modulation Scaling  [Note this is inverted: maximum Audio IN = 0 scaling ("off")]
/// AUDIO OUT       Out
/// DIGITAL OUT (D) [Unused]
///
/// POT 1           Pitch Scaling        [Set the switch to In1]
///
/// POT 2           Index of Modulation Scaling
///
/// POT 3           Modulator Relative Pitch	[One of 0.5, 1, 2, 3 ... 15]


/// 3-OPERATOR (2 MODULATOR, 1 CARRIER) CONFIGURATION
///
/// IN 1            Pitch CV
/// IN 2            Index of Modulation CV, Modulator 1 to Carrier
/// IN 3            Index of Modulation CV, Modulator 2 to Carrier
/// AUDIO IN (A)    Modulator 2 Relative Pitch			[One of 0.5, 1, 2, 3 ... 15]  [There is no Tune CV]
/// AUDIO OUT       Out
/// DIGITAL OUT (D) [Unused]
///
/// POT 1           Pitch Scaling        [Set the switch to In1]
///
/// POT 2           Index of Modulation Scaling, Modulator 1 to Carrier
///
/// POT 3           Modulator 1 Relative Pitch	[One of 0.5, 1, 2, 3 ... 15]


/// 3-OPERATOR (1 MODULATOR, 2 CARRIER) CONFIGURATION
///
/// IN 1            Pitch CV
/// IN 2            Index of Modulation CV, Modulator to Carrier 1
/// IN 3            Index of Modulation CV, Modulator to Carrier 2
/// AUDIO IN (A)    Carrier 2 Relative Pitch	[Same octave as, to 2 octaves above, Carrier 1] [There is no Tune CV]
/// AUDIO OUT       Out
/// DIGITAL OUT (D) [Unused]
///
/// POT 1           Pitch Scaling        [Set the switch to In1]
///
/// POT 2           Index of Modulation Scaling, Modulator to Carrier 1
///
/// POT 3           Modulator 1 Relative Pitch	[One of 0.5, 1, 2, 3 ... 15]



/// 3-OPERATOR (2 MODULATOR, 1 CARRIER IN SERIES) CONFIGURATION
///
/// IN 1            Pitch CV
/// IN 2            Index of Modulation, Modulator 1 to Carrier
/// IN 3            Index of Modulation, Modulator 2 to Modulator 1
/// AUDIO IN (A)    Modulator 2 Relative Pitch			[One of 0.5, 1, 2, 3 ... 15]  [There is no Tune CV]
/// AUDIO OUT       Out
/// DIGITAL OUT (D) [Unused]
///
/// POT 1           Pitch Scaling        [Set the switch to In1]
///
/// POT 2           Index of Modulation Scaling, Modulator 1 to Carrier
///
/// POT 3           Modulator 1 Relative Pitch	[One of 0.5, 1, 2, 3 ... 15]





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
#define CONTROL_RATE 128

#include <MozziGuts.h>
#include <Oscil.h>

#include <tables/sin512_int8.h>

Oscil<SIN512_NUM_CELLS, AUDIO_RATE> carrier(SIN512_DATA);
Oscil<SIN512_NUM_CELLS, AUDIO_RATE> modulator(SIN512_DATA);
#if (ALGORITHM > 1)
Oscil<SIN512_NUM_CELLS, AUDIO_RATE> modulator2(SIN512_DATA);
#endif
#define carrier2 modulator2			// for convenience

#define CV_POT_IN1    A2    // Note In, Pitch Scaling 
#define CV_POT_IN2    A1    
#define CV_POT3       A0    
#define CV_IN3        A3
#define CV_AUDIO_IN   A4
#define CV_AUDIO_OUT  9     // Out
#define CV_GATE_OUT   8
#define RANDOM_PIN    A5



void setup() 
    {
//    Serial.begin(115200);
    initializeFrequency(CV_POT_IN1, CV_AUDIO_IN);
    startMozzi();
    }


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
    int16_t finalPitch = pitchCV + (tuneCV >> 1) + TRANSPOSE_SEMITONES * 17 + TRANSPOSE_OCTAVES * 205 + TRANSPOSE_BITS;
    return FREQUENCY(finalPitch < 0 ? 0 : (finalPitch > 1535 ? 1535 : finalPitch));
    }

uint16_t indexOfModulation = 0;
uint16_t indexOfModulation2 = 0;
uint16_t indexOfModulationScaling2 = 0;
float relativeFrequency = 0;
float relativeFrequency2 = 0;
int16_t lastModulationValue = 0;

void updateControl() 
    {
	uint16_t pot3 = mozziAnalogRead(CV_POT3);
	uint16_t pot2 = mozziAnalogRead(CV_POT_IN2);
	uint16_t in3 = mozziAnalogRead(CV_IN3);
    uint16_t audioIn = mozziAnalogRead(CV_AUDIO_IN);
    float frequency = getFrequency(CV_POT_IN1, 0);		// Note no pitch scaling

#if (ALGORITHM == 1)			// 2 Operator
	indexOfModulation = pot2;
	indexOfModulation2 = in3;
	indexOfModulationScaling2 = (1023 - audioIn);
	uint8_t rf = pot3 >> 5;		// 0 ... 15
	float relativeFrequency = (rf == 0 ? frequency * 0.5 : frequency * rf);
    carrier.setFreq(frequency);
    modulator.setFreq(relativeFrequency);
#elif (ALGORITHM == 2)			// 2 Modulators, 1 Carrier
	indexOfModulation = pot2;
	indexOfModulation2 = in3;
	uint8_t rf = pot3 >> 5;		// 0 ... 15
	float relativeFrequency = (rf == 0 ? frequency * 0.5 : frequency * rf);
	rf = audioIn >> 5;		// 0 ... 15
	float relativeFrequency2 = (rf == 0 ? frequency * 0.5 : frequency * rf);
    carrier.setFreq(frequency);
    modulator.setFreq(relativeFrequency);
    modulator2.setFreq(relativeFrequency2);
#elif (ALGORITHM == 3)
	indexOfModulation = pot2;
	indexOfModulation2 = in3;
	uint8_t rf = pot3 >> 5;		// 0 ... 15
	float relativeFrequency = (rf == 0 ? frequency * 0.5 : frequency * rf);
	float relativeFrequency2 = frequency * ((audioIn * 1.0 / 1024.0) + 1.0) * 2.0;
    carrier.setFreq(frequency);
    modulator.setFreq(relativeFrequency);
    carrier2.setFreq(relativeFrequency2);
#else
	indexOfModulation = pot2;
	indexOfModulation2 = in3;
	uint8_t rf = pot3 >> 5;		// 0 ... 15
	float relativeFrequency = (rf == 0 ? frequency * 0.5 : frequency * rf);
	rf = audioIn >> 5;		// 0 ... 15
	float relativeFrequency2 = (rf == 0 ? frequency * 0.5 : frequency * rf);
    carrier.setFreq(frequency);
    modulator.setFreq(relativeFrequency);
    modulator2.setFreq(relativeFrequency2);
#endif	
    }

// Scale from -128...+127 to -240 ... +240
inline int16_t scaleAudio(int16_t val)
  {
  //Serial.println(val);
  return (val * 15) >> 3;
  }


int updateAudio()    
    {
#if (ALGORITHM == 1)
    // indexOfModulation is 1024 in range.  Going >> 1 and we have 512 in range.  indexOfModulationScaling2 is
    // 1024 in range.  Multiply it in and >> 10 and we're still 512 range.
    // lastModulationValue is 128 in range.  Multiply it in and we have 65536 in range.
	Q15n16 selfpm = (((indexOfModulation2 >> 1) * (uint32_t)indexOfModulationScaling2) >> 10) * lastModulationValue;
    // indexOfModulation is 1024 in range.  INDEX_OF_MODULATION_SCALE is 4 in range.
    // Multiply them and >> 3 and we have 512 in range.  
    // The modulator is 128 in range.  Multiply it in and we have 65536 in range.
	Q15n16 pm = (indexOfModulation >> 1) * (uint32_t)(lastModulationValue = modulator.phMod(selfpm));
    int16_t s1 = carrier.phMod(pm);
    return s1;
#elif (ALGORITHM == 2)
    // indexOfModulation is 1024 in range.
    // INDEX_OF_MODULATION_2_SCALING is 8 in range.  Multiply them and >> 1 and 
    // we are 512 in range.
    // The modulator is 128 in range.  Multiply it in and we have 65536 in range.
	Q15n16 pm2 = (((indexOfModulation2 * INDEX_OF_MODULATION_2_SCALING) >> 4) * ((uint32_t)modulator2.next())) >> 1;
	// Like PM above
	Q15n16 pm = ((indexOfModulation >> 1) * ((uint32_t)modulator.next())) >> 1;
    int16_t s1 = carrier.phMod(pm + pm2);
    return s1;
#elif (ALGORITHM == 3)
	uint32_t mod = modulator.next();
	// Like PM2 above
	Q15n16 pm2 = ((indexOfModulation2 * INDEX_OF_MODULATION_2_SCALING) >> 4) * mod;
	// Like PM above
	Q15n16 pm = (indexOfModulation >> 1) * mod;
    int16_t s1 = (carrier.phMod(pm) + carrier2.phMod(pm2)) >> 1;
    return s1;
#else
	uint32_t mod = modulator2.next();
	// Like PM2 above
	Q15n16 pm2 = ((indexOfModulation2 * INDEX_OF_MODULATION_2_SCALING) >> 4) * mod;
	// Like PM above
	Q15n16 pm = (indexOfModulation >> 1) * modulator.phMod(pm2);
    int16_t s1 = carrier.phMod(pm);
    return s1;
#endif	
    }


void loop() 
    {
    audioHook();
    }
