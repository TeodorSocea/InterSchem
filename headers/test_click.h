#include <run.h>


bool isWhitinTheScreen( instr curr_blk ) {
    int S_rgt_brdr   = SCREEN_WIDTH   - BLK_TRTRY,
         S_dwn_brdr = SCREEN_HEIGHT - BLK_TRTRY,
         S_up_brdr   = dim_of_butt[ 0 ].height,
         S_lft_brdr    = dim_of_butt[ 1 ].width;

    return curr_blk.up_lft.xPos     >= S_lft_brdr    &&
              curr_blk.up_lft.yPos     >= S_up_brdr   &&
              curr_blk.dwn_rgt.xPos <= S_rgt_brdr   &&
              curr_blk.dwn_rgt.yPos <= S_dwn_brdr;
}

bool isWhitinTheUpMenu( Spot pos ) {
    int lft_brdr                 = menuPoster.width,
         dwn_brdr             = dim_of_butt[ 0 ].height - BUTT_PADDING,
         spc_from_lft_brdr = (pos.xPos - lft_brdr) % dim_of_butt[ 0 ].width;

    return pos.xPos              >= lft_brdr       &&
              pos.yPos              <= dwn_brdr   &&
              spc_from_lft_brdr >= BUTT_PADDING;
}

bool isWhitinTheLeftMenu( Spot pos ) {
    int rgt_brdr                    = dim_of_butt[1].width - BUTT_PADDING,
         lft_brdr                     = BUTT_PADDING + 1,
         up_brdr                    = menuPoster.height  + 1,
         spc_from_up_brdr   = (pos.yPos - up_brdr) % dim_of_butt[ 1 ].height;

    return pos.xPos              <= rgt_brdr &&
              pos.xPos               >= lft_brdr  &&
              pos.yPos               >= up_brdr &&
              spc_from_up_brdr >= BUTT_PADDING;
}


bool no_blk_collision( instr blk, int blk_ind ) {
    int ind;
    for ( ind = 0; ind < instrs.len; ind++ ) {
        if ( ind == blk_ind )
                continue;

         if ( blk_collision( blk, instrs.inst_vec[ ind ] ) )
                return false;
         }
    return true;
}

int last_prssed_horiz_butt_ind = EMPTY;
int get_pressed_blk_ind( Spot init_clck ) {
    int ind;
     for ( ind = 0; ind < instrs.len; ind++ )
        if ( isWhitinTheBlock( init_clck, instrs.inst_vec[ ind ] ) )
                return ind;

    return EMPTY;
}

bool mov_blk( int blk_ind, int movX, int movY ) {
    instr* curr_blk =  &(instrs.inst_vec[ blk_ind ]);
     if ( !( movX == 0 && movY == 0) ) {
            instr nxtBlk = *curr_blk;

            assign2Spot( nxtBlk.up_lft    , nxtBlk.up_lft.xPos    + movX, nxtBlk.up_lft.yPos      + movY );
            assign2Spot( nxtBlk.dwn_rgt, nxtBlk.dwn_rgt.xPos + movX, nxtBlk.dwn_rgt.yPos + movY );

            if ( isWhitinTheScreen( nxtBlk ) && no_blk_collision( nxtBlk, blk_ind ) ) {
                hide_blk( blk_ind );
                *curr_blk = nxtBlk;
                draw_blk( *curr_blk, BLK_MOD_COL );
            }
            else
                return true;
        }
    return false;
}

bool was_a_blk_pressed( Spot &init_clck ) {
    Spot nxt_clck;
    int blk_ind = get_pressed_blk_ind( init_clck );
    if ( blk_ind == EMPTY ) {
        clearmouseclick(WM_LBUTTONUP);
        return false;
    }
    draw_incident_cnnctns_of_blk( blk_ind, BLK_MOD_COL );
    draw_blk( instrs.inst_vec[ blk_ind ], BLK_MOD_COL );

    assign2Spot( nxt_clck, mousex(), mousey() );
    bool invalid_move,
            cnnctns_removed = false;
    int movX, movY;
    int color = BLK_COLOR;
    while ( ismouseclick(WM_LBUTTONUP) == false ) {
        movX = init_clck.xPos - nxt_clck.xPos;
        movY = init_clck.yPos - nxt_clck.yPos;
        if ( !( movX == 0 && movY == 0) && !cnnctns_removed ) {
            redraw_all_cnnctns( BK_COLOR );
            cnnctns_removed = true;
        }

        invalid_move = mov_blk( blk_ind, movX, movY );
        if ( invalid_move == true) {
            draw_blk( instrs.inst_vec[ blk_ind ], INVLD_BLK_COL );
            delay( DELAY );
            break;
        }

        nxt_clck = init_clck;
        assign2Spot( init_clck,  mousex(), mousey() );
        clearmouseclick(WM_LBUTTONDOWN);
    }
    redraw_all_Blks(     color  );
    redraw_all_cnnctns(color );
    clearmouseclick(WM_LBUTTONUP);
    return true;
}

