#include <draw_blks.h>
#include <fstream>

char *Bar = "|";

void mem_text( char* tmp, char* output ) {
     strcpy(tmp,output);
}

void print_Bar(int x, int y, int color ) {
    setcolor( color );
    y -= 4;
    outtextxy( x, y, Bar );
}

void blink_Bar( int x, int y, char *txt, int& wait ) {
    x += textwidth( txt );
    int ind;
    if ( wait  == 1 ) {
        wait = 0;
        for ( ind = 0; ind <= wait_time_blink; ind++ )
            if ( kbhit() )
                return;
    }
    print_Bar( x, y, BK_COLOR );
    for ( ind = 0; ind <= wait_time_blink; ind++ )
        if ( kbhit() ) {
            print_Bar( x, y, INTRGTION_TXT_COL );
            return;
        }
    print_Bar( x, y, INTRGTION_TXT_COL );
}

void place_text( int x, int y, char* tmp, int color ) {
    setcolor(color);
    outtextxy(x,y,tmp);
    print_Bar( x + textwidth( tmp ), y, color );
}

void add_key( char pressed_key, char output[MAX], int char_lim ) {
    char t[2];
    t[0] = pressed_key;
    t[1] = '\0';
    strcat( output, t );
}

void scan_window_text(char output[MAX],Spot point, int char_lim, Spot interogation ) {
    strcpy(output,"");                                                      // initialize
    char tmp[MAX];
    char pressed_key;

    int wait = 1;
    do {
        blink_Bar( point.xPos, point.yPos, tmp, wait );

        if ( !kbhit() ) {
            wait = 1;
            continue;
        }
        pressed_key=getch();
        if ( pressed_key == ENTER_KEY )
            break;

        if (pressed_key==8) {                                         // backspace
            if (strlen(output)>0) {
                    mem_text( tmp, output );
                    place_text(point.xPos, point.yPos, tmp, BK_COLOR);

                    output[strlen(output)-1]='\0';                  // get rid of the last char

                    mem_text( tmp, output );

                    place_text(point.xPos, point.yPos, tmp, BK_COLOR);

                    place_text(point.xPos, point.yPos, tmp, INTRGTION_TXT_COL );
                }
            else
                Beep(1000,100);
        }
        else {
            if ( char_lim > strlen( tmp ) )
                add_key( pressed_key, output, char_lim );
            else {
                int x1, y1;
                char message[ 10 ];
                if ( interogation.xPos == 0 ) {
                    strcpy( message, "(max. 30)");
                    x1 = SCREEN_WIDTH - textwidth(": ") - textwidth( message ) - SPACE_BTW;
                }
                else {
                    strcpy( message, "(max. 10)");
                    x1 = interogation.xPos;
                }
                setcolor( BUTTON_COLOR );
                outtextxy( x1, interogation.yPos, message );
                delay( DELAY );

                setcolor( INTRGTION_TXT_COL );
                outtextxy( x1, y1, message );
            }

            mem_text( tmp, output );

            place_text(point.xPos, point.yPos, tmp, INTRGTION_TXT_COL );
        }
    } while ( true );

    place_text(point.xPos, point.yPos, tmp, BK_COLOR);
}

void place_intrgtion_txt( Spot up_lft, int color, char* txt ) {
    setcolor(color);
    outtextxy( up_lft.xPos, up_lft.yPos, txt );
}

void hide_interfering_blks( Spot border, int color ) {
    int ind;
    Spot dwn_rgt;
     for ( ind = 0; ind < instrs.len; ind++ ) {
        dwn_rgt = instrs.inst_vec[ ind ].dwn_rgt;
        if ( isBtw( dwn_rgt.xPos, border.xPos, SCREEN_WIDTH )&&
             isBtw( dwn_rgt.yPos, border.yPos, SCREEN_HEIGHT ) )
                draw_blk( instrs.inst_vec[ ind ], color );
    }
}

