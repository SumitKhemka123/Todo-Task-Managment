#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include <ctime> //for getting the current time
#include <stack>
#include <string>
#include <vector>
#include <algorithm>
using namespace std;

class ToDoNode //class for each individual to do item, instances of class will be store in BST
{public:
  int urgency; //how urgent the task is to complete
  string note; //what the task is (ex: "calc hw")
  string creationTime; //when the notes was created

  ToDoNode *parent;
  ToDoNode *leftChild;
  ToDoNode *rightChild;

  ToDoNode()
  {
    parent = leftChild = rightChild = nullptr;
  }

  ToDoNode(int u, string n, string c)
  {
    urgency = u;
    note = n;
    creationTime = c;
    parent = leftChild = rightChild = nullptr;
  }
};

class ToDoTree //class for our BST (Binary Search Tree) of to-dos organized by urgency
{

public:
  ToDoTree(); //constructor
  ~ToDoTree(); //destructor
  void printAllToDo(); //print entire tree
  void filePrint(string name); //print data to file for saving
  void editToDo(int oldUrgency, string note, int newUrgency, string newNote); //edit a todo item
  void addToDo(int urgency, string note, string time, bool newTime); //add new item to tree
  void findToDo(int urgency, string note); //find a to do
  bool deleteToDo(int urgency, string note); //delete from the tree
  void countToDo(); //count up todos
  void setTasks(int com, int curr); //set the number of completed and current tasks from our file data
  int getCompleted(); //get the number of completed tasks
  int getCurrent(); //get the number of current/incomplete tasks
  ToDoNode* getRoot(); //get the root
  void getMaxInfo(); //get the most urgent task's information
  void random(); //find random task in the tree

private:
  ToDoNode *search(int urgency, string note); //search for a node
  ToDoNode *findRand(ToDoNode *root, int count); //find a random node
  ToDoNode *root; //root of tree
  int completed; //number of completed to-dos
  int current; //number of current tasks that are not yet completed
};




ToDoTree::ToDoTree() //constructor
{
  root=NULL;
  completed=0;
  current=0;
}


void deleteToDoTree(ToDoNode* root) //recursive function to delete nodes in the tree in postorder
{
    if (root == NULL) //go to end of branch
    {
      return;
    }

    deleteToDoTree(root->leftChild); //go to left tree
    deleteToDoTree(root->rightChild); //go to right tree
    delete root; //delete node
}

ToDoTree::~ToDoTree() //destructor
{
  deleteToDoTree(root); //delete all nodes
  root = NULL; //root is now null
}

ToDoNode* ToDoTree::findRand(ToDoNode *root, int count) //iterative method to find a random node given a random integer between 0 and current-1
{
    stack<ToDoNode *> stack; //stack to store nodes
    ToDoNode *current = root; //set current at root

    while (current != NULL || stack.empty() == false) //go through whole tree
    {
        while (current !=  NULL) //go all the way to the left
        {
            stack.push(current); //put in stack
            current = current->leftChild; //move down to the left again
        }

        //current is now null

        current = stack.top(); //go back up the tree to last node
        stack.pop(); //pop stack

        if (count==0) //we have reached our random node
        {
          return current; //exit and return
        }

        current = current->rightChild; //now we go to the right

        count--; //decrement our random count
    }
  return current;
}

ToDoNode* ToDoTree::getRoot() //get root (private member)
{
  return root;
}

void ToDoTree::getMaxInfo() //get root info (the most urgent task)
{
  ToDoNode* mostUrgent = getRoot(); //get the root of the tree

  if (mostUrgent==NULL)
  {
    cout << "Empty List" << endl;
  }

  if (mostUrgent!=NULL)
  {
    while (mostUrgent->rightChild != NULL) //go to most urgent (rightmost child)
    {
      mostUrgent = mostUrgent->rightChild;
    }
    cout << endl;
    cout << "Most Urgent To-Do:" << endl;
    cout << "===========" << endl;
    cout << "Urgency: " << mostUrgent->urgency << endl;
    cout << "Task: " << mostUrgent->note << endl;
    cout << "Time Created: " << mostUrgent->creationTime<<endl;
    cout << endl;
  }
}

