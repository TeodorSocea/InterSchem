#define SCREEN_WIDTH                              800
#define SCREEN_HEIGHT                            800
#define BUTTON_COLOR                            WHITE
#define TXT_SZE                                           1
#define TXT_FONT                                        3 //SANS_SERIF_FONT
#define BUTT_PADDING                               6
#define ESC_KEY                                          27
#define BK_COLOR                                       LIGHTBLUE
#define BLK_COLOR                                     BLUE
#define BLK_MOD_COL                                 WHITE
#define CNNCTN_COL                                  WHITE
#define INVLD_BLK_COL                             LIGHTRED
#define DEL_BLK_COL                                  RED
#define MIN_HEIGHT_BLOCK                      35
#define MIN_WIDTH_BLOCK                        50
#define radius_cnnctn                                     6
#define EMPTY                                                -1
#define INTRGTION_TXT_COL                   BLACK
#define ENTER_KEY                                    13
#define CRVD_RCTGLE_RAD                      10
#define UP_TYPE                                           0
#define LFT_TYPE                                         1
#define BLK_TRTRY                                      BUTT_PADDING - 1
#define LFT                                                    1
#define RGT                                                   0
#define DWN                                                  0
#define UP                                                      1
#define INCREASING                                     0
#define DECREASING                                   1
#define DELAY                                              150
#define DRAW                                               0
#define DELETE                                            1
#define HEIGHT_INTRGTION_BOX             50
#define SPACE_BTW                                    10
#define wait_time_blink                                  10000000
#define STOP_OR_START_BLK_WIDTH      50
#define STOP_OR_START_BLK_HEIGHT    40
#define BLK_MOD_MODE                               1
#define BLK_CREATION_MODE                     0
#include <structs.h>

bool start_exists = false;
bool stop_exists = false;
char file_name[ 20 ] = "input.in";

const int nr_of_horiz_butt = 6;
Button horiz_butt[] = {
    {0, false, "INSERT"},
    {0, false, "RUN"},
    {0, false, "BUILD"},
    {0, false, "FILE"},
    {0, false, "SAVE"},
    {0, false, "LOAD"}
};

const int nr_of_vert_butt = 6;
Button vert_butt[] = {
    {1, false, "start"},
    {1, false, "read"},
    {1, false, "if_statement"},
    {1, false, "expression"},
    {1, false, "write"},
    {1, false, "stop"}
};
Dim menuPoster = { 100, 100 };
Dim dim_of_butt[] = {
    {(SCREEN_WIDTH - menuPoster.width - BUTT_PADDING)/nr_of_horiz_butt, 65}, // horiz
    {115, (SCREEN_HEIGHT - menuPoster.height - BUTT_PADDING)/nr_of_vert_butt} // vert
};

int colors[] = {BLACK, LIGHTGREEN };
