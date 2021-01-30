#include <graphics.h>
#include <winbgim.h>
#include <instr.h>


int getMid( int a, int b ) { return (a + b)/2; }

bool rgt_sgmnts_collision (int lft, int rgt, int lft1, int rgt1 ) {
    if ( ( lft1 <= rgt && rgt <= rgt1 ) ||
         ( lft1  <= lft &&  lft <= rgt1 ) )
        return true;

    return false;
}

bool dwn_sgmnts_collision( int up, int dwn, int up1, int dwn1 ) {
    if ( ( up1 <= dwn && dwn <= dwn1 ) ||
         ( up1 <= up    &&   up <= dwn1 ) ) {
        return true;
         }
    return false;
}

bool blk_collision( instr blk, instr blk_1 ) {
    int offset = BLK_TRTRY + 1;
    int lft    = blk.up_lft.xPos       - offset,
         rgt   = blk.dwn_rgt.xPos  +offset,
         up   = blk.up_lft.yPos      - offset,
         dwn = blk.dwn_rgt.yPos + offset;

    int lft1    = blk_1.up_lft.xPos       - offset,
         rgt1   = blk_1.dwn_rgt.xPos  +offset,
         up1   = blk_1.up_lft.yPos      - offset,
         dwn1 = blk_1.dwn_rgt.yPos  + offset;

    /* if we have an intersection both on Ox and Oy axis,
        then we have a block collision
    */
    if ( (rgt_sgmnts_collision(    lft,    rgt, lft1, rgt1 )   ||
          rgt_sgmnts_collision(   lft1, rgt1,   lft,   rgt ) ) &&
        (dwn_sgmnts_collision(   up,    dwn, up1, dwn1 )  ||
         dwn_sgmnts_collision( up1,   dwn1,  up,   dwn ) ) )
            return true;
}

void addInstrInd(int ind){
    string type;

    switch( ind ) {
        case 0: type = "start";             break;
        case 1: type = "read";             break;
        case 2: type = "if_statement";  break;
        case 3: type = "expression";    break;
        case 4: type = "write";             break;
        case 5: type = "stop";              break;
    }
    addinstr(type, instrs);
}

void assign2Spot( Spot& instance, int xPos, int yPos ){
    instance.xPos = xPos;
    instance.yPos = yPos;
}

void draw_line_btw_spots( Spot tail, Spot head ) {
     line( tail.xPos, tail.yPos, head.xPos, head.yPos );
}

void draw_curved_line( Spot tail, Spot head ) {
    int radius = CRVD_RCTGLE_RAD;
    if ( tail.xPos  == head.xPos ) {
        tail.yPos    += radius;
        head.yPos -=  radius;
    }
    else
    if ( tail.yPos == head.yPos ) {
        tail.xPos    += radius;
        head.xPos -= radius;
    }
   draw_line_btw_spots( tail, head );
}

void pos_blk_cnnctn_circle( instr& blk ) {
    if ( blk.type == "if_statement") {
            /*left*/
            blk.cnnctn_crcle1.xPos = blk.up_lft.xPos + radius_cnnctn/2;
            blk.cnnctn_crcle1.yPos = ( blk.up_lft.yPos + blk.dwn_rgt.yPos )/2 - radius_cnnctn/2;
            /*right*/
            blk.cnnctn_crcle2.xPos = blk.dwn_rgt.xPos - radius_cnnctn/2;
            blk.cnnctn_crcle2.yPos = blk.cnnctn_crcle1.yPos;
    }
    else
    if ( blk.type == "stop")
        assign2Spot( blk.cnnctn_crcle1, EMPTY, EMPTY );
    else {
        blk.cnnctn_crcle1.xPos = ( blk.up_lft.xPos   + blk.dwn_rgt.xPos )/2;
        blk.cnnctn_crcle1.yPos = blk.dwn_rgt.yPos - radius_cnnctn/2;
    }
}

