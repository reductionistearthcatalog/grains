#define DATA_LENGTH 1155
#define DATA BD2500

const PROGMEM int8_t DATA[DATA_LENGTH] = {-6, 110, -45, -78, -87, -95, -98,
-101, -103, -105, -107, -108, -110, -111, -112, -113, -113, -113, -113, -112,
-110, -108, -107, -105, -103, -101, -98, -96, -92, -88, -84, -80, -72, -63, -46,
108, 75, 62, 55, 48, 44, 41, 37, 34, 31, 30, 28, 27, 26, 25, 23, 22, 21, 20, 20,
19, 18, 17, 17, 16, 16, 16, 16, 15, 15, 15, 16, 16, 16, 16, 17, 17, 18, 19, 19,
20, 21, 22, 23, 24, 25, 26, 27, 28, 30, 31, 33, 36, 39, 42, 44, 47, 51, 56, 60,
65, 73, 81, 96, -13, -38, -50, -56, -63, -67, -70, -73, -76, -78, -81, -82, -83,
-85, -86, -87, -88, -89, -91, -92, -93, -94, -95, -96, -96, -97, -97, -98, -98,
-99, -99, -99, -100, -100, -100, -100, -101, -101, -101, -101, -102, -102, -102,
-102, -102, -103, -103, -103, -103, -103, -103, -103, -103, -103, -103, -103,
-103, -103, -103, -103, -103, -103, -103, -103, -103, -103, -102, -102, -102,
-102, -102, -102, -102, -101, -101, -101, -101, -100, -100, -100, -100, -99,
-99, -99, -99, -98, -98, -98, -97, -97, -97, -96, -95, -95, -94, -93, -92, -92,
-91, -90, -89, -88, -87, -86, -85, -84, -83, -82, -81, -80, -78, -76, -74, -72,
-70, -67, -65, -61, -56, -52, -46, -36, -21, 112, 94, 85, 77, 73, 68, 63, 61,
58, 56, 54, 51, 49, 47, 46, 45, 44, 43, 41, 40, 39, 38, 37, 36, 35, 34, 33, 32,
32, 31, 31, 30, 30, 29, 29, 29, 28, 28, 28, 27, 27, 27, 27, 26, 26, 26, 26, 25,
25, 25, 25, 25, 25, 25, 25, 24, 24, 24, 24, 24, 24, 24, 24, 25, 25, 25, 25, 25,
25, 25, 25, 25, 26, 26, 26, 26, 26, 27, 27, 27, 27, 28, 28, 28, 28, 29, 29, 29,
29, 30, 30, 30, 31, 31, 31, 32, 32, 33, 33, 34, 35, 35, 36, 37, 37, 38, 39, 39,
40, 41, 41, 42, 42, 43, 44, 44, 45, 46, 46, 47, 48, 49, 50, 51, 53, 54, 55, 56,
58, 59, 60, 61, 62, 64, 66, 68, 71, 73, 75, 77, 80, 84, 88, 93, 98, 106, 118,
-8, -20, -28, -34, -38, -42, -46, -49, -51, -53, -54, -56, -58, -59, -61, -63,
-64, -65, -66, -67, -67, -68, -69, -69, -70, -71, -71, -72, -73, -73, -74, -74,
-75, -75, -76, -76, -77, -77, -78, -78, -78, -79, -79, -80, -80, -80, -81, -81,
-81, -81, -81, -81, -81, -81, -81, -82, -82, -82, -82, -82, -82, -82, -82, -82,
-82, -82, -82, -82, -82, -82, -82, -82, -82, -82, -82, -82, -82, -81, -81, -81,
-81, -81, -81, -81, -81, -81, -81, -80, -80, -80, -79, -79, -79, -78, -78, -78,
-77, -77, -77, -76, -76, -76, -75, -75, -74, -74, -74, -73, -73, -72, -72, -71,
-71, -70, -70, -70, -69, -69, -68, -68, -67, -67, -66, -66, -65, -65, -64, -63,
-62, -61, -60, -59, -58, -57, -56, -55, -54, -53, -52, -51, -50, -49, -47, -45,
-43, -41, -39, -37, -35, -33, -30, -26, -22, -18, -13, -5, 123, 115, 110, 106,
102, 99, 95, 94, 92, 90, 88, 87, 85, 84, 82, 80, 79, 78, 78, 77, 76, 75, 75, 74,
73, 73, 72, 71, 71, 70, 70, 69, 68, 68, 67, 67, 66, 66, 65, 65, 64, 64, 64, 63,
63, 63, 63, 63, 62, 62, 62, 62, 62, 62, 62, 62, 62, 61, 61, 61, 61, 61, 61, 61,
61, 61, 61, 61, 61, 61, 61, 61, 61, 61, 61, 61, 61, 61, 61, 61, 61, 61, 61, 62,
62, 62, 62, 62, 62, 62, 62, 63, 63, 63, 63, 63, 63, 63, 64, 64, 64, 65, 65, 66,
66, 66, 67, 67, 68, 68, 68, 69, 69, 70, 70, 71, 71, 72, 72, 73, 73, 74, 74, 75,
75, 76, 76, 77, 77, 78, 78, 79, 79, 81, 82, 83, 84, 85, 86, 87, 88, 89, 90, 91,
93, 94, 95, 96, 98, 101, 103, 105, 107, 109, 111, 116, 120, 124, -3, -7, -12,
-15, -18, -20, -22, -24, -27, -28, -30, -32, -33, -34, -35, -36, -37, -38, -39,
-40, -41, -42, -43, -43, -44, -45, -46, -47, -47, -48, -49, -49, -49, -50, -50,
-51, -51, -51, -52, -52, -52, -53, -53, -53, -53, -54, -54, -54, -54, -55, -55,
-55, -55, -56, -56, -56, -56, -56, -57, -57, -57, -57, -57, -57, -57, -58, -58,
-58, -58, -58, -58, -58, -58, -58, -58, -58, -58, -58, -59, -59, -59, -59, -59,
-59, -58, -58, -58, -58, -58, -58, -58, -58, -58, -58, -58, -58, -58, -58, -58,
-57, -57, -57, -57, -57, -57, -57, -57, -56, -56, -56, -56, -56, -56, -55, -55,
-55, -55, -55, -54, -54, -54, -54, -54, -53, -53, -53, -53, -52, -52, -52, -52,
-51, -51, -51, -51, -50, -50, -50, -50, -49, -49, -49, -49, -48, -48, -47, -46,
-46, -45, -45, -44, -44, -43, -43, -42, -42, -41, -40, -40, -39, -39, -38, -38,
-37, -37, -36, -35, -35, -34, -34, -33, -33, -32, -31, -30, -29, -28, -27, -26,
-25, -24, -23, -22, -21, -20, -19, -18, -17, -16, -14, -13, -11, -9, -7, -6, -4,
-2, 126, 125, 123, 122, 120, 118, 117, 116, 114, 113, 111, 111, 110, 109, 109,
108, 108, 107, 106, 106, 105, 105, 104, 104, 103, 103, 102, 102, 102, 101, 101,
100, 100, 100, 99, 99, 99, 98, 98, 98, 98, 98, 97, 97, 97, 97, 97, 97, 96, 96,
96, 96, 96, 96, 96, 96, 96, 96, 96, 96, 96, 96, 96, 96, 96, 96, 96, 96, 97, 97,
97, 97, 97, 98, 98, 98, 98, 98, 99, 99, 99, 99, 100, 100, 100, 101, 101, 101,
102, 102, 102, 103, 103, 104, 104, 104, 105, 105, 106, 106, 106, 107, 107, 108,
108, 109, 109, 109, 110, 110, 111, 112, 112, 113, 114, 115, 116, 117, 118, 119,
120, 121, 122, 122, 123, 124, 125, -2, -2, -3, -4, -5, -6, -7, -8, -9, -10, -11,
-12, -13, -14, -15, -16, -17, -17, -18, -18, -19, -19, -19, -20, -20, -21, -21,
-21, -22, -22, -23, -23, -24, -24, -24, -25, -25, -25, -26, -26, -27, -27, -27,
-28, -28, -28, -29, -29, -29, -29, -29, -29, -29, -29, -30, -29, -29, -29, -29,
-29, -29, -30, -29, -29, -29, -29, -29, -29, -29, -29, -28, -28, -28, -28, -28,
-28, -28, -27, -27, -27, -27, -26, -26, -26, -26, -25, -25, -25, -25, -25, -24,
-24, -24, -23, -23, -23, -23, -22, -22, -22, -22, -21, -21, -21, -20, -20, -20,
-19, -19, -19, -18, -18, -18, -17, -17, -17, -17, -16, -16, -15, -15, -14, -14,
-13, -13, -11, -11, -10, -10, -9, -9, -9, -8, -8, -7, -7, -6, -5, -5, -4, -4,
-3, -3, -3, -3, -2, -2, -1, };
