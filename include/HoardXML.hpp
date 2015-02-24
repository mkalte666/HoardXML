//File: HoardXML.hpp
//Info: Contains the library "HoardXML", a header based library for XML loading
//Info: HoardXML does NOT follow the complete w3c definition of xml 1.0. It does not verify the document using a DTD, nor it needs any kind of version tag for the XML.

#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <map>
#include <list>

//namespace: HoardXML
//info: holds all the classes and functions used by HoardXML
namespace HoardXML {

//class: Tag
//info: Tag contains a document-tag. 
class Tag 
{
public:
	//constructor: Tag
	//note: Creates an empty Tag
	Tag() 
	{
	
	}
	
	//constructor: Tag
	//note: Creates an Tag with the name "tagName"
	//param: name: name of the new Tag
	Tag(std::string tagName) : name(tagName)
	{
		
	}
	
	//constructor: Tag
	//note: Creates an Tag with the name "tagName" and parses the string "toParse" to fill itself with content
	//param: name: name of the new Tag
	Tag(std::string tagName, std::string toParse) 
	{
	
	}
	
private:
	std::string name;
	std::map<std::string, std::string> attributes;
	std::list<Tag> children;
protected:
	

};

}; //end of namespace HoardXML