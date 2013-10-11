/***************************************************************
       Author:       Nick Carlson
       Assignment:   Major #2 - Binary Search Tree
       Due Date:     March 22, 2012
       Class:        CSC 300
       Instructor:   Dr. Krebsbach

     Description:  This program uses several functions to create
					and work with binary search trees as well as a
					main driver to test them.

****************************************************************/

//******************************
//INCLUDES
//******************************
#include <iostream>
#include <fstream>
#include <iomanip>
#include <cstring>
#include <cstdlib>
using namespace std;

//*******************************************************
//GLOBAL CONSTANTS, STRUCTS, AND PROTOTYPES
//*******************************************************
const char file_name[] = "C:/cygwin/home/DSU/csc300data/treedata.txt"; //global constant for filepath

struct TREENODE;   								//need to have for typedef
typedef TREENODE *  TREENODE_PTR; 				//create new type

//STRUCT
struct TREENODE 								//tree nodes
{
   int key;
   TREENODE_PTR Lchild;
   TREENODE_PTR Rchild;
   TREENODE(void){Lchild = NULL; Rchild=NULL;} 	//default contructor to set children to NULL
};

//PROTOTYPES
void load_file(TREENODE_PTR &r);
void insert_node(TREENODE_PTR &root, int key);
void print_tree(TREENODE_PTR r,bool & flag);
void hit_continue();
void get_root(TREENODE_PTR r);
void delete_all(TREENODE_PTR & t);
void delete_all_message();
TREENODE_PTR find_min(TREENODE_PTR & t);
void delete_treeNode(TREENODE_PTR &r,int num);
void fill_data(TREENODE_PTR t,int data[],int &count);
void balance(TREENODE_PTR & root,int num,int data[],int & fp);
void balance_tree(TREENODE_PTR &tree, bool & flag);
void search_tree(TREENODE_PTR r,const int num, bool & flag);
void clearScreen();
void printQuitOption();
bool valid_menu_input(const int convert_flag, const int min, const int max, const char * valchar_list, const char c, const int n);
int console_reader(char * prompt, int input_type, char &c1, int &i1, double &d1);
int print_menu();
int lookup_menu();
int in_del_menu();
int insert_menu(TREENODE_PTR &r);
int delete_menu(TREENODE_PTR &r);
int get_count(TREENODE_PTR r, int & count);
int get_height(TREENODE_PTR r,int & max_depth,int cur);

//===============================================================
// load_file : loads values into a binary tree from a text file
//===============================================================
void load_file(TREENODE_PTR &r){
	int key_value; // value read in from the file
	//create a file variable
	fstream fv;
	//assign FV to a physical file and open in the correct format
	fv.open(file_name,ios::in);
	if(!fv){
		cout<<"ERROR - File not found!!!"<<endl;
		exit(1);
	}
	//perform I/O
	fv >> key_value;						//"prime the pump"
	while (!fv.fail() && key_value != -1)	//traversing all values and stopping at -1
	 {
	     insert_node(r,key_value);		//put values into the tree as they are read in
		 fv >> key_value;				//move to next value
	 }
	 
	 cout<<""<<endl;
	 cout<<"The tree has been built."<<endl;	//lets the user know that all of the above has executed
	 cout<<""<<endl;
	 
	//close the file
	fv.close();
}

//=========================================================================
// get_count : uses a counter to find the number of nodes in a binary tree
//=========================================================================
int get_count(TREENODE_PTR r, int & count){
	if(r==NULL){
		return 0;						//if the tree is empty, the count is zero
	}
	count++;							//increment count each time the function is recursively called
	get_count(r->Lchild,count);			//recursively call get_count
	get_count(r->Rchild,count);
}

//==================================================
// insert_node : inserts a node into a binary tree
//==================================================
void insert_node(TREENODE_PTR &root, int key)
{
    if (root == NULL)  // GROUND  fell off tree so create node
   {
      root = new TREENODE; //create node and assign to local root
      root-> key = key;    // assign key to the node
      return; // by returning connects local root to either Left or Right child of parent
    }

    if (root->key == key ) return;  // GROUND  if key already in tree then nothing to do

    if (root-> key > key)
       insert_node(root->Lchild,key); // move down left subtree
    else
       insert_node(root->Rchild,key); // move down right subtree
}

//==================================================================
// print_tree : prints the values of a binary tree in sorted order
//==================================================================
void print_tree(TREENODE_PTR r,bool & flag){
	if(r==NULL){
		return;						//if tree is empty, then there's nothing to print
	}
	print_tree(r->Lchild,flag);		//traverse left subtree
	flag = true;					//flag displays EMPTY TREE if false
	cout<<r->key<<endl;				//in-order traversal
	print_tree(r->Rchild,flag);		//traverse right subtree
}

