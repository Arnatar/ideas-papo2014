// ------for all------
// the maximum value + 1 for an idea's attributes BETTER DO NOT TWEAK if you want to run make pygame (not enough colors => crash)
#define IDEA_MAX 10

// ------for logic.c------
// competition test, maximum difference between target human wv and origin ideas wv
#define MAX_CWV_VS_HWV  6

// competition test, maximum difference between origin & targen cmplxty
#define MAX_CMPLX_DIFF 5

// conversation barrier (lower = less likely) 10000-1 to 90000-1 max val (based on complexity-difference)
#define CONVERSATION_BARRIER 10000

// winner calculation, with random represented other environment influences range
#define ENV_INF_RANGE 7

// construct looser, wv adjustment range (should not be tweaked imo, but feel free to try)
#define CL_ADJ_RANGE 1

// quality & cmplxty mutation chance, 100000 - 1 max val
#define QC_MUT_CHANCE 100

// quality & cmplxty mutation range
#define QC_MUT_RANGE 1

// WV mutation chance, 100000 - 1 max val both WVs
#define WV_MUT_CHANCE 50

// WV mutation range both WVs
#define WV_MUT_RANGE 1

// max WV difference when muting WVs
#define WV_MUT_MAX_DIFF 3


// ------for idea.c------
// chances in pro mil
// spawn chance for best possible ideas
#define BEST_IDEA_CHANCE 0

// spawn chance for medium possible ideas
#define MED_IDEA_CHANCE 10

// spawn quality-cmplxty dependency range
#define QUAL_CMPLXTY_DEP_RANGE 2 

// spawn WV-differenc range
#define WV_DIFF_RANGE 2