void draw_cnnctn_crcle( instr blk, string brnch, int color ) {
    Spot cnnct_crcle;
    if ( brnch == "T" )
        cnnct_crcle = blk.cnnctn_crcle1;
    else
    if ( brnch == "F")
            cnnct_crcle = blk.cnnctn_crcle2;

    setcolor( color );
    if ( blk.type == "if_statement" ) {
        settextjustify( CENTER_TEXT,BOTTOM_TEXT );
        setbkcolor( BK_COLOR );
        outtextxy( cnnct_crcle.xPos, cnnct_crcle.yPos - radius_cnnctn/2, &brnch[0] );
        setbkcolor(BUTTON_COLOR);
        settextjustify( LEFT_TEXT     ,TOP_TEXT );
    }
    circle( cnnct_crcle.xPos, cnnct_crcle.yPos, radius_cnnctn );
}

void draw_crvd_rctngle( Spot up_lft, Spot dwn_rgt, int color ) {
    int radius = CRVD_RCTGLE_RAD;

    Spot up_rgt, dwn_lft;
    assign2Spot( up_rgt , dwn_rgt.xPos , up_lft.yPos    );
    assign2Spot( dwn_lft, up_lft.xPos     , dwn_rgt.yPos);

    setcolor( color );
    draw_curved_line( up_lft  , up_rgt    );
    draw_curved_line( dwn_lft, dwn_rgt );
    draw_curved_line( up_lft  , dwn_lft   );
    draw_curved_line( up_rgt, dwn_rgt  );

    arc(up_lft.xPos     + radius, up_lft.yPos    + radius, 90 , 180,  radius );
    arc(dwn_lft.xPos  + radius, dwn_lft.yPos  - radius, 180, 270,  radius );
    arc(up_rgt.xPos    - radius, up_rgt.yPos   + radius, 0   ,  90 ,   radius );
    arc(dwn_rgt.xPos - radius, dwn_rgt.yPos - radius, 270, 360,   radius );
}

void draw_blk_START_STOP( instr blk, int color ) {
    draw_crvd_rctngle( blk.up_lft, blk.dwn_rgt, color );

    if ( blk.type == "start" )
        draw_cnnctn_crcle( blk, "T", color );
}

void draw_blk_IN_OUT( instr blk, int color ) {
    Spot up_lft, up_rgt, dwn_lft, dwn_rgt;

    up_lft = blk.up_lft;
    dwn_rgt = blk.dwn_rgt;

    int diff_btw_bases = (dwn_rgt.xPos - up_lft.xPos)/3;

    assign2Spot( dwn_lft, up_lft.xPos   , dwn_rgt.yPos );
    assign2Spot( up_rgt , dwn_rgt.xPos, up_lft.yPos    );

    if ( blk.type == "read" ) {
        up_lft.xPos  += diff_btw_bases;
        up_rgt.xPos -= diff_btw_bases;
    }
    else
    if ( blk.type == "write" )
    {
        dwn_lft.xPos  += diff_btw_bases;
        dwn_rgt.xPos -= diff_btw_bases;
    }

    setcolor( color );
    draw_line_btw_spots( up_lft   , up_rgt    );
    draw_line_btw_spots( up_lft   , dwn_lft   );
    draw_line_btw_spots( dwn_lft, dwn_rgt  );
    draw_line_btw_spots( up_rgt , dwn_rgt  ) ;

    draw_cnnctn_crcle( blk, "T", color );
}

void draw_blk_assign( instr blk, int color ) {
    setcolor( color );
    rectangle( blk.up_lft.xPos, blk.up_lft.yPos, blk.dwn_rgt.xPos, blk.dwn_rgt.yPos );

    Spot mid_dwn = blk.dwn_rgt;
    mid_dwn.xPos = (blk.up_lft.xPos + blk.dwn_rgt.xPos) / 2;

    draw_cnnctn_crcle( blk, "T", color );
}

void draw_blk_IF( instr blk, int color) {
     Spot mid_up = blk.up_lft;
     mid_up.xPos = ( mid_up.xPos + blk.dwn_rgt.xPos ) / 2;

     Spot mid_dwn = blk.dwn_rgt;
     mid_dwn.xPos = mid_up.xPos;

     Spot mid_lft = blk.up_lft;
     mid_lft.xPos += radius_cnnctn;
     mid_lft.yPos = ( mid_lft.yPos + blk.dwn_rgt.yPos ) / 2;

     Spot mid_rgt = blk.dwn_rgt;
     mid_rgt.xPos -= radius_cnnctn;
     mid_rgt.yPos = mid_lft.yPos;

     setcolor( color );

     draw_line_btw_spots( mid_up  , mid_lft    );
     draw_line_btw_spots( mid_lft   , mid_dwn );
     draw_line_btw_spots( mid_dwn, mid_rgt  );
     draw_line_btw_spots( mid_rgt  , mid_up  );

     draw_cnnctn_crcle( blk, "T", color );
     draw_cnnctn_crcle( blk, "F", color );
}