void ask_for_blk_txt( instr& blk, int mode ) {
    int char_lim, ind, len, max_width_per_char, x, y;
    char txt[ 50 ], arr[ 40 ], *txt_ptr;
    Spot border;
    if(blk.type == "read")
        strcpy( txt, "Specify variable to read");
    else if(blk.type == "if_statement")
       strcpy( txt, "Specify expression to test for");
    else if(blk.type == "expression")
        strcpy( txt,  "Specify expression");
    else if(blk.type == "write")
       strcpy( txt,  "Specify variable to write");

    char_lim = 30;
    strcat( txt, " (max. 30): ");

    txt_ptr = txt;
    Spot up_lft_intrgtion;
    up_lft_intrgtion.xPos = SCREEN_WIDTH   - textwidth( txt_ptr )  - SPACE_BTW ;
    up_lft_intrgtion.yPos = SCREEN_HEIGHT - HEIGHT_INTRGTION_BOX;

    max_width_per_char = textwidth("V");
    x = SCREEN_WIDTH - char_lim*max_width_per_char - SPACE_BTW;
    y = up_lft_intrgtion.yPos + textheight( txt_ptr ) + SPACE_BTW;

    border.yPos = up_lft_intrgtion.yPos;
    border.xPos = x;

    hide_interfering_blks( border, BK_COLOR );

    setbkcolor( BK_COLOR );

    /*set the normal mode of printing text*/
    settextjustify(LEFT_TEXT, TOP_TEXT );

    place_intrgtion_txt( up_lft_intrgtion , BLACK,         txt_ptr);

    scan_window_text( arr, {x, y}, char_lim, {0, up_lft_intrgtion.yPos} );
    place_intrgtion_txt( up_lft_intrgtion , BK_COLOR, txt_ptr);

    setbkcolor( BUTTON_COLOR );

    len = strlen( arr );
    if ( len == 0 ) {
        strcpy(arr, "empty");
    }

    int nxtHght  = Max(MIN_HEIGHT_BLOCK, 2*textheight(arr) );
    int nxtWdth = Max(MIN_WIDTH_BLOCK  , 2*textwidth(arr)  );

    if ( mode == BLK_CREATION_MODE ) {
        blk.up_lft.xPos = blk.dwn_rgt.xPos - nxtWdth;
        blk.up_lft.yPos = blk.dwn_rgt.yPos - nxtHght;
    }
    else
    if ( mode == BLK_MOD_MODE ){
        blk.dwn_rgt.xPos = blk.up_lft.xPos + nxtWdth;
        blk.dwn_rgt.yPos = blk.up_lft.yPos + nxtHght;
    }

    if ( blk.type == "expression" ) {
         int diff = ( blk.dwn_rgt.xPos - blk.up_lft.xPos ) / 4;
         blk.dwn_rgt.xPos -= diff;
         blk.up_lft.xPos += diff;
    }

    blk.container = arr;

    hide_interfering_blks( border, BLK_COLOR );
}

bool verify_file_name( char f_name[20] ) {
    ifstream fin(f_name);
    if ( fin.fail() )
        return false;
    else
        return true;
}

void ask_for_file_name( char f_name[ 20 ]) {
    char* print_txt = "Give the file name(max. 30): ";
    char arr[ 31 ];
    int width, height,  char_lim,  max_width_per_char, x, y;
    width = textwidth( print_txt );
    height = textheight( print_txt );
    char_lim = 30;

    Spot up_lft_intrgtion;
    up_lft_intrgtion.xPos = SCREEN_WIDTH   - width - SPACE_BTW;
    up_lft_intrgtion.yPos = SCREEN_HEIGHT - HEIGHT_INTRGTION_BOX;

    max_width_per_char = textwidth("V");
    x = SCREEN_WIDTH - char_lim*max_width_per_char - SPACE_BTW;
    y = up_lft_intrgtion.yPos + height + SPACE_BTW;

    Spot border;
    assign2Spot( border, x, up_lft_intrgtion.yPos );

    hide_interfering_blks( border, BK_COLOR );

    setbkcolor( BK_COLOR );
    /*set the normal mode of printing text*/
    settextjustify(LEFT_TEXT, TOP_TEXT );
    place_intrgtion_txt( up_lft_intrgtion, INTRGTION_TXT_COL, print_txt );

    scan_window_text( arr, {x, y}, char_lim, { 0, up_lft_intrgtion.yPos } );

    int color;

        if ( verify_file_name( arr ) ) {
            strcpy( f_name, arr );
            color = LIGHTGREEN;
        }
        else
            color = LIGHTRED;

    place_intrgtion_txt( up_lft_intrgtion, color , print_txt );
    delay( 2*DELAY );
    place_intrgtion_txt( up_lft_intrgtion, BK_COLOR, print_txt );

    setbkcolor( BUTTON_COLOR );
     hide_interfering_blks( border, BLK_COLOR );
}

