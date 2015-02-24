# HoardXML
A Small and simple header based XML library. 

## How to use it
HoardXML is headerbased, so you only need the HoardXML.h file to use it in your project. However, please dont forget the terms of the MIT license: credit please :D

## How to USE it
The lib uses 2 classes, and you need to know them. 

### Tags
The first is `HoardXML::Tag`. Its, as it name sayes, a tag as you find it in XML. 
You can set/access attributes of Tags via the functions `SetAttribute(name,value)` and `GetAttribute(name)`.
Also you can set/get the Data (content) of a tag by `SetData(data)` and GetData
Adding childs to elements is possible by using `AddChild(c)`, where c is a child tag.

Currently there is no way to remove a Tag from a parent, but normally that isn't needed at all. Raw access to the children is provided by `GetChildren()`, wich returns a vector of tags. 

### Documents 
A document is not that different from a tag. In fact, `HoardXML::Document` is a child class of `HoardXML::Tag`.
So you have all functions of Tag, though they might not be that sensefull here. 
What is new is that you can Load a Document by passing a fstream or a filepath to its constructor, and that you can save it. 
`HoardXML::Tag::Save()` does use the method used for constructing the Document, however there are functions for saving into fstreams and directly via filepaths. 

### Thats it?
Yes, i am lazy. Im happy to forward you to the examples/example.cpp and to the header file cause they are commented. 
Also, beside the loading code (REGEX!) everything should be more or less well understandable

Greetings 
Malte "mkalte" Kießling
