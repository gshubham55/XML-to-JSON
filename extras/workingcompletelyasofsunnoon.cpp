#include <iostream>
#include <stdio.h>
#include <fstream>
#include <string>
#define	input "input.xml"
using namespace std;
ofstream out;

string trimString(string & str)
{
	str.erase(0, str.find_first_not_of(' '));
	str.erase(str.find_last_not_of(' ')+1,str.length());
	return str;
}

void removeDeclarationAndComments(string &s)
{
	bool dec = 0,quoteFlag=0;
	for (int i=0;i<s.length();i++)
	{
		if(s[i]=='"' &&quoteFlag==1) 
		{
			quoteFlag = 0;
			continue;
		}
		if(quoteFlag) {
			continue;
		}
		if(s[i]=='"'&&quoteFlag==0) 
			{
				quoteFlag = 1;
				continue;
			}
		if (s[i]=='?')
		{
			s.erase();
			return;
		}
		else if(i+2<s.length()&&s[i]=='!'&&s[i+1]=='-'&&s[i+2]=='-')
		{
			s.erase();
			return;
		}

	}
}

string tab(int n)
{
	string ret;
	for(int i=0;i<n;i++)
		ret+="  ";
	return ret;
}


string getTagName(string s)
{
	string tag;
	bool collect = 0;
	for (int i=0;i<s.length();i++)
	{
		if(s[i]=='>') break;
		if(collect) tag+=s[i];
		if(s[i]=='<') collect=1;
	}
	trimString(tag);
	return tag;
}

bool stringHasChildren(string s)
{
	for(int i=0;i<s.length();i++)
	{
		if(s[i]=='<') return 1;
	}
	return 0;
}

bool isOpeningtag(string s,int pos)
{
	for(int i = pos-1;i>=0;i--)
	{
		if(s[i]=='<') return true;
		else if(s[i]==' ') continue;
		else return false;
	}
	return false;
}

bool isClosingtag(string s,int pos)
{
	int i=pos-1;
	for (;i>=0;i--)
	{
		if(s[i]==' ') continue;
		else if(s[i]=='<') return false;
		else if (s[i]=='/') break;
		else return false;
	}
	return isOpeningtag(s,i);
}


int correctPosOfAngClosingBracket(string s,int pos)
{
	for(int i=pos;i<s.length();i++)
	{
		if(s[i]=='>') return i;
	}
}

int correctPosOfAngClosingBracketOrSlash(string s,int pos ) // s is attr, when called
{
	for(int i=pos;i<s.length();i++)
	{
		if(s[i]=='>'||s[i]=='/') return i;
	}
}

int posOfEndOfApprTag(string s,string firsttag)
{
	int childrenWithSametag = 0;
	int start = 0;
	while(1)
	{
		int pos = s.find(firsttag, start);
		if(pos<0||pos>s.length()) 
		{
			cout<<"closingTag not found "<<endl;
			return pos;
		}
		bool openingTag = isOpeningtag(s,pos);
		bool closingTag = isClosingtag(s,pos);
		if(openingTag)
		{
			childrenWithSametag++;
			start=pos+firsttag.length();
			continue;
		}
		else if(closingTag)
			{
				childrenWithSametag--;
				start=pos+firsttag.length();
				if(childrenWithSametag==0)
				{
					return correctPosOfAngClosingBracket(s,pos);
				} 
			}
		else 
		{
			start=pos+firsttag.length();
		}
	}
}

int checkIfNullDataSet(string &s) // returns position of '/'
{
	bool null = false;
	int i;
	bool quoteFlag = false;
	for (i=0;i<s.length();i++)
	{
		if(s[i]=='"' &&quoteFlag==1) 
		{
			quoteFlag = 0;
			continue;
		}
		if(quoteFlag) {
			continue;
		}
		if(s[i]=='"'&&quoteFlag==0) 
			{
				quoteFlag = 1;
				continue;
			}
		if(s[i]=='>') break;
		if(s[i]=='/') 
		{
			null = true;
			break;
		}
	}
	if(null) return i;
	else return 0;
}

