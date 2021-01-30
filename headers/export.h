#include<cdc.h>

string removeSpaces(string str)
{
    string output = "";
    for(int i = 0; i < str.length(); i++)
        if(str[i] != ' ')
            output += str[i];
    return output;
}

vector<string> parse_read(string container)
{
    vector<string> output;
    for(int i = 0; i < container.length(); i++)
    {
        char c = container[i];
        string obj = "";
        do
        {
            if(strchr(" ,", c) == NULL)
                obj += c;
            i++;
            c = container[i];
        }
        while(strchr(" ,", c) == NULL);
        if(!obj.empty())
        {
            output.push_back(obj);
            obj = "";
        }
    }
    for(int i = 0; i < output.size(); i++)
        cout<<output[i]<<endl;
    return output;
}

vector<string> parse_write(string container)
{
    vector<string> output;
    for(int i = 0; i < container.length(); i++)
    {
        char c = container[i];
        string obj = "";
        int index = 0;
        // "
        if(container[i] == '"')
        {
            do
            {
                obj += c;
                i++;
                c = container[i];
            }
            while(container[i]!='"');
            obj+=container[i];
        }
        else
            do
            {
                if(strchr(" ,", c) == NULL)
                {
                    obj += c;
                    i++;
                    c = container[i];
                }
            }
            while(strchr(" ,", c) == NULL);
        if(!obj.empty())
        {
            output.push_back(obj);
            index++;
            obj = "";
        }
    }
    for(int i = 0; i < output.size(); i++)
        cout<<output[i]<<endl;
    return output;
}
vector<string> parse_if(string container)
{
    vector<string> output;
    container = removeSpaces(container);
    for(int i = 0; i < container.length(); i++)
    {
        char c = container[i];
        string obj = "";
        do
        {
            obj += c;
            i++;
            c = container[i];
        }
        while(strchr("<=>&|!", c) == NULL && i < container.length());
        output.push_back(obj);
        string op = "";
        if ( i < container.length())
        {
            string op = "";
            do
            {
                op += c;
                i++;
                c = container[i];
            }
            while(strchr("<=>&|!", c) != NULL && i < container.length());
            output.push_back(op);
            i--;
        }
    }
    //cout<<"size"<<output.size();
    for(int i = 0; i < output.size(); i++)
        cout<<output[i];
    return output;
}

void generate_regs()
{
    for(int k = 0; k < instrs.len; k++)
    {
        instr inst = instrs.inst_vec[k];
        if(inst.type == "read")
        {
            vector<string> container = parse_read(inst.container);
            for(int m = 0; m < container.size(); m++)
                if( !checkreg(container[m]) )
                {
                    addreg(container[m], 0);
                }
        }
        else if(inst.type == "if_statement" || inst.type == "expression")
        {
            char *p = &inst.container[0];
            for(int i = 0; i < strlen(p); i++)
            {
                char e[256] = "";
                int index = 0;
                while(strchr(" =+-/*|^1234567890<>()", p[i]) == 0)
                {
                    e[index] = p[i];
                    i++;
                    index++;
                }
                e[index] = '\0';
                if(strcmp("sqrt", e) != 0 && strcmp("", e) != 0 && strcmp("abs", e) != 0 && strcmp("cos", e) != 0 && strcmp("sin", e) != 0)
                {
                    if(!checkreg(e))
                        addreg(e, 0);
                }
            }
        }
    }
}

void export_regs()
{
    out<<"float ";
    for(int i = 0; i < regs.len - 1; i++)
    {
        out<<regs.reg_vec[i].var<<" = "<<regs.reg_vec[i].val<<", ";
    }
    out<<regs.reg_vec[regs.len-1].var<<" = "<<regs.reg_vec[regs.len-1].val<<';'<<'\n';
}

int export_inst(instr inst, int tab, int index);

bool if_has_sons(instr inst)
{
    if(inst.cnx.EndA != -1 && inst.cnx.EndF != -1 )
        return true;
    else
        return false;
}

bool has_son(instr inst)
{
    if(inst.cnx.EndA != -1)
        return true;
    else
        return false;
}

int parents(int index)
{
    int output = 0;
    for(int i = 0; i < instrs.len; i++)
    {
        if( instrs.inst_vec[i].cnx.EndA == index)
            output++;
        if( instrs.inst_vec[i].cnx.EndF == index)
            output++;
    }
    return output;
}

int if_parents(int index)
{
    int output = 0;
    for(int i = 0; i < instrs.len; i++)
    {
        if(instrs.inst_vec[i].type == "if_statement")
            if( instrs.inst_vec[i].cnx.EndF == index || instrs.inst_vec[i].cnx.EndA == index)
                output++;
    }
    return output;
}
bool has_parent(int index)
{
    for(int i = 0; i < instrs.len; i++)
        if( instrs.inst_vec[i].cnx.EndA == index || instrs.inst_vec[i].cnx.EndF == index )
            return true;
    return false;
}

