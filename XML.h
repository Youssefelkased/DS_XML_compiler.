#ifndef XML_H_
#define XML_H_

/*******************************************************************************
 *                                Includes Needed                              *
 *******************************************************************************/
#include <bits/stdc++.h>
using namespace std;

/*******************************************************************************
 *                             Class Decelerations                             *
 *******************************************************************************/
class XMLNode
{
	public:
	/*	Attributes	*/
    string tag;
    string data;
    bool isClosed = false;

	vector<XMLNode*> childrenPtrVector ;

	/*	Constructor */
	XMLNode();
	XMLNode(string Name  , string Value ) ;


	/* Methods */
	bool isLeaf();
	bool hasArrayOfSimilarChildren(void);
    void appendChild(XMLNode* node);
};

/*******************************************************************************
 *                             Types Decelerations                             *
 *******************************************************************************/
typedef  vector<XMLNode*> 						 Vec_Of_XMLNodePtr;
typedef  Vec_Of_XMLNodePtr::const_iterator		 Vec_Of_XMLNodePtr_Iterator;

typedef  string::const_iterator					 Str_Iterator;
typedef  vector<string> 						 Vec_Of_Strings;
typedef  Vec_Of_Strings::const_iterator 		 Vec_Of_Strings_Iterator;

typedef struct
{
	string orignalPair ;
	unsigned char   encryptedPair ;
}EncryptionPair;

/*******************************************************************************
 *                      Public Global Variables Decelerations                  *
 *******************************************************************************/
extern vector<EncryptionPair> g_encryptionTable ;
extern string g_XMLFilePath;

/*******************************************************************************
 *               		 Class Helper Functions Prototypes                     *
 *******************************************************************************/

/***********Extractions|Modifications Functions*****************/
/*
 * Arguments:
 * string XMLPath: the path of the XML file.
 * return: a single string that contain all the file contains.
 *
 * Description:
 * This function takes the XML file path, open it and extract all the characters in it into
 * a single string.
 */
string inputXMLFileToString(string XMLPath);

/*
 * Arguments:
 * Str_Iterator begin , Str_Iterator end : iterators to the begin and end of the string
 * 										   that contains the contents of the XML file.
 * returns:
 * vector of strings of the elements of XML file either data opening, or closing tags.
 * Description:
 * This function takes iterator to a string that contains the contents of the XML file
 * and extract each tag and each data and push it to a vector, then return this vector.
 */
Vec_Of_Strings XMLStrToXMLVecOfStr(Str_Iterator begin, Str_Iterator end);

/*
 * Arguments:
 * Vec_Of_Strings XMLInStringVector: vector of stings that contain the components of the XML file.
 * returns:
 * pointer to XMl node.
 * Description:
 * This function takes a vector of stings that contain the components of the XML file
 * and create a tree from it and return a pointer to its root node.
 */
XMLNode* VecOfStringToTree(Vec_Of_Strings XMLInStringVector);

/*
 * Arguments:
 * Vec_Of_Strings_Iterator itr: pointer to the first element of the XML elements vector.
 * XMLNode * parent: pointer to the parent node.
 * Description:
 * This recursive function takes a vector of stings that contain the components of the XML file
 * and create a tree from it and return a pointer to its root node.
 */
XMLNode * creadeXMLNode(Str_Iterator begin, Str_Iterator end);

/*
 * Description:
 * This function takes two iterators that point to the begin and end of the tag.
 * and extract the tag name from it, then create a new node and assign this tag name to
 * its tag name.
 */
Vec_Of_Strings_Iterator createXMLTree(Vec_Of_Strings_Iterator itr, XMLNode* parent);

/*
 * Description:
 * This function create number of indentations as determined in the parameters.
 */
string indent(int numberOfSpaces);