void print_txt_mid_blk( instr blk, char* txt, int color ) {
    Spot mid_blk;
    mid_blk.xPos = (blk.up_lft.xPos + blk.dwn_rgt.xPos)/2;
    mid_blk.yPos = (blk.up_lft.yPos + blk.dwn_rgt.yPos)/2;

    setbkcolor(BK_COLOR);
    settextjustify( CENTER_TEXT,CENTER_TEXT );
    setcolor(color);

    outtextxy( mid_blk.xPos, mid_blk.yPos, txt );

    settextjustify( LEFT_TEXT,TOP_TEXT );
    setbkcolor(BUTTON_COLOR);
}

void ID_cnnctn_Spots( Spot& tail_pk, Spot& head_pk,  instr tail_blk, string brnch ) {
    cnnctn curr_cnx = tail_blk.cnx;

    instr head_blk;
    if ( brnch == "T" ) {
        tail_pk = tail_blk.cnnctn_crcle1;
        head_blk = instrs.inst_vec[ curr_cnx.EndA ];
    }
    if ( brnch == "F" ) {
        tail_pk = tail_blk.cnnctn_crcle2;
        head_blk = instrs.inst_vec[ curr_cnx.EndF ];
    }
    head_pk.xPos = ( head_blk.up_lft.xPos  + head_blk.dwn_rgt.xPos )/2;
    head_pk.yPos = head_blk.up_lft.yPos;
}

bool isWhitinTheBlock( Spot currentSpot, instr curr_blk ) {
    int lft_brdr    = curr_blk.up_lft.xPos,
         rgt_brdr   = curr_blk.dwn_rgt.xPos,
         up_brdr   = curr_blk.up_lft.yPos,
         dwn_brdr = curr_blk.dwn_rgt.yPos;

    return  lft_brdr      <= currentSpot.xPos &&
               rgt_brdr     >= currentSpot.xPos &&
               up_brdr     <= currentSpot.yPos &&
                dwn_brdr  >= currentSpot.yPos;
}

void expand_trtry( instr& blk ) {
    blk.up_lft.xPos -= BLK_TRTRY;
    blk.up_lft.yPos -= BLK_TRTRY;
    blk.dwn_rgt.xPos += BLK_TRTRY;
    blk.dwn_rgt.yPos += BLK_TRTRY;
}

bool isBtw( int val, int lft, int rgt ) {   return lft <= val && val <= rgt; }

bool blk_and_line_collision_Ox( Spot tail, Spot head, instr blk, int dir_x ) {
    int y      =  tail.yPos,
        lft      = tail.xPos,
        rgt    = head.xPos;
    if ( dir_x == LFT )
        Swap( lft, rgt );
    int lft1    = blk.up_lft.xPos,
        rgt1   = blk.dwn_rgt.xPos,
        up1   = blk.up_lft.yPos,
        dwn1 = blk.dwn_rgt.yPos;

    if ( isBtw( y, up1, dwn1 ) &&
        ( isBtw(lft1, lft, rgt) || isBtw(rgt1, lft, rgt) ) )
            return true;

    return false;
}

bool blk_and_line_collision_Oy( Spot tail, Spot head, instr blk, int dir_y ) {
    int x      =  tail.xPos,
        up     = tail.yPos,
        dwn = head.yPos;
    if ( dir_y == UP )
        Swap( up, dwn );
    int lft1    = blk.up_lft.xPos,
        rgt1   = blk.dwn_rgt.xPos,
        up1   = blk.up_lft.yPos,
        dwn1 = blk.dwn_rgt.yPos;

    if ( isBtw( x, lft1, rgt1 ) &&
        ( isBtw(up1, up, dwn) || isBtw(dwn1, up, dwn) ) )
            return true;

    return false;
}

