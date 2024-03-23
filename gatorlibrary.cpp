#include <bits/stdc++.h>
using namespace std;


//GLOBALLY DEFINED COLORFLIPS TO STORE THE NUMBER OF COLOR FLIPS PERFORMED
int colorflips = 0;


//RESERVATIONS NODE TO CREATE THE RESERVATION MIN HEAP
struct reservations{
  int priority;
  int patron_ID;
  int timestamp;
};

//RB-TREE NODE DEFINITION
struct Book {
  int BookId;
  string BookName;
  string AuthorName;
  bool Availability;
  int BorrowedBy;
  vector<reservations> ReservationHeap;
  Book *parent;
  Book *left;
  Book *right;
  int color;
  set<int> patrons_reserved;
};

//POINTER TO BOOK NODE
typedef Book *book_node_pointer;

class RedBlackTree {
   private:
  book_node_pointer root;
  book_node_pointer TNULL;
// INITILIAzING THE NULL NODE(LEAF NODES)
  void initializeNULLNode(book_node_pointer node, book_node_pointer parent) {
    node->BookId = 0;
    node->parent = parent;
    int BorrowedBy = 0;
    node->left = nullptr;
    node->right = nullptr;
    node->color = 0;
  }
  //FUNCTION TO GET THE BITH CLOSEST GREATER AND SMALLER VALUES TO THE KEY
  pair<int, int> getFloorAndCeil(vector<int> &a, int n, int x) {
    int start = 0, end = n - 1, ans = -1;
    pair<int,int> ret;
    while(start <= end){
      int mid = (start + end)/2;
      if(a[mid] <= x){
        ans = a[mid];
        start = mid + 1;
      }else
        end = mid - 1;
    }

    ret.first = ans;
    start = 0, end = n - 1, ans = -1;
    while(start <= end){
      int mid = (start + end)/2;
      if(a[mid] >= x){
        ans = a[mid];
        end = mid - 1;
      }else
        start = mid + 1;
    }
    ret.second = ans;
    return ret;

}
//FUNCTION TO PRINT THE BOOK DATA
  void printNode(book_node_pointer node) {
    if (node == TNULL) {
      return;
    }
    cout<<"Book ID =  "<<node->BookId<<endl;
    cout<<"Title = "<<node->BookName<<endl;
    cout<<"Availiability = "<<node->AuthorName<<endl;
    cout<<"Borrowed by = "<<(node->BorrowedBy == 0? 0: node->BorrowedBy)<<endl;
    cout<<"Reserved by = [";
    for(int i = 0; i < node->ReservationHeap.size(); i++){
      cout<<node->ReservationHeap[i].patron_ID;
      if(i < node->ReservationHeap.size() - 1)
        cout<<",";
      cout<<" ";
    }
    cout<<"]"<<endl;
  }
//FUNCTION TO MAKE THE INORDER TRAVERSAL OF THE TREE
  void inOrderHelper(book_node_pointer node, vector<int> &inord) {
    if (node != TNULL) {
      inOrderHelper(node->left, inord);
      inord.push_back(node->BookId);
      inOrderHelper(node->right, inord);
    }
  }
//FUNCTION TO PRINT THE BOOKS IN THE GIVEN RANGE
  void printBooksHelper(book_node_pointer node, int start, int end){
    if(node != TNULL){
      printBooksHelper(node->left, start, end);
      if(node->BookId >= start && node->BookId <= end){
        printNode(node);
      }
      printBooksHelper(node->right, start, end);
    }
  }
//FUNCTION TO SEARCH THE TREE
  book_node_pointer searchTreeHelper(book_node_pointer node, int key) {
    if (node == TNULL || key == node->BookId) {
      return node;
    }

    if (key < node->BookId) {
      return searchTreeHelper(node->left, key);
    }
    return searchTreeHelper(node->right, key);
  }
//FUNTION TO FIX THE TREE AFTER DELETION
  void deleteFix(book_node_pointer x) {
    book_node_pointer s;
    while (x != root && x->color == 0) {
      if (x == x->parent->left) {
        s = x->parent->right;
        if (s->color == 1) {
          colorflips += s->color == 0?0:1;
          s->color = 0;
          colorflips += x->parent->color == 1?0:1;
          x->parent->color = 1;
          leftRotate(x->parent);
          s = x->parent->right;
        }

        if (s->left->color == 0 && s->right->color == 0) {
          colorflips += s->color == 1?0:1;
          s->color = 1;
          x = x->parent;
        } else {
          if (s->right->color == 0) {
            colorflips += s->left->color == 0?0:1;
            s->left->color = 0;
            colorflips += s->color == 1?0:1;
            s->color = 1;
            rightRotate(s);
            s = x->parent->right;
          }
          s->color = x->parent->color;
          colorflips += x->parent->color == 0? 0: 1;
          x->parent->color = 0;
          colorflips += s->right->color == 0? 0: 1;
          s->right->color = 0;
          leftRotate(x->parent);
          x = root;
        }
      } else {
        s = x->parent->left;
        if (s->color == 1) {
          colorflips += s->color == 0? 0: 1;
          s->color = 0;
          colorflips += x->parent->color == 1? 0: 1;
          x->parent->color = 1;
          rightRotate(x->parent);
          s = x->parent->left;
        }

        if (s->right->color == 0) {
          colorflips += s->color == 0? 0: 1;
          s->color = 1;
          x = x->parent;
        } else {
          if (s->left->color == 0) {
            colorflips += s->right->color == 0? 0: 1;
            s->right->color = 0;
            colorflips += s->color == 1? 0: 1;
            s->color = 1;
            leftRotate(s);
            s = x->parent->left;
          }
          colorflips += s->color == x->parent->color? 0: 1;
          s->color = x->parent->color;
          colorflips += x->parent->color == 0?0: 1;
          x->parent->color = 0;
          colorflips += s->left->color == 0? 0:1;
          s->left->color = 0;
          rightRotate(x->parent);
          x = root;
        }
      }
    }
    x->color = 0;
  }
//FUNCTION TO TRANSPLANT THE NODES
  void rbTransplant(book_node_pointer u, book_node_pointer v) {
    if (u->parent == nullptr) {
      root = v;
    } else if (u == u->parent->left) {
      u->parent->left = v;
    } else {
      u->parent->right = v;
    }
    v->parent = u->parent;
  }
//FUNTION TO DELETE THE NODE
  void deleteNodeHelper(book_node_pointer node, int key) {
    book_node_pointer z = TNULL;
    book_node_pointer x, y;
    while (node != TNULL) {
      if (node->BookId == key) {
        cout<<"Book "<<node->BookId<<" no longer available.";
        if(node->ReservationHeap.size() != 0){
          cout<<" Reservations made by patrons ";
          for(int i = 0; i < node->ReservationHeap.size(); i++){
            cout<<node->ReservationHeap[i].patron_ID;
            if(i < node->ReservationHeap.size() - 1)
              cout<<",";
            cout<<" ";
          }
          cout<<"have been cancelled";
        }
        cout<<endl;
          
        z = node;
      }

      if (node->BookId <= key) {
        node = node->right;
      } else {
        node = node->left;
      }
    }

    if (z == TNULL) {
      cout << "Book "<<key<<" not found in library." << endl;
      return;
    }

    y = z;
    int y_original_color = y->color;
    if (z->left == TNULL) {
      x = z->right;
      rbTransplant(z, z->right);
    } else if (z->right == TNULL) {
      x = z->left;
      rbTransplant(z, z->left);
    } else {
      y = maximum(z->left);
      y_original_color = y->color;
      x = y->left;
      if (y->parent == z) {
        x->parent = y;
      } else {
        rbTransplant(y, y->left);
        y->right = z->left;
        y->left->parent = y;
      }

      rbTransplant(z, y);
      y->right = z->right;
      y->right->parent = y;
      y->color = z->color;
    }
    delete z;
    if (y_original_color == 0) {
      deleteFix(x);
    }
  }
//FUNCTION TO FIX THE TREE AFTER INSERTION
  void insertFix(book_node_pointer k) {
    book_node_pointer u;
    while (k->parent->color == 1) {
      if (k->parent == k->parent->parent->right) {
        u = k->parent->parent->left;  
        if (u != NULL && u->color == 1) {
          colorflips += k->color == 0? 0: 1;
          u->color = 0;
          colorflips += k->parent->color == 0? 0: 1;
          k->parent->color = 0;
          colorflips += k->parent->parent->color == 1? 0: 1;
          k->parent->parent->color = 1;
          k = k->parent->parent;
        } else {
          if (k == k->parent->left) {
            k = k->parent;
            rightRotate(k);
          }
          colorflips += k->parent->color == 0? 0: 1;
          k->parent->color = 0;
          colorflips += k->parent->parent->color == 1? 0: 1;
          k->parent->parent->color = 1;
          leftRotate(k->parent->parent);
        }
      } else {
        u = k->parent->parent->right;  // Uncle
        if (u != NULL && u->color == 1) {
          colorflips += k->color == 0? 0: 1;
          u->color = 0;
          colorflips += k->parent->color == 0? 0: 1;
          k->parent->color = 0;
          colorflips += k->parent->parent->color == 1? 0: 1;
          k->parent->parent->color = 1;
          k = k->parent->parent;
        } else {
          if (k == k->parent->right) {
            k = k->parent;
            leftRotate(k);
          }
          colorflips += k->parent->color == 0? 0: 1;
          k->parent->color = 0;
          colorflips += k->parent->parent->color == 1? 0: 1;
          k->parent->parent->color = 1;
          rightRotate(k->parent->parent);
        }
      }
    if (k == root)
      break;
  }
  root->color = 0;

  }
//FUNCTION TO ADD THE NEW RESERVATION TO THE RESERVATION HEAP
  void heapify(vector<reservations>& myHeap){
    int i = myHeap.size() - 1;
    for(;i > 0; i = (i - 1)/2){
      if(myHeap[i].priority < myHeap[(i - 1)/2].priority){
        reservations temp = myHeap[i];
        myHeap[i] = myHeap[(i - 1)/2];
        myHeap[(i - 1)/2] = temp;
      }else if(myHeap[i].priority == myHeap[(i - 1)/2].priority){
        if(myHeap[i].timestamp < myHeap[(i - 1)/2].timestamp){
          reservations temp = myHeap[i];
          myHeap[i] = myHeap[(i - 1)/2];
          myHeap[(i - 1)/2] = temp;
        }
      }
    }
  }
//FUNCTION TO DELETE THE TOP VALUE OF THE RESERVATION HEAP
void deleteMIN(vector<reservations> &myHeap){
    if(myHeap.size() == 1){
        myHeap.pop_back();
        return;
    }
    myHeap[0] = myHeap[myHeap.size() - 1];
    myHeap.pop_back();
    int i = 0;
    while (2*i + 1 < myHeap.size()) {
        int smallerChild = 2*i + 1;
        if (2*i + 2 < myHeap.size() && myHeap[2*i + 2].priority < myHeap[smallerChild].priority) {
            smallerChild = 2*i + 2;
        }
        if (myHeap[i].priority <= myHeap[smallerChild].priority) {
            break;
        }
        swap(myHeap[i], myHeap[smallerChild]);
        i = smallerChild;
    }
}
//FUNCTION TO PRINT THE TREE// REQUIRED TO VISUALIZE THE TREE
  void printHelper(book_node_pointer root, string indent, bool last) {
    if (root != TNULL) {
      cout << indent;
      if (last) {
        cout << "R----";
        indent += "   ";
      } else {
        cout << "L----";
        indent += "|  ";
      }

      string sColor = root->color? "RED" : "BLACK";
      cout << root->BookId << "(" << sColor << ")" << endl;
      printHelper(root->left, indent, false);
      printHelper(root->right, indent, true);
    }
  }

