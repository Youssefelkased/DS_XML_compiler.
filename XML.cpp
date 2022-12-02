/*******************************************************************************
 *                                Includes Needed                              *
 *******************************************************************************/
#include "XML.h"

/*******************************************************************************
 *                         Public Global Variables Definitions                 *
 *******************************************************************************/
vector<EncryptionPair> g_encryptionTable ;
string g_XMLFilePath ;

/*******************************************************************************
 *                             Class Definitions                             *
 *******************************************************************************/
/*	Constructors */
XMLNode::XMLNode()
{
}

XMLNode::XMLNode(string tag , string data)
{
	this->tag = tag;
	this->data = data;
}

/*
 * Description:
 * this function determine if this node is a leaf or not. If its vector of chilren is empty
 * then it is leaf node.
 */
bool XMLNode::isLeaf()
{
	return this->childrenPtrVector.empty();
}

/*
 * Description:
 * This function append the given node(child) to the current node(parent), by adding the a
 * pointer to this child nodes in the vector of children of the parent node.
 */
void XMLNode::appendChild(XMLNode *node)
{
	this->childrenPtrVector.push_back(node);
}

/*
 * Description:
 * This function determines whether this node has a vector of the children that have the same tag name.
 */
bool XMLNode::hasArrayOfSimilarChildren(void)
{
	/* If this nod has no or only one node return false. */
	if(this->childrenPtrVector.size() < 2)
		return false;

	/* Loop on the children of the node and compare its tag name to the first child tag name
	 * If all have the same tag name return true, else return false.
	 */
	string firstChildName = this->childrenPtrVector.at(0)->tag;
	for(auto child: this->childrenPtrVector)
	{
		if(firstChildName != child->tag)
			return false;
	}
	return true;
}


/*******************************************************************************
 *               		   Class Helper Functions Definitions                  *
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
string inputXMLFileToString(string XMLPath)
{
	/* Open the XML file. */
	ifstream in_file(XMLPath);

	string temp ,XMLString;

	/* read the first line */
	getline(in_file, temp,'>');
	in_file.unget();
	temp += in_file.get();

	/* Check if the first line is a header */
	bool hasHeader = *(temp.begin() +1 ) == '?' && *(temp.end() - 2) == '?' \
												&& temp.substr(2,3) == "xml";
	/* If the first line is a header then neglect it, else add it to the string. */
	if (!hasHeader)
	{
		XMLString += temp +"\r\n";
	}

	/*extract all the following lines in the file into this string. */
	while (getline(in_file,temp))
	{
		XMLString += temp+"\r\n";
	}

	return XMLString;
}

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
Vec_Of_Strings  XMLStrToXMLVecOfStr(Str_Iterator begin, Str_Iterator end)
{
	Vec_Of_Strings XMLVecOfStr;

	string dataStr, tagStr;

	Str_Iterator i = begin , j = begin , DataForwardItr, DataBackwardItr;

	/* Loop till reach the end of the string. */
	while (j != end)
	{
		/* Notice that j always point to '<' and i always point to '>' */

		j = find(i, end, '<');
		dataStr = string(i,j); /* Extract the data that lies between '>' and '<' */

		/* Check if this string isn't empty or a comment or only empty spaces */
		if (! dataStr.empty()
			&& dataStr.substr(0,4) != "<!--"
			&& find_if_not(dataStr.begin(), dataStr.end(), ::isspace) != dataStr.end())
		{
			/* Move DataForwardItr to point to the first character of the data and DataBackwardItr
			 * to point to the last character in the data, in order not to have many spaces at the
			 * beginning and at the end of the data.
			 * */
			for(DataForwardItr = dataStr.begin(); isspace(*DataForwardItr); DataForwardItr++);
			for(DataBackwardItr = dataStr.end()-1; isspace(*DataBackwardItr); DataBackwardItr--);

			XMLVecOfStr.push_back(string(DataForwardItr, DataBackwardItr+1)); /* Push this data to the vector. */
		}


		i = find(j, end, '>');
		if (i < end && j < end)
		{
			tagStr = string(j, ++i); /* Extract the tag that lies between '<' and '>' */
		}

		/* Check if this string isn't empty or a comment or only empty spaces */
		if (!tagStr.empty()
			&& tagStr.substr(0,4) != "<!--"
			&& find_if_not(tagStr.begin(),tagStr.end(), ::isspace) != tagStr.end())
		{
			XMLVecOfStr.push_back(tagStr); /* Push this tag to the vector. */
		}
	}
	return XMLVecOfStr;
}

