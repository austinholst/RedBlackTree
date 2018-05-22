#include <iostream>
#include <fstream>
#include <cstring>
#include <stdlib.h>

using namespace std;

/* Author: Austin Holst
 * Date: 5 - 22 - 18
 * Code: Creates and prints out a red black data tree from user input. Let's you delete any nodes that you want.
 * Note: Big shout out to Raveen for showing me the two function delete structure
 */


//Struct for nodes
struct Node {
  Node* parent;
  Node* left;
  Node* right;
  int color;
  int data;
};

//Prototypes
void print(Node* head, int space);
Node* parent(Node* n); 
Node* grandparent(Node* n);
Node* sibling(Node* n);
Node* unlce(Node* n);
void rotate_left(Node* &head, Node* n);
void rotate_right(Node* &head, Node* n);
void insert(Node* &head, Node* n);
void insert_recurse(Node* &head, Node* n);
void insert_repair_tree(Node* &head, Node* n);
void case1(Node* &head, Node* n); 
void case2(Node* &head, Node* n);
void case3(Node* &head, Node* n);
void case4(Node* &head, Node* n);
void case4Part2(Node* &head, Node* n);
Node* searchReturn(Node* head, int number);
Node* replace_node(Node* n, Node* child);
Node* remove(Node* &head, int removing);
void removal(Node* n, int side);
Node* findPre(Node* n);
bool isBlack(Node* n);
//Constants for red or black
const int BLACK = 0;
const int RED = 1;
const int LEFT = 1;
const int RIGHT = 2;
//Head declaration
Node* head = new Node;

int main() {
  bool running = true;
  Node* nodeArray[100];
  int* numbers = new int[200];
  int counter = 0;
  head = NULL;

  cout << "Welcome to Red Black Tree! Where coders go to die." << endl;
  while(running == true) {
    
    cout << "Would you like to read in numbers 'manually' or from a 'file' or 'print' or 'search' or 'delete' or 'quit'?" << endl;
    char answer[10];
    cin >> answer;
    if(strcmp(answer, "manually") == 0) {
      int number;
      Node* newNode = new Node;
      cout << "Input a number" << endl;
      cin >> newNode->data;
      counter++;
      insert(head, newNode);
    }
    else if(strcmp(answer, "file") == 0) {
      cout << "What is the name of the file you wish to read in from (be sure to add the .txt)" << endl;
      char fileName[100];
      //Take in the name of the file
      cin >> fileName;
      char input[200];
      //Set the file to the user input
      ifstream myFile(fileName);
      //If the file is opened correctly
      if(myFile.is_open()) {
	myFile.getline(input, 200);
	//Close the file
	myFile.close();
	//From here on down it's the same as the manual input
	for(int i = 0; i < strlen(input); i++) {
	  int start = i;
	  int length = 1;
	  while(input[i + 1] != char(44) && i < strlen(input)) {
	    length++;
	    i++;
	  }
	  char* charNum = new char[length + 1];
	  charNum[length] = '\0';
	  for(int j = 0; j < length; j++) {
	    charNum[j] = input[start + j];
	  }
	  numbers[counter] = atoi(charNum);
	  counter++;
	  i++;
	}
	for(int i = 0; i < counter; i++) {
	  Node* newNode = new Node;
	  newNode->data = numbers[i];
	  nodeArray[i] = newNode;
	}
	for(int i = 0; i < counter; i++) {
	  cout << "Node " << i << " Data: " << nodeArray[i]->data << endl;
	  insert(head, nodeArray[i]);
	}
      }
    }
    else if(strcmp(answer, "print") == 0) {
      print(head, 0);
    }
    else if(strcmp(answer, "quit") == 0) {
      cout << "Ending program" << endl;
      running = false;
    }
    else if(strcmp(answer, "search") == 0) {
      int number;
      cout << "What number do you want to search for?" << endl;
      cin >> number;
      if(searchReturn(head, number) == NULL) {
	cout << "That number is not in the tree" << endl;
      }
      else {
	cout << "That number is in the tree" << endl;
      }
    }
    else if(strcmp(answer, "delete") == 0) {
      if(head == NULL || head->data == 0) {
	cout << "The tree is empty, nothing to delete" << endl;
      }
      else {
	int number;
	cout << "What number do you want to delete?" << endl;
	cin >> number;
	if(searchReturn(head, number) == NULL) {
	   cout << "That number is not in the tree" << endl;
	}
	else {
	   head = remove(head, number);

	   if(head == NULL) {
	     delete head;
	     head = NULL;
	     break;
	   }
       	 }
       }
     }
     else {
      cout << "That wasn't one of the options..." << endl;
    }
  }  
  return 0;
}