int verify()
{

    int start;
    bool has_start, has_stop;
    has_start = has_stop = false;

    for(int i = 0; i < instrs.len; i++)
    {
        cout<<"parsing: "<<i<<endl;
        string type = instrs.inst_vec[i].type;
        instr inst = instrs.inst_vec[i];

        if( type == "start" )
            if( !has_start && has_son(inst) && !has_parent(i) )
            {
                has_start = true;
                start = i;
            }
            else
                return -1;

        else if( type == "stop" )
            if( !has_stop && has_parent(i) && !has_son(inst) )
                has_stop = true;
            else
                return -1;

        else if( type == "if_statement" )
        {
            if( !if_has_sons(inst) || !has_parent(i) )
                return -1;
        }

        else if( type == "expression" )
        {
            cout<<"entered expression verification"<<'\n';
            if( !has_son(inst) || !has_parent(i) )
                return -1;
        }

    }

    if( has_start && has_stop )
        return start;
    else
        return -1;

}

void put_tab(int tab)
{
    char t = 9;
    for(int i = 0; i < tab; i++)
        out<<t;
}

int export_start(instr inst, int tab)
{

    put_tab(tab);
    out<<"#include<iostream>"<<'\n';

    put_tab(tab);
    out<<"#include<cmath>"<<'\n';

    put_tab(tab);
    out<<"using namespace std;"<<'\n';

    put_tab(tab);
    export_regs();

    put_tab(tab);
    out<<"int main(){"<<'\n';

    int next_index = inst.cnx.EndA;

    return next_index;

}

void export_stop()
{
    out<<"return 0;"<<'\n';
    out<<'}';
}

int export_read(instr inst, int tab)
{
    vector<string> container = parse_read(inst.container);
    for(int i = 0; i < container.size(); i++)
    {
        put_tab(tab);
        out<<"cin>>"<<container[i]<<';'<<'\n';
    }

    int next_index = inst.cnx.EndA;

    return next_index;

}

int export_write(instr inst, int tab)
{
    vector<string> container = parse_write(inst.container);
    for(int i = 0; i < container.size(); i++)
    {
        put_tab(tab);
        out<<"cout<<"<<container[i]<<';'<<'\n';
    }

    int next_index = inst.cnx.EndA;

    return next_index;

}

int export_if(instr inst, int tab, int index)
{

    put_tab(tab);
    out<<"if("<<inst.container<<"){"<<'\n';

    int next_index = inst.cnx.EndA;
    instr next_inst = instrs.inst_vec[next_index];
    while((parents(next_index) == 1 || if_parents(next_index) > 1) && instrs.inst_vec[next_index].type != "stop")
    {
        next_index = export_inst(next_inst, tab+1, next_index);
        next_inst = instrs.inst_vec[next_index];
    }
    put_tab(tab);
    out<<'}'<<'\n';

    next_index = inst.cnx.EndF;
    next_inst = instrs.inst_vec[next_index];

    if((parents(next_index) == 1 || if_parents(next_index) > 1) && instrs.inst_vec[next_index].type != "stop")
    {

        put_tab(tab);
        out<<"else{"<<'\n';
        do
        {

            next_index = export_inst(next_inst, tab+1, next_index);

            next_inst = instrs.inst_vec[next_index];

        }
        while(parents(next_index) == 1);
        put_tab(tab);
        out<<'}'<<'\n';


    }
    return next_index;
}

int export_expression(instr inst, int tab)
{

    put_tab(tab);
    out<<inst.container<<';'<<'\n';

    int next_index = inst.cnx.EndA;

    return next_index;
}

int export_inst(instr inst, int tab, int index)
{

    int next_index;

    if(inst.type == "start")
    {
        next_index = export_start(inst, tab);
        cout<<"exported start"<<endl;
    }

    else if(inst.type == "read")
    {
        next_index = export_read(inst, tab);
        cout<<"exported read"<<endl;
    }

    else if(inst.type == "if_statement")
    {

        next_index = export_if(inst, tab, index);

        cout<<"exported if"<<endl;

    }

    else if(inst.type == "expression")
    {

        next_index = export_expression(inst, tab);

        cout<<"exported if"<<endl;


    }

    else if(inst.type == "write")
    {

        next_index = export_write(inst, tab);

        cout<<"exported write"<<endl;

    }

    else if(inst.type == "stop")
    {

        export_stop();
        cout<<"exported stop"<<endl;

    }

    return next_index;
}

void exprt()
{

    regs = emptyRegsVec();

    int index = verify();

    if(index != -1)
    {
        generate_regs();

        out.open( "output.cpp", std::ofstream::trunc );

        instr inst;

        int tab = 0;

        do
        {

            inst = instrs.inst_vec[index];

            if(inst.type == "start" || inst.type == "stop")
                tab = 0;
            else
                tab = 1;

            index = export_inst(inst, tab, index);

        }
        while(inst.type != "stop");


        out.close();

    }
    else
    {
        cout<<index;
    }

}