/*
 * Arguments:
 * Vec_Of_Strings XMLInStringVector: vector of stings that contain the components of the XML file.
 * returns:
 * pointer to XMl node.
 * Description:
 * This function takes a vector of stings that contain the components of the XML file
 * and create a tree from it and return a pointer to its root node.
 */
XMLNode* VecOfStringToTree(Vec_Of_Strings XMLInStringVector)
{
	/* Create a temporary root node. */
	XMLNode* tempRoot = new XMLNode;

	/* Call createXMLTree function and pass this tempRoot and the reting to it*/
	createXMLTree(XMLInStringVector.begin(),tempRoot);

	/* The root of the XML tree is then the first child of this tempRoot */
	XMLNode* root = tempRoot->childrenPtrVector.at(0);

	delete tempRoot;

	return root;
}

/*
 * Arguments:
 * Vec_Of_Strings_Iterator itr: pointer to the first element of the XML elements vector.
 * XMLNode * parent: pointer to the parent node.
 * Description:
 * This recursive function takes a vector of stings that contain the components of the XML file
 * and create a tree from it and return a pointer to its root node.
 */
Vec_Of_Strings_Iterator createXMLTree(Vec_Of_Strings_Iterator itr, XMLNode * parent)
{
	/* if the vector element is a data, it assign this data to the data of the parent. */
	if (find(itr->begin(), itr->end(), '<') == itr->end())
	{
		parent->data = string(itr->begin(), itr->end());
	}
	else
	{
		/* Recursive case: if the vector element is a tag, it create a new node with this tag names. */
		XMLNode* node = creadeXMLNode(itr->begin(), itr->end());

		/* check if the node is a closing tag */
		bool isClosing = (*(node->tag.begin()) == '/');

		if (isClosing)
		{
			/* If closing tag, check if it is closing tag of the parent by comparing the parent tag name and closing tag name */
			bool isClosingOfPar = std::equal(node->tag.begin() + 1,
					node->tag.end() , parent->tag.begin());

			/* Assign true to isclosed attribute of the parent, so that it can reach the base case*/
			if (isClosingOfPar)
				parent->isClosed = true;
		}
		else
		{
			/* If it is not closing tag then append child to the parent*/
			parent->appendChild(node);

			while (! node->isClosed)	/* recursive and complete creating children until we reach a closing tag */
			{
				itr = createXMLTree(++itr, node);	/* this is the base case we won't recursive if the parent is closed */
			}
		}
	}
	return itr;
}

/*
 * Description:
 * This function takes two iterators that point to the begin and end of the tag.
 * and extract the tag name from it, then create a new node and assign this tag name to
 * its tag name.
 */
XMLNode * creadeXMLNode(Str_Iterator begin, Str_Iterator end)
{
	/* Extract the tag name */
	Str_Iterator i,j;

	i = find(begin, end, '<');
	j = find(i, end, '>');

	string tag = string(++i,j);

	/* Create a new node with this tag name. */
	XMLNode * node = new XMLNode;
	node->tag = tag;

	return node;
}

/*
 * Description:
 * This function create number of indentations as determined in the parameters.
 */