void ToDoTree::random() //random function
{
  int curr=getCurrent(); //current number of nodes in the tree
  srand(time(0)); //seed the function
  if (curr != 0) //tree is not empty
  {
    int randomNum = rand() % curr; //between 0 and current-1
    ToDoNode* randNode = findRand(getRoot(),randomNum); //get random node and print out it's data
    cout << endl;
    cout << "Random To-Do:" << endl;
    cout << "===========" << endl;
    cout << "Urgency: " << randNode->urgency << endl;
    cout << "Task: " << randNode->note << endl;
    cout << "Time Created: " << randNode->creationTime<<endl;
    cout << endl;
  }
  else
  {
    cout << "Empty To-Do List!" << endl;
  }
}


void ToDoTree::editToDo(int urgency, string note, int newUrgency, string newNote) //edit a task
{
  ToDoNode* foundToDo = search(urgency, note); //search for task
  if (foundToDo == NULL) //null returned by search function
  {
    cout << "To-Do not found." << endl;
  }
  else
  {
    string time = foundToDo->creationTime; //reset the time it was created to now
    deleteToDo(urgency, note); //get rid of old note
    addToDo(newUrgency, newNote, time, true); //make new note
  }
}

void ToDoTree::findToDo(int urgency, string title) //find to do in tree
{
  ToDoNode* foundToDo = search(urgency, title); //search for to do
  if (foundToDo == NULL)
  {
    cout << "To-Do not found." << endl;
  }
  else
  {
    cout << endl;
    cout << "To-Do Info:" << endl;
    cout << "===========" << endl;
    cout << "Urgency: " << foundToDo->urgency << endl;
    cout << "Task: " << foundToDo->note << endl;
    cout << "Time Created: " << foundToDo->creationTime<<endl;
    cout << endl;
  }
}

ToDoNode* searchToDoTree(ToDoNode* root, int searchUrgency, string searchNote) {
    if (root == nullptr || root->note == searchNote) {
        return root;
    }

    if (searchUrgency <= root->urgency) {
        return searchToDoTree(root->leftChild, searchUrgency, searchNote);
    } else {
        return searchToDoTree(root->rightChild, searchUrgency, searchNote);
    }
}



ToDoNode* ToDoTree::search(int urgency, string note) //calls recursive helper function above
{
  return searchToDoTree(root, urgency, note); //return node from recursive search function above
}

void ToDoTree::addToDo(int urgency, string note, string myTime, bool oldTime)
{

  //-----------------------------------------------------

  // from: https://stackoverflow.com/a/16358224
  time_t _tm =time(NULL);

  struct tm * curtime = localtime ( &_tm );

  //-----------------------------------------------------

  if (!oldTime) //this is a brand new todo (not one loaded from the file) so we need the current time
  {
    myTime=asctime(curtime);
  }


  ToDoNode *newToDo = new ToDoNode(urgency, note, myTime); //create new todo node
  ToDoNode *currentNode = root; //set current to root

  newToDo->parent = NULL; //parent is set to NULL

  if (root == NULL) //first todo in tree
  {
      root = newToDo; //set root
      return;
  }
  while (currentNode != NULL)
  {
      newToDo->parent = currentNode; //move down parent
      if (newToDo->urgency <= currentNode->urgency) //urgency is less than or equal to current
      {
        currentNode = currentNode->leftChild; //move down to left
      }
      else //greater urgency than current
      {
        currentNode = currentNode->rightChild; //move down to right
      }
  }

  //We have now reached the leaves

  if (newToDo->urgency <= newToDo->parent->urgency) //urgency is less than or equal to current
  {
    newToDo->parent->leftChild = newToDo; //set left child
  }
  else //greater urgency than current
  {
    newToDo->parent->rightChild = newToDo; //set right child
  }
}

void inOrderTraverse(ToDoNode *root) //in order traversal of tree based on urgency
{
  if(root != NULL) //"go back up" in stack frame
  {
    inOrderTraverse(root->rightChild); //we print max to min urgency
    cout << "Task: " << root->note <<endl;
    cout<< "Urgency: " << root->urgency <<endl;
    cout<< "Time Created: " << root->creationTime << endl <<endl;
    inOrderTraverse(root->leftChild);
  }
}

void ToDoTree::printAllToDo()
{
  if (root == NULL)
  {
    std::ifstream inFile("a.txt");

    if (inFile.is_open()) {
        std::string line;

        while (std::getline(inFile, line)) {
            std::cout << line << std::endl;
        }
  }}
  inOrderTraverse(root); //call recursive function to print out todos
}