int compute_nxt_x( int width ) {
    int middle_of_box;
    middle_of_box = (dim_of_butt[ 1 ].width)/2;
    return Max( 5, middle_of_box - width/2 );
}

bool is_digit( char chr ) {
    return  '0' <= chr && chr <= '9';
}

bool verify_txt_float( char* txt ) {
    int ind;
    for ( ind = 0; txt[ ind ]; ++ind )
        if ( !( is_digit( txt[ ind ] ) || txt[ ind ] == '.') )
            return false;
    return true;
}

void string2float( char* txt, float &num ) {
    int ind, pow_10, char2int;
    num = 0;
    for ( ind = 0; txt[ ind ] != '.' && txt[ ind ]; ind++ ) {
        char2int = txt[ ind ] -'0';
        num = num*10 + char2int;
    }
    if ( !txt[ ind ] )
        return;

    for ( ++ind, pow_10 = 10; txt[ ind ]; ++ind, pow_10 *= 10 ) {
        char2int = txt[ ind ] -'0';
        num = num + (float)( char2int )/pow_10;
    }
}


void ask_for_reg_value( float& value, string reg_name ) {
    int color = INTRGTION_TXT_COL;
    int yPos, xPos, middle, max_W = 0;
    char txt[ 30 ];
    setbkcolor( BK_COLOR );
    settextjustify(LEFT_TEXT, TOP_TEXT );

    yPos = 300;
    middle = (dim_of_butt[ 1 ].width)/2;

    strcpy( txt, "What's");
    xPos = compute_nxt_x( textwidth( txt ) );
    max_W = Max( max_W, xPos + textwidth( txt ) );
    place_intrgtion_txt( { xPos, yPos }, color, txt );
    yPos += textheight( txt );

    strcpy( txt, "the value of");
    xPos = compute_nxt_x( textwidth( txt ) );
    max_W = Max( max_W, xPos + textwidth( txt )  );
    place_intrgtion_txt( { xPos, yPos }, color, txt );
    yPos += textheight( txt );

    reg_name += " ?";
    char *txt_string = &reg_name[ 0 ];
    xPos = compute_nxt_x( textwidth( txt_string ) );
    max_W = Max( max_W, xPos + textwidth( txt )  );
    place_intrgtion_txt( { xPos, yPos }, color, txt_string );
    yPos += textheight( txt_string );

    strcpy( txt, "(max 10)");
    xPos = compute_nxt_x( textwidth( txt ) );
    max_W = Max( max_W, xPos + textwidth( txt )  );
    place_intrgtion_txt( { xPos, yPos }, color, txt );

    char output[ 50 ];
    scan_window_text( output, { 10, yPos + textheight( txt ) }, 10, { xPos, yPos } );

    output[ strlen( output ) ] = '\0';

    while ( verify_txt_float( output ) == false ) {
             place_intrgtion_txt( { 10, yPos + textheight( txt ) }, LIGHTRED, output );
             delay( DELAY );
             place_intrgtion_txt( { 10, yPos + textheight( txt ) }, BK_COLOR, output );

             scan_window_text( output, { 10, yPos + textheight( txt ) }, 10, { xPos, yPos } );
             output[ strlen( output ) ] = '\0';
    }
    place_intrgtion_txt( { 10, yPos + textheight( txt ) }, LIGHTGREEN, output );
    delay( DELAY );
    place_intrgtion_txt( { 10, yPos + textheight( txt ) }, BK_COLOR, output );

    yPos += textheight( txt ) + textheight( output );
    max_W = Max( max_W, 10 + textwidth( output ) );

    setfillstyle( SOLID_FILL, BK_COLOR );
    /*hide the printed text*/
    int x = dim_of_butt[ 1 ].width;
    bar( 0, 300, x, yPos );

    setbkcolor( BUTTON_COLOR );

    string2float( output, value );
}



