string getTagName(string s)
{
	// returns tagname
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
	// check if the data input has plain text or even further children
	for(int i=0;i<s.length();i++)
	{
		if(s[i]=='<') return 1;
	}
	return 0;
}

bool isOpeningtag(string s,int pos)
{
	// check if the tag is opening or not
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
	// chech if the tag is closing or not
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
	//  given a string it returns the correct position of the closing angular bracket
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
	// check if the data has only children or even data
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



void insert_after(node* &N, node* &newN)
{
	// insert new node after N
	newN->sibling = N->sibling;
	N->sibling = newN;
}

void deleteBwhichIsAfterA(node* &temp, node* &cur2)
{
	// delete new node which is the next sibbling of temp
	temp->sibling = cur2->sibling;
}