   public:
   //CONSTRUCTOR
  RedBlackTree() {
    

    TNULL = new Book;

    TNULL->color = 0;
    TNULL->left = nullptr;
    TNULL->right = nullptr;
    root = TNULL;
  }
//FUNCTION TO GET THE INORDER TRAVERSAL ARRAY OF THE TREE
  vector<int> inorder() {
    vector<int> inord;
    inOrderHelper(this->root,inord);
    return inord;
  }
//FUNCTION TO CALL THE FUNCTION TO SEARCH THE TREE
  book_node_pointer searchTree(int k) {
    return searchTreeHelper(this->root, k);
  }
//FUNCTION TO GET THE MAXIMUM VALUE OF THE SUBTREE AT A GIVEN node
  book_node_pointer maximum(book_node_pointer node) {
    while (node->right != TNULL) {
      node = node->right;
    }
    return node;
  }
//FUNCTION TO LEFT ROTATE THE TREE AT A GIVEN NODE
  void leftRotate(book_node_pointer x) {
    book_node_pointer y = x->right;
    x->right = y->left;
    if (y->left != TNULL) {
      y->left->parent = x;
    }
    y->parent = x->parent;
    if (x->parent == nullptr) {
      this->root = y;
    } else if (x == x->parent->left) {
      x->parent->left = y;
    } else {
      x->parent->right = y;
    }
    y->left = x;
    x->parent = y;
  }
//FUNCTION TO RIGHT ROTATE THE TREE AT A GIVEN NODE
  void rightRotate(book_node_pointer x) {
    book_node_pointer y = x->left;
    x->left = y->right;
    if (y->right != TNULL) {
      y->right->parent = x;
    }
    y->parent = x->parent;
    if (x->parent == nullptr) {
      this->root = y;
    } else if (x == x->parent->right) {
      x->parent->right = y;
    } else {
      x->parent->left = y;
    }
    y->right = x;
    x->parent = y;
  }
//FUNCTION TO INSERT A NEW BOOK NODE IN THE TREE
  void InsertBook(int key, string book, string AuthorName, string Availability) {
    book_node_pointer node = new Book;
    node->parent = nullptr;
    node->BookId = key;
    node->AuthorName = AuthorName;
    node->Availability = true;
    node->BorrowedBy = 0;
    node->BookName = book;
    node->left = TNULL;
    node->right = TNULL;
    node->color = 1;

    book_node_pointer y = nullptr;
    book_node_pointer x = this->root;

    while (x != TNULL) {
      y = x;
      if(node->BookId == x->BookId){
          cout<<"Book already Exists!"<<endl;
          return;
      }
      if (node->BookId < x->BookId) {
        x = x->left;
      } else {
        x = x->right;
      }
    }

    node->parent = y;
    if (y == nullptr) {
      root = node;
    } else if (node->BookId < y->BookId) {
      y->left = node;
    } else {
      y->right = node;
    }

    if (node->parent == nullptr) {
      node->color = 0;
      return;
    }

    if (node->parent->parent == nullptr) {
      return;
    }

    insertFix(node);
  }
//FUNCTION TO DELETE A BOOK FROM THE TREE
  void deleteNode(int BookId) {
    deleteNodeHelper(this->root, BookId);
  }
//FUNTION TO CALL THE FUNCTION TO PRINT THE TREE
  void printTree() {
    if (root) {
      printHelper(this->root, "", true);
    }
  }
//PUBLIC FUNCTION TO CALL THE PRIVATE FUNCTION TO BORROW A BOOK
  void BorrowBook(int patronId, int bookID, int priority){
      book_node_pointer node = searchTree(bookID);
      if(node == TNULL){
          cout<<"Book not found"<<endl;
          return;
      }
      if(node->Availability == true){
          node->Availability = false;
          node->BorrowedBy = patronId;
          cout<<"Book "<<bookID<<" borrowed by Patron "<<patronId<<endl;
          return;
      }else if(node->Availability == false){
        if(node->BorrowedBy == patronId){
          cout<<"Book already borrowed by you"<<endl;
          return;
        }else{
          if(node->patrons_reserved.find(patronId) != node->patrons_reserved.end()){
            cout<<"Book already reserved by you"<<endl;
            return;
          }else{
            node->patrons_reserved.insert(patronId);
            reservations r;
            r.patron_ID = patronId;
            r.priority = priority;
            r.timestamp = time(0);
            node->ReservationHeap.push_back(r);
            heapify(node->ReservationHeap);
            cout<<"Book "<<bookID<<" reserved by Patron "<<patronId<<endl;
          }

        }

      }
  }
  //PUBLIC FUNCTION TO CALL THE PRIVATE FUNCTION TO RETURN A BOOK
  void returnBook(int patronID, int bookID){
    
    book_node_pointer node = searchTree(bookID);
    if(node->BorrowedBy == patronID){
      cout<<node->BookId<<" Returned by Patron "<<patronID<<endl;
      if(node->ReservationHeap.size() != 0){
        cout<<node->BookId<<" Allotted to Patron "<<node->ReservationHeap[0].patron_ID<<endl;
        node->BorrowedBy == node->ReservationHeap[0].patron_ID;
        node->patrons_reserved.erase(node->BorrowedBy);
        deleteMIN(node->ReservationHeap);
      }else{
        node->BorrowedBy = 0;
        node->Availability = true;
      }
    }else{
      cout<<"Book not borrowed by you"<<endl;
    }
  }
//PUBLIC FUNCTION TO CALL THE PRIVATE FUNCTION TO PRINT THE BOOKS IN THE GIVEN RANGE
  void printBooks(int start, int end){
    printBooksHelper(this->root, start, end);
  }
  void PrintBook(int key);
//PROTOTYPE OF THE FUNCTION OF THE SEARCH CLOSEST FUNCTION
  void searchClosest(int key);
};
//FUNCTION DEFINITION TO SEARCH THE BOOKS CLOSEST TO THE GIVEN KEY
void RedBlackTree::searchClosest(int key){
      book_node_pointer node = this->root;
      vector<int> inord = inorder();
      pair<int,int> ans = getFloorAndCeil(inord,inord.size(),key);
      if(ans.first == -1){
          book_node_pointer node = searchTree(ans.second);
          printNode(node);
      }else if(ans.second == -1){
          book_node_pointer node = searchTree(ans.first);
          printNode(node);
      }else{
          if(key - ans.first < ans.second - key){
            book_node_pointer node = searchTree(ans.first);
            printNode(node);
          }else if (key - ans.first > ans.second - key){
              book_node_pointer node = searchTree(ans.second);
              printNode(node);
          }else{
              book_node_pointer node = searchTree(ans.first);
              book_node_pointer node1 = searchTree(ans.second);
              printNode(node);
              printNode(node1);

            }
      } 
  }

