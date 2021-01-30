#include <cstring>
#include <stack>
#include <vector>
#include <cmath>

using namespace std;
/*New*/
struct sgmnt {
    int start,
         end;
    int pos_on_the_other_axis;
};

struct Dim {
    int width;
    int height;
};

struct Spot {
    int xPos;
    int yPos;
};

struct Button {
    int type;
    bool isPressed;
    char * message;
};

struct block {
    Spot upLeft;
    Spot downRight;
};

struct node {
    int index;
    node* urmA;
    node* urm;
};

struct cnnctn {
    int    EndF,
            EndA;
};

struct instr {
    string type, container;
    Spot up_lft, dwn_rgt;
    Spot cnnctn_crcle1,
            cnnctn_crcle2;
    cnnctn cnx;
};

#define MAX                                    50
#define MAXCON                            100
struct cnnctns_vec {
    int len;
    cnnctn connex_vec[ MAXCON ];
};

struct instrs_vec {
    int len;
    instr inst_vec[MAX];
};

struct reg{
    string var;
    float val;
};

#define MAXREG                            50

struct regs_vec{
    int len;
    reg reg_vec[MAXREG];
};