void ToDoTree::filePrint(string name) //"save" the todos to a file which we can reload in the future
{

  ofstream myFileStream; //output file stream variable
  myFileStream.open(name, ofstream::out | ofstream::trunc); //open file
  if (myFileStream.is_open()) //successfully opened
  {

    //Print to file
    myFileStream << completed << "," << current << endl; //put completed and current numbers at the top

    stack<ToDoNode *> stack;
    ToDoNode *current = root;

    while (current != NULL || stack.empty() == false) //use stack to print entire tree
    {
        while (current !=  NULL) //go all the way to the rightmost node
        {
            stack.push(current); //put in stack
            current = current->rightChild; //go to right child
        }

        //current is now null

        current = stack.top(); //reset current to the previous parent
        stack.pop(); //pop

        myFileStream << current->urgency << "," << current->note << "," << current->creationTime << "\n"; //print to file

        //Note: The way we print means that when we rebuild the tree, the most urgent task will be the root,
        //this allows for a faster access time in the future

        current = current->leftChild; //now go to the left

    }

    cout << "To-Do List saved to file. Goodbye!" << endl; //Success message
  }
  else //failed to open
  {
    cout << "Error! Could not open file to save data" << endl; //Error message
  }
  myFileStream.close(); //close file
}

bool ToDoTree::deleteToDo(int deleteUrgency, string deleteNote) //delete a todo in the tree
{
  //Nodes for going through and deleting an element of the movie tree

  ToDoNode *temp;
  ToDoNode *tempParent;
  ToDoNode *newParent;
  ToDoNode *temp2;
  ToDoNode *tempChild;

  bool left = false; //true for left, false for right

  //Step 1: Find the node to delete. Using this method as opposed to search function so we know which child (left or right) we have
  tempParent = NULL;
  temp=root; //start at root
  while(temp != NULL) //go through whole tree
  {
     if (deleteNote == temp->note)
     {
       break; // Found the todo to delete
     }
     else if (deleteUrgency <= temp->urgency) //go to left
     {
         tempParent = temp;
         left = true;
         temp = temp->leftChild;
     }
     else //go to right
     {
         tempParent = temp;
         left = false;
         temp = temp->rightChild;
     }
  }

  //Now, our temp is the todo to be deleted, or NULL if it was not found

  if (temp == NULL) //not found
  {
      cout << "To-Do not found." << endl;
      return false;
  }

  //if here, todo is the movie to delete

   if ((temp->leftChild == NULL) && (temp->rightChild == NULL)) //leaf node
   {
       if (tempParent == NULL) //it is the root
       {
            delete temp;
            root = NULL;
       }
       else //has a parent
       {
             delete temp; //delete the node
             if (left==true) //it was the left child of the parent
             {
               tempParent->leftChild = NULL;
             }
             else //right child
             {
               tempParent->rightChild = NULL;
             }
       }
   }
   else if ((temp->leftChild == NULL) || (temp->rightChild == NULL)) //only 1 child
   {
     if (temp->leftChild != NULL) //has a left child
     {
       tempChild = temp->leftChild;
     }
     else //has a right child
     {
       tempChild = temp->rightChild;
     }
     if (tempParent == NULL) //root
     {
        root = tempChild;
        delete temp;
     }
     else //not the root, the parent of the deleted movie will take the child of the deleted movie
     {
        if (left==true)
        {
          tempParent->leftChild = tempChild;
        }
        else
        {
          tempParent->rightChild =  tempChild;
        }
        delete temp;
      }
   }
   else //both children exist, find max urgency in the left subtree
   {
      newParent = temp;
      temp2 =  temp->leftChild; //go to left so we find the max in the left subtree
      left = true; //left child
      while(temp2->rightChild != NULL) //go down to bottom of right branch, or do not go in this loop at all if it is the left child
      {
        newParent = temp2; //increment
        temp2 = temp2->rightChild;
        left = false; //right child
      }

      //now we have found the max in the left subtree, we now copy the data to our node to delete
      temp->note = temp2->note;
      temp->urgency = temp2->urgency;
      temp->creationTime = temp2->creationTime;

       //now, our todo data has been copied and temp2 is either the left child of temp or is a right child of temp->leftChild

      if (left==true) //left child case
      {
            temp->leftChild = temp2->leftChild; //move the branch up
            delete temp2; //get rid of old node we copied data from
      }
      else //other case
      {
          newParent->rightChild = temp2->leftChild; //move the branch up in order to delete the old node we copied from
          delete temp2;
      }
    }

    cout << "Task Completed. Nice Job!" << endl;
    return true; //return type for function is void
}

void ToDoTree::setTasks(int com, int curr) //given data from file, set the num of tasks
{
  completed=com;
  current=curr;
}