// FINCTION DEFINITION TO SEARCH THE NODE WITH A GIVEN KEY
void RedBlackTree::PrintBook(int key){
    book_node_pointer node = searchTree(key);
    if(node == NULL){
        cout<<"Book not found"<<endl;
        return;
    }
    printNode(node);
}

//MAIN DRIVER FUNCTION
int main(int argc, char* argv[]) {

    RedBlackTree bst;
    //TO GET THE INPUT FILENAME
    ifstream file(argv[1]);
    string line;
    string command;
    //TO MAKE THE OUTPUT FILENAME
    string fname = string(argv[1]).substr(0, string(argv[1]).length() - 4);
    string outputFileName = fname + "_output_file.txt";

    
    //CHANGING THE OUTPUT BUFFER FROM CONSOLE TO AN OUTPUT FILE
    ofstream outputFile(outputFileName);
    streambuf* coutbuf = cout.rdbuf(); 
    cout.rdbuf(outputFile.rdbuf());
    
    //GETTING THE CONTENT OF THE FILE
    while (!file.eof()) {
        getline(file, line);
        istringstream iss(line);
        getline(iss, command, '(');
        if (command == "InsertBook") {
            int key;
            string title, AuthorName, availability;
            iss >> key;
            getline(iss, title, ',');
            getline(iss, title, ',');
            getline(iss, AuthorName, ',');
            getline(iss, availability, ')' );
            bst.InsertBook(key, title, AuthorName, availability);
        } else if (command == "PrintBook") {
            int key;
            iss >> key;
            bst.PrintBook(key);
        } else if (command == "searchClosest") {
            int key;
            iss >> key;
            bst.searchClosest(key);
        } else if (command == "FindClosestBook") {
            int key;
            iss >> key;
            bst.searchClosest(key);
        } else if (command == "BorrowBook") {
            string args;
            getline(iss, args); 

            for (char& c : args) {
                if (c == '(' || c == ')' || c == ',') {
                    c = ' ';
                }
            }

            istringstream argsStream(args);
            int patronId, bookID, priority;
            argsStream >> patronId >> bookID >> priority;

            bst.BorrowBook(patronId, bookID, priority);
        } else if (command == "ReturnBook") {
            string args;
            getline(iss, args); 

            for (char& c : args) {
                if (c == '(' || c == ')' || c == ',') {
                    c = ' ';
                }
            }
            istringstream argsStream(args);
            int bookID, patronID;
            argsStream >> bookID >> patronID;
            bst.returnBook(bookID, patronID);
        } else if (command == "DeleteBook") {
            int key;
            iss >> key;
            bst.deleteNode(key);
        } else if (command == "PrintBooks") {
            string args;
            getline(iss, args); 

            for (char& c : args) {
                if (c == '(' || c == ')' || c == ',') {
                    c = ' ';
                }
            }

            std::istringstream argsStream(args);
            int start, end;
            argsStream >> start >> end;
            bst.printBooks(start, end);


        } else if (command == "Quit") {
            cout<<"Program Terminated!!"<<endl;
            cout.rdbuf(coutbuf);
            return 0;
        } else if (command == "ColorFlipCount") {
            cout<<"Color Flip Count:"<<colorflips<<endl;
        } else {
            cout << "Invalid command: " << command << endl;
        }
    }
    cout.rdbuf(coutbuf);
    return 0;
}