/****************Conversion to XML Functions********************/
/*
 * Arguments:
 * XMLNode* treeRootNode: the node of the XML tree, which we will start to depth
 * 						  first traverse from to add to the string the whole tree
 * 						  below it.
 * string& XMLString: this string is passed by alias to the function, so the
 * 					  function will modify it and write in it the XML code with
 * 					  indentation.
 * int indentation: the required indentation to start writing the node from.
 *
 * Description:
 * This function extract the tag and data from an XML tree and into a
 * single string written in XML syntax and add the required indentations.
 *
 * It is a recursive function
 * base case:
 * If the node is a leaf node, it will add to the string required indentation followed
 * by the opening tag followed by the data followed by the closing tag.
 * recursive case:
 * If the node isn't a leaf node, it will display the required indentation followed by
 * the opening tag then recursively call itself for each child node in its vector of
 * children to be displayed then it display its closing tag.
 */
void XMLWithIndentations(XMLNode* treeRootNode, string& XMLString, int indentation =0);

/*
 * Arguments:
 * XMLNode* treeRootNode: the node of the XML tree, which we will start to depth
 * 						  first traverse from to add to the string the whole tree
 * 						  below it.
 * string& XMLString: this string is passed by alias to the function, so the
 * 					  function will modify it and write in it the XML code with
 * 					  no indentations.
 *
 * Description:
 * This function extract the tag and data from an XML tree and into a single string
 * but with no indentations.
 *
 * It is a recursive function
 * base case:
 * If the node is a leaf node, it will add to the string the opening tag followed
 * by the data followed by the closing tag.
 * recursive case:
 * If the node isn't a leaf node, it will display the opening tag then recursively
 * call itself for each child node in its vector of children to be displayed
 * then it display its closing tag.
 */
void XMLMinified(XMLNode* treeRootNode, string& XMLString);

/***************Conversion to JSON Functions********************/
/*
 * Arguments:
 * XMLNode* treeRootNode: the node of the XML tree, which we will start to depth
 * 						  first traverse from, to write the tree in JSON syntax
 * 						  into the string, with the required indentation.
 * string& JSONString: this string is passed by alias to the function, so the
 * 					  function will modify it and write in it the JSON code with
 * 					  indentations.
 * int indentation: the required indentation to start writing the node from, it is
 * 					1 by default in JSON as the first node is indented by one.
 * bool isSimilarChild: it determines if this node is an element of an array or not
 * 						it has a default value false.
 * bool isFileBeginOrEnd: it determines if it is the root node in the JSON file.
 *
 * Description:
 * This function extract the tag and data from an XML tree and into a
 * single string written in JSON syntax and add the required indentations.
 */
void XML_to_JSON(XMLNode* treeRootNode , string& JSONString , int indentation = 1 , \
					bool isSimilarChild = false, bool isFileBeginOrEnd = true);

/**************Compression|Decompression Functions**************/
/*
 * Arguments:
 * string str: The string that is required to determine the most frequent pair
 * 			   of characters in.
 * Returns: the most frequent pair of characters.
 * Description:
 * This function search for the most frequent pair of characters in a given string
 * then return if and return "Not Found" in case there is no repeated pair of characters
 * in the string.
 */
string detectMostFrequentPair(string pair);

/*
 * Arguments:
 * string &compressedStr: The string that is required to be compressed, it is passed by alias
 * 						  so any modification will affect the input string.
 * unsigned char encode: the character that will replace the most frequent pair of characters
 * 						 it has a default value 128,
 *
 * Description:
 * This function takes a string and compress it using byte pair encoding technique, it
 * search through the string for the most frequent pair of character and replace it with encode character
 * and push these to values to the encryption table. and then repeat this process until there is no more
 * repeated pair of characters or the available range of characters finish.
 */
void bytePairEncoding(string &compressedStr , unsigned char encode = 128 );

/*
 * Arguments:
 * string &decompressedStr: The string that is required to be decompressed, it is passed by alias
 * 						  so any modification will affect the input string.
 *
 * Description:
 * This function takes a string that is compressed using byte pair encoding technique and de-compress it,
 * it search through the string for the encoded byte from the end of the vector to the beginning and replace
 * it by its original pair of byte. when it finishes from replacing all elements of the encryption table
 * from the bottom to top the string will return to its original value.
 */
void bytePairDecoding(string &decompressedStr);


#endif /* XML_H_ */








