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
	virtual ~Tag() 
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
	
	//function: Load
	//note: Loads tag-content into this tag
	//param: 	toParse: data to parse
	void Load(std::string toParse)
	{
		static std::regex completeTagRE("(<\\s*/*\\s*[\\w-]*\\s*[!-%'-;=?-~\\s]*>)");
		std::smatch m;
		while(std::regex_search(toParse,m,completeTagRE)) {
			Tag newTag = _ParseTag(m[1]);
			//tag is invalid? remove and handle next
			if(newTag.GetName()=="") {
				toParse = m.suffix().str();
				continue;
			}
			//its a tag without content? add as child, remove and handle next
			if(newTag.GetSingleTag()) {
				toParse = m.suffix().str();
				AddChild(newTag);
				continue;
			}
			//this regex is generated so that it caputres the content between <name>[content]</name>
			std::regex thisTagRegex(std::string("(")+m[1]+std::string(")([\\w\\W]*)(<\\s*\\/\\s*"+newTag.GetName()+std::string("\\s*>)"));
			std::smatch m2;
			//Get the data inside of the tag. dont panic if we cant find an end tag. if we cant, it will be handeld as a tag without content.
			if(std::regex_search(toParse, m2, thisTagRegex)) {
				newTag.Load(m2[2]);
				toParse = m2.suffix().str();
			} else {
				toParse = m.suffix().str();
			}
			AddChild(newTag);
		}
		//Everything that survived the stuff above must be data
		SetData(toParse);
	}
	
	static _ParseTag(std::string toParse)
	{
		static std::regex tagRE("<\\s*([\\w-]*)\\s*([!-%'-;=?-~\\s]*)>");
		static std::regex attributeRE("([\\w-]*)=\"([!#-%'-;=?-~\\s]*)\"");
		static std::regex noContentRE("(/)");
		std::smatch m;
		if(std::regex_search(toParse,m,ragRE)) {
			Tag newTag(m[1]);
			std::string argsAndTypeString = m[2];
			std::smatch m2:
			while(std::regex_search(argsAndTypeString, m2, attributeRE)) {
				newTag.SetAttribute(m[1], m[2]);
				argsAndTypeString = m2.suffix().str();
			}
			if(std::regex_search(argsAndTypeString, m2, noContentRE)) {
				newTag.SetSingleTag(true);
			}
			return newTag;
		}
		return Tag();
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

//class: Document
//info: Document is the "root" tag of a xml. 	
//		while you can do nearly everything with Tag itself, document manages saving, loading and other stuff for you. 
//parent: Tag
class Document : public Tag
{
public:
	//constructor: Document
	//note: Creates an empty document
	Document() 
	: saveToRaw(false)
	{
	
	}
	
	//constructor: Document
	//note: Creates an document form a file with name "filename". 
	//		calling save without argbuments on this will save it to that exact file. 
	//param: the file to load from and save to by name
	Document(std::string filename)
	: saveToRaw(false)
	{
		std::ifstream infile(filename);
		std::string indata = std::string(std::istreambuf_iterator<char>(infile), std::istreambuf_iterator<char>());
		Load(indata);
	}
	
	//constructor: Document
	//note: Creates an document form a file given
	//		calling save without argbuments on this will save it to that exact file.
	//param:	file: the file to load from and save to by std::fstream
	Document(std::fstream file)
	: saveToRaw(true)
	{
		std::string indata = std::string(std::istreambuf_iterator<char>(file), std::istreambuf_iterator<char>());
		Load(indata);
	}
	
	//function: Save
	//note: Saves a Document. Uses method intended by constructor
	void Save() 
	{
		if(saveToRaw) {
			Save(rawSavefile);
		} else {
			Save(savefile);
		}
	}
	
	//function: Save
	//note: Saves a document to a file named by "filename"
	//param: 	filename: file to save to
	void Save(std::string filename)
	{
		std::fstream outfile(filename, std::ios::out);
		Save(outfile);
		outfile.close();
	}
	
	//function: Save
	//note: Saves a document to the given file
	//param: 	outfile: the file to save to
	void Save(std::fstream outfile)
	{
		outfile << Serialize();
	}
private:
	std::string savefile;
	std::fstream rawSavefile;
	bool saveToRaw;
protected:

}

}; //end of namespace HoardXML