string checkForTextBeforeTags(string &s)
{
	string text;
	trimString(s);
	for(int i=0;i<s.length();i++)
	{
		if(s[i]=='<') 
			{
				s.erase (0,i);
				trimString(text);
				trimString(s);
				return text;
			}
		text+=s[i];
	}
	return text;
}

class node
{
	public:
		string tag,data,text;
		string key,value;
		node* child;
		node* sibling;
		node* attr;
		node()
		{
			tag='\0';
			data='\0';
			text='\0';
			key='\0';
			value='\0';
			child=NULL;
			sibling=NULL;
			attr=NULL;
		}
};

class chart
{
	public:
		node* root;
		int tabs;
		void getChildren(node* &N,string s);
		void define(node* &N,string s);
		void printouter(node* N);
		void printinner(node* &cur,bool makeArray);
		void groupSameTags(node* &N);
		void print()
		{
			out<<tab(tabs)<<"{"<<endl;
			tabs++;
			printouter (root);
			tabs--;
			out<<tab(tabs)<<"}"<<endl;
		}
		chart(string s)
		{
			tabs=0;
			define(root,s);
		}
};

void replaceFirstOccurance( string &s, const string &search, const string &replace ) { // Later. remove the loop
    for( size_t pos = 0; ; pos += replace.length() ) {
        pos = s.find( search, pos );
        if( pos == string::npos ) break;
        s.erase( pos, search.length() );
        s.insert( pos, replace );
        break;
    }
}

void removeSlashes(string &attr)
{
	bool quoteFlag = false;
	for (int i=0;i<attr.length();i++)
	{
		if(attr[i]=='"' &&quoteFlag==1) 
		{
			quoteFlag = 0;
			continue;
		}
		if(quoteFlag) {
			continue;
		}
		if(attr[i]=='"'&&quoteFlag==0) 
			{
				quoteFlag = 1;
				continue;
			}
		if(attr[i]=='/') attr.erase(i);
	}
}

void getAttributes(node * &N,string &tagname,string &s)
{
	trimString(tagname);
	trimString(s);
	string intialTagname = tagname;
	bool collect = 0;
	int i;
	string newtag,attr;
	for(i=0;i<tagname.length();i++)
	{
		if((tagname[i]==' '||tagname[i]=='<')&&collect == 0) continue;
		else if (collect==0) {
			collect = 1;
			newtag+=tagname[i];
		}
		else if((isalnum(tagname[i])||tagname[i]=='/'||tagname[i]=='-'||tagname[i]=='_'||tagname[i]=='.')&&collect==1) newtag+=tagname[i];
		else break;
	}
	for (int j=i;j<tagname.length();j++)
	{
		attr+=tagname[j];
	}
	replaceFirstOccurance(s,intialTagname,newtag);
	trimString(newtag);
	trimString(attr);
	removeSlashes(attr);
	tagname=newtag;
	N->tag=newtag;
	if(attr[0]!='\0') 
	{
		string key,value;
		node* baseAttr = new node; 
		bool baseCaseUnderConsideration=true;
		while (attr[0]!='\0')
		{
			node* temp = new node;
			int posOfEqualSign = attr.find('=');
			int posOfFirstQuotes = attr.find('"');
			int posOfSecondQuotes = attr.find('"',posOfFirstQuotes+1);
			key = attr.substr(0,posOfEqualSign);
			value = attr.substr(posOfEqualSign + 1, posOfSecondQuotes - posOfEqualSign + 1 );
			temp->key = key;
			temp->value = value;
			if(baseCaseUnderConsideration)
			{
				baseAttr = temp;
				baseCaseUnderConsideration=false;
			}
			else 
			{
				node* curAttr = new node;
				curAttr = baseAttr;
				while(curAttr->attr)
					curAttr = curAttr->attr;
				curAttr->attr=temp;

			}
			N->attr=baseAttr;
			attr.erase(0,posOfSecondQuotes+1);
			trimString(attr);
		}	
	}
}

