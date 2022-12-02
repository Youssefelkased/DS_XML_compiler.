#include <windows.h>
#include "GUI.h"

/*---------------------------------------------------------
                       Global Variables
-----------------------------------------------------------*/

string g_compressedXML;
string path;
string g_XML_file_in_one_string;

HMENU hmenu;
HWND hedit, imagehandler, hdialog, houtput;
HBITMAP hlogoImage;


/*---------------------------------------------------------
                       Function Definitions
-----------------------------------------------------------*/
int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevinst, LPSTR args, int cmdshow) {


	WNDCLASSW wc = { 0 };
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hInstance = hInst;
	wc.lpszClassName = L"My Window Class";// L" for lpstr
	wc.lpfnWndProc = windowprocedure;

	if (!RegisterClassW(&wc)) {
		return -1;
	}

	CreateWindowW(L"my window class", L"XML Compiler", WS_OVERLAPPEDWINDOW | WS_VISIBLE, 100, 100, 1200, 700, NULL, NULL, NULL, NULL);


	MSG msg = {0};

	while ( GetMessage(&msg, NULL, NULL, NULL))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return 0;



}

LRESULT CALLBACK windowprocedure(HWND hwnd, UINT msg, WPARAM wp, LPARAM lp) {


	switch (msg) {
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_CREATE:
		AddControls(hwnd);
		break;
	case WM_COMMAND://lw ay haga etdast

		switch (wp)
		{
		case OPEN_FILE_BOTTON:
			open_file(hwnd);
			break;

		case 6:
			MessageBeep(MB_ABORTRETRYIGNORE);
			break;

		case EXIT_BOTTON:
			DestroyWindow(hwnd);
			break;

		case 5:
			wchar_t text[100];
			GetWindowTextW(hedit, text, 100);
			SetWindowTextW(hwnd, text);
			break;

		case COMPILE:
		{
			if(g_XML_file_in_one_string.empty())
			{
				string msg = "You must Browse XML file First !!!";
				SetWindowText(houtput, (LPCSTR)msg.c_str());
				break;
			}

			compilation_msdg.clear();
			compile(g_XML_file_in_one_string);
			compilation_message();
			SetWindowText(houtput, (LPCSTR)compilation_msdg.c_str());
			final_edited= g_XML_file_in_one_string;
		}
		break;
		case CORRECT:
		{
			if(g_XML_file_in_one_string.empty())
			{
				string msg = "You must Browse XML file First !!!";
				SetWindowText(houtput, (LPCSTR)msg.c_str());
				break;
			}

			compile(g_XML_file_in_one_string);
			SetWindowText(houtput, (LPCSTR)final_edited.c_str());
			final_edited= g_XML_file_in_one_string;
			compilation_message();
			break;
		}
		case MINIFY:
		{
			if(g_XML_file_in_one_string.empty())
			{
				string msg = "You must Browse XML file First !!!";
				SetWindowText(houtput, (LPCSTR)msg.c_str());
				break;
			}

			XMLNode * root_ptr;
			string minifiedXML = g_XML_file_in_one_string;
			Vec_Of_Strings vec = XMLStrToXMLVecOfStr(minifiedXML.begin(), minifiedXML.end());
			root_ptr = VecOfStringToTree(vec);
			minifiedXML = "";
			XMLMinified(root_ptr, minifiedXML);
			SetWindowText(houtput, (LPCSTR)minifiedXML.c_str());
		}

		break;

		case CONVERT_TO_JSON:
		{
			if(g_XML_file_in_one_string.empty())
			{
				string msg = "You must Browse XML file First !!!";
				SetWindowText(houtput, (LPCSTR)msg.c_str());
				break;
			}

			XMLNode * root_ptr;
			string JSONStr = g_XML_file_in_one_string;
			Vec_Of_Strings vec = XMLStrToXMLVecOfStr(JSONStr.begin(), JSONStr.end());
			root_ptr = VecOfStringToTree(vec);
			JSONStr = "";
			XML_to_JSON(root_ptr, JSONStr);
			SetWindowText(houtput, (LPCSTR)JSONStr.c_str());
		}
		break;

		case COMPRESS:
		{
			if(g_XML_file_in_one_string.empty())
			{
				string msg = "You must Browse XML file First !!!";
				SetWindowText(houtput, (LPCSTR)msg.c_str());
				break;
			}

			XMLNode * root_ptr;
			g_compressedXML = g_XML_file_in_one_string;
			Vec_Of_Strings vec = XMLStrToXMLVecOfStr(g_compressedXML.begin(), g_compressedXML.end());
			root_ptr = VecOfStringToTree(vec);
			g_compressedXML = "";
			XMLMinified(root_ptr, g_compressedXML);
			bytePairEncoding(g_compressedXML);
			SetWindowText(houtput, (LPCSTR)g_compressedXML.c_str());
		}
		break;

		case DECOMPRESS:
		{
			if(g_compressedXML.empty())
			{
				string msg = "You must Compress XML file First !!!";
				SetWindowText(houtput, (LPCSTR)msg.c_str());
				break;
			}

			bytePairDecoding(g_compressedXML);
			XMLNode * root_ptr;
			g_compressedXML = g_XML_file_in_one_string;
			Vec_Of_Strings vec = XMLStrToXMLVecOfStr(g_compressedXML.begin(), g_compressedXML.end());
			root_ptr = VecOfStringToTree(vec);
			g_compressedXML = "";
			XMLWithIndentations(root_ptr, g_compressedXML);
			SetWindowText(houtput, (LPCSTR)g_compressedXML.c_str());
		}
		break;

		case INDENTATE:
		{
			if(g_XML_file_in_one_string.empty())
			{
				string msg = "You must Browse XML file First !!!";
				SetWindowText(houtput, (LPCSTR)msg.c_str());
				break;
			}

			XMLNode * root_ptr;
			string indentedXML = g_XML_file_in_one_string;
			Vec_Of_Strings vec = XMLStrToXMLVecOfStr(indentedXML.begin(), indentedXML.end());
			root_ptr = VecOfStringToTree(vec);
			indentedXML = "";
			XMLWithIndentations(root_ptr, indentedXML);
			SetWindowText(houtput, (LPCSTR)indentedXML.c_str());
			break;
		}
		case SAVE_FILE:
		{
			save_file(hwnd);
		}
	}
	break;
	default:
		return DefWindowProcW(hwnd, msg, wp, lp);
	}
}





