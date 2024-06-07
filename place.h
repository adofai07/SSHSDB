#ifndef PLACE_H_INCLUDED
#define PLACE_H_INCLUDED

#include <stdio.h>

const char *opt_plc[100] = {
	 "stdy",   // 자습실
	 "sem1",   // 세미나실1
	 "sem2",   // 세미나실2
	 "sem3",   // 세미나실3
	 "sem4",   // 세미나실4
	 "libr",   // 도서관
	 "emp1",   // 1학년공강실
	 "emp2",   // 2학년공강실
	 "emp3",   // 3학년공강실
	 "eng1",   // 외국어강의실1
	 "eng2",   // 외국어강의실2
	 "eng3",   // 외국어강의실3
	 "phy1",   // 물리강의실1
	 "phy2",   // 물리강의실2
	 "med1",   // 미디어부스1
	 "med2",   // 미디어부스2
	 "scix",   // 융합과학실험실
	 "geox",   // 지구과학실험실
	 "meet",   // 대회의실
	 "hist",   // 역사관
	 "dorm",   // 기숙사
	 NULL
};


const char *opt_plc_kor[100] = {
	"\uC790\uC2B5\uC2E4",     						// 자습실
	"\uC138\uBBF8\uB098\uC2E41",  					// 세미나실1
	"\uC138\uBBF8\uB098\uC2E42",  					// 세미나실2
	"\uC138\uBBF8\uB098\uC2E43",  					// 세미나실3
	"\uC138\uBBF8\uB098\uC2E44",  					// 세미나실4
	"\uB3C4\uC11C\uAD00",      						// 도서관
	"1\uD559\uB144\uACF5\uAC15\uC2E4", 				// 1학년공강실
	"2\uD559\uB144\uACF5\uAC15\uC2E4", 				// 2학년공강실
	"3\uD559\uB144\uACF5\uAC15\uC2E4", 				// 3학년공강실
	"\uC678\uAD6D\uC5B4\uAC15\uC758\uC2E41", 		// 외국어강의실1
	"\uC678\uAD6C\uC5B4\uAC15\uC758\uC2E42", 		// 외국어강의실2
	"\uC678\uAD6C\uC5B4\uAC15\uC758\uC2E43", 		// 외국어강의실3
	"\uBB3C\uB9AC\uAC15\uC758\uC2E41", 				// 물리강의실1
	"\uBB3C\uB9AC\uAC15\uC758\uC2E42", 				// 물리강의실2
	"\uBBF8\uB514\uC5B4\uBD80\uC2A41", 				// 미디어부스1
	"\uBBF8\uB514\uC5B4\uBD80\uC2A42", 				// 미디어부스2
	"\uc735\ud569\uacfc\ud559\uc2e4\ud5d8\uc2e4", 	// 융합과학실험실
	"\uC9C0\uAD6C\uACFC\uD559\uC2E4\uD5D8\uC2E4", 	// 지구과학실험실
	"\uB300\uD68C\uC758\uC2E4",     				// 대회의실
	"\uC5ED\uC0AC\uAD00",      						// 역사관
	"\uCCA8\uB2E8\uAC15\uC5F0",    					// 첨단강연
	"\uAE30\uC219\uC0AC",        					// 기숙사
	NULL
};