string removeAttributes(string tagname,string &s) //tagname is the firsttag
{
	trimString(tagname);
	trimString(s);
	string intialTagname = tagname;
	bool collect = 0;
	int i;
	string newtag,attr;
	for(i=0;i<tagname.length();i++)
	{
		if((tagname[i]==' '||tagname[i]=='<')&&collect == 0) continue;
		else if (collect==0) {
			collect = 1;
			newtag+=tagname[i];
		}
		else if((isalnum(tagname[i])||tagname[i]=='/'||tagname[i]=='-'||tagname[i]=='_'||tagname[i]=='.')&&collect==1) newtag+=tagname[i];
		else break;
	}
	for (int j=i;j<tagname.length();j++)
	{
		attr+=tagname[j];
	}
	return newtag;
}

void chart::define(node* &N, string xml)
{

	//get TAGNAME
	node* temp = new node;
	string tagname = getTagName(xml);
	temp->tag = tagname;

	getAttributes(temp,tagname,xml);

	int null;
	null = checkIfNullDataSet(tagname);
	if(null)
	{
		tagname.erase(null);
		temp->tag = tagname;
		N=temp;
		return;
	}

	// get data inside the tags
	int posDataStart = xml.find_first_of('>')+1;
	int posDataEnd = xml.find_last_of('<');
	string data = xml.substr(posDataStart, posDataEnd - posDataStart);
	trimString(data);
	temp->data = data;

	string text;
	text = checkForTextBeforeTags(data);
	
	temp->text = text;

	// check if tha data is plain or has childrens
	bool itHasChildren;
	itHasChildren = stringHasChildren(data);

	if(itHasChildren)
		getChildren(temp,data);
	N = temp;
	
}

void chart::getChildren(node* &N,string s)
{
	node * baseChild = new node;
	trimString(s);
	bool firstChildUnderConsideration = 1;
	while(1)
	{
		// get tagname and the end position of the string containing the first child
		string firsttag = getTagName(s);

		firsttag = removeAttributes(firsttag,s); 

		int null;
		null = checkIfNullDataSet(s); // null has the position of the '/' , else 0
		int posEnd;
		if(null)
			posEnd = correctPosOfAngClosingBracket(s,null);
		else 
			posEnd = posOfEndOfApprTag(s,firsttag);
		if (posEnd == -1) return;

		// get the string containing child 
		string childStr = s.substr(0,posEnd+1);
		node* temp = new node;

		// define the child
		define(temp,childStr);

		// create the basechild (firstchild) and its siblings
		if (firstChildUnderConsideration)
		{
			baseChild = temp;
			firstChildUnderConsideration = false;
		}
		else 
		{
			node * curChild = new node;
			curChild = baseChild;
			while (curChild->sibling)
			{
				curChild = curChild->sibling;
			}
			curChild->sibling = temp;
		}

		if(posEnd==s.length()-1) break;
		
		// if the string contains more children, remove the first child string and operate again.
		s.erase (0,posEnd+1); 
	}
	N->child = baseChild;
}

string brackets(string s)
{
	return "\""+s+"\"";
}

void insert_after(node* &list_node, node* &new_node)
{
	new_node->sibling = list_node->sibling;
	list_node->sibling = new_node;
}

void deleteBwhichIsAfterA(node* &temp, node* &cur2)
{
	temp->sibling = cur2->sibling;
}


