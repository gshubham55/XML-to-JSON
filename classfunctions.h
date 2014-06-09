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

	// check if the data is plain or has children
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

void chart::groupSameTags(node* &N)
{
	node * cur1, * cur2;
	cur1 = N;
	while (cur1)
	{
		// while the adjacent elements have same tag, keep on traversing freely
		while (cur1->sibling && cur1->tag == cur1->sibling->tag)
			cur1=cur1->sibling;
		cur2 = cur1;
		while(cur2)
		{
			node* temp = cur2;
			cur2= cur2->sibling;
			if(cur1&&cur2&&cur1->tag == cur2->tag)
			{
				// if two non adjacent tags have the same tag, delete the second element and put it after the first element
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
			//  if it has no children or attributes, it calls for a simpler output , ie directly printing elements' data if present
			string data = trimString(cur->data);
			if(makeArray) 
			{
				if(data[0]=='\0')
					out<<tab(tabs)<<"null";
				else 
					out<<tab(tabs)<<brackets(trimString(data));
			}
			else if (data[0]!='\0' )
				out<<" "<<brackets(trimString(cur->data));
			else 
				out<<" "<<"null";

			if(!makeArray && cur->sibling) 
				out<<",";
			else if (makeArray && cur->sibling&& trimString (cur->tag) ==trimString (cur->sibling->tag)) 
				out<<",";
			out<<endl;
		}
	else
	{
		// otherwise the elements output will be inside the {} tags and contain appropriate data
		if(!makeArray)out<<endl;
		out<<tab(tabs)<<"{"<<endl;
		tabs++;
		string text = cur->text;
		if(cur->attr)
		{
			node * traveseAttributes = cur;
			while(traveseAttributes->attr)
				{
					// print out all the attributes with an @
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
	if(tag=="ROOT_TAG")
	{
		// We ourselves add ROOT_TAG to the ml input and is not supposed to be printed
		printouter(cur->child);
		return;
	}
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
		// if an array needs to be made, include appropriate [] tags
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
		// print the next element in the same level
		printouter(cur->sibling);
	}
}
