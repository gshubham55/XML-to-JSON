#include <iostream>
#include <stdio.h>
#include <fstream>
#include <string>
#define	input "XML.txt"
using namespace std;

string trimString(string & str)
{
	str.erase(0, str.find_first_not_of(' '));
	str.erase(str.find_last_not_of(' ')+1);
	return str;
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
		if(s[i]=='<') return false;
		if (s[i]=='/') break;
	}
	return isOpeningtag(s,i);
}


int correctPosOfAngClosingBracket(string s,int pos)
{
	// cout<<"s "<<s<<"pos "<<pos<<endl;
	for(int i=pos;i<s.length();i++)
	{
		if(s[i]=='>') return i;
	}
	// cout<<"wbke"<<endl;
	// 
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
			cout<<"closingTag not found"<<endl;
			return pos;
		}
		bool openingTag = isOpeningtag(s,pos);
		bool closingTag = isClosingtag(s,pos);
		// cout<<openingTag<<" "<<closingTag<<" "<<pos<<endl;
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
	for (i=0;i<s.length();i++)
	{
		if(s[i]=='/') 
		{
			null = true;
			break;
		}
	}
	// s.erase (i);
	// return null;
	if(null) return i;
	else return 0;
}

string checkForTextBeforeTags(string &s)
{
	string text;
	trimString(s);
	for(int i=0;i<s.length();i++)
	{
		if(s[i]==' ')continue;
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
		void getChildren(node* &N,string s);
		void define(node* &N,string s);
		void print(node* N);
		void printcomplex(node* &cur,bool makeArray);
		chart(string s)
		{
			define(root,s);
		}
};

void replaceFirstOccurance( string &s, const string &search, const string &replace ) { // Later. remove the loop
    for( size_t pos = 0; ; pos += replace.length() ) {
        // Locate the substring to replace
        pos = s.find( search, pos );
        if( pos == string::npos ) break;
        // Replace by erasing and inserting
        s.erase( pos, search.length() );
        s.insert( pos, replace );
        break;
    }
}

void getAttributes(node * &N,string &tagname,string &s)
{
	trimString(tagname);
	trimString(s);
	string intialTagname = tagname;
	// cout<<tagname<<endl;
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
		else if((isalnum(tagname[i])||tagname[i]=='/')&&collect==1) newtag+=tagname[i];
		else break;
	}
	for (int j=i;j<tagname.length();j++)
	{
		attr+=tagname[j];
	}
	replaceFirstOccurance(s,intialTagname,newtag);
	trimString(newtag);
	trimString(attr);
	tagname=newtag;
	N->tag=newtag;
	if(!attr.empty()) 
	{
		string key,value;
		node* baseAttr = new node; 
		bool baseCaseUnderConsideration=true;
		while (!attr.empty())
		{
			node* temp = new node;
			int posOfEqualSign = attr.find('=');
			int posOfFirstQuotes = attr.find('"');
			int posOfSecondQuotes = attr.find('"',posOfFirstQuotes+1);
			key = attr.substr(0,posOfEqualSign);
			value = attr.substr(posOfEqualSign + 1, posOfSecondQuotes- posOfFirstQuotes + 1 );
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

			// cout<<key[count]<<"|"<<value[count]<<endl;
			// count++;
			N->attr=baseAttr;
			// cout<<baseAttr->key<<endl;
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
	// cout<<tagname<<endl;
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
		else if((isalnum(tagname[i])||tagname[i]=='/')&&collect==1) newtag+=tagname[i];
		else break;
	}
	for (int j=i;j<tagname.length();j++)
	{
		attr+=tagname[j];
	}
	// replaceFirstOccurance(s,intialTagname,newtag);
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
	
	// cout<<"def "<<N<<" "<<" "<<N->child<<" "<<data<<endl;
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

		firsttag = removeAttributes(firsttag,s); //Later

		int null;
		null = checkIfNullDataSet(firsttag); // null has the position of the '/' , else 0
		int posEnd;
		if(null)
			posEnd = correctPosOfAngClosingBracket(s,null);
		else 
			posEnd = posOfEndOfApprTag(s,firsttag);
		
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
	// cout<<N->child<<endl;
}
string tab(int n)
{
	string ret;
	for(int i=0;i<n;i++)
		ret+="  ";
	return ret;
}
int tabs = 0;
// bool makeArray = false;
// void chart::printcomplex(node* &cur, bool makeArray)
// {
// 		cout<<tab(tabs)<<"{"<<endl;
// 		// cout<<cur<<endl;
// 		if(cur&&cur->attr) 
// 		{
// 			node * traveseAttributes = cur;
// 			tabs++;
// 			while(traveseAttributes->attr)
// 				{

// 					cout<<tab(tabs)<<trimString( traveseAttributes->attr->key)<<" : "<<trimString (traveseAttributes->attr->value)<<" ,"<<endl;
// 					traveseAttributes = traveseAttributes->attr;
// 				}
// 			// if(!cur->attr->sibling)
// 				// tabs--;
// 			// cout<<"here";
// 		}
// 		// cout<<endl;

// 		if(cur->child) 
// 		{
// 			string text = cur->text;
// 			if(text.empty())
// 				print(cur->child);
// 			else 
// 			{
// 				cout<<"{"<<endl;
// 				cout<<"#text"<<" : "<<trimString(text)<<endl;
// 				print(cur->child);
// 				cout<<"}"<<endl;
// 			}
// 		}
// 		if(!cur->child) 
// 		{
// 			tabs--;
// 			cout<<tab(++tabs)<<"#text"<<" : "<<trimString (cur->data)<<endl; // no ,
// 			if(!makeArray)
// 				tabs--;
// 		} 
// 		if(makeArray)tabs--;
// 		cout<<tab(tabs)<<"}";
// 		if(makeArray)
// 			{
// 				tabs++;
// 			}
// 		if(cur->sibling&&cur->tag==cur->sibling->tag) cout<<",";
// 		cout<<endl;
// 		// tabs--;
// }

// void chart::print(node* N)
// {
// 	node* cur = N;
// 	bool plain=false;
// 	bool makeArray = false;
// 	if(cur->sibling&&cur->tag==cur->sibling->tag)
// 	{
// 		makeArray = true;
// 	}
// 	string text = cur->text;
// 	if(cur->child||cur->attr||(!text.empty() &&(cur->child||cur->attr))) 
// 	{
// 		cout<<tab(tabs)<<trimString (cur->tag)<<":";
// 		if(makeArray)
// 		{
// 			// cout<<endl<<"(complex starts)"<<endl;

// 			cout<<"["<<endl;
// 			tabs++;
// 		} 
// 		cout<<endl;
// 		printcomplex(cur,makeArray);
// 		if(makeArray)
// 		{
// 			while(cur->sibling&&cur->tag==cur->sibling->tag)
// 			{
// 				tabs--;
// 				printcomplex(cur->sibling, makeArray);
// 				cur = cur->sibling;
// 			}
// 			--tabs;
// 			cout<<endl<<tab(--tabs)<<"]"; // later check for comma
// 			tabs++;
// 			if(cur->sibling)cout<<",";
// 			cout<<endl;
// 			tabs--;
// 			// cout<<endl<<"(complex ENDS)"<<endl;
// 		}
		

// 	}
// 	else
// 	{
// 		plain=true;
// 		cout<<tab(++tabs)<<trimString (cur->tag)<<" : "<<trimString (cur->data);
// 		tabs--;
// 		if(cur->sibling) cout<<",";
// 		cout<<endl;
		
// 	} 
// 	// tabs--;
// 	if (cur->sibling)
// 	{
// 		// if (plain) tabs++;
// 		print(cur->sibling);
// 		// if(plain)tabs--;
// 	}
// }

void chart::printcomplex(node* &cur, bool makeArray)
{
	string text = cur->text;
	string attr;
	if(cur->attr)
		attr = cur->attr->data;
	if(!cur->child&&attr[0]=='\0')
		{
			string data = trimString(cur->data);
			if(makeArray) 
			{
				// cout<<endl<<data<<" "<<data.empty()<<endl;
				if(data[0]=='\0')
					cout<<tab(tabs)<<"#text : "<<"null";
				else 
					cout<<tab(tabs)<<"#text : "<<data;

					
			}
			else cout<<" "<<cur->data;
			if(!makeArray && cur->sibling) cout<<",";
			else if (makeArray && cur->sibling&& trimString (cur->tag) ==trimString (cur->sibling->tag)) cout<<",";
			cout<<endl;
		}
	else
	{
		// cout<<endl;
		cout<<tab(tabs)<<"{"<<endl;
		tabs++;
		string text = cur->text;
		if(cur->attr)
		{
			node * traveseAttributes = cur;
			while(traveseAttributes->attr)
				{
					
					cout<<tab(tabs)<<"@"<<trimString( traveseAttributes->attr->key)<<" : "<<trimString (traveseAttributes->attr->value)<<" ,"<<endl;
					traveseAttributes = traveseAttributes->attr;
				}
		}
		if(text[0]!='\0')
			cout<<tab(tabs)<<"#text"<<" : "<<cur->text<<","<<endl; // Later: check for ,
		if(cur->child) print(cur->child);
		tabs--;
		cout<<tab(tabs)<<"}";
		if(cur->sibling)
			{
				cout<<",";
			}
		string data = cur->data;
		cout<<endl;
	}
}
void chart::print(node* N)
{
	node* cur = N;
	// if(cur->child) print(cur->child);
	// cout<<cur->tag<<"|"<<cur->text<<endl;
	// if(cur->sibling)
	// 	print(cur->sibling);
	bool makeArray = false;
	cout<<tab(tabs)<<cur->tag<<":";
	if(cur->sibling&&cur->tag==cur->sibling->tag)
	{
		makeArray = true;
	}
	if(!makeArray)
	{
		printcomplex(cur,makeArray);		

	}
	else 
	{
		cout<<endl;
		cout<<tab(tabs)<<"["<<endl;
		tabs++;
		printcomplex(cur,makeArray);
		while(cur->sibling&&cur->tag==cur->sibling->tag)
			{
				printcomplex(cur->sibling, makeArray);
				cur = cur->sibling;
			}
		tabs--;
		cout<<tab(tabs)<<"]"<<endl;
	}
	if (cur->sibling)
	{
		print(cur->sibling);
	}
}


int main()
{
	ifstream file(input);
	string temp,xml;
	while(getline(file, temp)) 
	{
		xml+=temp;
		xml+=" ";
	}
	trimString(xml);
	chart c(xml);
	c.print(c.root);
}