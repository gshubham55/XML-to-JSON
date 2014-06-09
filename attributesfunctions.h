void getAttributes(node * &N,string &tagname,string &s)
{
	//assignes attributes to the element by parsing through the opening tag 
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
	// removes attributes from the string s 
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