void Swap_sgmnts(sgmnt& a, sgmnt& b) {
    sgmnt aux = a;
    a = b;
    b = aux;
}

bool cmp( sgmnt a, sgmnt b, int order ) {
    if ( order == DECREASING )
        return a.end >= b.end;
    else
    if ( order == INCREASING )
        return a.end <= b.end;
}


void percolate( sgmnt* v, int n, int pos, int order ) {
    int k;
    bool isHeap = false;
    k = 2*pos + 1;
    while ( k < n && !isHeap ) {
        if ( k+1 < n && cmp( v[ k ], v[ k + 1], order ) )
            k++;
        if ( cmp( v[ pos ], v[ k ], order ) ) {
            Swap_sgmnts( v[pos], v[ k ] );
            pos = k;
        }
        else
            isHeap = true;
        k = 2*pos + 1;
    }
}

void heapSort (int n, sgmnt* v, int order ) {
    int ind;
    for ( ind = (n - 1)/2; ind >= 0; ind-- )
        percolate( v, n, ind, order );

    for ( ind = n - 1; ind > 0; ind-- ) {
        Swap_sgmnts( v[0], v[ ind ] );
        percolate( v, ind, 0, order );
    }
}

/* Draw only the part that normally would go
    through the block, but this time does not do that
*/
void draw_line_beside_blk_Oy( int x_tail, sgmnt subline ) {
    Spot joint, joint1;
    joint.yPos = joint1.yPos = subline.start;
    joint.xPos = x_tail;
    joint1.xPos = subline.pos_on_the_other_axis;
    draw_line_btw_spots( joint, joint1 );

    joint = joint1;
    joint1.yPos = subline.end;
    draw_line_btw_spots( joint, joint1 );

    joint = joint1;
    joint1.xPos = x_tail;
    draw_line_btw_spots( joint, joint1 );
}

/*
    Draw the line beside the current block plus the one that
    makes connect the current block with the next one, if it exists
*/
void draw_subline_Oy( int x_tail, sgmnt subline_1, sgmnt subline_2 ) {
    Spot joint, joint1;
    draw_line_beside_blk_Oy( x_tail, subline_1 );

    joint.xPos = joint1.xPos = x_tail;
    joint.yPos = subline_1.end;
    joint1.yPos = subline_2.start;
    draw_line_btw_spots( joint, joint1);
}

void draw_line_beside_blk_Ox( int y_tail, sgmnt subline ) {
    Spot joint, joint1;
    joint.xPos = joint1.xPos = subline.start;
    joint.yPos = y_tail;
    joint1.yPos = subline.pos_on_the_other_axis;
    draw_line_btw_spots( joint, joint1 );

    joint = joint1;
    joint1.xPos = subline.end;
    draw_line_btw_spots( joint, joint1 );

    joint = joint1;
    joint1.yPos = y_tail;
    draw_line_btw_spots( joint, joint1 );
}

void draw_subline_Ox( int y_tail, sgmnt subline_1, sgmnt subline_2 ) {
    Spot joint, joint1;
    draw_line_beside_blk_Ox( y_tail, subline_1 );

    joint.yPos = joint1.yPos = y_tail;
    joint.xPos = subline_1.end;
    joint1.xPos = subline_2.start;
    draw_line_btw_spots( joint, joint1);
}