//Print out the tree
void print(Node* head, int space) {
  if(head == NULL || head->data == 0) {
    cout << "The tree is empty" << endl;
  }
  else {
    if(head->right != NULL) {
      print(head->right, ++space);
      space--;
    }
    for(int i = 1; i <= space; i++) {
      cout << "\t";
    }
    cout << head->data;
    if(head->color == RED) {
      cout << "R" << endl;
    }
    else {
      cout << "B" << endl;
    }
    if(head->left != NULL) {
      print(head->left, ++space);
      space--;
    }
  }
}

//Gets the nodes parent
Node* parent(Node* n) {
  return n->parent;
}

//Gets the nodes grandparent
Node* grandparent(Node* n) {
  Node* p  = parent(n);
  if(p == NULL) {
    return NULL;
  }
  else {
    return parent(p);
  }
}

//Gets the nodes sibling
Node* sibling(Node* n) {
  Node* p = parent(n);
  if(p == NULL) {
    return NULL;
  }
  if(n == p->left) {
    return p->right;
  }
  else {
    return p->left;
  }
}

//Gets the nodes uncle
Node* uncle(Node* n) {
  Node* p = parent(n);
  Node* g = grandparent(n);
  if(g == NULL) {
    return NULL;
  }
  return sibling(p);
}

//Rotate to the left 
void rotate_left(Node* &head, Node* n) {
  Node* nNew = n->right;
  //Make sure nNew is not a leaf
  if(nNew != NULL) {
    //set connections
    n->right = nNew->left;
    if(nNew->left != NULL) {
      nNew->left->parent = n;
    }
    nNew->left = n;
    nNew->parent = n->parent;
    n->parent = nNew;
    //Set the parent to child conneciton
    if(nNew->parent != NULL && nNew->parent->left == n) {
      nNew->parent->left = nNew;
    }
    else if(nNew->parent != NULL && nNew->parent->right == n) {
      nNew->parent->right = nNew;
    }
    if(n == head) {
      head = nNew;
      head->color = BLACK;
      head->parent = NULL;
    }
  }
}

//Rotate to the right
void rotate_right(Node* &head, Node* n) {
  Node* nNew = n->left;
  //Make sure nNew is not a leaf
  if(nNew != NULL) {
    //set parent to child connection
    if(n->parent != NULL && n == n->parent->left) {
      n->parent->left = n->left;
    }
    else if(n->parent != NULL && n == n->parent->right) {
      n->parent->right = n->left;
    }
    n->left = nNew->right;
    //set child to parent connection
    if(nNew->right != NULL) {
      nNew->right->parent = n;
    }
    nNew->right = n;
    nNew->parent = n->parent;
    n->parent = nNew;
    //if you swapped out the head then change it
    if(n == head) {
      head = nNew;
      head->color = BLACK;
      head->parent = NULL;
    }
  }
}

//Put the new node into the tree
void insert(Node* &head, Node* n) {
  insert_recurse(head, n);
  //repair the tree in case any of the red-black properties have been violated
  insert_repair_tree(head, n);
   
}

//Recursively go down the tree until a leaf is found
void insert_recurse(Node* &head, Node* n) {
  //If theres no head
  if(head == NULL) {
    head = n;
    //insert the n node
    n->parent = NULL;
    n->left = NULL;
    n->right = NULL;
    n->color = RED;
  }
  //if n is less than head
  else if(head != NULL && n->data < head->data) {
    if(head->left != NULL) {
      insert_recurse(head->left, n);
      return;
    }
    else {
      head->left = n;
      //insert the n node
      n->parent = head;
      n->left = NULL;
      n->right = NULL;
      n->color = RED;
    }
  }
  //If n is greater than head
  else {
    if(head->right != NULL) {
      insert_recurse(head->right, n);
      return;
    }
    else {
      head->right = n;
      //insert the n node
      n->parent = head;
      n->left = NULL;
      n->right = NULL;
      n->color = RED;
    }
  }
}

