//File: example.cpp
//Info: Contains examples how to use the library 

#include <string>
#include <iostream>

#include "../include/HoardXML.h"

//function: main
//note: contains the examples how to use the library. 
int main(int argc, char**argv)
{
	//At first we want to create a single tag
	//Output its serialization to see what it does. 
	HoardXML::Tag t1("test");
	t1.SetAttribute("testattribute", "cant touch this");
	t1.SetData("Hello World!");
	std::cout << t1.Serialize() << std::endl;

	//Next thing is: lets add a child to the tag t1, give it some values and output it again!
	std::cout << "--------------------------------" << std::endl;
	HoardXML::Tag t2("moartest");
	t2.SetData("I can do more layers!");
	t2.SetAttribute("a", "I am an attribute");
	t1.AddChild(t2);
	std::cout << t1.Serialize() << std::endl;

	//since t2 is now part of t1, we cann access it via [].
	//only problem is that it returns a vector, so we need the first element 
	std::vector<HoardXML::Tag*> ts = t1["moartest"];
	std::cout << "content of t2: " << ts[0]->GetData() << std::endl;

	//we also can append stuff to these retured things, since we have pointers to them 
	HoardXML::Tag t3("foo");
	ts[0]->AddChild(t3);
	std::cout << "fooooo" << std::endl;
	//now we can do that: 
	t1["moartest.foo"][0]->SetData("woop!");
	
	//print it to see it
	std::cout << t1.Serialize();

	//lets add all this to a Document and save it. 
	HoardXML::Document d;
	d.AddChild(t1);
	d.Save("test.xml");

	//and because we can, lets load it into another Document
	std::cout << "\n\n\n"; 
	HoardXML::Document d2("test.xml");
	std::cout << "______________________________________" << std::endl;
	std::cout << d2.Serialize() << std::endl;

	//the rest here basically is stuff you can see above, used for testing

	HoardXML::Tag t4("single");
	t4.SetEmptyTag(true);
	t4.SetAttribute("morning", "true");
	t1.AddChild(t4);
	std::cout << "_-_-_-_-_-_-_-_-__-_-_-_----_--_-" << std::endl;
	std::cout << t1.Serialize() << std::endl;

	//The next thing loads a file that uses alot of nested tags. see if its parsed correctly. if yes: yay
	HoardXML::Document ntdoc("testread.xml");
	std::cout << "Does the nest thing have nested tags? if yes: YAAY\n\n\n" << std::endl;
	std::cout << ntdoc.Serialize() << std::endl;		
}