const char *all_plc_kor[100] = {
	"\uC790\uC2B5\uC2E4",              		// 자습실
	"\uC138\uBBF8\uB098\uC2E41",        	// 세미나실1
	"\uC138\uBBF8\uB098\uC2E42",        	// 세미나실2
	"\uC138\uBBF8\uB098\uC2E43",        	// 세미나실3
	"\uC138\uBBF8\uB098\uC2E44",        	// 세미나실4
	"\uB3C4\uC11C\uAD00",               	// 도서관
	"1\uD559\uB144\uACF5\uAC15\uC2E4",   	// 1학년공강실
	"2\uD559\uB144\uACF5\uAC15\uC2E4",   	// 2학년공강실
	"3\uD559\uB144\uACF5\uAC15\uC2E4",   	// 3학년공강실
	"\uC678\uAD6D\uC5B4\uAC15\uC758\uC2E41",// 외국어강의실1
	"\uC678\uAD6D\uC5B4\uAC15\uC758\uC2E42",// 외국어강의실2
	"\uC678\uAD6D\uC5B4\uAC15\uC758\uC2E43",// 외국어강의실3
	"\uBB3C\uB9AC\uAC15\uC758\uC2E41",    	// 물리강의실1
	"\uBB3C\uB9AC\uAC15\uC758\uC2E42",    	// 물리강의실2
	"\uC9C0\uAD6C\uACFC\uD559\uAC15\uC758\uC2E41", // 지구과학강의실1
	"\uC9C0\uAD6C\uACFC\uD559\uAC15\uC758\uC2E42", // 지구과학강의실2
	"\uBBF8\uB514\uC5B4\uBD80\uC2A41",    	// 미디어부스1
	"\uBBF8\uB514\uC5B4\uBD80\uC2A42",    	// 미디어부스2
	"\uc735\ud569\uacfc\ud559\uc2e4\ud5d8\uc2e4", // 융합과학실험실
	"\uC9C0\uAD6C\uACFC\uD559\uC2E4\uD5D8\uC2E4", // 지구과학실험실
	"\uB300\uD68C\uC758\uC2E4",           	// 대회의실
	"\uC5ED\uC0AC\uAD00",                 	// 역사관
	"\uCCA8\uB2E8\uAC15\uC5F0",           	// 첨단강연
	"\uAE30\uC219\uC0AC",        			// 기숙사	
	"\uC74C\uC545\uC2E4",                  	// 음악실
	"\uBBF8\uC220\uC2E4",                  	// 미술실
	"\uCC9C\uBB38\uB300",                  	// 천문대
	"\uAC15\uB2F9",                       	// 강당
	"\uC2DC\uCCAD\uAC1D\uC2E4",            	// 시청각실
	"\uC6B0\uC554\uACF5\uD1B5\uAC15\uC758\uC2E41", // 우암공통강의실1
	"\uC6B0\uC554\uACF5\uD1B5\uAC15\uC758\uC2E42", // 우암공통강의실2
	"\uC218\uD559\uAC15\uC758\uC2E41",     	// 수학강의실1
	"\uC218\uD559\uAC15\uC758\uC2E42",     	// 수학강의실2
	"\uC218\uD559\uAC15\uC758\uC2E43",     	// 수학강의실3
	"\uD654\uD559\uAC15\uC758\uC2E41",     	// 화학강의실1
	"\uD654\uD559\uAC15\uC758\uC2E42",     	// 화학강의실2
	"\uC0AC\uD68C\uAC15\uC758\uC2E41",     	// 사회강의실1
	"\uC0AC\uD68C\uAC15\uC758\uC2E42",     	// 사회강의실2
	"\uAD6D\uC5B4\uAC15\uC758\uC2E41",     	// 국어강의실1
	"\uAD6D\uC5B4\uAC15\uC758\uC2E42",     	// 국어강의실2
	"\uAD6D\uC5B4\uAC15\uC758\uC2E43",     	// 국어강의실3
	"\uC0DD\uBB3C\uAC15\uC758\uC2E41",     	// 생물강의실1
	"\uC0DD\uBB3C\uAC15\uC758\uC2E42",     	// 생물강의실2
	"\uC0DD\uBB3C\uC2E4\uD5D8\uC2E41",     	// 생물실험실1
	"\uC0DD\uBB3C\uC2E4\uD5D8\uC2E42",     	// 생물실험실2
	"\uC0DD\uBB3C\uC2E4\uD5D8\uC2E43",     	// 생물실험실3
	"\uBB3C\uB9AC\uC2E4\uD5D8\uC2E41",     	// 물리실험실1
	"\uBB3C\uB9AC\uC2E4\uD5D8\uC2E42",     	// 물리실험실2
	"\uD654\uD559\uC2E4\uD5D8\uC2E41",     	// 화학실험실1
	"\uD654\uD559\uC2E4\uD5D8\uC2E42",     	// 화학실험실2
	"\ud1f4\uc0ac",							// 퇴사
	"\ubbf8\uc785\uc0ac",					// 미입사
	NULL
};


