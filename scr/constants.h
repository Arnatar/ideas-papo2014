// ------for all------
// the maximum value for an idea's attributes
#define IDEA_MAX 10

// ------for logic.c------
// competition test, maximum difference between target human wv and origin ideas wv
#define MAX_CWV_VS_HWV  4

// competition test, maximum difference between origin & targen cmplxty
#define MAX_CMPLX_DIFF 4

// conversation barrier (lower = less likely) 10000-1 to 90000-1 max val (based on complexity-difference)
#define CONVERSATION_BARRIER 2000

// winner calculation, with random represented other environment influences range
#define ENV_INF_RANGE 7

// construct looser, wv adjustment range (should not be tweaked imo, but feel free to try)
#define CL_ADJ_RANGE 1

// quality & cmplxty mutation chance, 100000 - 1 max val
#define QC_MUT_CHANCE 50

// quality & cmplxty mutation range
#define QC_MUT_RANGE 1

// WV mutation chance, 100000 - 1 max val both WVs
#define WV_MUT_CHANCE 10

// WV mutation range both WVs
#define WV_MUT_RANGE 1

// max WV difference when muting WVs
#define WV_MUT_MAX_DIFF 3


// ------for idea.c------
// chances in pro mil
// spawn chance for best possible ideas
#define BEST_IDEA_CHANCE 50

// spawn chance for medium possible ideas
#define MED_IDEA_CHANCE 100

// spawn quality-cmplxty dependency range
#define QUAL_CMPLTY_DEP_RANGE 1 

// spawn WV-differenc range
#define WV_DIFF_RANGE 2