void draw_line_beside_blks( Spot tail, Spot& head, int dir_x, int dir_y ) {
    /* I iterate through all the block and if there is a collision, I store the spots
        of 2 corners from that block, which would help me draw the next lines,

    */
    int ind;
    int add = 1;
    instr curr_blk;
    bool tailRemoved = false;
    bool headRemoved = false;
    /*Oy axis*/
    if ( tail.xPos == head.xPos ) {
        if ( tail.yPos == head.yPos )
            return;
        sgmnt Oy_sublines[ MAX ];
        int size = 0;

        for ( ind = 0; ind < instrs.len; ind++ ) {
            curr_blk = instrs.inst_vec[ ind ];
            expand_trtry( curr_blk );

            if ( blk_and_line_collision_Oy( tail, head, curr_blk, dir_y ) ) {
                /*We store the where this line is on the other axis*/
                if ( dir_x == RGT )
                    Oy_sublines[ size ].pos_on_the_other_axis = curr_blk.dwn_rgt.xPos + add;
                else
                if ( dir_x == LFT )
                    Oy_sublines[ size ].pos_on_the_other_axis = curr_blk.up_lft.xPos - add;


                 /*We store the spots of 2 corners from that block, which would help drawing the next lines*/
                if ( dir_y == DWN ) {
                     /*take the max*/
                    if ( curr_blk.up_lft.yPos < tail.yPos)
                        Oy_sublines[ size ].start = tail.yPos, tailRemoved = true;
                    else
                        Oy_sublines[ size ].start = curr_blk.up_lft.yPos;
                     /*take the min*/
                    if ( curr_blk.dwn_rgt.yPos > head.yPos )
                        Oy_sublines[ size ].end = head.yPos, headRemoved = true;
                    else
                        Oy_sublines[ size ].end = curr_blk.dwn_rgt.yPos;
                }
                else
                if ( dir_y == UP ) {
                    /*take the min*/
                    if ( curr_blk.dwn_rgt.yPos > tail.yPos )
                        Oy_sublines[ size ].start  = tail.yPos, tailRemoved = true;
                    else
                        Oy_sublines[ size ].start  = curr_blk.dwn_rgt.yPos;
                     /*take the max*/
                    if ( curr_blk.up_lft.yPos < head.yPos) {
                        Oy_sublines[ size ].end = head.yPos, headRemoved = true;
                    }
                    else {
                        Oy_sublines[ size ].end = curr_blk.up_lft.yPos;
                    }
                }
                size++;
            }
        }
        /*start drawing*/
         if ( size == 0 ) {
            draw_line_btw_spots( tail, head );
            return;
         }

        if ( dir_y == DWN )
            heapSort( size, Oy_sublines, INCREASING );
        else
        if ( dir_y == UP )
            heapSort( size, Oy_sublines, DECREASING );

        /*start drawing*/
        Spot joint, joint1;
        /* check if the tail spot was shifted because it was intersecting
            with a block
        */
        if ( tailRemoved == false ) {
            joint.xPos = tail.xPos;
            joint.yPos = Oy_sublines[ 0 ].start;
            draw_line_btw_spots( tail, joint );

            if ( size > 1)
                draw_subline_Oy( tail.xPos, Oy_sublines[ 0 ], Oy_sublines[ 1 ]);
        }
        else
        {
            tail.xPos = joint.xPos = Oy_sublines[ 0 ].pos_on_the_other_axis;
            joint.yPos = Oy_sublines[ 0 ].end;
            draw_line_btw_spots( tail, joint );

            tail = joint;
            joint.xPos = head.xPos;
            draw_line_btw_spots( tail, joint );

            if ( size > 1) {
                tail = joint;
                joint.yPos = Oy_sublines[ 1 ].start;
                draw_line_btw_spots( tail, joint );
            }
        }
        int ind1;
        for ( ind1 = 1; ind1 < size - 1; ind1++ ) {
            draw_subline_Oy( tail.xPos, Oy_sublines[ ind ], Oy_sublines[ ind + 1] );
        }
        sgmnt last = Oy_sublines[ size - 1];
        if ( headRemoved == false ) {
            if ( Abs( last.end - tail.yPos) > 1 )
                draw_line_beside_blk_Oy( tail.xPos, last );

            joint.xPos = head.xPos;
            joint.yPos = last.end;
            draw_line_btw_spots( head, joint );
        }
        else
        {
            int x = head.xPos;
            head.xPos = last.pos_on_the_other_axis;
            joint1 = head;
            joint1.yPos = last.start;
            draw_line_btw_spots( head, joint1 );

            joint = joint1;
            joint.xPos = x;
            draw_line_btw_spots( joint1, joint );
        }


    }
    else
    /*Ox axis*/
    if ( tail.yPos == head.yPos ) {
        if ( tail.xPos == head.xPos )
            return;
        sgmnt Ox_sublines[ MAX ];
        int size = 0;

        for ( ind = 0; ind < instrs.len; ind++ ) {
            curr_blk = instrs.inst_vec[ ind ];
            expand_trtry( curr_blk );
            if ( blk_and_line_collision_Ox( tail, head, curr_blk, dir_x ) ) {
                if ( dir_y == DWN )
                        Ox_sublines[ size ].pos_on_the_other_axis = curr_blk.dwn_rgt.yPos + add;
                else
                if ( dir_y == UP )
                        Ox_sublines[ size ].pos_on_the_other_axis = curr_blk.up_lft.yPos - add;

                if ( dir_x == RGT ) {
                     /*take the max*/
                    if ( curr_blk.up_lft.xPos < tail.xPos )
                            Ox_sublines[ size ].start = tail.xPos, tailRemoved = true;
                    else
                            Ox_sublines[ size ].start = curr_blk.up_lft.xPos;
                     /*take the min*/
                    if ( curr_blk.dwn_rgt.xPos > head.xPos )
                        Ox_sublines[ size ].end = head.xPos, headRemoved = true;
                    else
                        Ox_sublines[ size ].end = curr_blk.dwn_rgt.xPos;
                }
                else
                if ( dir_x == LFT ) {
                     /*take the max*/
                    if ( curr_blk.up_lft.xPos < head.xPos )
                        Ox_sublines[ size ].end  = head.xPos, headRemoved = true;
                    else
                        Ox_sublines[ size ].end  = curr_blk.up_lft.xPos;
                     /*take the min*/
                    if ( curr_blk.dwn_rgt.xPos > tail.xPos )
                            Ox_sublines[ size ].start = tail.xPos, tailRemoved = true;
                    else
                            Ox_sublines[ size ].start = curr_blk.dwn_rgt.xPos;
                }
                size++;
            }
        }
        /*start to draw*/
        if ( size == 0 ) {
            draw_line_btw_spots( tail, head );
            return;
         }
        if ( dir_x == RGT )
            heapSort( size, Ox_sublines, INCREASING );
        else
        if ( dir_x == LFT )
            heapSort( size, Ox_sublines, DECREASING );
        Spot joint, joint1;
        if ( tailRemoved == false ) {
            joint.yPos = tail.yPos;
            joint.xPos = Ox_sublines[ 0 ].start;
            draw_line_btw_spots( tail, joint );

            if ( size > 1)
                draw_subline_Ox( tail.yPos, Ox_sublines[ 0 ], Ox_sublines[ 1 ]);
        }
        else
        {
            tail.yPos = joint.yPos = Ox_sublines[ 0 ].pos_on_the_other_axis;
            joint.xPos = Ox_sublines[ 0 ].end;
            draw_line_btw_spots( tail, joint );

            tail = joint;
            joint.yPos = head.yPos;
            draw_line_btw_spots( tail, joint );

            if ( size > 1) {
                tail = joint;
                joint.xPos = Ox_sublines[ 1 ].start;
                draw_line_btw_spots( tail, joint );
            }
        }
        int ind1;
        for ( ind1 = 1; ind1 < size - 1; ind1++ ) {
            draw_subline_Ox( tail.yPos, Ox_sublines[ ind1 ], Ox_sublines[ ind1 + 1] );
        }
        sgmnt last = Ox_sublines[ size - 1];
        if ( headRemoved == false  ) {
            if ( Abs( last.end - tail.yPos) > 1 )
            draw_line_beside_blk_Ox( tail.yPos, last );

            joint.yPos = head.yPos;
            joint.xPos = last.end;
            draw_line_btw_spots( head, joint );
        }
        else
        {
            int y = head.yPos;
            head.yPos = last.pos_on_the_other_axis;
            joint1.yPos =  head.yPos;
            joint1.xPos = last.start;
            draw_line_btw_spots( head, joint1 );

            joint = joint1;
            joint.yPos = y;
            draw_line_btw_spots( joint1, joint );
        }
    }

}

