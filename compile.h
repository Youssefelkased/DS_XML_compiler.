
#ifndef COMPILE_H_
#define COMPILE_H_

#include <bits/stdc++.h>
using namespace std;
/*---------------------------------------------------------
                        PreProccessor
-----------------------------------------------------------*/
#define CLOSE_TAG_MISSING 0
#define MISMATCH 1
#define OPEN_TAG_MISSING 2


/*---------------------------------------------------------
                       Function Prototypes
-----------------------------------------------------------*/
string get_file(string);
void compile(string);
void check_close(string, int);
void check_open(string, int);
void check_mismatch(string, int, int);
void compile_open_tag_missing(string);
void compile_mismatch(string);
int error_case(string);
void compilation_message();

/*---------------------------------------------------------
                       Global Variables
-----------------------------------------------------------*/
extern vector<string> errs;
extern int count_errors; //global variables
extern bool errorflag;
extern int index;
extern string final_edited; //global variable
extern string compilation_msdg ; //global VARIABLE
extern stack<string> stck;


#endif