void int2array( int value,  char repres[50] ) {
    int ind = 0;
    do{
        repres[ ind ] = value%10 + '0';
        value /= 10;
        ind++;
    } while ( value );
    repres[ ind ] = '\0';
    int ind1;
    char aux;
    /*reverse it*/
    for ( ind1 = 0, --ind; ind1 < ind; ++ind1, --ind ) {
        aux                = repres[ ind1 ];
        repres[ ind1 ] = repres[ ind ];
        repres[ ind ]   = aux;
    }
}

void print_on_screen_rows_of_9( char output[ 50 ], int& startY ) {
    int x = 5;
    int nr_dig_printed = 9;
    char tmp[10];
    int ind = 0;
    int curr_len;
    int len = strlen( output ) - 1;
    int nr_val_btw;

    settextjustify(LEFT_TEXT, TOP_TEXT );
    setbkcolor( BK_COLOR );
    setcolor( INTRGTION_TXT_COL );

    while ( ind <= len ) {
        nr_val_btw = len - ind +1;
        if ( nr_val_btw < nr_dig_printed )
            nr_dig_printed = nr_val_btw;

        strncpy( tmp, output + ind, nr_dig_printed );
        tmp[ nr_dig_printed ] = '\0';

        outtextxy( x, startY, tmp );
        startY += textheight( tmp );
        ind += nr_dig_printed;
    }
     setbkcolor( BUTTON_COLOR );
}

void float2array( float value, int& yPos ) {

    char output[ 50 ];
    output[ 0 ] = '\0';
    char repres_bef_point[ 50 ];
    if ( value < 0 ) {
        value = -value;
        output[ 0 ] = '-';
        output[ 1 ] = '\0';
    }
    int value_int = (int)( value );
   value -= value_int;
   int2array( value_int, repres_bef_point );
   strcat( output, repres_bef_point );

   value *= 1000000;
   value_int = ( int )( value );
   if ( value_int ) {
        char repres_after_point[ 50 ];
        int2array( value_int, repres_after_point );
        strcat( output, ".");
        strcat( output, repres_after_point);
   }
   print_on_screen_rows_of_9( output, yPos );
}


void print_reg_value( reg curr_reg, int& yPos ) {
    float value_reg = curr_reg.val;
    int xPos;
     setbkcolor( BK_COLOR );
    settextjustify(LEFT_TEXT, TOP_TEXT );

    xPos = 5;

    float2array( curr_reg.val, yPos );

     setbkcolor( BUTTON_COLOR );
}

void return_to_normal() {
    setbkcolor( BK_COLOR );

    setcolor( YELLOW );
    char message[ 20 ] = "Press any key";
    int xPos, yPos;
    xPos = 5;
    yPos = 300;
    outtextxy( xPos, yPos, message );

    yPos += textheight( message );

    strcpy( message, "to continue");
    outtextxy( xPos, yPos, message );

    while ( !kbhit() );

    setfillstyle( SOLID_FILL, BK_COLOR );
    int x = dim_of_butt[ 1 ].width;
    bar( 5, 300, x, 500 );
    redraw_all_Blks( BLK_COLOR );
    redraw_all_cnnctns( BLK_COLOR );

    setbkcolor( BUTTON_COLOR );
}

