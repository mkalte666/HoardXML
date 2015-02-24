//File: example.cpp
//Info: Contains examples how to use the library 

#include <string>
#include <iostream>

#include "../include/HoardXML.hpp"

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

		
}
