/* this file is a part of amp software, (C) tomislav uzelac 1996,1997
*/
 
/* misc2.h  
 *
 * Created by: tomislav uzelac  May 1996
 * Last modified by: tomislav uzelac Jan  8 1997
 */
#include "real.h"

extern void requantize_downmix(int gr,struct SIDE_INFO *info,struct AUDIO_HEADER *header);
extern void requantize_mono(int gr,int ch,struct SIDE_INFO *info,struct AUDIO_HEADER *header);
extern void requantize_ms(int gr,struct SIDE_INFO *info,struct AUDIO_HEADER *header);
extern void alias_reduction(int ch);
extern void calculate_t43(void);

extern int no_of_imdcts[2];

#ifdef MISC2

#define i_sq2   0.707106781188
#define IS_ILLEGAL 0xfeed

void requantize_downmix(int gr,struct SIDE_INFO *info,struct AUDIO_HEADER *header);
void requantize_mono(int gr,int ch,struct SIDE_INFO *info,struct AUDIO_HEADER *header);
void requantize_ms(int gr,struct SIDE_INFO *info,struct AUDIO_HEADER *header);
void alias_reduction(int ch);

static inline REAL fras_l(int sfb,int global_gain,int scalefac_scale,int scalefac,int preflag);
static inline REAL fras_s(int global_gain,int subblock_gain,int scalefac_scale,int scalefac);
static inline REAL fras2(int is,REAL a);
static int find_isbound(int isbound[3],int gr,struct SIDE_INFO *info,struct AUDIO_HEADER *header);
static inline void stereo_s(int l,REAL a[2],int pos,int ms_flag,int is_pos,struct AUDIO_HEADER *header);
static inline void stereo_l(int l,REAL a[2],int ms_flag,int is_pos,struct AUDIO_HEADER *header);

int no_of_imdcts[2];

static const int t_pretab[22]={0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,2,2,3,3,3,2,0};
static const REAL t_is[7]={ 1.0, 0.788675134596, 0.633974596215,
		 0.5, 0.366025403784, 0.211324865405, 0.0};
static const REAL t_is2[2][32]={
{             1,       0.840896,       0.707107,       0.594604,            0.5,       0.420448,
       0.353553,       0.297302,           0.25,       0.210224,       0.176777,       0.148651,
          0.125,       0.105112,      0.0883883,      0.0743254},
{             1,       0.707107,            0.5,       0.353553,           0.25,       0.176777,
          0.125,      0.0883883,         0.0625,      0.0441942,        0.03125,      0.0220971,
       0.015625,      0.0110485,      0.0078125,     0.00552427}
};
static const REAL t_downmix[2][32]={
{ 1.000000,   0.920448,   0.853554,   0.797302,   0.750000,   0.710224,
  0.676776,   0.648651,   0.625000,   0.605112,   0.588389,   0.574326,
  0.562500,   0.552556,   0.544194,   0.537163},
{ 1.000000,   0.853554,   0.750000,   0.676776,   0.625000,   0.588389,
  0.562500,   0.544194,   0.531250,   0.522097,   0.515625,   0.511049,
  0.507813,   0.505524,   0.503906,   0.502762}
};

static const REAL Cs[8]={0.857492925712, 0.881741997318, 0.949628649103,
	      0.983314592492, 0.995517816065, 0.999160558175,
	      0.999899195243, 0.999993155067};
static const REAL Ca[8]={-0.5144957554270, -0.4717319685650, -0.3133774542040,
	      -0.1819131996110, -0.0945741925262, -0.0409655828852,
	      -0.0141985685725,	-0.00369997467375};
static const REAL tab[4]={1,1.189207115,1.414213562,1.6817928301};
static const REAL tabi[4]={1,0.840896415,0.707106781,0.594603557};

static REAL t_43[8192];


/* leftmost index denotes header->ID, so first three are for MPEG2
 * and the others are for MPEG1
 */
