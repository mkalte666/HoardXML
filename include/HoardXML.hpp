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
#include <regex>
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
	: name(""), isSingleTag(false) 
	{
	
	}
	
	//constructor: Tag
	//note: Creates an Tag with the name "tagName"
	//param: name: name of the new Tag
	Tag(std::string tagName) 
	: name(tagName), isSingleTag(false)
	{
		
	}
	
	//constructor: Tag
	//note: Creates an Tag with the name "tagName" and parses the string "toParse" to fill itself with content
	//param: 	name: name of the new Tag
	//		toParse: the string to parse into the tag. 
	Tag(std::string tagName, std::string toParse) 
	: name(tagName), isSingleTag(false)
	{
		//_ParseData(toParse);
	}

	//destructor: ~Tag
	//note: does not much since all subclasses are self-deliting
	~Tag() 
	{
		//nothing 
	}
	

	//function: GetName
	//note: returns the name of the Tag
	std::string GetName() 
	{
		return name;
	}
	
	//function: SetName
	//note: Sets the name of the Tag. 
	//param: 	newName: name to set the tags to. 
	void SetName(std::string newName) 
	{
		name=newName;
	}

	//function: GetAttribute 
	//note: Returns a specific attribute. Returns an empty string if the Attribute is unknown.
	std::string GetAttribute(std::string attribute)
	{
		return attributes[attribute];
	}

	//function: SetAttribute
	//note: sets a specific attribute to value "data"
	//param: 	attribute: the attribute to set 
	//		data: the data to set the value of the attribute to.		
	void SetAttribute(std::string attribute, std::string data) 
	{
		attributes[attribute] = data;
	}

	//function: GetData
	//note: returns the data of this tag 
	std::string GetData() 
	{
		return data;
	}

	//function: SetData
	//note: sets the data of this Tag
	//param:	newData: data to set 
	void SetData(std::string newData) 
	{
		data = newData;
	}

	//function: GetAttributes()
	//note: returns a reference to the attributes of the tag. All of them, packed in a std::map 
	std::map<std::string, std::string>& GetAttributes()
	{
		return attributes;
	}

	//function: GetChildren
	//note: returns a reference to the children of this tag. 
	std::vector<Tag>& GetChildren()
	{
		return children;
	}
	
	//function: SetSingleTag
	//note: Sets this tag to be witout data and children. 
	//note: it will be Serialized as <name [attributes]\>. 
	//note: SETTING TO TRUE ERASES ALL DATA AND CHILD ELEMENTS!
	//param:	b: Parameter to set isSingleTag to.
	void SetSingleTag(bool b)
	{
		isSingleTag = b;
		if(isSingleTag) {
			data = "";
			children.clear();
		}
	}

	//function: GetSingleTag
	//note: Returns if this tag is stand-alone without data and children 
	bool GetSingleTag() 
	{
		return isSingleTag;
	}
	

	//function: AddChild
	//note: Adds a child to this tag. 
	//param:	c: Tag to add
	void AddChild(Tag c) 
	{
		children.push_back(c);
	}
 	
	//function: Serialize 
	//note: serializes the tag and its subtags including the data into a xml-tree
	//param:	depth: The depth of this operation. causes depth times '/t's added at the biginning to every line of the output 
	std::string Serialize(int depth=0)
	{
		std::string tabs;
		for (int i = 0; i<depth;i++) {
			tabs+='\t';
		}
		std::string result;
		result += tabs+std::string("<")+name+std::string(" ");
		for (auto elem = attributes.begin(); elem!=attributes.end(); elem++) {
			result += elem->first+std::string("=\"")+elem->second+std::string("\" ");
		}
		if(isSingleTag) {
			result+=std::string("/>\n");
			return result;
		}
		result+=std::string(">\n");
		for (Tag t : children) {
			result+=t.Serialize(depth+1);
		}
		result += tabs+data+std::string("\n");
		result+=tabs+std::string("</")+name+std::string(">\n");
	}
	
	//operator: []
	//note: reurns a vector of pointers to child tags with matching namepath.
	//	 if there are multiple tags of one name in a layer AND you want tags from deeper layers, it will use the first known.
	//	 if you want to get all of them, use GetChildren and search them yourself. 
	//	 syntax is name(.name2(.name3...))
	//param:	tagName: name of the tag to get, as dom-path (foo.bar.blahrg) 	
	std::vector<Tag*> operator[](std::string tagName)  
	{
		std::vector<Tag*> resultlist;
		std::regex nameRE("[.]?([\\w\\d]+)([\\w\\d.]*)");
		std::smatch m;
		if(std::regex_search(tagName, m, nameRE)) {
			if(m[1]!="") {
				for(auto t = children.begin(); t!=children.end(); t++) {
					if(m[1]==t->GetName()) {
						if(m[2]=="") {
							resultlist.push_back(&(*t));
						}
						else {
							std::vector<Tag*> l = (*t)[m[2]];
							resultlist.insert(resultlist.end(), l.begin(), l.end());
							break;
						}
					}
				}
			}
		}
		return resultlist;
	}
private:
	//var: name. Holds the name of this tag. 
	std::string name;
	//var: data. Holds the data of this tag. 
	std::string data;
	//var: attributes. Holds the attributes of this tag. 
	std::map<std::string, std::string> attributes;
	//var: children. Holds the children of this tag. 
	std::vector<Tag> children;
	//var: isSingleTag. Holds if it is a single tag without data. 
	bool isSingleTag;
protected:
	

};

}; //end of namespace HoardXML
