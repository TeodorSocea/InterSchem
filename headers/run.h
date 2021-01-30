#include <calculator.h>

void split_expression(string container, string &var, string &expression)
{
    container = removeSpaces(container);
    string delimiter = "=";
    int pos = container.find(delimiter);
    var = container.substr(0, pos);
    container.erase(0, pos + delimiter.length());
    expression = container.substr(0, container.length());
}

int execute_block(instr inst, string& brnch)
{
    brnch = "T";
    if(inst.type == "start")
    {
        cout<<"Program started"<<endl;  // to be replaced with outtextxy
        return inst.cnx.EndA;
    }
    else if(inst.type == "stop")
        cout<<"Program stopped"<<endl;
    else if(inst.type == "read")
    {
        vector<string> container = parse_read(inst.container);
        for(int i = 0; i < container.size(); i++)
        {
            int index = find_reg(container[i]);
            float *dest = &( regs.reg_vec[index].val );
            string name = regs.reg_vec[index].var;
            ask_for_reg_value( *dest, name );
        }
        return inst.cnx.EndA;
    }
    else if(inst.type == "write")
    {
        int yPos = 300;
        vector<string> output = parse_write(inst.container);
        for(int i = 0; i < output.size(); i++)
            // "
            if(output[i][0] != '"') {
                reg new_reg;
                new_reg.var = "output";
                new_reg.val = EvaluatePostfix(convertToVec(infixToPostfix(output[i])));
                cout<<new_reg.val;
                print_reg_value(  new_reg, yPos );
            }
            else
            {
                string sub = output[i].substr(1, output[i].length()-2);
                char* txt = &sub[ 0 ];
                setbkcolor( BK_COLOR );
                print_on_screen_rows_of_9( txt, yPos );
//                place_intrgtion_txt( {5, yPos}, INTRGTION_TXT_COL, txt );
                setbkcolor( BUTTON_COLOR );

//                yPos += textheight( txt );
                cout<<sub;
            }
        delay( 10*DELAY );
        int x = dim_of_butt[ 1 ].width;
        bar( 5, 300, x, yPos );
        cout<<endl;
        return inst.cnx.EndA;
    }
    else if(inst.type == "expression")
    {
        string var;
        string expression;
        split_expression(inst.container, var, expression);
        int index = find_reg(var);
        regs.reg_vec[index].val = EvaluatePostfix(convertToVec(infixToPostfix(expression)));
        return inst.cnx.EndA;
    }
    else if(inst.type == "if_statement")
    {
        vector<string> container = parse_if(inst.container);
        //cout<<container[0]<<container[1]<<container[2]<<endl;
        float result1;
        float operand1 = EvaluatePostfix(convertToVec(infixToPostfix(container[0])));
        float operand2 = EvaluatePostfix(convertToVec(infixToPostfix(container[2])));
        //cout<<operand1<<" "<<operand2<<" ";
        string op = container[1];
        //cout<<op;
        if(op == "<")
            result1 = MaiMic(operand1, operand2);
        else if(op == ">")
            result1 = MaiMare(operand1, operand2);
        else if(op == "==")
            result1 = Egal(operand1, operand2);
        else if(op == "!=")
            result1 = Diferit(operand1, operand2);
        else if(op == "<=")
            result1 = MaiMic(operand1, operand2) || Egal(operand1, operand2);
        else if(op == ">=")
            result1 = MaiMare(operand1, operand2) || Egal(operand1, operand2);
        // mai multe conditii
        // a + b            <       a-c
        if(container.size()!=3)
        {
            float result2;
            operand1 = EvaluatePostfix(convertToVec(infixToPostfix(container[4])));
            operand2 = EvaluatePostfix(convertToVec(infixToPostfix(container[6])));
            op = container[5];
            string op2 = container[3];
            if(op == "<")
                result2 = MaiMic(operand1, operand2);
            else if(op == ">")
                result2 = MaiMare(operand1, operand2);
            else if(op == "==")
                result2 = Egal(operand1, operand2);
            else if(op == "!=")
                result2 = -Egal(operand1, operand2);
            else if(op == "<=")
                result2 = MaiMic(operand1, operand2) || Egal(operand1, operand2);
            else if(op == ">=")
                result2 = MaiMare(operand1, operand2) || Egal(operand1, operand2);
            if(op2 == "&&")
                result1 = result1 && result2;
            else
                result1 = result1 || result2;
        }
        if(result1 == 1){
            cout<<"adevarat"<<endl;
            return inst.cnx.EndA;
        }
        else{
            brnch = "F";
            cout<<"fals"<<endl;
            return inst.cnx.EndF;
        }
    }
}

void run()
{
    int curr_color, path_color, index, color;
    string nxt_brnch, prev_brnch;
    bool start_passed = false;
    curr_color = YELLOW;
    path_color = LIGHTGREEN;

    regs = emptyRegsVec();

    index = verify();
    if(index != -1)
    {

        generate_regs();

        instr inst, prev_instr;
        nxt_brnch = "NO";
        do
        {
            prev_instr = inst;
            prev_brnch = nxt_brnch; /*make a copy*/
            inst = instrs.inst_vec[index];

            setlinestyle( 0, 0, 2 );
            color = curr_color;
            draw_blk( inst, color );

            setlinestyle( 0, 0, 1 );
            if ( prev_brnch != "NO" )
                draw_cnnctn(prev_instr, prev_brnch, path_color );

            delay(10*DELAY);

            index = execute_block(inst, nxt_brnch);

            setlinestyle( 0, 0, 2 );
            draw_blk( inst, BK_COLOR );
            setlinestyle( 0, 0, 1 );

            color = path_color;
            draw_blk(inst, color );

            if ( prev_brnch != "NO" )
                draw_cnnctn(prev_instr, prev_brnch, color );

            draw_cnnctn( inst, nxt_brnch, color );
            delay(DELAY);
        }
        while(inst.type != "stop");

        return_to_normal();
    }
}
