#include <iostream>
#include <stdio.h>
#include <fstream>
#include <string>
#define	input "XML.txt"
using namespace std;
ofstream jsonfile;

string trimString(string & str)
{
	str.erase(0, str.find_first_not_of(' '));
	str.erase(str.find_last_not_of(' ')+1);
	return str;
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

int correctPosOfAngClosingBracketOrSlash(string s,int pos ) // s is attr, when called
{
	// cout<<"s "<<s<<"pos "<<pos<<endl;
	for(int i=pos;i<s.length();i++)
	{
		if(s[i]=='>'||s[i]=='/') return i;
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
			cout<<"closingTag not found "<<endl;
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
	// cout<<s<<endl;
	bool null = false;
	int i;
	bool quoteFlag = false;
	for (i=0;i<s.length();i++)
	{
		if(s[i]=='"' &&quoteFlag==1) 
		{
			// cout<<"*";
			quoteFlag = 0;
			// cout<<endl;
			continue;
		}
		if(quoteFlag) {
			// cout<<s[i];
			continue;
		}
		if(s[i]=='"'&&quoteFlag==0) 
			{
				quoteFlag = 1;
				// cout<<"flaggy ";
				continue;
			}
		if(s[i]=='>') break;
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
	// cout<<"s "<<s<<endl;
	for(int i=0;i<s.length();i++)
	{
		// if(s[i]==' ')continue;
		if(s[i]=='<') 
			{
				s.erase (0,i);
				trimString(text);
				trimString(s);
				return text;
			}
		text+=s[i];
	}
	// cout<<text<<endl;
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
		void prinptinner(node* &cur,bool makeArray);
		void print()
		{
			jsonfile<<tab(tabs)<<"{"<<endl;
			tabs++;
			printouter (root);
			tabs--;
			jsonfile<<tab(tabs)<<"}"<<endl;
		}
		chart(string s)
		{
			tabs=0;
			// cout<<s<<endl;
			define(root,s);
			// cout<<2<<endl;
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
		else if((isalnum(tagname[i])||tagname[i]=='/'||tagname[i]=='-'||tagname[i]=='_')&&collect==1) newtag+=tagname[i];
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
	// cout<<attr<<endl;
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

			// cout<<key[count]<<"|"<<value[count]<<endl;
			// count++;
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
	// cout<<"1";
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
		// cout<<s<<endl;
		// get tagname and the end position of the string containing the first child
		string firsttag = getTagName(s);

		firsttag = removeAttributes(firsttag,s); //Later

		int null;
		null = checkIfNullDataSet(s); // null has the position of the '/' , else 0
		// cout<<null<<endl;
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
	// cout<<N->child<<endl;
}
string brackets(string s)
{
	return "\""+s+"\"";
}

void chart::prinptinner(node* &cur, bool makeArray)
{
	string text = cur->text;
	string attr;
	if(cur->attr)
		attr = cur->attr->key;
	if(!cur->child&&attr[0]=='\0')
		{
			// jsonfile<<"("<<attr<<")";
			string data = trimString(cur->data);
			if(makeArray) 
			{
				// jsonfile<<endl<<data<<" "<<data.empty()<<endl;
				if(data[0]=='\0')
					jsonfile<<tab(tabs)<<"\"#text\" : "<<"null";
				else 
					jsonfile<<tab(tabs)<<"\"#text\" : "<<brackets(trimString(data));

					
			}
			else if (data[0]!='\0' )jsonfile<<" "<<brackets(trimString(cur->data));
			else jsonfile<<" "<<"null";
			if(!makeArray && cur->sibling) jsonfile<<",";
			else if (makeArray && cur->sibling&& trimString (cur->tag) ==trimString (cur->sibling->tag)) jsonfile<<",";
			jsonfile<<endl;
		}
	else
	{
		if(!makeArray)jsonfile<<endl;
		jsonfile<<tab(tabs)<<"{"<<endl;
		tabs++;
		string text = cur->text;
		if(cur->attr)
		{
			node * traveseAttributes = cur;
			while(traveseAttributes->attr)
				{

					jsonfile<<tab(tabs)<<"\"@"<<trimString( traveseAttributes->attr->key)<<"\" : "<<trimString (traveseAttributes->attr->value);
					if(text[0]!='\0'||cur->child||traveseAttributes->attr->attr) jsonfile<<",";
					jsonfile<<endl;
					traveseAttributes = traveseAttributes->attr;
				}
		}
		if(text[0]!='\0')
		{
			jsonfile<<tab(tabs)<<"\"#text\""<<" : "<<brackets(trimString (cur->text));
			if(cur->child) jsonfile<<",";
			jsonfile<<endl; 
		}
		if(cur->child) printouter(cur->child); 
		tabs--;
		jsonfile<<tab(tabs)<<"}";
		if(!makeArray && cur->sibling)
			{
				jsonfile<<",";
			}
		else if(makeArray && cur->sibling&&cur->tag==cur->sibling->tag)
		{
			jsonfile<<",";
		}
		string data = cur->data;
		jsonfile<<endl;
	}
}
void chart::printouter(node* N)
{
	node* cur = N;
	
	bool makeArray = false;
	jsonfile<<tab(tabs)<<brackets(trimString (cur->tag))<<":";
	if(cur->sibling&&cur->tag==cur->sibling->tag)
	{
		makeArray = true;
	}
	if(!makeArray)
	{
		prinptinner(cur,makeArray);		

	}
	else 
	{
		jsonfile<<endl;
		jsonfile<<tab(tabs)<<"["<<endl;
		tabs++;
		prinptinner(cur,makeArray);
		while(cur->sibling&&cur->tag==cur->sibling->tag)
			{
				prinptinner(cur->sibling, makeArray);
				cur = cur->sibling;
			}
		tabs--;
		jsonfile<<tab(tabs)<<"]";
		if(cur->sibling) jsonfile<<",";
		jsonfile<<endl;
	}
	if (cur->sibling)
	{
		printouter(cur->sibling);
	}


	// jsonfile<<cur->child->child->attr->attr->key<<endl;
}

int main()
{
	ifstream file(input);
	jsonfile.open ("json.txt");
	string temp,xml;
	while(getline(file, temp)) 
	{
		xml+=temp;
		xml+=" ";
	}
	trimString(xml);
	chart c(xml);
	c.print();
	// jsonfile << "Writing this to a file.\n";
	jsonfile.close();

}