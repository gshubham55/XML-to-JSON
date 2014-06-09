Well formed XML
========

The following assumptions are taken which define the "well formendness" of XML data( <a src="http://en.wikipedia.org/wiki/XML#Well-formedness_and_error-handling/">wikipedia-Well formed XML</a> ) :

1. The document contains only properly encoded legal Unicode characters

2. None of the special syntax characters such as < and & appear except when performing their markup-delineation roles

3. The begin, end, and empty-element tags that delimit the elements are correctly nested, with none missing and none overlapping

4. The element tags are case-sensitive; the beginning and end tags must match exactly. Tag names cannot contain any of the characters !"#$%&'()*+,/;<=>?@[\]^`{|}~, nor a space character, and cannot start with -, ., or a numeric digit.

5. A single "root" element contains all the other elements
		(The code works just fine even without any such root tag)

Transfromation
========

Transformation is shown below:

N	XML	                         	 	 JSON	
1	<e/>	                     	 	"e": null	
2	<e>text</e>	                  	 	"e": "text"
3	<e name="value" />	          	 	"e":{"@name": "value"}
4	<e name="value">text</e>	  	 	"e": { "@name": "value", "#text": "text" }	
5	<e> <a>text</a> <b>text</b> </e>	"e": { "a": "text", "b": "text" }	
6	<e> <a>text</a> <a>text</a> </e>	"e": { "a": ["text", "text"] }	
7	<e> text <a>text</a> </e>			"e": { "#text": "text", "a": "text" }

Support
========

It parses through/ support:

1. XML declaration
		<?xml version="1.0" encoding="UTF-8"?>

2. Comments
		<!-- <book>Happy Potter</book> -->

3. Multi-level tags 

4. ** Grouping to tags of same name even when they dont occur in sequence (in the same level) and this feature can easily be switched of if required.
	~	<book>Harry Potter</book>
	~	<book>Harry Potter</book>
		<fruit> Mango </fruit>
	~	<book>Harry Potter</book>
	All 3 books will be clubbed together in the same array.

5. Empty tags, Attributes and all other fundamentals. 

How To
========
1. Paste the input XML in input.xml file (File name can be changed in the main.cpp MACRO)

2. Run main.cpp. (In terminal, type: g++ main.cpp)

3. Get the output in the output.json file

