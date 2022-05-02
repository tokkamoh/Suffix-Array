#include<iostream>
#include<cstring>
using namespace std;
int CharToInt[256];
char IntToChar[4];
int num=0;
void InitializeTables()
{
	CharToInt['A']=0;
	CharToInt['C']=1;
    CharToInt['G']=2;
    CharToInt['T']=3;
    IntToChar[0]='A';
    IntToChar[1]='C';
    IntToChar[2]='G';
    IntToChar[3]='T';
}
struct Node /// the Node that contain the data of each node in the trie
{
	Node* Child[4];
	Node *Failure;
	char edge;
	int id; // read ids of reads ending at this location
	int NumN;
};
Node* root;
/////////////////////////////////////////////////////////////////////////////////////////////
Node* CreateTrieNode() /// Create and Initialization Trie Node
{
	Node* node=new Node;
	int i;
	for(i=0;i<4;i++)
    {
      node->Child[i]=0;
      node->Failure=0;
      node->id=-1;
    }

	return node;
}
void InitializeTrie() ///create the root that all it's values are null
{
	root=CreateTrieNode();
	root->Failure=NULL;
	root->edge='0';
	root->NumN=num;
	num++;
}
char*properSuff(int indexx , int ending,char *p) /// i used this function firstly to return at each call the proper suffix i was going to check
{
    char * suffix=new char [200]; ///make an array that store our suffix
    int j = 0;
    for(int i=indexx; i<= ending; i++) /// making deep copying
    {
        suffix[j]=p[i];
        j++;
    }
    return suffix;

}
Node * RetFailure(char *s) /// the function that search for the suffix and return the last node this suffix exist on
{
    Node *curr=root; /// we take our root and store it to make sure thereis any change happen to it
    for (int i = 0; i < strlen(s); i++) /// loop will iterate on the size of the suffix
    {
        int index = CharToInt[s[i]]; /// convert each char to the corresponding integer
        if (!curr->Child[index]) /// if a char don't exist it means that this suffix not the proper one
            return NULL;

        curr = curr->Child[index]; /// else here it means a match happens
    }
    if(curr!=NULL) return curr; /// if we reached this node we are seeking on it will be returned
    else return NULL; /// if not will return null
}
void BuildFailue(Node *node,char *p) /// the main function that build the failure
{
    int i=1;
    while(i<strlen(p)) /// will loop on the size of the largest suffix as each time it decrease by one
    {
       char *s=properSuff(i,strlen(p),p); /// we get a suffix
       node->Failure=RetFailure(s);/// search for it's node
       if( node->Failure !=0) /// if this node was found will break
       break;
       i++; /// if not found will move to the next suffix
    }
   if(node->Failure==NULL) /// if nothing was found this means the failure link of this node will be the root;
   {
       node->Failure=root;
   }

}
char *append(char *s,char c) /// the function that create the char array that will carry each node's prefix
{
    char *neww =new char (strlen(s)+2); /// we create anew char array that it's size is greater than that of the main one
    for(int i=0;i<strlen(s);i++) /// we make deep coping the previous chars
    {
        neww[i]=s[i];
    }
    neww[strlen(s)]=c; /// then adding the new character
    neww[strlen(s)+1]='\0'; /// add 0 at the end to terminate our perfix
    return neww;
}
void Build(Node* node,char *currP) /// the function that traverse on the trie and return each node to get it's failure link and it's prefix
{
    int i;
    if(node !=root) BuildFailue(node,currP);/// will build it's failure link
	for(i=0;i<4;i++) if(node->Child[i]) /// if there is a child
    {
        Build(node->Child[i],append(currP,node->Child[i]->edge)); /// then call recursively using it
    }
}
void InsertTrieRead(int* r, int n, int I) /// Insert a pattern with length n with its id in the trie
{
	int i;

	Node* cur_node=root;/// store root in another variable
	for(i=0;i<n;i++) /// loop on the size of the pattern
	{
		if(!cur_node->Child[r[i]]) /// if in this place there is no node
        {
            cur_node->Child[r[i]]=CreateTrieNode(); /// will create this node
            cur_node->Child[r[i]]->edge=IntToChar[r[i]]; /// start filling our chars
            cur_node->Child[r[i]]->NumN=num;/// i'm just giving each node an id to help in getting the suffixes
            num++; /// it increase always as the id from 0 till total length of all patterns

        }
		cur_node=cur_node->Child[r[i]]; /// then moving to our newly made node
	}

	cur_node->id=I; /// then filling the id of each pattern
}
void Rep(int id ,int i) /// the function that return the location of each pattern in our text
/// the location is calculated to be the last index we reached in the text - the size of the pattern it reached for
{
    int location=0;
    if(id==0)
    {
       location=i-3;
    }
    else if (id==2)
    {
      location=i-3;
    }
    else if (id==1)
    {
        location=i-2;
    }
    else if (id==3)
    {
        location=i-4;
    }
    else if (id==4)
    {
        location=i-3;
    }
    cout<<"(t"<<i<<" , "<<"P"<<id<<")"<<endl;
    cout<<"the occurrence of id : "<<id<<" at index = "<<location<<endl;
}
void Compare(char *text,int sizee2) /// the function we start looking for our pattern in the main text
{
   Node *p=root;
   int i=0;
   int   x=0;
   while (true)
   {
       if(i==sizee2 && p->Failure==0) break;/// it will break when we reach the end of our text and failure link of the root
       if(p->Child[CharToInt[text[i]]]!= 0)  /// if there is a match
       {
           p=p->Child[CharToInt[text[i]]]; /// will go to this node that made the match
           i++; /// increment the text iterator by 1
           Node *check=p; /// here we store it in another variable to see all the failure links w may use to return if any occurence happens
           while(check!=root) /// while you did not reach the root keep looking in all failure links in your path
           {
              if(check->Failure->id!=-1) /// if an id was found will return this id
            {
               Rep(p->Failure->id,i); /// will call the function that will print the location of the occurence
            }
            check=check->Failure; /// iterating to the next Failure
           }
           if(p->id!= -1) Rep(p->id,i); ///  if there was an id in the matching node

       }
        else if(p->Child[CharToInt[text[i]]]== 0 ) /// if no match happens
        {
            /// if we reached the root and no match happens
            p=p->Failure; /// the text iterator will remain the same but the node will go to it's failure link
        }
        if( p->Child[CharToInt[text[i]]] ==0 && p==root )  i++; /// if we reached the root and still there was no match the text will iterate by 1
    }
    delete p;
}
void InsertRead(char* read,int n, int id) /// this function take the pattern and convert it using IntToChar array
{

	int *Pint=new int [1000];
	for(int i=0;i<n;i++) Pint[i]=CharToInt[(int)read[i]]; /// start filling the array with our pattern

	InsertTrieRead(Pint, n, id); /// insert our pattern after converting it
	delete Pint;
	//return true;
}
/////////////////////////////////////////////////////////////////////////////////////////////
void MainFunc(char *text,int sizz2) /// the main function that suppose to initialize the trie then call the search function
{
	InitializeTables();
	InitializeTrie();
    InsertRead("AGA",3,0);
    InsertRead("AA",2,1);
    InsertRead("AAG",3,2);
    InsertRead("GAAG",4,3);
    InsertRead("TCG",3,4);
    Build(root,"");
    Compare(text,sizz2);

}
void DeleteTrie(Node* node) /// this function only traverse on the trie and delete each node;
{
	int i;
	for(i=0;i<4;i++) if(node->Child[i]) DeleteTrie(node->Child[i]);
	delete node;
}
/////////////////////////////////////////////////////////////////////////////////////////////
int main()
{
    int size2=23; /// size of the main text
    char *text=new char [size2];
    text="GAACAAGTGAAGTGAGAAGAAGT";
	MainFunc(text,size2); /// calling the main function
	///DeleteTrie(root); when calling this function after printing and every thing run correctly i cause crash before return 0
	return 0;
}