int ToDoTree::getCompleted()
{
  return completed;
}

int ToDoTree::getCurrent()
{
  return current;
}

int countFunc(ToDoNode* root) //count up nodes recursively
{
  if( root == NULL) //empty
  {
    return 0;
  }
  else //not empty
  {
    if (root->rightChild == NULL && root->leftChild == NULL) //only the root in the tree
    {
      return 1;
    }
    else //not just the root
    {
      return (1 + (countFunc(root->leftChild) + countFunc(root->rightChild))); //1 is the root and then recursively call for right and left children to add up
    }
  }
}

void ToDoTree::countToDo() //count movie function
{
  cout<< "Tasks = " << countFunc(root) << endl; //print out result from above recursive function
}

int main()
{

  ToDoTree myTree; //create instance of the class (our BST of todos)
  ifstream inFile; //input file stream object
  inFile.open("a.txt"); //open text file

  if (! inFile) //failed to open
  {
    cout << "Error";
    return -1;
  }



  // ...


  inFile.close(); //open text file


  string stringInput = ""; //user's choice
  int input = 0; //integer for input

  //Welcome message
  cout<<"Welcome to your To-Do List!"<<endl;
  cout << "Current: " << myTree.getCurrent() <<endl;
  cout << "Completed: " << myTree.getCompleted() <<endl;

  while (input != 9) //main menu display for user interaction
  {

    cout << "======Main Menu======" << endl;
    cout << "1. Find a Task" << endl;
    cout << "2. Edit a Task" << endl;
    cout << "3. View All Tasks" << endl;
    cout << "4. Complete a Task" << endl;
    cout << "5. View Task Count" << endl;
    cout << "6. Add a Task" << endl;
    cout << "7. Random Task" << endl;
    cout << "8. Most Urgent" << endl;
    cout << "9. Exit and Save" << endl;

    getline(cin, stringInput); //get input
    while (stringInput != "1" && stringInput != "2" && stringInput != "3" && stringInput != "4"&& stringInput != "5"&& stringInput != "6" && stringInput != "7"&& stringInput != "8"&& stringInput != "9")
    {
      cout << "Incorrect Menu Input. Try again:"<<endl;
      getline(cin, stringInput);
    }

    input = stoi(stringInput); //convert to int

    if (input == 1) //find a task
    {
        cout << "Enter Urgency (1-10):" << endl;
        string myUrgency;
        getline(cin, myUrgency);

        if (myUrgency == "1" || myUrgency == "2" || myUrgency == "3" || myUrgency == "4" || myUrgency == "5" || myUrgency == "6" || myUrgency == "7" || myUrgency == "8" || myUrgency == "9" || myUrgency == "10")
        {
            int urg = stoi(myUrgency);
            cout << "Enter Note:" << endl;
            string myNote;
            getline(cin, myNote);

            // Read the contents of "a.txt" and search for the task
            ifstream inFile("a.txt");
            string line;

            while (getline(inFile, line))
            {
                stringstream ss(line);
                string token;

                // Tokenize the line using ',' as a delimiter
                getline(ss, token, ',');
                int fileUrg = stoi(token);

                // Check if urgency matches
                if (fileUrg == urg)
                {
                    // Get the second part of the line as the note
                    getline(ss, token, ',');
                    string fileNote = token;

                    // Check if note matches
                    if (fileNote == myNote)
                    {
                        // Task found in the file
                        cout << "Task found in file: " << line << endl;
                        break;
                    }
                  else{
                    cout << "Task not found in file :(" << endl;
                  }
                }
            }

            inFile.close();

            // If the loop completes, the task was not found


            // You can still call myTree.findToDo(urg, myNote) if needed
        }
        else
        {
            cout << "Invalid Input :(" << endl;
        }
    }

    if (input == 2) // edit a task, requires new and old info for the todo
    {
        cout << "Enter Old Urgency (1-10):" << endl;
        string oldUrgency;
        getline(cin, oldUrgency);
        if (oldUrgency == "1" || oldUrgency == "2" || oldUrgency == "3" || oldUrgency == "4" || oldUrgency == "5" || oldUrgency == "6" || oldUrgency == "7" || oldUrgency == "8" || oldUrgency == "9" || oldUrgency == "10")
        {
            cout << "Enter Old Note:" << endl;
            string oldNote;
            getline(cin, oldNote);

            // Read the contents of "a.txt" and store them in a vector
            ifstream inFile("a.txt");
            vector<string> lines;
            string line;

            while (getline(inFile, line))
            {
                lines.push_back(line);
            }

            inFile.close();

            // Find the line that matches the old urgency and note
            auto it = find(lines.begin(), lines.end(), oldUrgency + "," + oldNote);
          if (it != lines.end())
          {
              // Erase the old line and add the new line
              lines.erase(it);
              cout << "Enter New Urgency (1-10):" << endl;
              string newUrgency;
              getline(cin, newUrgency);

              if (newUrgency == "1" || newUrgency == "2" || newUrgency == "3" || newUrgency == "4" || newUrgency == "5" || newUrgency == "6" || newUrgency == "7" || newUrgency == "8" || newUrgency == "9" || newUrgency == "10")
              {
                  int newUrg = stoi(newUrgency);
                  cout << "Enter New Note:" << endl;
                  string newNote;
                  getline(cin, newNote);

                  // Add the new urgency and note to the vector
                  lines.push_back(newUrgency + "," + newNote);

                  // Write the updated vector back to "a.txt"
                  ofstream outFile("a.txt");
                  for (const auto &l : lines)
                  {
                      outFile << l << "\n";
                  }

                  outFile.close();

                  int oldUrg = stoi(oldUrgency);  // Convert oldUrgency to an integer
                  myTree.editToDo(oldUrg, oldNote, newUrg, newNote);
              }
              else
              {
                  cout << "Invalid Input :(" << endl;
              }
          }

            else
            {
                cout << "Task not found :(" << endl;
            }
        }
        else
        {
            cout << "Invalid Input :(" << endl;
        }
    }

    if (input == 3) //see all current tasks
    {
      myTree.printAllToDo();
    }

    if (input == 4) //delete a task (it is now completed)
    {
      cout << "Enter Urgency (1-10):" << endl;
      string delUrgency;
      getline(cin, delUrgency);
      if (delUrgency == "1" ||delUrgency == "2" ||delUrgency == "3" ||delUrgency == "4" ||delUrgency == "5" ||delUrgency == "6" ||delUrgency == "7" ||delUrgency == "8" ||delUrgency == "9" ||delUrgency == "10")
      {
        int delUrg = stoi(delUrgency);
        cout << "Enter Note:" << endl;
        string delNote;
        getline(cin, delNote);
        if (myTree.deleteToDo(delUrg, delNote))
        {
          int newComp = myTree.getCompleted() + 1;
          int newCurr = myTree.getCurrent() - 1;
          myTree.setTasks(newComp, newCurr);
        }
      }
      else
      {
        cout << "Invalid Input :("<< endl;
      }
    }

    if (input == 5) //see how many tasks you have and how many have been completed
    {
      cout << "Current: " << myTree.getCurrent() <<endl;
      cout << "Completed: " << myTree.getCompleted() <<endl;
    }

    #include <ctime>

    if (input == 6) // add new task
    {
        cout << "Enter Urgency (1-10):" << endl;
        string addUrgency;
        getline(cin, addUrgency);
        if (addUrgency == "1" || addUrgency == "2" || addUrgency == "3" || addUrgency == "4" || addUrgency == "5" || addUrgency == "6" || addUrgency == "7" || addUrgency == "8" || addUrgency == "9" || addUrgency == "10")
        {
            int addUrg = stoi(addUrgency);
            cout << "Enter Note:" << endl;
            string addNote;
            getline(cin, addNote);

            // Get the current time for the new task
            time_t now = time(nullptr);
            tm tm = *localtime(&now);
            char buffer[80];
            strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", &tm);
            string addTime = buffer;

            // Add the new task to the tree
            myTree.addToDo(addUrg, addNote, addTime, false);

            // Update the task counts
            int add = myTree.getCurrent() + 1;
            int comp = myTree.getCompleted();
            myTree.setTasks(comp, add);
          ofstream outFile("a.txt", ios::app);
          if (outFile.is_open())
          {
              outFile << addUrg << "," << addNote << "," <<  addTime<<"\n";
              outFile.close();
          }
          else
          {
              cerr << "Error opening file for appending." << endl;
          }
        }
        else
        {
            cout << "Invalid Input :(" << endl;
        }
    }


    if (input == 7) //see a random task
    {
      myTree.random();
    }

    if (input == 8) //see your most urgent task
    {
      myTree.getMaxInfo();
    }

    if (input == 9) //print all the tree data to the original txt file for future use
    {
      cout << "Saving your To-List..." << endl;

      myTree.filePrint("todo.txt");
    }

  }

  return 0;
}