bool was_up_menu_pressed( Spot init_clck ) {
    if ( isWhitinTheUpMenu( init_clck ) ){
        clearmouseclick(WM_LBUTTONDOWN);

        int ind_butt = (init_clck.xPos - menuPoster.width)/dim_of_butt[ 0 ].width;
        switch_color_horiz_butt( ind_butt );

         if ( last_prssed_horiz_butt_ind == 0 ) {
            if ( ind_butt != 0 )
                switch_color_horiz_butt( 0 );
            printf("2");
            setfillstyle( SOLID_FILL, BK_COLOR );
            bar( BUTT_PADDING, menuPoster.height, dim_of_butt[ 1 ].width + 1, getmaxy() );
        }

        switch ( ind_butt ) {
        case 0:
            if ( last_prssed_horiz_butt_ind != 0 )
            draw_butt_for_blks_ins();
            break;
        case 1: run();                                           break;
        case 2: exprt();                                       break;
        case 3: ask_for_file_name( file_name ); break;
        case 4: save();                                        break;
        case 5: load();                                        break;

        }

        if ( last_prssed_horiz_butt_ind == ind_butt)
            last_prssed_horiz_butt_ind = EMPTY;
        else
            last_prssed_horiz_butt_ind = ind_butt;
        return true;
    }
    return false;
}

bool was_left_menu_pressed( Spot init_clck ) {
    if ( horiz_butt[0].isPressed && isWhitinTheLeftMenu( init_clck ) ) {
        clearmouseclick(WM_LBUTTONDOWN);

        int ind_butt = ( init_clck.yPos - menuPoster.height )/dim_of_butt[1].height;
        switch_color_vert_butt(ind_butt);
        return true;
    }
    return false;
}

int get_head_of_new_cnnctn() {
    Spot clck;
    int ind_blk;
    instr blk;
    do {
        clearmouseclick(WM_LBUTTONDOWN);
        assign2Spot( clck, mousex(), mousey() );
        if ( ismouseclick(WM_LBUTTONDOWN) ) {
            ind_blk = get_pressed_blk_ind( clck );
            if ( ind_blk != -1 )
                return ind_blk;
            }
        } while( 1 );
}

bool is_within_blk_crcle( Spot clck, Spot cnnctn_crcle ) {
    int dist_x = Abs( cnnctn_crcle.xPos  - clck.xPos );
    int dist_y = Abs( cnnctn_crcle.yPos -  clck.yPos );
    if (dist_x <= radius_cnnctn/2 &&
        dist_y <= radius_cnnctn/2)
            return true;

    return false;
}

int get_head_of_cnnctn( instr blk, string brnch ) {
    if ( brnch == "T" )
        return blk.cnx.EndA;
    else
    if ( brnch == "F" )
        return blk.cnx.EndF;
}

void wait_till_rgt_clck_is_released() {
       while( ismouseclick( WM_RBUTTONUP ) == false )
            clearmouseclick(WM_RBUTTONDOWN);
    clearmouseclick(WM_RBUTTONUP);
}

void empty_cnnctn_brnch( instr* blk, string brnch ) {
    if ( brnch == "T" )
        (*blk).cnx.EndA = EMPTY;
    else
    if ( brnch == "F" )
        (*blk).cnx.EndF = EMPTY;
}

