#include <iostream>
#include <fstream>
#include <cstring>
#include <stdlib.h>

using namespace std;

/* Author: Austin Holst
 * Date: 4 - 30 - 18
 * Code: Creates and prints out a red black data tree from user input
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

//Constants for red or black
const int BLACK = 0;
const int RED = 1;

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
    
    cout << "Would you like to read in numbers 'manually' or from a 'file' or 'print' or 'quit'?" << endl;
    char answer[10];
    cin >> answer;
    if(strcmp(answer, "manually") == 0) {
      int number;
      Node* newNode = new Node;
      cout << "Input a number" << endl;
      cin >> newNode->data;
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
	  char* charNum = new char[length];
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
    else {
      cout << "That wasn't one of the options..." << endl;
    }
  }  
  return 0;
}

//Print out the tree
void print(Node* head, int space) {
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
  cout << "ROTATE LEFT" << endl;
  Node* nNew = n->right;
  //Make sure nNew is not a leaf
  if(nNew != NULL) {
    //set connections
    n->right = nNew->left;
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
    }
  }
}

//Rotate to the right
void rotate_right(Node* &head, Node* n) {
  cout << "ROTATE RIGHT" << endl;
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
    cout << "TEST" << endl;
    if(head->right != NULL) {
      insert_recurse(head->right, n);
      return;
    }
    else {
      cout << "TEST 2" << endl;
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
    cout << "CASE 1" << endl;
    n->color = BLACK;
  }
}

//if the node entered has a black parent then you are all gucci
void case2(Node* &head, Node* n) {
  cout << "CASE 2" << endl;
  return;
}

//Parent and uncle are red
void case3(Node* &head, Node* n) {
  cout << "CASE 3" << endl;
  parent(n)->color = BLACK;
  uncle(n)->color = BLACK;
  if(grandparent(n) != head) {
    grandparent(n)->color = RED;
  }
  insert_repair_tree(head, grandparent(n));
}

//Basically every other possibility
void case4(Node* &head, Node* n) {
  cout << "CASE 4" << endl;
  Node* p = parent(n);
  Node* g = grandparent(n);
  print(head, 0);
  
  if(g->left != NULL && g->left->right != NULL && n == g->left->right) {
    print(head, 0);
    rotate_left(head, p);
    print(head, 0);
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
	cout << "here" << endl;
	g->parent = p;
      }
      cout << "Test 1111" << endl;
      g->right = p->left;
      p->left = g;
      head = p;
      p->right = n;
      n->parent = p;
      p->color = BLACK;
      g->color = RED;
    }
    else if(g->parent->right != NULL && g == g->parent->right) {//seg fault
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
  cout << "CASE 4 Part 2" << endl;
  cout << "Case 4 part 2 Head: " << head->data << endl;
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