//===================================================================================
// hit_continue : prompts the user to press a button before going back to main menu
//===================================================================================
void hit_continue(){
	char junk[11];									//an array for the input to go into
	cout<<""<<endl;
	cout<<"Press enter to continue:"<<endl;			//the user is told to hit enter but any character would work
	cin.getline(junk,10,'\n');						//the input is taken in and the program moves on
}

//=====================================================
// get_root : displays to root value of a binary tree
//=====================================================
void get_root(TREENODE_PTR r){
	if(r == NULL){
		cout<<"Empty Tree"<<endl;			//if the is nothing at the root location, the tree is empty
	}
	else{
		cout<<r->key<<endl;					//if a value is present, it is printed out
	}
}

//=============================================================================
// delete_all : deletes all values of a binary tree and gives back the memory
//=============================================================================
void delete_all(TREENODE_PTR & t){
	if(t==NULL){
		return;						//if null, then there is nothing to delete
	}
	
	delete_all(t->Lchild);			//delete left subtree
	delete_all(t->Rchild);			//delete right subtree
	
	delete t;						//delete root
	
	t = NULL;						//pointer is set to null, memory is returned to the system
}

//=============================================================================================
// delete_all_message : displays a message after all tree values are deleted by delete_all()
//=============================================================================================
void delete_all_message(){
	cout<<""<<endl;
	cout<<"The tree has been cleared."<<endl;			//a simple message is displayed (cont.)
	cout<<"All values have been deleted."<<endl;		//to tell the user the tree has been deleted
}

//=====================================================
// find_min : finds the smallest number in the tree
//=====================================================
TREENODE_PTR find_min(TREENODE_PTR & t){
	if(t->Lchild==NULL){				//cannot move farther left
		return t;						//if there is no left subtree, then the root value is also the smallest in the tree
	}
	else
		return find_min(t->Lchild);		//if left subtree is present, search it for the smallest value, will be left-most
}

//======================================================
// delete_treeNode : deletes a single node in the tree
//======================================================
void delete_treeNode(TREENODE_PTR &r,int num){
	TREENODE_PTR tmpCell;						//declare a temporary pointer to use for swapping and deleting
	if(r == NULL){
		return;									//if tree is empty, there is no deleting to be done
	}
	
	if(num < r->key){
		delete_treeNode(r->Lchild,num);			//check to see if the number is in the left subtree
	}
	else
	if(num > r->key){
		delete_treeNode(r->Rchild,num);			//check to see if the number is in the right subtree
	}
	else{
		if(r->Lchild == NULL){					//if there is no left subtree (cont.)
			tmpCell = r;						//and we are deleting root (cont.)
			r = r->Rchild;						//we change the pointer to the Rchild of root (cont.)
			delete tmpCell;						//and delete the root
		}
		else
		if(r->Rchild == NULL){					//same process for deleting a root no right subtree
			tmpCell = r;						//change pointer and delete
			r = r->Lchild;
			delete tmpCell;
		}
		else{
			tmpCell = find_min(r->Rchild);		//if deleting root with both subtrees present
			int t = r->key;						//three-phase swap of keys with root and smallest value in the right sub-tree
			r->key = tmpCell->key;				//then delete the value where the new root used to be
			tmpCell->key = t;
			delete_treeNode(r->Rchild,num);
		}
	}
}

//================================================================================
// fill_data : used with balance_tree(), fills the array with sorted tree values
//================================================================================
void fill_data(TREENODE_PTR t,int data[],int &count){
	if(t==NULL){
		return;								//if tree is empty then there is no data for filling
	}
	fill_data(t->Lchild,data,count);		//recursively call fill_data for left subtree
	count++;								//increment count
	data[count-1] = t->key;					//we put t->key at the location data[count-1]
	fill_data(t->Rchild,data,count);		//recursively call fill_data for right subtree
}

//==================================================================================
// balance : places tree values in balanced order, builds new tree and fills it up
//==================================================================================
void balance(TREENODE_PTR & root,int num,int data[],int & fp){
	if(num > 0){									//num must be greater than zero so that there are more nodes to insert
		root  = new TREENODE;						//create new TREENODE which is empty
		balance(root->Lchild,num/2,data,fp);		//traverse left subtree, remaining data array
		root->key = data[fp];						//we unload our array, current data goes into empty node
		fp++;										//increment field pointer, moves to next number in data array
		balance(root->Rchild,(num-1)/2,data,fp);	//traverse right subtree, remaining data array
	}
}