bool was_cnnctn_crcle_pressed( Spot init_clck, bool mode ) {
    if ( instrs.len == 0 ) return false;

    int     ind,
         dist_x,
         dist_y;
    string brnch;
    Spot cnnctn_crcle;
    instr* blk;
    for ( ind = 0; ind < instrs.len; ind++ ) {
        blk = &(instrs.inst_vec[ ind ]);

        cnnctn_crcle = (*blk).cnnctn_crcle1;
        if ( is_within_blk_crcle( init_clck, cnnctn_crcle ) ) {
            brnch = "T";
            break;
        }
        cnnctn_crcle = (*blk).cnnctn_crcle2;
        if  ( is_within_blk_crcle( init_clck, cnnctn_crcle ) ) {
            brnch = "F";
            break;
        }
    }
    if ( ind == instrs.len )    return false;

    if ( mode == DELETE ) {
        int color, head_ind;
        color = DEL_BLK_COL;
        head_ind = get_head_of_cnnctn( (*blk), brnch );
        draw_cnnctn_crcle( (*blk), brnch,  color );
        draw_cnnctn( (*blk), brnch,color );

        wait_till_rgt_clck_is_released();

        draw_cnnctn( (*blk), brnch, BK_COLOR );
        draw_blk( (*blk), BLK_COLOR );
        draw_cnnctn_crcle( (*blk), brnch,  BLK_COLOR );
        empty_cnnctn_brnch(  blk , brnch );
        redraw_all_cnnctns( BLK_COLOR );
        return true;
    }

    draw_cnnctn_crcle( (*blk), brnch,  BLK_MOD_COL );


    int tail_ind = ind;
    instr *tail_blk = &(instrs.inst_vec[ tail_ind ]);

    int head = get_head_of_new_cnnctn();
    if ( brnch == "T" ) {
        /*hide the old cnnctn*/
        draw_cnnctn( *tail_blk, brnch, BK_COLOR );

        (*tail_blk).cnx.EndA = head;
        draw_cnnctn_crcle( *tail_blk, brnch, BLK_COLOR );
    }
    else
    if ( brnch == "F" ) {
        /*hide the old cnnctn*/
        draw_cnnctn( *tail_blk, brnch, BK_COLOR );

        (*tail_blk).cnx.EndF = head;
        draw_cnnctn_crcle( *tail_blk, brnch, BLK_COLOR );
    }
    draw_cnnctn( *tail_blk, brnch, BLK_COLOR);
    return true;
}

void del_pressed_blk( Spot clck ) {
    int ind_butt, color;

    ind_butt = get_pressed_blk_ind( clck );
    if ( ind_butt == EMPTY )
        return;

    instr* blk = &(instrs.inst_vec[ ind_butt ]);

    if ( (*blk).type == "start" )
        start_exists = false;

     if ( (*blk).type == "stop" )
        start_exists = false;

    color = DEL_BLK_COL;
    draw_blk( (*blk), color );
    draw_incident_cnnctns_of_blk( ind_butt, color );

    wait_till_rgt_clck_is_released();

    color = BK_COLOR;
    draw_blk( (*blk), color );
    draw_incident_cnnctns_of_blk( ind_butt, color );

    del_instr( ind_butt );

    color = BLK_COLOR;
    redraw_all_Blks( color );
    redraw_all_cnnctns( color );
}

void change_pressed_blk_text() {
    int blk_ind;
    string type;
    instr* blk, twin;
    Spot clck;
    assign2Spot( clck,  mousex(), mousey() );
    blk_ind = get_pressed_blk_ind( clck );
    if ( blk_ind == EMPTY )
        return;
    type = instrs.inst_vec[ blk_ind ].type;
    if ( type == "start" || type == "stop"  )
        return;
    blk = &(instrs.inst_vec[ blk_ind ] );
    twin = *blk;
    char *txt = &( (*blk).container[0] );

    redraw_all_cnnctns( BK_COLOR );
    draw_blk( *( blk ), BLK_MOD_COL );
    ask_for_blk_txt( *(blk), BLK_MOD_MODE );
    draw_blk( twin, BK_COLOR );
    draw_blk( *( blk ), BLK_COLOR );
    redraw_all_cnnctns( BLK_COLOR );
}

void testClck() {
    Spot init_clck;
    if ( ismouseclick(WM_LBUTTONDOWN) ) {
        clearmouseclick(WM_LBUTTONDOWN);
        clearmouseclick(WM_LBUTTONUP);

        assign2Spot( init_clck,  mousex(), mousey() );

        if ( was_cnnctn_crcle_pressed( init_clck, DRAW )     == true);
        else if ( was_a_blk_pressed( init_clck )                      == true);
        else if ( was_up_menu_pressed( init_clck )                == true);
        else if ( was_left_menu_pressed( init_clck )               == true);
    }
    if( ismouseclick( WM_RBUTTONDOWN ) ) {
        clearmouseclick(WM_RBUTTONDOWN);
        clearmouseclick(WM_RBUTTONUP);
        assign2Spot( init_clck,  mousex(), mousey() );

        if ( was_cnnctn_crcle_pressed( init_clck, DELETE ) == true );
        else
            del_pressed_blk( init_clck );
    }

    if( ismouseclick( WM_LBUTTONDBLCLK ) ) {
        clearmouseclick( WM_LBUTTONDBLCLK );
        change_pressed_blk_text();
    }
}

void wait_click() {
    char pressedKey = 0;
    while ( pressedKey != ESC_KEY ){
        testClck();

        if( kbhit() )
            pressedKey = getch();
    }
}
