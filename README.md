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

<table>
<tbody><tr>
<td bgcolor="#CCCCCC"><strong>Pattern</strong>
</td>
<td bgcolor="#f0f0f0"><strong>XML</strong>
</td>
<td bgcolor="#CCCCCC"><strong>JSON</strong>
</td>
</tr>

<tr>
<td bgcolor="#CCCCCC">1</td>
<td bgcolor="#f0f0f0">
<code>&lt;e/&gt;</code>
</td>
<td bgcolor="#CCCCCC">
<code>"e": null</code>
</td>
</tr>

<tr>
<td bgcolor="#CCCCCC">2</td>
<td bgcolor="#f0f0f0">
<code>&lt;e&gt;text&lt;/e&gt;</code>
</td>
<td bgcolor="#CCCCCC">
<code>"e": "text"</code>
</td>
</tr>

<tr>
<td bgcolor="#CCCCCC">3</td>
<td bgcolor="#f0f0f0">
<code>&lt;e name="value" /&gt;</code>
</td>
<td bgcolor="#CCCCCC">
<code>"e":{"@name": "value"}</code>
</td>
</tr>

<tr>
<td bgcolor="#CCCCCC">4</td>
<td bgcolor="#f0f0f0">
<code>&lt;e name="value"&gt;text&lt;/e&gt;</code>
</td>
<td bgcolor="#CCCCCC">
<code>"e": {
  "@name": "value",
  "#text": "text"
}</code>
</td>
</tr>

<tr>
<td bgcolor="#CCCCCC">5</td>
<td bgcolor="#f0f0f0">
<code>&lt;e&gt;
  &lt;a&gt;text&lt;/a&gt;
  &lt;b&gt;text&lt;/b&gt;
&lt;/e&gt;</code>
</td>
<td bgcolor="#CCCCCC">
<code>"e": { 
  "a": "text",
  "b": "text"
}</code>
</td>
</tr>

<tr>
<td bgcolor="#CCCCCC">6</td>
<td bgcolor="#f0f0f0">
<code>&lt;e&gt;
  &lt;a&gt;text&lt;/a&gt;
  &lt;a&gt;text&lt;/a&gt;
&lt;/e&gt;</code>
</td>
<td bgcolor="#CCCCCC">
<code>"e": {
  "a": ["text", "text"]
}</code>
</td>
</tr>

<tr>
<td bgcolor="#CCCCCC">7</td>
<td bgcolor="#f0f0f0">
<code>&lt;e&gt;
  text
  &lt;a&gt;text&lt;/a&gt;
&lt;/e&gt;</code>
</td>
<td bgcolor="#CCCCCC">
<code>"e": {
  "#text": "text",
  "a": "text"
}</code>
</td>
</tr>
</tbody></table>
Support
========

It parses through/ support:

1. XML declaration </br>
		<code><?xml version="1.0" encoding="UTF-8"?></code>

2. Comments </br>
		<code><!-- <book>Happy Potter</book> --></code>

3. Multi-level tags 

4. ** Grouping to tags of same name even when they dont occur in sequence (in the same level) and this feature can easily be switched of if required.
	<code>~	<code><book></code>Harry Potter<code></book></code></code>
	<code>~	<code><book></code>Harry Potter<code></book></code></code>
	<code>	<code><fruit></code> Mango <code></fruit></code></code>
	<code>~	<code><book></code>Harry Potter<code></book></code></code>
	All 3 books will be clubbed together in the same array.

5. Empty tags, Attributes and all other fundamentals. 

How To
========
1. Paste the input XML in input.xml file (File name can be changed in the main.cpp MACRO).

2. Run main.cpp. (In terminal, type: <code>g++ main.cpp</code>)

3. Get the output in the output.json file.