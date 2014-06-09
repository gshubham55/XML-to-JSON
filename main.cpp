#include <iostream>
#include <stdio.h>
#include <fstream>
#include <string>
#define	input "input.xml"
using namespace std;
ofstream out;
#include "basicfunctions.h"
#include "classes.h"
#include "secondaryfunctions.h"
#include "attributesfunctions.h"
#include "classfunctions.h"

int main()
{ 
	ifstream xmlfile(input);
	out.open ("output.json");
	string temp,xml;
	xml+="<ROOT_TAG> ";
	while(getline(xmlfile, temp)) 
	{
		removeDeclarationAndComments(temp);
		xml += temp;
		xml += " ";
	}
	xml+=" </ROOT_TAG>";
	xmlfile.close();
	trimString(xml);
	chart c(xml);
	c.print();
	out.close();
}