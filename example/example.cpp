//File: example.cpp
//Info: Contains examples how to use the library 

#include <string>
#include <iostream>
#include <chrono>

#include "../include/HoardXML.h"
using namespace std;
using namespace std::chrono;
//function: main
//note: contains the examples how to use the library. 
int main(int argc, char**argv)
{
	
	//At first we want to create a single tag
	//Output its serialization to see what it does. 
	HoardXML::Tag t1("test");
	t1.SetAttribute("testattribute", "cant touch this");
	t1.SetData("Hello World!");
	for(int i=0;i<100;i++) {
		HoardXML::Tag t2("test2");
		t2.SetData("DAM DA DA DAM");
		for(int j=0;j<20;j++) {
			HoardXML::Tag t3("test2");
			t3.SetEmptyTag(true);
			if(j%5==0) {
				t3.SetAttribute("blub", "fish");
			}
			t2.AddChild(t3);
		}
		t1.AddChild(t2);
	}
	HoardXML::Document blub;
	blub.AddChild(t1);
	blub.Save("test.xml");
	std::chrono::high_resolution_clock::time_point curtime = std::chrono::high_resolution_clock::now();
	HoardXML::Document blubLoad("test.xml");
	std::chrono::high_resolution_clock::time_point newTime = std::chrono::high_resolution_clock::now();
	auto timeLeft=std::chrono::duration_cast<std::chrono::duration<double>>(newTime-curtime).count();
	std::cout << "took me: " << timeLeft << std::endl;
	blubLoad.Save("test2.xml");
}