void get_directions( Spot tail, Spot head, int& dir_x, int& dir_y) {
    /*I don't need to change them*/
    if ( tail.xPos != head.xPos)
        dir_x = RGT;
    if ( tail.yPos != head.yPos )
        dir_y = DWN;

    if ( tail.xPos > head.xPos )
        dir_x = LFT;
    if ( tail.yPos > head.yPos )
        dir_y = UP;
}

void draw_line_btw_blks( Spot tail_pk, Spot head_pk, int dir_x, int dir_y, int color ) {
    Spot joint_1, joint_2;
    int midX = ( tail_pk.xPos + head_pk.xPos )/2;

    assign2Spot( joint_1, midX, tail_pk.yPos );
    assign2Spot( joint_2, midX, head_pk.yPos );

    get_directions( tail_pk, joint_1, dir_x, dir_y );
    setcolor( color );
    draw_line_beside_blks( tail_pk, joint_1, dir_x, dir_y );

    get_directions( joint_1, joint_2, dir_x, dir_y );
    setcolor( color );
    draw_line_beside_blks( joint_1, joint_2, dir_x, dir_y );

    get_directions( joint_2, head_pk, dir_x, dir_y );
    setcolor( color );
    draw_line_beside_blks( joint_2, head_pk, dir_x, dir_y );
}

