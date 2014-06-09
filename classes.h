class node
{
	public:
		string tag,data,text; // text in case the contains data and other child elements
		string key,value; // in case of attribute node
		node* child; // points to the child of the element
		node* sibling; // points to the next element at the same level
		node* attr; // pts to attribute node
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
		void getChildren(node* &N,string s); // gets the children from from data inside the tags and defines them
		void define(node* &N,string s); // assigns children and all other values to the node
		void printouter(node* N); // takes care of Array case and other special cases and prints the tag name
		void printinner(node* &cur,bool makeArray); // print evertything else inside the tag name
		void groupSameTags(node* &N); // group elements with same tags in the same level so that they can be grouped in an Array
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