string indent(int numberOfSpaces)
{
	string stringOfSpaces = "";
	for(int i = 0; i < numberOfSpaces;  i++)
	{
		stringOfSpaces += "    ";
	}
	return stringOfSpaces;
}
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
void XMLWithIndentations(XMLNode* treeRootNode, string& XMLString, int indentation)
{
	/* Base case If the node is a leaf node, it will add to the string required
	 * indentation followed by the opening tag followed by the data followed by
	 * the closing tag.
	 */
	if( treeRootNode->isLeaf() )
	{
		XMLString += indent(indentation);
		XMLString += "<" + treeRootNode->tag + ">"+treeRootNode->data +"</" +\
													treeRootNode->tag + ">\r\n";
		return;
	}

	/* recursive case: If the node isn't a leaf node */
	/* Add the indentation to the string followed by the opening tag*/
	XMLString += indent(indentation);
	XMLString += "<" + treeRootNode->tag + ">\r\n";

	/* Loop on each child of the node, call the function recursively and pass for it
	 * this child and the string and the indentation incremented by one so it is written
	 * in the next level after the parent.
	 */
	for(unsigned long long i =0 ;i<( treeRootNode->childrenPtrVector.size() ); ++i )
	{
		XMLWithIndentations( treeRootNode->childrenPtrVector.at(i) , XMLString, indentation+1);
	}

	/* Add the indentation to the string followed by the closing tag*/
	XMLString += indent(indentation);
	XMLString += "</" + treeRootNode->tag + ">\r\n";

}

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
void XMLMinified(XMLNode* treeRootNode, string& XMLString)
{
	/* Base case If the node is a leaf node, it will add to the string the opening
	 * tag followed by the data followed by the closing tag.
	 */
	if( treeRootNode->isLeaf() )
	{
		XMLString += "<" + treeRootNode->tag + ">"+treeRootNode->data +"</" +\
												   treeRootNode->tag + ">";
		return;
	}

	/* Recursive case: If the node isn't a leaf node */
	/* Add the the opening tag to the string */
	XMLString += "<" + treeRootNode->tag + ">";

	/* Loop on each child of the node, call the function recursively and pass for it
	 * this child and the string .
	 */
	for(unsigned long long i =0 ;i<( treeRootNode->childrenPtrVector.size() ); ++i )
	{
		XMLMinified( treeRootNode->childrenPtrVector.at(i) , XMLString);
	}

	/* Add the closing tag to the string */
	XMLString += "</" + treeRootNode->tag + ">";

}

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
void XML_to_JSON(XMLNode* treeRootNode, string& JSONString ,int indentation, bool isSimilarChild, bool isFileBeginOrEnd)
{
	/* To display the open curly braces in the beginning of the file. */
	if(isFileBeginOrEnd)
		JSONString += "{\r\n";

	/*
	 * Base case:
	 * 1- If the leaf node isn't an element of an array: it will add to the string
	 * the tag name and the data with the required indentation.
	 * 2- If the leaf node is an element of an array (is a similar child), it will
	 * add to the string data only with the required indentation.
	 */
	if( treeRootNode->isLeaf() )
	{
		JSONString += indent(indentation);

		if(!isSimilarChild)
		{
			JSONString += char(34) + treeRootNode->tag + char(34) + ": " + char(34) + \
								     treeRootNode->data +char(34)+", \r\n" ;
		}
		else
		{
			JSONString += char(34) + treeRootNode->data + char(34) + ",\r\n";
		}
		return;
	}

	/* Recursive case: If the node isn't a leaf node add the required indentation then:
	 * 1- If the node has an array of similar children, it will add the tag name and the
	 * 	  the name of its first child (since all of its children have the same name), with
	 * 	  all required braces.
	 * 2- If the one is on of the array of similar children, it will add the curly braces
	 * 	  only as the name is already written.
	 * 3- If it is not any of the above, it will add the tag name only.
	 */
	JSONString += indent(indentation);

	if(treeRootNode->hasArrayOfSimilarChildren())
	{
		JSONString +=  char(34) + treeRootNode->tag + char(34) +": {\r\n" + indent(++indentation)\
				+ char(34) + treeRootNode->childrenPtrVector.at(0)->tag+ char(34) +": [\r\n";
	}
	else if(isSimilarChild)
	{
		JSONString +=  "{\r\n";
	}
	else
	{
		JSONString +=  char(34) + treeRootNode->tag + char(34) +": {\r\n";
	}


	 /* After displaying any of the above three cases, it will call the function recursively to add all
	  * its children.
	  */
	for(unsigned long long i =0 ;i<( treeRootNode->childrenPtrVector.size() ); ++i )
	{
		if(treeRootNode->hasArrayOfSimilarChildren())
		{
			XML_to_JSON( treeRootNode->childrenPtrVector.at(i) , JSONString, indentation+1, true, false);
		}
		else
		{
			XML_to_JSON( treeRootNode->childrenPtrVector.at(i) , JSONString, indentation+1, false, false);
		}
	}

	/* Add the required indentation then:
	 * 1- If the node has an array of similar children,it will add the required braces.
	 * 2- If the one is on of the array of similar children,it will add the required braces.
	 * 3- If it is not any of the above,it will add the required braces.
	 */
	JSONString += indent(indentation);

	if(treeRootNode->hasArrayOfSimilarChildren() )
	{
		JSONString += "]\r\n" + indent(--indentation) + "}\r\n";
	}
	else if(isSimilarChild)
	{
		JSONString +=  "},\r\n";
	}
	else
	{
		JSONString +=  "}\r\n";
	}

	/* To display the close curly braces in the end of the file. */
	if(isFileBeginOrEnd)
		JSONString += "}\r\n";
}
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
string detectMostFrequentPair(string pair)
{
	int frequency = 0 ;
	int maxFrequency = 1 ; /* As the frequency must be greater than one. */

	string temp;
	string mostFreqPair = "Not Found";

	/*
	 * Loop on the string, take each consecutive characters and calculate its frequency
	 * in the string then compare it to the maximum frequency, if its frequency is
	 * greater that it, then assign its frequency to the maximum frequency and assign this
	 * character to mostFreqPair.
	 */
	for( Str_Iterator itr = pair.begin() ; itr != (pair.end() -1 ) ; ++itr )
	{
		frequency = 0;
		temp = string(itr, itr+2);

		for(Str_Iterator itr2 = pair.begin() ; (itr2 != pair.end() -1 ) ; ++itr2)
		{
			if(temp == string(itr2, itr2+2) )
			{
				frequency ++;
			}
		}

		if( frequency > maxFrequency)
		{
			maxFrequency = frequency;
			mostFreqPair = temp;
		}
	}
	return mostFreqPair;
}

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
void bytePairEncoding(string &compressedStr , unsigned char encode  )
{
	/* Search for the most frequent character */
	string mostFreqPair = detectMostFrequentPair(compressedStr);

	/*
	 * Base case: There is no repeated pair of characters or the available range of characters finish.
	 */
	if( (encode == 255) || ( mostFreqPair == "Not Found") )
	{
		return;
	}

	/* Replace this most frequent frequent pair of characters by the encode in the whole string. */
	long long index =-2 ;
	while(index != string::npos)
	{
		if( index == -2)
		{
			index = compressedStr.find(mostFreqPair);
		}
		else
		{
			index = compressedStr.find(mostFreqPair , index+1); /* to start searching after the replaced 2 bytes. */
		}

		if(index != string::npos)
		{
			compressedStr.erase(index,2);
			compressedStr.insert(index, string(1,encode) );
		}
	}

	/* Store this most frequent pair and its corresponding encode into an EncryptionPair
	 * object and push this object to the encryption table.
	 */
	EncryptionPair pair = {mostFreqPair , encode};
	g_encryptionTable.push_back(pair);

	/* Call the function recursively to repeat the process. */
	bytePairEncoding(compressedStr , encode+1);
}

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
void bytePairDecoding(string &decompressedStr)
{
	/* If the encryption table is empty then return. */
	if(g_encryptionTable.empty())
	{
		return;
	}
	/* Loop on the elements of the encryption table and replace each encoded byte by its original two bytes. */
	for( int i = (g_encryptionTable.size()-1)  ; i >=0 ; i-- )
	{
		long long  index =-2 ;
		while(index != string::npos)
		{
			if(index == -2)
			{
				index = decompressedStr.find(g_encryptionTable.at(i).encryptedPair);
			}
			else
			{
				/* to start searching after the replaced byte. */
				index = decompressedStr.find(g_encryptionTable.at(i).encryptedPair , index+2);
			}

			if(index != string::npos)
			{
				decompressedStr.erase(index,1);
				decompressedStr.insert(index, g_encryptionTable.at(i).orignalPair );
			}
		}
	}
}