void draw_arrow_btw_spots( Spot tail, Spot head ) {
    int lenght = (head.yPos - tail.yPos) / 2;
    draw_line_btw_spots( tail, head );
    Spot lft_pt = tail;
    lft_pt.xPos -= lenght;
    lft_pt.yPos += lenght;
    draw_line_btw_spots( head, lft_pt );

    Spot rgt_pt = tail;
    rgt_pt.xPos += lenght;
    rgt_pt.yPos += lenght;
    draw_line_btw_spots( head, rgt_pt );
}

void get_tail_to_corner(Spot& curr_pos, instr blk, bool dir_x) {
    expand_trtry( blk );
    Spot pos1 = curr_pos;
    pos1.yPos = blk.dwn_rgt.yPos + 1;
    draw_line_btw_spots( curr_pos, pos1 );
    curr_pos = pos1;

    int add = 0;
    if ( dir_x == LFT )
            pos1.xPos = blk.up_lft.xPos - add;
    else
    if ( dir_x == RGT )
          pos1.xPos = blk.dwn_rgt.xPos + add;

    draw_line_btw_spots( curr_pos, pos1 );
    curr_pos = pos1;
}

void get_head_to_corner(Spot& curr_pos, instr blk, bool dir_x) {
    expand_trtry( blk );
    Spot pos1 = curr_pos;
    pos1.yPos = blk.up_lft.yPos - 1;
    draw_line_btw_spots( curr_pos, pos1 );
    curr_pos = pos1;

    int add = 0;
    if ( dir_x == LFT )
            pos1.xPos = blk.up_lft.xPos - add;
    else
    if ( dir_x == RGT )
          pos1.xPos = blk.dwn_rgt.xPos + add;

    draw_line_btw_spots( curr_pos, pos1 );
    curr_pos = pos1;
}

void draw_cnnctn( instr tail_blk, string brnch, int color ) {
    /* test if the cnx doesn't exist */
    if ( brnch == "T" && tail_blk.cnx.EndA == EMPTY ||
         brnch == "F" && tail_blk.cnx.EndF == EMPTY)
            return;
    Spot tail_pk, head_pk;
    ID_cnnctn_Spots( tail_pk, head_pk , tail_blk, brnch );

    Spot tail_pk_out_of_blk       = tail_pk;
    Spot head_pk_out_of_blk    = head_pk;
    tail_pk_out_of_blk.yPos     += BLK_TRTRY;
    head_pk_out_of_blk.yPos   -= BLK_TRTRY;

    int head_ind;
    if ( brnch == "T" )
        head_ind = tail_blk.cnx.EndA;
    else
        head_ind = tail_blk.cnx.EndF;
    instr head_blk = instrs.inst_vec[ head_ind ];

    int dir_x_tail = RGT;
    if ( tail_pk.xPos > head_pk.xPos )
        dir_x_tail = LFT;

    int dir_x_head = !dir_x_tail;
    bool collision = false;
    /*tackle the situation when the blks are parallel with  respect of Ox,
      thus they should have the same direction */
    int offset = BLK_TRTRY;
    int x_lft   = tail_blk.up_lft.xPos           - offset,
        x_rgt   = tail_blk.dwn_rgt.xPos      + offset,
        x1_lft  = head_blk.up_lft.xPos        - offset ,
        x1_rgt = head_blk.dwn_rgt.xPos   + offset ;
    if ( rgt_sgmnts_collision( x_lft, x_rgt, x1_lft, x1_rgt ) ||
         rgt_sgmnts_collision( x1_lft, x1_rgt, x_lft, x_rgt ) ) {
        dir_x_head = dir_x_tail;
        collision = true;
    }

    int dir_y = DWN;
    if ( tail_pk.yPos > head_pk.yPos)
        dir_y = UP;

    setcolor( color );
    draw_line_btw_spots( tail_pk, tail_pk_out_of_blk );

    get_tail_to_corner( tail_pk_out_of_blk, tail_blk, dir_x_tail);

    draw_arrow_btw_spots( head_pk_out_of_blk, head_pk );

    if ( collision) {
        head_blk.up_lft.xPos = tail_blk.up_lft.xPos;
        head_blk.dwn_rgt.xPos = tail_blk.dwn_rgt.xPos;
    }
    get_head_to_corner( head_pk_out_of_blk, head_blk, dir_x_head );

    draw_line_btw_blks( tail_pk_out_of_blk, head_pk_out_of_blk, dir_x_tail, dir_y, color );
}