//Call of the different case functions
void insert_repair_tree(Node* &head, Node* n) {
  if(parent(n) == NULL) {
    case1(head, n);
  }
  else if(parent(n)->color == BLACK) {
    case2(head,n);
  }
  else if(uncle(n)!= NULL && uncle(n)->color == RED) {
    case3(head, n);
  }
  else {
    case4(head, n);
  }
}


//if the node entered is the head then make it black
void case1(Node* &head, Node* n) {
  if(n->parent == NULL) {
    n->color = BLACK;
  }
}

//if the node entered has a black parent then you are all gucci
void case2(Node* &head, Node* n) {
  return;
}

//Parent and uncle are red
void case3(Node* &head, Node* n) {
  parent(n)->color = BLACK;
  uncle(n)->color = BLACK;
  if(grandparent(n) != head) {
    grandparent(n)->color = RED;
  }
  insert_repair_tree(head, grandparent(n));
}

//Basically every other possibility
void case4(Node* &head, Node* n) {
  Node* p = parent(n);
  Node* g = grandparent(n);
  
  if(g->left != NULL && g->left->right != NULL && n == g->left->right) {
    rotate_left(head, p);
  }
  else if(g->right != NULL && g->right->left != NULL && n == g->right->left) {
    rotate_right(head, p);
    n = n->right;
    case4Part2(head, n);
    return;
  }
  else if(g->right != NULL && g->right->right !=NULL && n == g->right->right) {
    if(g == head) {
      if(p-> left != NULL) {
	g->right = p->left;
	p->left->parent = g;
	g->parent = p;
      }
      else {
	g->parent = p;
      }
      g->right = p->left;
      p->left = g;
      head = p;
      head->parent = NULL;
      p->right = n;
      n->parent = p;
      p->color = BLACK;
      g->color = RED;
    }
    else if(g->parent->right != NULL && g == g->parent->right) {
      g->parent->right = p;
      p->left = g;
      p->parent = g->parent;
      g->parent = p;
      g->right = NULL;
      p->color = BLACK;
      g->color = RED;
    }
    else {
      g->parent->left = p;
      p->left = g;
      p->parent = g->parent;
      g->parent = p;
      g->right = NULL;
      p->color = BLACK;
      g->color = RED;
    }
    return;
  }
  else if(g->left != NULL && g->left->left != NULL && n == g->left->left && n->color == RED && n->parent->color == RED) {
    case4Part2(head, n);
    return;
  }
  case4Part2(head, n->left);
}

//rotates to the right or the left around the grandparent is needed and changes color
void case4Part2(Node* &head, Node* n) {
  Node* p = parent(n);
  Node* g = grandparent(n);
  
  if(n->parent->left != NULL && n == p->left) {
    rotate_right(head, g);
  }
  else {
    rotate_left(head, g);
  }
  p->color = BLACK;
  g->color = RED;
}


//Returns the searched for number in the tree
Node* searchReturn(Node* head, int number) {
  //If the number is bigger than head go right
  if(number > head->data) {
    //If heads right is not null then recursively go there
    if(head->right != NULL) {
      return searchReturn(head->right, number);
    }
    //if it is null then the number is not in the tree
    else {
      return NULL;
    }
  }
  else if(number == head->data) {
    return head;
  }
  else if(number < head->data) {
    if(head->left != NULL) {
      return searchReturn(head->left, number);
    }
    else {
      return NULL;
    }
  }
}


Node* replace_node(Node* old, Node* newNode) {
  //if the node being replaced is not the head
  if(parent(old) != NULL) {
    newNode->parent = parent(old);

    if(parent(old)->right == old) {
      parent(old)->right = newNode;
    }
    else {
      parent(old)->left = newNode;
    }
    old->parent = NULL;
    return old;
  }
  //If the node being replaced is the root, change the root's data
  else {
    old->data = newNode->data;
    old->left = newNode->left;
    old->right = newNode->right;
    if(old->left != NULL) {
      old->left->parent = old;
    }
    if(old->right != NULL) {
      old->right->parent = old;
    }
    return newNode;
  }
}

//Finds if the node is black or not
bool isBlack(Node* n) {
  if(n != NULL && n->color == BLACK) {
    return true;
  }
  else if(n != NULL && n->color == RED) {
    return false;
  }
  else {
    return true;
  }
}