//================================================================================================
// balance_tree : driver that uses fill_data() and balance() to put tree values in balanced order
//================================================================================================
void balance_tree(TREENODE_PTR &tree, bool & flag){
	int tree_count = 0;								//number in tree goes here
	if(tree == NULL){
		return;										//if there is nothing in the tree, it is already balanced
	}
	int node_count = get_count(tree,tree_count);	//retrieve number of nodes in the tree
	int * data = new int[node_count];				//create new dynamic array to hold sorted values
	int count = 0;									//count for # of nodes
	fill_data(tree,data,count);						//fill up the new array with sorted keys, passed back count
	delete_all(tree);								//clear out the old tree
	int fp = 0;										//field pointer points to the next key in the array
	balance(tree,count,data,fp);					//new tree is created in balance
	delete[] data;									//clear the memory of the dynamic array and give it back
	
	flag = true;									//if flag=false, will display NOTHING TO BALANCE
	cout<<""<<endl;
	cout<<"The tree has been balanced."<<endl;		//the user is told that the above has executed
}

//==========================================================================================================
// search_tree : searches through the tree for a number and displays FOUND or NOT FOUND via a boolean flag
//==========================================================================================================
void search_tree(TREENODE_PTR r,const int num, bool & flag){
	if(r == NULL){
		return;								//if the tree is empty, then the value is not in it
	}
	search_tree(r->Lchild,num,flag);		//traverse left subtree
	if(num == r->key){						//if value is found
	cout<<""<<endl;
	flag = true;							//set flag true, if false, NOT FOUND! is displayed
	/*if(flag == true){
		cout<<"FLAG TRUE!"<<endl;			//<- for testing purposes
	}else{
		cout<<"FLAG FALSE!"<<endl;
	}*/
	cout<<"NUMBER FOUND!"<<endl;
	cout<<"Found: "<<r->key<<endl;			//user is informed of success
	return;
	}
	search_tree(r->Rchild,num,flag);		//traverse right subtree
}

//========================================================================================
// clearScreen : prints enough space to clear the screen, used when displaying main menu
//========================================================================================
void clearScreen()
{
	cout << "\033[2J"; //this gives enough space to clear the screen
}

//===============================================================================
// printQuitOption : prints the option to quit, so that the user knows they can
//===============================================================================
void printQuitOption()
{
	cout << endl << endl << "Enter Q to Quit" << endl;		//displayes a simple message in a formatted spot
}

//========================================================================
// valid_menu_input : prevents bad user input from crashing the program
//========================================================================
bool valid_menu_input(const int convert_flag, const int min, const int max, const char * valchar_list, const char c, const int n)
{
    int length = strlen(valchar_list);
   // each case results in the function returning !!!!!
   switch (convert_flag)
   {
      case 0: return false;  // was invalid to start with
	  case 3:  // check double range
	             return true;
			 
	  case 2: if ((n >= min) && (n <= max)) // check int range
	             return true;
			  else
			     return false;
	  case 1: for( int i = 0; i < length; i++) // check for valid char in list
              {
                 if (valchar_list[i] == c)
                      return true;
              }
              return false;
    }
}

//========================================================================
// console_reader : takes in data from user and tests it for valid types
//========================================================================
int  console_reader(char * prompt, int input_type, char &c1, int &i1, double &d1)
{
   int convert_flag = 0;  					//set to invalid 
   char line_buffer[121]; 					//used to read a line of text up to 120 useable characters;
   cout << prompt;
   cin.getline(line_buffer,120,'\n');
   
   c1 = 'X';  i1 = -999; d1 = -999.99;		//clear output parameters
   
   char fchar = line_buffer[0]; 			//first character entered by user
   
   if (!isalnum(fchar))  					//check first character for letter or number
        return convert_flag; 				//invalid user input
   
   if (isalpha(fchar))  					//user typed a letter
     input_type = 1;
   
   switch (input_type)
   {
      case 1:								//expecting a single character
	          c1 = fchar;
			  convert_flag = 1;
			  break;
			  
	  case 2: 								//expecting an int
	          if (isdigit(fchar))
			  {
					i1 = atoi(line_buffer);
					convert_flag = 2;
              }
	          break;
			  
	  case 3: 								//expecting a double
	          if (isdigit(fchar))
			  {
			     d1 = atof(line_buffer);
			     convert_flag = 3;
              }				 
	          break;
			  
	  default:
	          cout << "Prog ERROR invalid input_type  CONTACT DEVELOPER !" << endl;
              exit(1);   					//violation of pre-condition
	}
    
	return convert_flag; 					//what was actually converted
}

