#include <settings.h>
#include <iostream>
using namespace std;


void Swap( int& a, int& b )  {
    a = a + b;
    b = a - b;
    a = a - b;
}

int Abs(int val) { return val>0 ?val :-val; }

int Max(int a, int b) {     return a>b ?a :b;   }


cnnctn emptyCnnctn(){
    cnnctn newCnnctn;
    newCnnctn.EndF = newCnnctn.EndA = EMPTY;
    return newCnnctn;
}

cnnctns_vec emptyConVec(){
    cnnctns_vec newCnnctns;
    newCnnctns.len = 0;
    for(int i = 0; i < MAXCON; i++)
        newCnnctns.connex_vec[i] = emptyCnnctn();
    return newCnnctns;
}

cnnctns_vec set_connex = emptyConVec();

void show_vec(instrs_vec instrs){
    cout<<instrs.len<<endl;
    for(int i = 0; i < instrs.len; i++){
        instr inst = instrs.inst_vec[i];
        cout<<inst.type<<" "<<inst.container<<endl;
    }
}

instr emptyinstr(){
    instr newinstr;
    newinstr.cnnctn_crcle1= {0, 0};
    newinstr.cnnctn_crcle2 = {0, 0};
    newinstr.type = "NoType";
    newinstr.container = "Empty";
    newinstr.dwn_rgt.xPos = SCREEN_WIDTH - 2*BLK_TRTRY;
    newinstr.dwn_rgt.yPos = SCREEN_HEIGHT - 2*BLK_TRTRY;
    newinstr.up_lft.xPos = newinstr.dwn_rgt.xPos - STOP_OR_START_BLK_WIDTH;
    newinstr.up_lft.yPos = newinstr.dwn_rgt.yPos - STOP_OR_START_BLK_HEIGHT;
    newinstr.cnx = emptyCnnctn();
    return newinstr;
}

instrs_vec emptyVec(){
    instrs_vec newinstrs;
    newinstrs.len = 0;
    for(int i=0; i < MAX; i++)
        newinstrs.inst_vec[i] = emptyinstr();
    return newinstrs;
}
instrs_vec instrs = emptyVec();

void mod_cnnctn_ind_head( int& head, int ind_del ) {
    if ( head == ind_del )
        head = EMPTY;
    else
    if ( head > ind_del )
        head--;
}

void del_cnx_that_end_in( int ind_strt, int ind_end ) {
    instr* blk = &( instrs.inst_vec[ ind_strt ] );
    mod_cnnctn_ind_head( (*blk).cnx.EndA, ind_end );
    mod_cnnctn_ind_head( (*blk).cnx.EndF, ind_end );
}

void del_instr(int index){
    int i;
    for( i = 0; i < instrs.len-1; i++ ) {
        if ( i >= index )
            instrs.inst_vec[ i ] = instrs.inst_vec[ i+1 ];
        del_cnx_that_end_in( i, index );
    }
    instrs.len--;
}

void addinstr(string type, instrs_vec &instrs){
    instr new_inst = emptyinstr();
    new_inst.type = type;
    instrs.inst_vec[instrs.len] = new_inst;
    instrs.len++;
}

reg emptyreg(){
    reg newreg;
    newreg.var = "NoVar";
    newreg.val = NULL;
    return newreg;
}

regs_vec emptyRegsVec(){
    regs_vec newregs;
    newregs.len = 0;
    for(int i = 0; i < MAXREG; i++)
        newregs.reg_vec[i] = emptyreg();
    return newregs;
}
regs_vec regs = emptyRegsVec();

void addreg(string var, float val){
    regs.reg_vec[regs.len].var = var;
    regs.reg_vec[regs.len].val = val;
    regs.len++;
}

bool checkreg(string var){
    for(int i = 0; i < regs.len; i++)
        if( var == regs.reg_vec[i].var )
            return true;
    return false;
}