const char *all_plc[100] = {
    "stdy",   // 자습실
    "sem1",   // 세미나실1
    "sem2",   // 세미나실2
    "sem3",   // 세미나실3
    "sem4",   // 세미나실4
    "libr",   // 도서관
    "emp1",   // 1학년공강실
    "emp2",   // 2학년공강실
    "emp3",   // 3학년공강실
    "eng1",   // 외국어강의실1
    "eng2",   // 외국어강의실2
    "eng3",   // 외국어강의실3
    "phy1",   // 물리강의실1
    "phy2",   // 물리강의실2
    "ear1",   // 지구과학강의실1
    "ear2",   // 지구과학강의실2
    "med1",   // 미디어부스1
    "med2",   // 미디어부스2
    "scix",   // 융합과학실험실
    "geox",   // 지구과학실험실
    "meet",   // 대회의실
    "hist",   // 역사관
    "tech",   // 첨단강연
	"dorm",   // 기숙사
    "musi",   // 음악실
    "arts",   // 미술실
    "obsv",   // 천문대
    "adtm",   // 강당
    "thtr",   // 시청각실
    "uam1",   // 우암공통강의실1
    "uam2",   // 우암공통강의실2
    "mat1",   // 수학강의실1
    "mat2",   // 수학강의실2
    "mat3",   // 수학강의실3
    "chm1",   // 화학강의실1
    "chm2",   // 화학강의실2
    "soc1",   // 사회강의실1
    "soc2",   // 사회강의실2
    "kor1",   // 국어강의실1
    "kor2",   // 국어강의실2
    "kor3",   // 국어강의실3
    "bio1",   // 생물강의실1
    "bio2",   // 생물강의실2
    "bix1",   // 생물실험실1
    "bix2",   // 생물실험실2
    "bix3",   // 생물실험실3
    "phx1",   // 물리실험실1
    "phx2",   // 물리실험실2
    "chx1",   // 화학실험실1
    "chx2",   // 화학실험실2
    "kick",   // 퇴사
	"home",   // 미입사
	NULL
};


const int classroom_limits[100] = {
    400,  // 자습실
    5,    // 세미나실1
    5,    // 세미나실2
    5,    // 세미나실3
    5,    // 세미나실4
    20,   // 도서관
    20,   // 1학년공강실
    20,   // 2학년공강실
    20,   // 3학년공강실
    20,   // 외국어강의실1
    20,   // 외국어강의실2
    20,   // 외국어강의실3
    20,   // 물리강의실1
    20,   // 물리강의실2
    20,   // 지구과학강의실1
    20,   // 지구과학강의실2
    1,    // 미디어부스1
    1,    // 미디어부스2
    20,   // 융합과학실험실
    20,   // 지구과학실험실
    20,   // 대회의실
    20,   // 역사관
    20,   // 첨단강연
	400,  // 기숙사
    20,   // 음악실
    20,   // 미술실
    20,   // 천문대
    400,  // 강당
    400,  // 시청각실
    20,   // 우암공통강의실1
    20,   // 우암공통강의실2
    20,   // 수학강의실1
    20,   // 수학강의실2
    20,   // 수학강의실3
    20,   // 화학강의실1
    20,   // 화학강의실2
    20,   // 사회강의실1
    20,   // 사회강의실2
    20,   // 사회강의실3
    20,   // 국어강의실1
    20,   // 국어강의실2
    20,   // 국어강의실3
    20,   // 생물강의실1
    20,   // 생물강의실2
    20,   // 생물실험실1
    20,   // 생물실험실2
    20,   // 생물실험실3
    20,   // 물리실험실1
    20,   // 물리실험실2
    20,   // 화학실험실1
    20,   // 화학실험실2
    400,  // 퇴사
	400,  // 미입사
	0     // 배열의 끝(굳이 필요할까?)
};


#endif