//removes the node from the tree
void removal(Node* n, int side) {
  //if then node being removed is the root, special case
  if(side != 0) {
    //if the sibling is red
    if(isBlack(sibling(n)) == false) {
      //switch the parent and sibling colors
      sibling(n)->color = BLACK;
      parent(n)->color = RED;
      
      //roteat around parent, making sibling the grandparent
      if(side == RIGHT) {
	rotate_right(head, parent(n));
      }
      else {
	rotate_left(head, parent(n));
      }
    }

    //sibling is black
    //if siblings children are black

    if(isBlack(sibling(n)->right) && isBlack(sibling(n)->left)) {
      //if n is black
      if(isBlack(parent(n)) == true) { //USED TO BE ISBLACK(N)
	//balance tree by making sibling red
	sibling(n)->color = RED;

	//basically this section of the tree is unbalanced relative to the entire tree
	//so call recursively on n's parent
	if(!parent(parent(n))) {
	  removal(parent(n), 0);
	}
	else if(parent(parent(n))->right == parent(n)) {
	  removal(parent(n), RIGHT);
	}
	else {
	  removal(parent(n), LEFT);
	}
	return;
      }
      
      //the parent is red
      else {
	parent(n)->color = BLACK;
	sibling(n)->color = RED;
	return;
      } 
    }
    
    

    //n sibling has to have one red child
    if(side == RIGHT && (sibling(n)->left == NULL || isBlack(sibling(n)->left) == true) && (isBlack(sibling(n)->right) == false)) {
      sibling(n)->right->color = BLACK;
      sibling(n)->color = RED;
      rotate_left(head, sibling(n));
    }
    else if(side == LEFT && (sibling(n)->right == NULL || isBlack(sibling(n)->right) == true) && (isBlack(sibling(n)->left) == false)) {
      if(sibling(n)->left != NULL) {
	sibling(n)->left->color = BLACK;
      }
      sibling(n)->color = RED;
      rotate_right(head, sibling(n));
    }
    
    //outside child of n sibling has to be red now

    //finish balancing tree
    sibling(n)->color = parent(n)->color;
    parent(n)->color = BLACK;
    if(side == RIGHT) {
      sibling(n)->left->color = BLACK;
      rotate_right(head, parent(n));
    }
    else {
      if(sibling(n)->right != NULL) {
	sibling(n)->right->color = BLACK;
      }
      rotate_left(head, parent(n));
    }
  }
}

//returns the tree after removing
Node* remove(Node* &head, int removing) {
  Node* deleting = searchReturn(head, removing);
  
    
  if(deleting != NULL) {
    //If there are two children for the node being deleted
    if(deleting->right != NULL && deleting->left != NULL) {
      //Find the next smallest number
      Node* replacement = deleting->left;
      while(replacement->right != NULL) {
	replacement = replacement->right;
      }
      //replace the node
      deleting->data = replacement->data;
      deleting = replacement;
    }
    
    int side = 0;
    //find if deleting is left or right child (2 = left, 1 = right)
    if(parent(deleting) != NULL) {
      if(deleting == parent(deleting)->right) {
	side = RIGHT;
      }
      else {
	side = LEFT;
      }
    }

    //if deleting is red, it can be deleted
    if(isBlack(deleting) == false) {
      if(side == RIGHT) {
	parent(deleting)->right = NULL;
      }
      else if(side == LEFT) {
	parent(deleting)->left = NULL;
      }
    }
    //deleting is black with two kids
    else if(deleting->right != NULL || deleting->left != NULL) {
      //child is whichever child of deleting isn't null
      Node* child = new Node;
      if(deleting->right != NULL) {
	child = deleting->right;
      }
      else {
	child = deleting->left;
      }
      
      child->color = BLACK;
      deleting = replace_node(deleting, child);
    }
    //deleting is black with no kids
    else {
      removal(deleting, side);
    }
    

    //If its the root then special case
    if(parent(deleting) == NULL) {
      deleting->data = 0;
    }
    //if its not you can delete it
    else {
      if(side == RIGHT && parent(deleting) != NULL) {
	parent(deleting)->right = NULL;
      }
      else if(side == LEFT && parent(deleting) != NULL) {
	parent(deleting)->left = NULL;
      }
      delete deleting;
    }
  }
  else {
    cout << "That number wasn't in the tree" << endl;
  }
  
  while(parent(head) != NULL) {
    head = parent(head);
  }
  return head;
}