void chart::groupSameTags(node* &N)
{
	node * cur1, * cur2;
	cur1 = N;
	while (cur1)
	{
		while (cur1->sibling && cur1->tag == cur1->sibling->tag)
			cur1=cur1->sibling;
		cur2 = cur1;
		while(cur2)
		{
			node* temp = cur2;
			cur2= cur2->sibling;
			if(cur1&&cur2&&cur1->tag == cur2->tag)
			{
				deleteBwhichIsAfterA(temp,cur2);
				insert_after(cur1,cur2);
				break;
			}
		}
		cur1=cur1->sibling;
	}
}



void chart::printinner(node* &cur, bool makeArray)
{
	string text = cur->text;
	string attr;
	if(cur->attr)
		attr = cur->attr->key;
	if(!cur->child&&attr[0]=='\0')
		{
			string data = trimString(cur->data);
			if(makeArray) 
			{
				out<<tab(tabs)<<"{"<<endl;
				tabs++;
				if(data[0]=='\0')
					out<<tab(tabs)<<"\"#text\" : "<<"null";
				else 
					out<<tab(tabs)<<"\"#text\" : "<<brackets(trimString(data));
				tabs--;
				out<<endl<<tab(tabs)<<"}";

					
			}
			else if (data[0]!='\0' )out<<" "<<brackets(trimString(cur->data));
			else out<<" "<<"null";
			if(!makeArray && cur->sibling) out<<",";
			else if (makeArray && cur->sibling&& trimString (cur->tag) ==trimString (cur->sibling->tag)) out<<",";
			out<<endl;
		}
	else
	{
		if(!makeArray)out<<endl;
		out<<tab(tabs)<<"{"<<endl;
		tabs++;
		string text = cur->text;
		if(cur->attr)
		{
			node * traveseAttributes = cur;
			while(traveseAttributes->attr)
				{

					out<<tab(tabs)<<"\"@"<<trimString( traveseAttributes->attr->key)<<"\" : "<<trimString (traveseAttributes->attr->value);
					if(text[0]!='\0'||cur->child||traveseAttributes->attr->attr) out<<",";
					out<<endl;
					traveseAttributes = traveseAttributes->attr;
				}
		}
		if(text[0]!='\0')
		{
			out<<tab(tabs)<<"\"#text\""<<" : "<<brackets(trimString (cur->text));
			if(cur->child) out<<",";
			out<<endl; 
		}
		if(cur->child) printouter(cur->child); 
		tabs--;
		out<<tab(tabs)<<"}";
		if(!makeArray && cur->sibling)
			{
				out<<",";
			}
		else if(makeArray && cur->sibling&&trimString (cur->tag)==trimString (cur->sibling->tag))
		{
			out<<",";
		}
		out<<endl;
	}
}
void chart::printouter(node* N)
{
	node* cur = N;
	groupSameTags(cur);
	bool makeArray = false;
	string tag=cur->tag;
	trimString (tag);
	if(tag[0]=='\0') return;
	out<<tab(tabs)<<brackets(tag)<<":";
	if(cur->sibling&&trimString (cur->tag)==trimString (cur->sibling->tag))
	{
		makeArray = true;
	}
	if(!makeArray)
	{
		printinner(cur,makeArray);		

	}
	else 
	{
		out<<endl;
		out<<tab(tabs)<<"["<<endl;
		tabs++;
		printinner(cur,makeArray);
		while(cur->sibling&&trimString (cur->tag)==trimString (cur->sibling->tag))
			{
				printinner(cur->sibling, makeArray);
				cur = cur->sibling;
			}
		tabs--;
		out<<tab(tabs)<<"]";
		if(cur->sibling) out<<",";
		out<<endl;
	}
	if (cur->sibling)
	{
		printouter(cur->sibling);
	}
}

int main()
{
	ifstream xmlfile(input);
	out.open ("output.json");
	string temp,xml;
	while(getline(xmlfile, temp)) 
	{
		removeDeclarationAndComments(temp);
		xml += temp;
		xml += " ";
	}
	xmlfile.close();
	trimString(xml);
	chart c(xml);
	c.print();
	out.close();
}