#include <build_menu.h>


ofstream out;
ifstream in;


void writeInst(instr inst){

    out<<"{\n"<<inst.type<<'\n'<<inst.container<<'\n';

    out<<inst.cnx.EndA<<' '<<inst.cnx.EndF<<'\n';

    out<<inst.up_lft.xPos<<' '<<inst.up_lft.yPos<<'\n';

    out<<inst.dwn_rgt.xPos<<' '<<inst.dwn_rgt.yPos<<'\n';

    out<<"}\n";
}

void saveInstVec(){

    out<<instrs.len<<'\n';

    for(int i = 0; i < instrs.len; i++){
        instr inst;
        inst = instrs.inst_vec[i];
        writeInst(inst);
    }
}

void loadInst(instr &inst){

    in.get(); in.get();

    in>>inst.type;
    if ( inst.type == "start" )
        start_exists = true;
    if ( inst.type == "stop" )
        stop_exists = true;

    in.get();

    char container[255];
    in.getline(container, 255);
    inst.container = container;

    in>>inst.cnx.EndA>>inst.cnx.EndF;
    in.get();

    in>>inst.up_lft.xPos>>inst.up_lft.yPos;
    in.get();

    in>>inst.dwn_rgt.xPos>>inst.dwn_rgt.yPos;
    in.get(); in.get();
}

void loadInstVec(){

    in>>instrs.len;

    for(int i = 0; i < instrs.len; i++){
        instr inst;
        loadInst(inst);
        instrs.inst_vec[i] = inst;
    }

}

void save(){
    //saveConnectionsVec();

    out.open(file_name, std::ofstream::trunc);

    saveInstVec();

    out.close();

}

void load(){
    //loadConnectionsVec();

    redraw_all_Blks(BK_COLOR);

    redraw_all_cnnctns(BK_COLOR);

    instrs = emptyVec();
    start_exists = false;
    stop_exists = false;

    /*changed*/

    in.open(file_name);

    loadInstVec();

    in.close();

    redraw_all_Blks(BLK_COLOR);

    redraw_all_cnnctns(BLK_COLOR);

}