//===============================================================
// print_menu : displays the main menu and accepts valid input
//===============================================================
int print_menu(){
	int flag;												//used in valid_menu_input
	int num;												//used for returning a value
    int n = 0;												//goes into console_reader
    char c = 'x';											//goes into console_reader
    double d = 999.99;										//goes into console_reader
	
	cout<<"Main Menu"<<endl;								//displays menu information
	cout<<"{Nick Carlson}"<<endl<<endl;						//author's name :)
	cout<<"1) Load text file into tree"<<endl;
	cout<<"2) Balance the tree"<<endl;
	cout<<"3) Look up a value in the tree"<<endl;
	cout<<"4) Report the height of the tree"<<endl;
	cout<<"5) Print in sorted order"<<endl;
	cout<<"6) Report the root value of the tree"<<endl;
	cout<<"7) Insert or delete values in the tree"<<endl;
	cout<<"8) Delete the entire tree"<<endl;
	printQuitOption();										//displays the option to quit the program
	
	do{
	flag = console_reader("Please enter your selection from the menu [1-8 or Q]: ",2,c,n,d);	//take in valid input
	}while(!valid_menu_input(flag,1,8,"Qq",c,n));			//while input is valid
	
	if(flag == 2){
		num = n;											//pass back user's choice to move on
		return num;
	}else{
		return 0;											//if value is of incorrect type, return 0
	}
}

//========================================================================
// lookup_menu : displays a sub-menu when looking up a value in the tree
//========================================================================
int lookup_menu(){
	int flag;														//used in valid_menu_input
	int num;														//used to return a value
    int n = 0;														//goes into console_reader
    char c = 'x';													//goes into console_reader
    double d = 999.99;												//goes into console_reader
	
	cout<<""<<endl;
	
	do{
	flag = console_reader("Please enter a number to look up in the tree: ",2,c,n,d);	//take in valid input
	}while(!valid_menu_input(flag,1,1000000,"Qq",c,n));									//while input is valid
	
	if(flag == 2){
		num = n;
		return num;													//return input (goes to look up function)
	}else{
		return 0;													//if value is of incorrect type, return 0
	}
}

//=========================================================================
// in_del_menu : displays a menu for choosing whether to insert or delete
//=========================================================================
int in_del_menu(){
	int flag;														//used in valid_menu_input
	int num;														//for returning a value
    int n = 0;														//goes into console_reader
    char c = 'x';													//goes into console_reader
    double d = 999.99;												//goes into console_reader
	
	cout<<""<<endl;
	cout<<"1) Insert a value into the tree"<<endl;					//ask the user what to do
	cout<<"2) Delete a value from the tree"<<endl;
	cout<<""<<endl;
	cout<<"Press M to return to Main Menu"<<endl;					//give user option to return to main menu
	
	do{
	flag = console_reader("Please enter a 1-2 or M: ",2,c,n,d);		//take in valid input
	}while(!valid_menu_input(flag,1,2,"Mm",c,n));					//while input is valid
	
	if(flag == 2){
		num = n;													//return user choice of insert or delete (cont.)
		return num;													//to move to next sub-menu
	}
}

//=============================================================================
// insert_menu : displays a menu for entering a value to insert into the tree
//=============================================================================
int insert_menu(TREENODE_PTR &r){
	int flag;														//used in valid_menu_input
	int num;														//for passing back a value
    int n = 0;														//goes into console_reader
    char c = 'x';													//goes into console_reader
    double d = 999.99;												//goes into console_reader
	
	cout<<""<<endl;
	
	do{
	flag = console_reader("Please enter a number to insert into the tree: ",2,c,n,d);	//take in valid input
	}while(!valid_menu_input(flag,1,100000,"Mm",c,n));									//while input is valid
	
	if(flag == 2){
		num = n;																		//set num to user value
		insert_node(r,num);																//insert num into the list
		return num;																		//return num for printing
	}
}

//=================================================================
// delete_menu : displays a menu for delete a value from the tree
//=================================================================
int delete_menu(TREENODE_PTR &r){
	int flag;																	//used in valid_menu_input
	int num;																	//used for returning a value
    int n = 0;																	//goes into console_reader
    char c = 'x';																//goes into console_reader
    double d = 999.99;															//goes into console_reader
	
	cout<<""<<endl;
	
	do{
	flag = console_reader("Please enter a number to delete from the tree: ",2,c,n,d);	//take in valid input
	}while(!valid_menu_input(flag,1,100000,"Mm",c,n));									//while input is valid
	
	if(flag == 2){
		num = n;																		//num is given user value
		delete_treeNode(r,num);															//value is deleted from tree
		return num;																		//return value for printing
	}
}

