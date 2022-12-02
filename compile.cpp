#include "compile.h"


/*---------------------------------------------------------
                       Global Variables
-----------------------------------------------------------*/
vector<string> errs;
int count_errors; //global variables
bool errorflag = false;
int index;
string final_edited; //global variable
string compilation_msdg = ""; //global VARIABLE
stack<string> stck;

/*---------------------------------------------------------
                       Function Definitions
-----------------------------------------------------------*/


string get_file(string path)
{
	string readLine;
	string allXMLinOneString = "";
	ifstream in_file;
	in_file.open(path);
	if (!(in_file.is_open()))
	{
		exit(1);
	}
	while (!in_file.eof())
	{
		getline(in_file, readLine);
		allXMLinOneString += "\r\n" + readLine;
	}
	in_file.close();
	final_edited = allXMLinOneString;


	return allXMLinOneString;
}


void compilation_message() {
	string sui;
	sui = to_string(count_errors) + " Errors Detected :\r\n";
	compilation_msdg.insert(0, sui);
	count_errors=0;
}

void compile(string x) {
	int count = 0;
	int count2 = 0;
	string open;
	string close;
	int error_type;

	for (int i = 0; i < x.length(); i++) {
		if ((x[i] == '<') && x[i + 1] == '/') {
			for (int j = i + 2; j < x.length(); j++) {
				count++;
				if (x[j] == '>') {
					close = x.substr(i + 2, count - 1);
					error_type = error_case(x);

					switch (error_type) {
					case CLOSE_TAG_MISSING:
						check_close(close, i);
						break;
					case MISMATCH:
						check_mismatch(close, i, count);
						break;
					case OPEN_TAG_MISSING:
						check_open(close, i);
						break;
					}
					count = 0;
					if (errorflag) {
						for (int i = 0; i < stck.size(); i++) {
							stck.pop();
						}
						compile(final_edited);
						return;
					}
					break;
				}
			}
		}
		else if (x[i] == '<') {
			for (int j = i + 1; j < x.length(); j++) {
				count2++;
				if (x[j] == '>') {
					open = x.substr(i + 1, count2 - 1);
					count2 = 0;
					stck.push(open); // Pushing every Opening tag
					break;
				}
			}
		}
	}
}

void check_close(string x, int close_index) {
	string temp;
	if (x == stck.top()) {
		stck.pop();
		errorflag = false;
	}
	else {
		errs.push_back(stck.top());
		temp = "</" + stck.top() + ">";
		final_edited.insert(close_index - 1, temp);
		errorflag = true;
		compilation_msdg = compilation_msdg + stck.top() + " Has no Closing Tag " + "\r\n";
		stck.pop();
		count_errors++;

	}
}


void check_mismatch(string x, int close_index, int length) {
	string temp;
	if (x == stck.top()) {
		stck.pop();
		errorflag = false;
	}
	else {
		errs.push_back(stck.top());
		temp = "</" + stck.top() + ">";
		final_edited.replace(close_index, length + 2, temp);
		errorflag = true;
		compilation_msdg = compilation_msdg + stck.top() + " Has a Mismatching Closing Tag " + "\r\n";
		count_errors++;
		stck.pop();

	}
}

void check_open(string close_tag, int close_index) {
	string temp;
	if (close_tag == stck.top()) {
		stck.pop();
		errorflag = false;
	}
	else {
		errs.push_back(close_tag);
		temp = "<" + close_tag + ">";
		final_edited.insert(close_index, temp);
		errorflag = true;
		stck.pop();
		compilation_msdg = compilation_msdg + close_tag + " Has No Opening Tag " + "\r\n";
		count_errors++;
	}
}

int error_case(string x) {
	int closing_tags = 0;
	int open_tags = 0;

	for (int i = 0; i < x.length(); i++) {
		if ((x[i] == '<') && x[i + 1] == '/') {
			closing_tags++;

		}
		else if (x[i] == '<') {
			open_tags++;

		}
	}
	if (closing_tags > open_tags) {
		return OPEN_TAG_MISSING;
	}
	else if (closing_tags < open_tags) {
		return CLOSE_TAG_MISSING;
	}
	else { // Closing tags = open tags
		return MISMATCH;
	}

}
