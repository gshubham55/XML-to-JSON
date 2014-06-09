string trimString(string & str) 
{
	// removes white spaces the both ends of the string
	str.erase(0, str.find_first_not_of(' '));
	str.erase(str.find_last_not_of(' ')+1,str.length());
	return str;
}

string tab(int n)
{
	string ret;
	for(int i=0;i<n;i++)
		ret+="  ";
	return ret;
}

void removeDeclarationAndComments(string &s)
{
	// remove lines of xml input which are either declaration or comments
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

void replaceFirstOccurance( string &s, const string &search, const string &replace ) { 
	// remove first occurance of "search" by "replace" in string "s"
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
	// removes slashes outside of quotes
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


string brackets(string s)
{
	return "\""+s+"\"";
}
