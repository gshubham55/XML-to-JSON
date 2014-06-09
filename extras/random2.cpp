void chart::prinptinner(node* &cur, bool makeArray)
{
	string text = cur->text;
	string attr;
	if(cur->attr)
		attr = cur->attr->key;
	if(!cur->child&&attr[0]=='\0')
		{
			// myfile<<"("<<attr<<")";
			string data = trimString(cur->data);
			if(makeArray) 
			{
				// myfile<<endl<<data<<" "<<data.empty()<<endl;
				if(data[0]=='\0')
					myfile<<tab(tabs)<<"\"#text\" : "<<"null";
				else 
					myfile<<tab(tabs)<<"\"#text\" : "<<brackets(trimString(data));

					
			}
			else if (data[0]!='\0' )myfile<<" "<<brackets(trimString(cur->data));
			else myfile<<" "<<"null";
			if(!makeArray && cur->sibling) myfile<<",";
			else if (makeArray && cur->sibling&& trimString (cur->tag) ==trimString (cur->sibling->tag)) myfile<<",";
			myfile<<endl;
		}
	else
	{
		if(!makeArray)myfile<<endl;
		myfile<<tab(tabs)<<"{"<<endl;
		tabs++;
		string text = cur->text;
		if(cur->attr)
		{
			node * traveseAttributes = cur;
			while(traveseAttributes->attr)
				{

					myfile<<tab(tabs)<<"\"@"<<trimString( traveseAttributes->attr->key)<<"\" : "<<trimString (traveseAttributes->attr->value);
					if(text[0]!='\0'||cur->child||traveseAttributes->attr->attr) myfile<<",";
					myfile<<endl;
					traveseAttributes = traveseAttributes->attr;
				}
		}
		if(text[0]!='\0')
		{
			myfile<<tab(tabs)<<"\"#text\""<<" : "<<brackets(trimString (cur->text));
			if(cur->child) myfile<<",";
			myfile<<endl; 
		}
		if(cur->child) printouter(cur->child); 
		tabs--;
		myfile<<tab(tabs)<<"}";
		if(!makeArray && cur->sibling)
			{
				myfile<<",";
			}
		else if(makeArray && cur->sibling&&cur->tag==cur->sibling->tag)
		{
			myfile<<",";
		}
		string data = cur->data;
		myfile<<endl;
	}
}
void chart::printouter(node* N)
{
	node* cur = N;
	
	bool makeArray = false;
	myfile<<tab(tabs)<<brackets(trimString (cur->tag))<<":";
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
		myfile<<endl;
		myfile<<tab(tabs)<<"["<<endl;
		tabs++;
		prinptinner(cur,makeArray);
		while(cur->sibling&&cur->tag==cur->sibling->tag)
			{
				prinptinner(cur->sibling, makeArray);
				cur = cur->sibling;
			}
		tabs--;
		myfile<<tab(tabs)<<"]";
		if(cur->sibling) myfile<<",";
		myfile<<endl;
	}
	if (cur->sibling)
	{
		printouter(cur->sibling);
	}


	// myfile<<cur->child->child->attr->attr->key<<endl;
}