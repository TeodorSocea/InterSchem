#include <visual_txt.h>

Spot findCorner( Button newButton, Spot up_lft ) {
    Spot dwn_rgt;
    dwn_rgt.xPos = up_lft.xPos + dim_of_butt[ newButton.type ].width;
    dwn_rgt.yPos = up_lft.yPos + dim_of_butt[ newButton.type ].height;
    dwn_rgt.xPos -= BUTT_PADDING;
    dwn_rgt.yPos -= BUTT_PADDING;

    if ( newButton.type == UP_TYPE )
        dwn_rgt.yPos -= BUTT_PADDING;
    if ( newButton.type == LFT_TYPE )
        dwn_rgt.xPos -= BUTT_PADDING;
    return dwn_rgt;
}

void place_mid_of_butt( Button newButton, Spot up_lft ) {
    settextjustify( CENTER_TEXT, CENTER_TEXT );

    Spot dwn_rgt = findCorner( newButton, up_lft );
    Spot outText;
    outText.xPos = (up_lft.xPos + dwn_rgt.xPos )/2;
    outText.yPos = (up_lft.yPos + dwn_rgt.yPos )/2;

    setcolor( colors[ newButton.isPressed ] );
    outtextxy( outText.xPos, outText.yPos, newButton.message );
}

void switch_color_vert_butt( int ind ) {
    Button* butt = &( vert_butt[ind] );
    (*butt).isPressed ^= 1;

    Spot up_lft_butt { BUTT_PADDING, BUTT_PADDING };
    up_lft_butt.yPos += ind*dim_of_butt[ 1 ].height+ menuPoster.height;

    settextstyle(TXT_FONT, HORIZ_DIR, TXT_SZE);

    place_mid_of_butt( (*butt), up_lft_butt );

    if ( ind == 0  ) {
        if ( start_exists == false )
            start_exists = true;
        else {
            delay( DELAY );
            (*butt).isPressed ^= 1;
            place_mid_of_butt( (*butt), up_lft_butt );
            return;
        }
    }
    else
    if ( ind == 5 ) {
        if ( stop_exists == false )
            stop_exists = true;
        else {
            delay( DELAY );
            (*butt).isPressed ^= 1;
            place_mid_of_butt( (*butt), up_lft_butt );
            return;
        }
    }
    redraw_all_cnnctns( BK_COLOR );
    addInstrInd( ind );
    if ( ind != 0 && ind != 5 )
        ask_for_blk_txt( instrs.inst_vec[instrs.len-1], BLK_CREATION_MODE );

    show_vec(instrs);

    draw_blk(instrs.inst_vec[instrs.len-1], BLK_COLOR);
    redraw_all_cnnctns( BLK_COLOR );

    (*butt).isPressed ^= 1;
    place_mid_of_butt( (*butt), up_lft_butt );
}

void switch_color_horiz_butt( int ind ) {
    horiz_butt[ ind ].isPressed  ^= 1;

    Spot up_lft_last_butt { BUTT_PADDING, BUTT_PADDING };
    up_lft_last_butt.xPos += ind*dim_of_butt[ 0 ].width + menuPoster.width;

    settextstyle(TXT_FONT, HORIZ_DIR, TXT_SZE);
    place_mid_of_butt(horiz_butt[ ind ], up_lft_last_butt );

    if ( ind != 0 ) {
        delay( DELAY );
        horiz_butt[ ind ].isPressed  ^= 1;
        place_mid_of_butt(horiz_butt[ ind ], up_lft_last_butt );
    }
}


void drawButton( Button newButton, Spot up_lft ) {
    up_lft.yPos += BUTT_PADDING;
    up_lft.xPos += BUTT_PADDING;

    Spot dwn_rgt = findCorner( newButton, up_lft );

    draw_crvd_rctngle( up_lft, dwn_rgt, BUTTON_COLOR );
    setfillstyle( SOLID_FILL, BUTTON_COLOR );
    int midX = ( up_lft .xPos + dwn_rgt.xPos )/2;
    int midY = ( up_lft .yPos + dwn_rgt.yPos )/2;
    floodfill( midX, midY, BUTTON_COLOR );

    place_mid_of_butt( newButton, up_lft );
}

void draw_horiz_row_of_butt() {
    settextstyle(TXT_FONT, HORIZ_DIR, TXT_SZE);
    int width = menuPoster.width;
    Spot up_lft_curr_butt = {  width, 0  };
    for ( int curr_ind = 0; curr_ind < nr_of_horiz_butt; curr_ind++ )
    {
        drawButton( horiz_butt[ curr_ind ], up_lft_curr_butt );
        up_lft_curr_butt.xPos += dim_of_butt[ 0 ].width;
    }
}

void draw_butt_for_blks_ins() {
    settextstyle(TXT_FONT, HORIZ_DIR, TXT_SZE);
    Spot up_lft_curr_butt = { 0, menuPoster.height };
    for ( int curr_ind = 0; curr_ind < nr_of_vert_butt; curr_ind++ ) {
        drawButton( vert_butt[ curr_ind ], up_lft_curr_butt );
        up_lft_curr_butt.yPos += dim_of_butt[ 1 ].height;
    }
}

void drawMenuPoster() {
    Spot up_lft, dwn_rgt;
    up_lft.xPos = BUTT_PADDING;
    up_lft.yPos = BUTT_PADDING;
    dwn_rgt.xPos = menuPoster.width;
    dwn_rgt.yPos = menuPoster.height;
    setfillstyle( SOLID_FILL, WHITE );
    bar( up_lft.xPos, up_lft.yPos, dwn_rgt.xPos, dwn_rgt.yPos );

    setcolor( BLACK );
    Spot menuPosterText;
    menuPosterText.xPos = dwn_rgt.xPos - textwidth( "Options" );
    menuPosterText.yPos = BUTT_PADDING;
    outtextxy( menuPosterText.xPos, menuPosterText.yPos, "Options" );

    setcolor( BLACK );
    menuPosterText.yPos = dwn_rgt.yPos - textheight( "Details" );
    menuPosterText.xPos = BUTT_PADDING;
    outtextxy( menuPosterText.xPos, menuPosterText.yPos, "Details" );

    setlinestyle( 3, 0, 3);
    setcolor( BLACK );
    line( BUTT_PADDING, BUTT_PADDING, dwn_rgt.xPos, dwn_rgt.yPos);
    setlinestyle( 0, 0, 1);
}

void prepareWindow() {
    initwindow( SCREEN_WIDTH, SCREEN_HEIGHT, "Interschem-Teodor Socea si Tudor-Cristian Balan" );

    setfillstyle( SOLID_FILL, BK_COLOR );
    setbkcolor( BUTTON_COLOR );
    floodfill( 1, 1, WHITE );

   settextstyle( TXT_FONT, HORIZ_DIR, TXT_SZE );
   drawMenuPoster();
   draw_horiz_row_of_butt();
}
