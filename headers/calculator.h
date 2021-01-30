#include <export.h>


#define infinit INT_MAX
#define epsi 0.0001

int find_reg(string var)
{
    int index = 0;
    while ( regs.reg_vec[index].var != var)
        index++;
    return index;
}

bool isOperator(char c)
{
    if(c == '+' || c == '-' || c == '*' || c == '/' || c == '^' || c == '%')
        return true;
    else
        return false;
}

int precedence(string c)
{
    if(c == "+" || c == "-")
        return 1;
    if(c == "*" || c == "/" || c == "%")
        return 2;
    if(c == "^")
        return 3;
    if(c == "sin" || c == "cos" || c == "abs" || c == "sqrt")
        return 4;
    else
        return -1;
}
int operatorType(string c){
    if(c == "sin" || c == "cos" || c == "abs" || c == "sqrt")
        return 1;
    else
        return 2;
}

string infixToPostfix(string infix)
{
    stack<string> s;
    string postfix;

    for(int i = 0; i < infix.length(); i++)
    {

        if((infix[i]>='a' && infix[i]<='z') || (infix[i]>='A' && infix[i]<='Z'))
        {
            string obj;
            do
            {
                obj+=infix[i];
                i++;
            }
            while(isalnum(infix[i]) || infix[i]=='_');
            i--;
            if(obj != "sqrt" && obj != "abs" && obj != "cos" && obj != "sin")
                postfix+= " " + obj;
            else
            {
                s.push(obj);
            }
        }
        else if(isdigit(infix[i]))
        {
            string numobj;
            do
            {
                numobj+=infix[i];
                i++;
            }
            while(isdigit(infix[i]) || infix[i] == '.');
            i--;
            postfix+=" " + numobj;
        }
        else if(infix[i] == '(')
        {
            string temp;
            temp += infix[i];
            s.push(temp);
        }
        else if(infix[i] == ')')
        {
            while(s.top()!="(" && !s.empty())
            {
                string temp = s.top();
                postfix+= " " + temp;
                s.pop();
            }
            if(s.top() == "(")
                s.pop();
        }
        else if(isOperator(infix[i]))
        {

            string temp;
            temp +=infix[i];

            if(s.empty())
            {
                s.push(temp);
            }
            else if(precedence(temp) > precedence(s.top()))
            {
                s.push(temp);
            }
            else if(precedence(temp) == precedence(s.top()) && temp == "^")
            {
                s.push(temp);
            }
            else
            {
                while(!s.empty() && precedence(temp) < precedence(s.top()))
                {
                    string temp = s.top();
                    postfix+=" " + temp;
                    s.pop();
                }
                s.push(temp);
            }
        }
    }//a+b*c
     //output:a b c
     //s:        + *
     // output: a b c * +
     //a*b+c
     //output: a b c
     //s:         * +
     // output: a b * c +
    while(!s.empty())
    {
        postfix+=" " + s.top();
        s.pop();
    }
    postfix+=" ";
    return postfix;
}
vector<string> convertToVec(string postfix)
{
    vector<string> output;
    string delimiter = " ";
    size_t pos = 0;
    string token;
    while ((pos = postfix.find(delimiter)) != std::string::npos)
    {
        token = postfix.substr(0, pos);
        output.push_back(token);
        postfix.erase(0, pos + delimiter.length());
    }
    return output;
}
bool DifInf(float x)
{
    return fabs(infinit - fabs(x)) > infinit / 2.0;
}
float Logaritm(float x)
{
    if (x>epsi && DifInf(x))
        return log(x);
    else
        return infinit;
}

float Exponential(float x)
{
    if (DifInf(x)) return exp(x);
    else return infinit;
}

float Inmultit(float x, float y)
{
    if (fabs(x < epsi) || fabs(y) < epsi) return 0;
        else if (DifInf(x) && DifInf(y)) return x*y;
            else return infinit;
}

float Putere(float x, float y)
{
    // float p;
    if (x==0) return 0;
    else if (y==0) return 1;
    else if (x==infinit || y==infinit) return infinit;
    else
        return pow(x,y);
}

float Egal(float x, float y)
{
    return x==y;
}

float Diferit(float x, float y)
{
    return x!=y;
}

float MaiMic(float x, float y)
{
    return x < y;
}

float MaiMare(float x, float y)
{
    return x > y;
}

float Plus(float x, float y)
{
    if (DifInf(x) && DifInf(y))  return x+y;
    else return infinit;
}

float Minus(float x, float y)
{
    if (DifInf(x) && DifInf(y))  return x-y;
    else return infinit;
}

float Impartit(float x, float y)
{
    if (fabs(y)>epsi) return x/y;
    else return infinit;
}

float Sinus(float x)
{
    if (DifInf(x))  return sin(x);
    else return infinit;
}

float Cosinus(float x)
{
    if (DifInf(x))  return cos(x);
    else return infinit;
}

float Modul(float x)
{
    if (DifInf(x)) return fabs(x);
    else return infinit;
}

float Radical(float x)
{
    if (DifInf(x) && (x>epsi)) return sqrt(x);
    else return infinit;
}

int Mod(float x, float y){
    int a = x;
    int b = y;
    return a%b;
}

bool isOperator(string op){
    if(op == "sin" || op == "cos" || op == "abs" || op == "sqrt" || op == "+" || op == "-" || op == "*" || op == "/" || op == "^" || op == "%")
        return true;
    else
        return false;
}

float performUnarOperation(string op, float operand){
    if( op == "sqrt") return Radical(operand);
    else if( op == "abs") return Modul(operand);
    else if( op == "cos") return Cosinus(operand);
    else if( op == "sin") return Sinus(operand);
}

float performBiOperation(string op, float operand1, float operand2){
    if( op == "*" ) return Inmultit(operand1, operand2);
    else if(op == "^") return Putere(operand1, operand2);
    else if(op == "+") return Plus(operand1, operand2);
    else if(op == "-") return Minus(operand1, operand2);
    else if(op == "/") return Impartit(operand1, operand2);
    else if(op == "%") return Mod(operand1, operand2);
}

float EvaluatePostfix(vector<string> expression)
{
    stack<float> S;

    for(int i = 0; i< expression.size(); i++)
    {
        if(isOperator(expression[i]))
        {
            int type = operatorType(expression[i]);
            float result = 0;
            if(type == 1){
                float operand = S.top();
                S.pop();
                result = performUnarOperation(expression[i], operand);
            }
            else{
                float operand2 = S.top();
                S.pop();
                float operand1 = S.top();
                S.pop();
                result = performBiOperation(expression[i], operand1, operand2);
            }
            S.push(result);
        }
        else if(isdigit(expression[i][0]))
        {
            float operand = 0, j = 0;
            do{
                operand = operand * 10 + expression[i][j] - '0';
                j++;
            }while(j < expression[i].length() && isdigit(expression[i][j]));
            if(j!=expression[i].length()){
                j++;
                float rat = 0;
                int p = 1;
                do{
                    rat = rat * 10 + expression[i][j] - '0';
                    p*=10;
                    j++;
                }while(j<expression[i].length());
                operand += rat/p;
            }
            S.push(operand);
        }
        else{
            int index = find_reg(expression[i]);
            S.push(regs.reg_vec[index].val);
        }
    }
    /*
    a+b*c
    a b c * +
    * b c
    + b*c a
    */
    return S.top();
}