//========================================================
// get_height : uses max to find the height of the tree
//========================================================
int get_height(TREENODE_PTR r,int & max,int cur){
	if(r==NULL){
		return 0;									//if there are no values in the tree, height is 0
	}
	if(cur > max){
		max = cur;									//if current height is greater than max height, change max height
	}
	get_height(r->Lchild,max,cur+1);				//traverse left subtree
	get_height(r->Rchild,max,cur+1);				//traverse right subtree
}

//===================================================
// main : main driver to test binary tree functions
//===================================================
int main(void){
	int currMenu = 1;								//used in switch, indicates which menu item is chosen
	int operation = 0;								//used to hold values passed back from functions
	int insert_num = 0;								//holds value being inserted into list, gets from insert menu
	int delete_num = 0;								//holds value being deleted from list, gets form delte menu
	int cur = 1;									//used in finding the height of the tree
	int max = 0;									//used in finding the height of the tree
	int root_num = 0;								//holds the value of root, gets from root menu
	bool flag = false;								//flag for displaying whether node found or not during search
	bool flag_2 = false;
	bool flag_3 = false;
	TREENODE_PTR root = NULL;						//creates empty tree
	do{clearScreen();								//clear screen each time main is returned to
		switch(currMenu){
			case 1: operation = print_menu();		//display main menu
					if(operation == 0){
						currMenu = 0;
					}else{
						operation = operation+1;	//accept user input and move forward
						currMenu = operation;
						}
					break;
			case 2: load_file(root);				//load values from text file into tree
					hit_continue();					//prompt user to continue
					currMenu = 1;					//return to main
					break;
			case 3: flag_3 = false;
					balance_tree(root,flag_3);		//balance the tree
					if(flag_3 == false){			//if nothing to balance, display NOTHING TO BALANCE
						cout<<""<<endl;
						cout<<"NOTHING TO BALANCE!"<<endl;
					}
					hit_continue();					//prompt user to continue
					currMenu = 1;					//return to main
					break;
			case 4: flag = false;
					operation = lookup_menu();					//go to lookup submenu
					search_tree(root,operation,flag);			//search for value from user
					if(flag == false){							//if not present, display NOT FOUND
						cout<<""<<endl;
						cout<<"NUMBER IS NOT FOUND!"<<endl;
					}
					hit_continue();								//prompt user to continue
					currMenu = 1;								//return to main
					break;
			case 5: cur = 1;
					max = 0;
					get_height(root,max,cur);							//calculate the height of the tree
					cout<<""<<endl;
					cout<<"The height of the tree is: "<< max <<endl;	//display height
					hit_continue();										//prompt the user to continue
					currMenu = 1;										//return to main
					break;
			case 6: flag_2 = false;
					cout<<""<<endl;
					print_tree(root,flag_2);					//print tree in sorted order
					if(flag_2 == false){
						cout<<"EMPTY TREE!"<<endl;				//tell user tree is empty if applicable
					}
					hit_continue();								//prompt user to continue
					currMenu = 1;								//return to main
					break;
			case 7: cout<<""<<endl;
					cout<<"The root value is: "<<endl;			//ready display for value
					get_root(root);								//find root value and display
					hit_continue();								//prompt user to continue
					currMenu = 1;								//return to main
					break;
			case 8: operation = in_del_menu();					//menu asks user to insert to delete
					if(operation == 0){
						currMenu = 1;							//if neither, return to main
					}
					if(operation == 1){							//if inserting
						insert_num = insert_menu(root);			//go to insert sub-menu
						cout<<""<<endl;
						cout << "You have inserted " << insert_num << " into the tree." << endl;	//insert number and report
						hit_continue();																//prompt user to continue
					}
					else
					if(operation == 2){							//if deleting
						delete_num = delete_menu(root);			//go to delete sub-menu
						cout<<""<<endl;
						cout << "You have deleted " << delete_num << " from the tree." << endl;		//delete number and report
						hit_continue();																//prompt user to continue
					}
					currMenu = 1;								//return to main
					break;
			case 9: delete_all(root);							//delete all value in tree
					delete_all_message();						//report to user
					hit_continue();								//prompt user to continue
					currMenu = 1;								//return to main
					break;
		}
	}while(currMenu != 0);		//while quit option has not been selected
	
	return 0;					//end of program
}