void draw_both_cnnctns( instr blk, int color ) {
    draw_cnnctn( blk, "T", color );
    draw_cnnctn( blk, "F", color );
}

void draw_incident_cnnctns_of_blk( int blk_ind, int color ) {
    instr curr_blk = instrs.inst_vec[ blk_ind ];
    /*draw the cnnctns that start from it*/
    draw_both_cnnctns( curr_blk, color );
    /*draw the cnnctns that end  in     it*/
    int ind;
    for ( ind = 0; ind < instrs.len; ++ind ) {
        if ( ind == blk_ind ) continue;

        curr_blk = instrs.inst_vec[ ind ];
        if ( curr_blk.cnx.EndA == blk_ind )
            draw_cnnctn( curr_blk, "T", color );
        if ( curr_blk.cnx.EndF == blk_ind )
            draw_cnnctn( curr_blk, "F", color );
    }
}

void draw_cnncts_except_from_a_blk( int blk_ind, int color ) {
    int ind;
    instr curr_blk;
    int head;
    for ( ind = 0; ind < instrs.len; ind++ ) {
        if ( blk_ind == ind )
            continue;
        curr_blk = instrs.inst_vec[ ind ];

        head = curr_blk.cnx.EndA;
        if (  head != blk_ind && head != EMPTY )
            draw_cnnctn( curr_blk, "T", color );

        head = curr_blk.cnx.EndF;
        if ( head != blk_ind && head != EMPTY )
            draw_cnnctn( curr_blk, "F", color );
    }
}

void draw_blk( instr& blk, int color ) {
    pos_blk_cnnctn_circle( blk );

    char* print_txt;
     if ( blk.type == "start" || blk.type == "stop" ) {
         print_txt = &blk.type[0];
         print_txt_mid_blk( blk, print_txt, color );

        draw_blk_START_STOP( blk, color );
     }
    else {
        print_txt = &blk.container[ 0 ];
        print_txt_mid_blk( blk, print_txt, color );
        if ( blk.type == "read" || blk.type == "write" )
                draw_blk_IN_OUT( blk, color) ;
        else
        if ( blk.type == "if_statement" ) {
                draw_blk_IF( blk, color );
        }
        else
        if ( blk.type == "expression" )
                draw_blk_assign( blk, color );
    }
}

void hide_blk( int blk_ind ) {
    int color = BK_COLOR;
    instr curr_blk = instrs.inst_vec[ blk_ind ];
    draw_blk( curr_blk, color );
}

void redraw_all_Blks( int color ) {
    for ( int i = 0; i < instrs.len; i++ )
        draw_blk( instrs.inst_vec[ i ], color );
}

void redraw_all_cnnctns( int color ) {
    int ind;
    for ( ind = 0; ind < instrs.len; ++ind )
        draw_both_cnnctns( instrs.inst_vec[ ind ], color );
}