static const short t_reorder[2][3][576]={{
{  0,   1,   2,   3,   6,   7,   8,   9,  12,  13,  14,  15,   4,   5,  18,  19,  10,  11,  24,  25,
  16,  17,  30,  31,  20,  21,  22,  23,  26,  27,  28,  29,  32,  33,  34,  35,  36,  37,  38,  39,
  40,  41,  42,  43,  44,  45,  46,  47,  48,  49,  50,  51,  52,  53,  54,  55,  56,  57,  58,  59,
  60,  61,  62,  63,  64,  65,  66,  67,  68,  69,  70,  71,  72,  73,  74,  75,  76,  77,  90,  91,
  78,  79,  80,  81,  82,  83,  96,  97,  84,  85,  86,  87,  88,  89, 102, 103,  92,  93,  94,  95,
 108, 109, 110, 111, 112, 113,  98,  99, 100, 101, 114, 115, 116, 117, 118, 119, 104, 105, 106, 107,
 120, 121, 122, 123, 124, 125, 126, 127, 128, 129, 130, 131, 144, 145, 146, 147, 148, 149, 162, 163,
 132, 133, 134, 135, 136, 137, 150, 151, 152, 153, 154, 155, 168, 169, 138, 139, 140, 141, 142, 143,
 156, 157, 158, 159, 160, 161, 174, 175, 164, 165, 166, 167, 180, 181, 182, 183, 184, 185, 198, 199,
 200, 201, 202, 203, 216, 217, 170, 171, 172, 173, 186, 187, 188, 189, 190, 191, 204, 205, 206, 207,
 208, 209, 222, 223, 176, 177, 178, 179, 192, 193, 194, 195, 196, 197, 210, 211, 212, 213, 214, 215,
 228, 229, 218, 219, 220, 221, 234, 235, 236, 237, 238, 239, 252, 253, 254, 255, 256, 257, 270, 271,
 272, 273, 274, 275, 288, 289, 290, 291, 224, 225, 226, 227, 240, 241, 242, 243, 244, 245, 258, 259,
 260, 261, 262, 263, 276, 277, 278, 279, 280, 281, 294, 295, 296, 297, 230, 231, 232, 233, 246, 247,
 248, 249, 250, 251, 264, 265, 266, 267, 268, 269, 282, 283, 284, 285, 286, 287, 300, 301, 302, 303,
 292, 293, 306, 307, 308, 309, 310, 311, 324, 325, 326, 327, 328, 329, 342, 343, 344, 345, 346, 347,
 360, 361, 362, 363, 364, 365, 378, 379, 380, 381, 382, 383, 298, 299, 312, 313, 314, 315, 316, 317,
 330, 331, 332, 333, 334, 335, 348, 349, 350, 351, 352, 353, 366, 367, 368, 369, 370, 371, 384, 385,
 386, 387, 388, 389, 304, 305, 318, 319, 320, 321, 322, 323, 336, 337, 338, 339, 340, 341, 354, 355,
 356, 357, 358, 359, 372, 373, 374, 375, 376, 377, 390, 391, 392, 393, 394, 395, 396, 397, 398, 399,
 400, 401, 414, 415, 416, 417, 418, 419, 432, 433, 434, 435, 436, 437, 450, 451, 452, 453, 454, 455,
 468, 469, 470, 471, 472, 473, 486, 487, 488, 489, 490, 491, 504, 505, 506, 507, 508, 509, 402, 403,
 404, 405, 406, 407, 420, 421, 422, 423, 424, 425, 438, 439, 440, 441, 442, 443, 456, 457, 458, 459,
 460, 461, 474, 475, 476, 477, 478, 479, 492, 493, 494, 495, 496, 497, 510, 511, 512, 513, 514, 515,
 408, 409, 410, 411, 412, 413, 426, 427, 428, 429, 430, 431, 444, 445, 446, 447, 448, 449, 462, 463,
 464, 465, 466, 467, 480, 481, 482, 483, 484, 485, 498, 499, 500, 501, 502, 503, 516, 517, 518, 519,
 520, 521, 522, 523, 524, 525, 526, 527, 540, 541, 542, 543, 544, 545, 558, 559, 560, 561, 562, 563,
 528, 529, 530, 531, 532, 533, 546, 547, 548, 549, 550, 551, 564, 565, 566, 567, 568, 569, 534, 535,
 536, 537, 538, 539, 552, 553, 554, 555, 556, 557, 570, 571, 572, 573, 574, 575},

{  0,   1,   2,   3,   6,   7,   8,   9,  12,  13,  14,  15,   4,   5,  18,  19,  10,  11,  24,  25,
  16,  17,  30,  31,  20,  21,  22,  23,  26,  27,  28,  29,  32,  33,  34,  35,  36,  37,  38,  39,
  40,  41,  42,  43,  44,  45,  46,  47,  48,  49,  50,  51,  52,  53,  54,  55,  56,  57,  58,  59,
  72,  73,  60,  61,  62,  63,  64,  65,  78,  79,  66,  67,  68,  69,  70,  71,  84,  85,  74,  75,
  76,  77,  90,  91,  92,  93,  94,  95,  80,  81,  82,  83,  96,  97,  98,  99, 100, 101,  86,  87,
  88,  89, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 126, 127, 128, 129, 130, 131,
 114, 115, 116, 117, 118, 119, 132, 133, 134, 135, 136, 137, 120, 121, 122, 123, 124, 125, 138, 139,
 140, 141, 142, 143, 144, 145, 146, 147, 148, 149, 162, 163, 164, 165, 166, 167, 180, 181, 150, 151,
 152, 153, 154, 155, 168, 169, 170, 171, 172, 173, 186, 187, 156, 157, 158, 159, 160, 161, 174, 175,
 176, 177, 178, 179, 192, 193, 182, 183, 184, 185, 198, 199, 200, 201, 202, 203, 216, 217, 218, 219,
 220, 221, 234, 235, 188, 189, 190, 191, 204, 205, 206, 207, 208, 209, 222, 223, 224, 225, 226, 227,
 240, 241, 194, 195, 196, 197, 210, 211, 212, 213, 214, 215, 228, 229, 230, 231, 232, 233, 246, 247,
 236, 237, 238, 239, 252, 253, 254, 255, 256, 257, 270, 271, 272, 273, 274, 275, 288, 289, 290, 291,
 292, 293, 306, 307, 242, 243, 244, 245, 258, 259, 260, 261, 262, 263, 276, 277, 278, 279, 280, 281,
 294, 295, 296, 297, 298, 299, 312, 313, 248, 249, 250, 251, 264, 265, 266, 267, 268, 269, 282, 283,
 284, 285, 286, 287, 300, 301, 302, 303, 304, 305, 318, 319, 308, 309, 310, 311, 324, 325, 326, 327,
 328, 329, 342, 343, 344, 345, 346, 347, 360, 361, 362, 363, 364, 365, 378, 379, 380, 381, 382, 383,
 396, 397, 398, 399, 314, 315, 316, 317, 330, 331, 332, 333, 334, 335, 348, 349, 350, 351, 352, 353,
 366, 367, 368, 369, 370, 371, 384, 385, 386, 387, 388, 389, 402, 403, 404, 405, 320, 321, 322, 323,
 336, 337, 338, 339, 340, 341, 354, 355, 356, 357, 358, 359, 372, 373, 374, 375, 376, 377, 390, 391,
 392, 393, 394, 395, 408, 409, 410, 411, 400, 401, 414, 415, 416, 417, 418, 419, 432, 433, 434, 435,
 436, 437, 450, 451, 452, 453, 454, 455, 468, 469, 470, 471, 472, 473, 486, 487, 488, 489, 490, 491,
 504, 505, 506, 507, 508, 509, 522, 523, 524, 525, 526, 527, 406, 407, 420, 421, 422, 423, 424, 425,
 438, 439, 440, 441, 442, 443, 456, 457, 458, 459, 460, 461, 474, 475, 476, 477, 478, 479, 492, 493,
 494, 495, 496, 497, 510, 511, 512, 513, 514, 515, 528, 529, 530, 531, 532, 533, 412, 413, 426, 427,
 428, 429, 430, 431, 444, 445, 446, 447, 448, 449, 462, 463, 464, 465, 466, 467, 480, 481, 482, 483,
 484, 485, 498, 499, 500, 501, 502, 503, 516, 517, 518, 519, 520, 521, 534, 535, 536, 537, 538, 539,
 540, 541, 542, 543, 544, 545, 558, 559, 560, 561, 562, 563, 546, 547, 548, 549, 550, 551, 564, 565,
 566, 567, 568, 569, 552, 553, 554, 555, 556, 557, 570, 571, 572, 573, 574, 575},

{  0,   1,   2,   3,   6,   7,   8,   9,  12,  13,  14,  15,   4,   5,  18,  19,  10,  11,  24,  25,
  16,  17,  30,  31,  20,  21,  22,  23,  26,  27,  28,  29,  32,  33,  34,  35,  36,  37,  38,  39,
  40,  41,  42,  43,  44,  45,  46,  47,  48,  49,  50,  51,  52,  53,  54,  55,  56,  57,  58,  59,
  72,  73,  60,  61,  62,  63,  64,  65,  78,  79,  66,  67,  68,  69,  70,  71,  84,  85,  74,  75,
  76,  77,  90,  91,  92,  93,  94,  95,  80,  81,  82,  83,  96,  97,  98,  99, 100, 101,  86,  87,
  88,  89, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 126, 127, 128, 129, 130, 131,
 114, 115, 116, 117, 118, 119, 132, 133, 134, 135, 136, 137, 120, 121, 122, 123, 124, 125, 138, 139,
 140, 141, 142, 143, 144, 145, 146, 147, 148, 149, 162, 163, 164, 165, 166, 167, 180, 181, 150, 151,
 152, 153, 154, 155, 168, 169, 170, 171, 172, 173, 186, 187, 156, 157, 158, 159, 160, 161, 174, 175,
 176, 177, 178, 179, 192, 193, 182, 183, 184, 185, 198, 199, 200, 201, 202, 203, 216, 217, 218, 219,
 220, 221, 234, 235, 188, 189, 190, 191, 204, 205, 206, 207, 208, 209, 222, 223, 224, 225, 226, 227,
 240, 241, 194, 195, 196, 197, 210, 211, 212, 213, 214, 215, 228, 229, 230, 231, 232, 233, 246, 247,
 236, 237, 238, 239, 252, 253, 254, 255, 256, 257, 270, 271, 272, 273, 274, 275, 288, 289, 290, 291,
 292, 293, 306, 307, 242, 243, 244, 245, 258, 259, 260, 261, 262, 263, 276, 277, 278, 279, 280, 281,
 294, 295, 296, 297, 298, 299, 312, 313, 248, 249, 250, 251, 264, 265, 266, 267, 268, 269, 282, 283,
 284, 285, 286, 287, 300, 301, 302, 303, 304, 305, 318, 319, 308, 309, 310, 311, 324, 325, 326, 327,
 328, 329, 342, 343, 344, 345, 346, 347, 360, 361, 362, 363, 364, 365, 378, 379, 380, 381, 382, 383,
 396, 397, 314, 315, 316, 317, 330, 331, 332, 333, 334, 335, 348, 349, 350, 351, 352, 353, 366, 367,
 368, 369, 370, 371, 384, 385, 386, 387, 388, 389, 402, 403, 320, 321, 322, 323, 336, 337, 338, 339,
 340, 341, 354, 355, 356, 357, 358, 359, 372, 373, 374, 375, 376, 377, 390, 391, 392, 393, 394, 395,
 408, 409, 398, 399, 400, 401, 414, 415, 416, 417, 418, 419, 432, 433, 434, 435, 436, 437, 450, 451,
 452, 453, 454, 455, 468, 469, 470, 471, 472, 473, 486, 487, 488, 489, 490, 491, 504, 505, 506, 507,
 508, 509, 404, 405, 406, 407, 420, 421, 422, 423, 424, 425, 438, 439, 440, 441, 442, 443, 456, 457,
 458, 459, 460, 461, 474, 475, 476, 477, 478, 479, 492, 493, 494, 495, 496, 497, 510, 511, 512, 513,
 514, 515, 410, 411, 412, 413, 426, 427, 428, 429, 430, 431, 444, 445, 446, 447, 448, 449, 462, 463,
 464, 465, 466, 467, 480, 481, 482, 483, 484, 485, 498, 499, 500, 501, 502, 503, 516, 517, 518, 519,
 520, 521, 522, 523, 524, 525, 526, 527, 540, 541, 542, 543, 544, 545, 558, 559, 560, 561, 562, 563,
 528, 529, 530, 531, 532, 533, 546, 547, 548, 549, 550, 551, 564, 565, 566, 567, 568, 569, 534, 535,
 536, 537, 538, 539, 552, 553, 554, 555, 556, 557, 570, 571, 572, 573, 574, 575}
},
{
{  0,   1,   2,   3,   6,   7,   8,   9,  12,  13,  14,  15,   4,   5,  18,  19,  10,  11,  24,  25,
  16,  17,  30,  31,  20,  21,  22,  23,  26,  27,  28,  29,  32,  33,  34,  35,  36,  37,  38,  39,
  42,  43,  44,  45,  48,  49,  50,  51,  40,  41,  54,  55,  56,  57,  46,  47,  60,  61,  62,  63,
  52,  53,  66,  67,  68,  69,  58,  59,  72,  73,  74,  75,  76,  77,  64,  65,  78,  79,  80,  81,
  82,  83,  70,  71,  84,  85,  86,  87,  88,  89,  90,  91,  92,  93,  94,  95, 108, 109, 110, 111,
  96,  97,  98,  99, 100, 101, 114, 115, 116, 117, 102, 103, 104, 105, 106, 107, 120, 121, 122, 123,
 112, 113, 126, 127, 128, 129, 130, 131, 144, 145, 146, 147, 118, 119, 132, 133, 134, 135, 136, 137,
 150, 151, 152, 153, 124, 125, 138, 139, 140, 141, 142, 143, 156, 157, 158, 159, 148, 149, 162, 163,
 164, 165, 166, 167, 180, 181, 182, 183, 184, 185, 154, 155, 168, 169, 170, 171, 172, 173, 186, 187,
 188, 189, 190, 191, 160, 161, 174, 175, 176, 177, 178, 179, 192, 193, 194, 195, 196, 197, 198, 199,
 200, 201, 202, 203, 216, 217, 218, 219, 220, 221, 234, 235, 236, 237, 238, 239, 204, 205, 206, 207,
 208, 209, 222, 223, 224, 225, 226, 227, 240, 241, 242, 243, 244, 245, 210, 211, 212, 213, 214, 215,
 228, 229, 230, 231, 232, 233, 246, 247, 248, 249, 250, 251, 252, 253, 254, 255, 256, 257, 270, 271,
 272, 273, 274, 275, 288, 289, 290, 291, 292, 293, 306, 307, 308, 309, 258, 259, 260, 261, 262, 263,
 276, 277, 278, 279, 280, 281, 294, 295, 296, 297, 298, 299, 312, 313, 314, 315, 264, 265, 266, 267,
 268, 269, 282, 283, 284, 285, 286, 287, 300, 301, 302, 303, 304, 305, 318, 319, 320, 321, 310, 311,
 324, 325, 326, 327, 328, 329, 342, 343, 344, 345, 346, 347, 360, 361, 362, 363, 364, 365, 378, 379,
 380, 381, 382, 383, 396, 397, 398, 399, 316, 317, 330, 331, 332, 333, 334, 335, 348, 349, 350, 351,
 352, 353, 366, 367, 368, 369, 370, 371, 384, 385, 386, 387, 388, 389, 402, 403, 404, 405, 322, 323,
 336, 337, 338, 339, 340, 341, 354, 355, 356, 357, 358, 359, 372, 373, 374, 375, 376, 377, 390, 391,
 392, 393, 394, 395, 408, 409, 410, 411, 400, 401, 414, 415, 416, 417, 418, 419, 432, 433, 434, 435,
 436, 437, 450, 451, 452, 453, 454, 455, 468, 469, 470, 471, 472, 473, 486, 487, 488, 489, 490, 491,
 504, 505, 506, 507, 508, 509, 522, 523, 524, 525, 526, 527, 540, 541, 542, 543, 544, 545, 558, 559,
 560, 561, 562, 563, 406, 407, 420, 421, 422, 423, 424, 425, 438, 439, 440, 441, 442, 443, 456, 457,
 458, 459, 460, 461, 474, 475, 476, 477, 478, 479, 492, 493, 494, 495, 496, 497, 510, 511, 512, 513,
 514, 515, 528, 529, 530, 531, 532, 533, 546, 547, 548, 549, 550, 551, 564, 565, 566, 567, 568, 569,
 412, 413, 426, 427, 428, 429, 430, 431, 444, 445, 446, 447, 448, 449, 462, 463, 464, 465, 466, 467,
 480, 481, 482, 483, 484, 485, 498, 499, 500, 501, 502, 503, 516, 517, 518, 519, 520, 521, 534, 535,
 536, 537, 538, 539, 552, 553, 554, 555, 556, 557, 570, 571, 572, 573, 574, 575},

{  0,   1,   2,   3,   6,   7,   8,   9,  12,  13,  14,  15,   4,   5,  18,  19,  10,  11,  24,  25,
  16,  17,  30,  31,  20,  21,  22,  23,  26,  27,  28,  29,  32,  33,  34,  35,  36,  37,  38,  39,
  42,  43,  44,  45,  48,  49,  50,  51,  40,  41,  54,  55,  56,  57,  46,  47,  60,  61,  62,  63,
  52,  53,  66,  67,  68,  69,  58,  59,  72,  73,  74,  75,  64,  65,  78,  79,  80,  81,  70,  71,
  84,  85,  86,  87,  76,  77,  90,  91,  92,  93,  94,  95, 108, 109,  82,  83,  96,  97,  98,  99,
 100, 101, 114, 115,  88,  89, 102, 103, 104, 105, 106, 107, 120, 121, 110, 111, 112, 113, 126, 127,
 128, 129, 130, 131, 144, 145, 116, 117, 118, 119, 132, 133, 134, 135, 136, 137, 150, 151, 122, 123,
 124, 125, 138, 139, 140, 141, 142, 143, 156, 157, 146, 147, 148, 149, 162, 163, 164, 165, 166, 167,
 180, 181, 182, 183, 152, 153, 154, 155, 168, 169, 170, 171, 172, 173, 186, 187, 188, 189, 158, 159,
 160, 161, 174, 175, 176, 177, 178, 179, 192, 193, 194, 195, 184, 185, 198, 199, 200, 201, 202, 203,
 216, 217, 218, 219, 220, 221, 234, 235, 190, 191, 204, 205, 206, 207, 208, 209, 222, 223, 224, 225,
 226, 227, 240, 241, 196, 197, 210, 211, 212, 213, 214, 215, 228, 229, 230, 231, 232, 233, 246, 247,
 236, 237, 238, 239, 252, 253, 254, 255, 256, 257, 270, 271, 272, 273, 274, 275, 288, 289, 290, 291,
 242, 243, 244, 245, 258, 259, 260, 261, 262, 263, 276, 277, 278, 279, 280, 281, 294, 295, 296, 297,
 248, 249, 250, 251, 264, 265, 266, 267, 268, 269, 282, 283, 284, 285, 286, 287, 300, 301, 302, 303,
 292, 293, 306, 307, 308, 309, 310, 311, 324, 325, 326, 327, 328, 329, 342, 343, 344, 345, 346, 347,
 360, 361, 362, 363, 364, 365, 298, 299, 312, 313, 314, 315, 316, 317, 330, 331, 332, 333, 334, 335,
 348, 349, 350, 351, 352, 353, 366, 367, 368, 369, 370, 371, 304, 305, 318, 319, 320, 321, 322, 323,
 336, 337, 338, 339, 340, 341, 354, 355, 356, 357, 358, 359, 372, 373, 374, 375, 376, 377, 378, 379,
 380, 381, 382, 383, 396, 397, 398, 399, 400, 401, 414, 415, 416, 417, 418, 419, 432, 433, 434, 435,
 436, 437, 450, 451, 452, 453, 454, 455, 468, 469, 470, 471, 472, 473, 486, 487, 488, 489, 490, 491,
 504, 505, 506, 507, 508, 509, 522, 523, 524, 525, 526, 527, 540, 541, 542, 543, 544, 545, 558, 559,
 560, 561, 562, 563, 384, 385, 386, 387, 388, 389, 402, 403, 404, 405, 406, 407, 420, 421, 422, 423,
 424, 425, 438, 439, 440, 441, 442, 443, 456, 457, 458, 459, 460, 461, 474, 475, 476, 477, 478, 479,
 492, 493, 494, 495, 496, 497, 510, 511, 512, 513, 514, 515, 528, 529, 530, 531, 532, 533, 546, 547,
 548, 549, 550, 551, 564, 565, 566, 567, 568, 569, 390, 391, 392, 393, 394, 395, 408, 409, 410, 411,
 412, 413, 426, 427, 428, 429, 430, 431, 444, 445, 446, 447, 448, 449, 462, 463, 464, 465, 466, 467,
 480, 481, 482, 483, 484, 485, 498, 499, 500, 501, 502, 503, 516, 517, 518, 519, 520, 521, 534, 535,
 536, 537, 538, 539, 552, 553, 554, 555, 556, 557, 570, 571, 572, 573, 574, 575},

{  0,   1,   2,   3,   6,   7,   8,   9,  12,  13,  14,  15,   4,   5,  18,  19,  10,  11,  24,  25,
  16,  17,  30,  31,  20,  21,  22,  23,  26,  27,  28,  29,  32,  33,  34,  35,  36,  37,  38,  39,
  42,  43,  44,  45,  48,  49,  50,  51,  40,  41,  54,  55,  56,  57,  46,  47,  60,  61,  62,  63,
  52,  53,  66,  67,  68,  69,  58,  59,  72,  73,  74,  75,  76,  77,  64,  65,  78,  79,  80,  81,
  82,  83,  70,  71,  84,  85,  86,  87,  88,  89,  90,  91,  92,  93,  94,  95, 108, 109, 110, 111,
 112, 113,  96,  97,  98,  99, 100, 101, 114, 115, 116, 117, 118, 119, 102, 103, 104, 105, 106, 107,
 120, 121, 122, 123, 124, 125, 126, 127, 128, 129, 130, 131, 144, 145, 146, 147, 148, 149, 162, 163,
 164, 165, 132, 133, 134, 135, 136, 137, 150, 151, 152, 153, 154, 155, 168, 169, 170, 171, 138, 139,
 140, 141, 142, 143, 156, 157, 158, 159, 160, 161, 174, 175, 176, 177, 166, 167, 180, 181, 182, 183,
 184, 185, 198, 199, 200, 201, 202, 203, 216, 217, 218, 219, 220, 221, 172, 173, 186, 187, 188, 189,
 190, 191, 204, 205, 206, 207, 208, 209, 222, 223, 224, 225, 226, 227, 178, 179, 192, 193, 194, 195,
 196, 197, 210, 211, 212, 213, 214, 215, 228, 229, 230, 231, 232, 233, 234, 235, 236, 237, 238, 239,
 252, 253, 254, 255, 256, 257, 270, 271, 272, 273, 274, 275, 288, 289, 290, 291, 292, 293, 306, 307,
 240, 241, 242, 243, 244, 245, 258, 259, 260, 261, 262, 263, 276, 277, 278, 279, 280, 281, 294, 295,
 296, 297, 298, 299, 312, 313, 246, 247, 248, 249, 250, 251, 264, 265, 266, 267, 268, 269, 282, 283,
 284, 285, 286, 287, 300, 301, 302, 303, 304, 305, 318, 319, 308, 309, 310, 311, 324, 325, 326, 327,
 328, 329, 342, 343, 344, 345, 346, 347, 360, 361, 362, 363, 364, 365, 378, 379, 380, 381, 382, 383,
 396, 397, 398, 399, 400, 401, 314, 315, 316, 317, 330, 331, 332, 333, 334, 335, 348, 349, 350, 351,
 352, 353, 366, 367, 368, 369, 370, 371, 384, 385, 386, 387, 388, 389, 402, 403, 404, 405, 406, 407,
 320, 321, 322, 323, 336, 337, 338, 339, 340, 341, 354, 355, 356, 357, 358, 359, 372, 373, 374, 375,
 376, 377, 390, 391, 392, 393, 394, 395, 408, 409, 410, 411, 412, 413, 414, 415, 416, 417, 418, 419,
 432, 433, 434, 435, 436, 437, 450, 451, 452, 453, 454, 455, 468, 469, 470, 471, 472, 473, 486, 487,
 488, 489, 490, 491, 504, 505, 506, 507, 508, 509, 522, 523, 524, 525, 526, 527, 420, 421, 422, 423,
 424, 425, 438, 439, 440, 441, 442, 443, 456, 457, 458, 459, 460, 461, 474, 475, 476, 477, 478, 479,
 492, 493, 494, 495, 496, 497, 510, 511, 512, 513, 514, 515, 528, 529, 530, 531, 532, 533, 426, 427,
 428, 429, 430, 431, 444, 445, 446, 447, 448, 449, 462, 463, 464, 465, 466, 467, 480, 481, 482, 483,
 484, 485, 498, 499, 500, 501, 502, 503, 516, 517, 518, 519, 520, 521, 534, 535, 536, 537, 538, 539,
 540, 541, 542, 543, 544, 545, 558, 559, 560, 561, 562, 563, 546, 547, 548, 549, 550, 551, 564, 565,
 566, 567, 568, 569, 552, 553, 554, 555, 556, 557, 570, 571, 572, 573, 574, 575}
}};

#endif