//controls are childs of parent window
// we create first ctrl window class
void AddControls(HWND hwnd){
	CreateWindowW(L"Static", L"Input XML Code", WS_VISIBLE | WS_CHILD , 430, 30, 110, 20, hwnd, NULL, NULL, NULL);
	hdialog = CreateWindowW(L"Edit", L"...", WS_VISIBLE | WS_CHILD |WS_BORDER| ES_MULTILINE | WS_VSCROLL | WS_HSCROLL, 290, 66, 400, 462, hwnd, NULL, NULL, NULL);


	CreateWindowW(L"Static", L"Output", WS_VISIBLE | WS_CHILD, 900, 30, 110, 20, hwnd, NULL, NULL, NULL);
	houtput = CreateWindowW(L"Edit", L"...", WS_VISIBLE | WS_CHILD | WS_BORDER | ES_MULTILINE | WS_VSCROLL | WS_HSCROLL, 725 , 66, 400, 462, hwnd, NULL, NULL, NULL);


	// Creating Buttoms:
	CreateWindowW(L"Button", L"Browse", WS_VISIBLE | WS_CHILD, 66, 66, 150, 30, hwnd,(HMENU)OPEN_FILE_BOTTON, NULL, NULL); //is element no to handle fo2
	CreateWindowW(L"Button", L"Compile", WS_VISIBLE | WS_CHILD, 66, 126, 150, 30, hwnd, (HMENU)COMPILE, NULL, NULL);
	CreateWindowW(L"Button", L"Correct", WS_VISIBLE | WS_CHILD, 66, 186, 150, 30, hwnd, (HMENU)CORRECT, NULL, NULL);
	CreateWindowW(L"Button", L"Indentate", WS_VISIBLE | WS_CHILD, 66, 246, 150, 30, hwnd, (HMENU)INDENTATE, NULL, NULL);
	CreateWindowW(L"Button", L"Convert to JSON", WS_VISIBLE | WS_CHILD, 66, 318, 150, 30, hwnd, (HMENU)CONVERT_TO_JSON, NULL, NULL);
	CreateWindowW(L"Button", L"Compress", WS_VISIBLE | WS_CHILD, 66, 378, 150, 30, hwnd, (HMENU)COMPRESS, NULL, NULL);
	CreateWindowW(L"Button", L"De-Compress", WS_VISIBLE | WS_CHILD, 66, 438, 150, 30, hwnd, (HMENU)DECOMPRESS, NULL, NULL);
	CreateWindowW(L"Button", L"Minify", WS_VISIBLE | WS_CHILD, 66, 498, 150, 30, hwnd, (HMENU)MINIFY, NULL, NULL);

	CreateWindowW(L"Button", L"Save", WS_VISIBLE | WS_CHILD, 892, 560, 100, 30, hwnd, (HMENU)SAVE_FILE, NULL, NULL);
	CreateWindowW(L"Button", L"Exit", WS_VISIBLE | WS_CHILD, 1025, 560, 100, 30, hwnd,(HMENU)EXIT_BOTTON, NULL, NULL);


	imagehandler = CreateWindowW(L"Static", NULL, WS_VISIBLE | WS_CHILD | SS_BITMAP, 50, 50, 110, 20, hwnd, NULL, NULL, NULL);
	SendMessageW(imagehandler, STM_GETIMAGE, IMAGE_BITMAP, (LPARAM)hlogoImage);
}




void open_file(HWND hwnd) {

	char file_name[100];
	OPENFILENAME ofn;
	ZeroMemory(&ofn,sizeof(OPENFILENAME));
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = hwnd;
	ofn.lpstrFile = file_name;
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = 1000;
	ofn.lpstrFilter = "XML files\0*.xml\0";
	ofn.nFilterIndex = 1;

	GetOpenFileName(&ofn);

	//MessageBox(NULL, ofn.lpstrFile, "", MB_OK);
	//lpstrfile has the path of chosen file.
	g_XMLFilePath = ofn.lpstrFile;
	g_XML_file_in_one_string = get_file(g_XMLFilePath);
	SetWindowText(hdialog, (LPCSTR)g_XML_file_in_one_string.c_str());


}

void save_file(HWND hwnd) {

	char file_name[100];
	OPENFILENAME ofn;
	ZeroMemory(&ofn,sizeof(OPENFILENAME));
	ofn.lStructSize = sizeof(OPENFILENAME);
	ofn.hwndOwner = hwnd;
	ofn.lpstrFile = file_name;
	ofn.lpstrFile[0] = '\0';
	ofn.nMaxFile = 1000;
	ofn.lpstrFilter = "XML files\0*.xml\0Text Files\0*.txt\0JSON Files\0*.json\0";
	ofn.nFilterIndex = 1;

	GetSaveFileName(&ofn);

	write_file(ofn.lpstrFile);


}

void write_file(char* path){

	FILE *file;
	file = fopen(path,"w");//w for write
	int size = GetWindowTextLength(houtput);
	char *data = new char[size+1];
	GetWindowText(houtput,data,size+1);

	fwrite(data,size+1,1,file);

	fclose(file);


}
