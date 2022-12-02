#ifndef GUI_H_
#define GUI_H_

#include <bits/stdc++.h>
#include "XML.h"
#include "commdlg.h"
#include "compile.h"

/*---------------------------------------------------------
                        PreProccessor
-----------------------------------------------------------*/
#define _CRT_SECURE_NO_DEPRECATE
#define OPEN_FILE_BOTTON 1
#define EXIT_BOTTON 2
#define COMPILE 20
#define CORRECT 30
#define CONVERT_TO_JSON 40
#define COMPRESS 41
#define DECOMPRESS 42
#define INDENTATE 43
#define MINIFY 44
#define SAVE_FILE 45


/*---------------------------------------------------------
                       Function Prototypes
-----------------------------------------------------------*/

/* TO MAKE BASIC WINDOW */
LRESULT CALLBACK windowprocedure(HWND, UINT, WPARAM, LPARAM);
void addimage();
void AddMenus(HWND);
void AddControls(HWND);
void open_file(HWND);
void save_file(HWND hwnd);
void write_file(char*);


#endif /* GUI_H_ */
