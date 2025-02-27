//============================================================================
// Name        : Project Two.cpp
// Author      : Freddy Case
// Description : Project 2
//============================================================================

#include <iostream>
#include <time.h>
#include <vector>
#include <string>
#include <fstream>
#include <sstream>

using namespace std;

//============================================================================
// Global definitions visible to all methods and classes
//============================================================================

// forward declarations
double strToDouble(string str, char ch);

// Structure to hold course information
struct Course {
    string courseId; // unique identifier
    string title;
    vector<string> prerequisite;
};

// Internal structure for tree node
struct Node {
    Course course;
    Node* left;
    Node* right;

    // default constructor
    Node() {
        left = nullptr;
        right = nullptr;
    }

    // initialize with a course
    Node(Course aCourse) :
        Node() {
        course = aCourse;
    }
};

//============================================================================
// Binary Search Tree class definition
//============================================================================

/**
 * Define a class containing data members and methods to
 * implement a binary search tree
 */
class BinarySearchTree {

private:
    Node* root;

    void addNode(Node* node, Course course);
    void insertRecursive(Node* node, Course course);
    Node* searchRecursive(Node* node, string courseId);
    Node* removeNode(Node* node, string courseId);
    void inOrder(Node* node);
    void postOrder(Node* node);
    void preOrder(Node* node);
    void traverseTree(Node* node);

public:
    BinarySearchTree();
    virtual ~BinarySearchTree();
    void Insert(Course course);
    void Remove(string courseId);
    Course Search(string courseId);
    void InOrder();
    void PostOrder();
    void PreOrder();
    bool validateFile(string filename);
    void loadCourses(string csvPath, BinarySearchTree& courses);
    void validateCourses(BinarySearchTree& courses);
    void displayCourse(Course course);
    void searchCourse(BinarySearchTree& courses, string courseId);
    void printCourse(BinarySearchTree& tree);
    void inOrderTraversal(Node* node);
};

/**
 * Splits a string into a vector based on specified delimeter
 *
 * @param s The input string to be split.
 * @param delimiter The character used to split the string.
 * @return A vector containing the extracted substrings.
 */
vector<string> split(const string& s, char delimiter) {
    // Vector that stores split substrings
    vector<string> tokens;
    // string to hold token
    string token;
    // input string stream from the given string
    istringstream tokenStream(s);
    // read each substring separated by delimeter
    while (getline(tokenStream, token, delimiter)) {
        // add the extracted token to the vector
        tokens.push_back(token);
    }
    // if delimiter is the last character
    if (s.back() == delimiter) {
        // add empty string as last token
        tokens.push_back("");
    }
    // return vector 'tokens'
    return tokens;
}

/**
 * Converts all characters in a selected string to upper case
 *
 * @param str The input string to be converted.
 * @return A new string where all characters are uppercase.
 */
string toUpperCase(string str) {
    // Iterate through each character in string
    for (size_t i = 0; i < str.length(); i++) {
        // convert each string to uppercase
        str[i] = toupper(str[i]);
    }
    // return new uppercase string
    return str;
}

/**
 * Default constructor
 */
BinarySearchTree::BinarySearchTree() {
    //root is equal to nullptr
    root = nullptr;
}

/**
 * Destructor
 */
BinarySearchTree::~BinarySearchTree() {
    // recurse from root deleting every node
    while (root != nullptr) {
        Remove(root->course.courseId);
    }
}

/**
 * Traverse the tree in order
 */
void BinarySearchTree::InOrder() {
    // call inOrder fuction and pass root 
    this->inOrder(root);
}

/**
 * Traverse the tree in post-order
 */
void BinarySearchTree::PostOrder() {
    // postOrder root
    this->postOrder(root);
}

/**
 * Traverse the tree in pre-order
 */
void BinarySearchTree::PreOrder() {
    // preOrder root
    this->preOrder(root);
}



/**
 * Insert a course
 */
void BinarySearchTree::Insert(Course course) {
    // if root equarl to null ptr
    if (root == nullptr) {
        // root is equal to new node bid
        root = new Node(course);
    }
    // else
    else {
        // add Node root and course
        addNode(root, course);
    }
}

/**
 * Remove a course
 */
void BinarySearchTree::Remove(string courseId) {
    // remove node root courseID
    this->removeNode(root, courseId);
}

/**
 * Search for a course
 */
Course BinarySearchTree::Search(string courseId) {
    // converts user input for courseId to upper case
    courseId = toUpperCase(courseId);
    // set current node equal to root
    Node* current = root;
    // keep looping downwards until bottom reached or matching courseId found
    while (current != nullptr) {
        // convert stored courseId to uppercase
        string currentCourseId = toUpperCase(current->course.courseId);
        // if match found, return current course
        if (current->course.courseId == courseId) {
            return current->course;
        }
        // if course is smaller than current node then traverse left
        if (courseId.compare(current->course.courseId) < 0) {
            current = current->left;
        }
        // else larger so traverse right
        else {
            current = current->right;
        }
    }
    // return empty course if not found
    return Course();
}

/**
 * Add a course to some node (recursive)
 *
 * @param node Current node in tree
 * @param Course course to be added
 */
void BinarySearchTree::addNode(Node* node, Course course) {
    // if node is larger then add to left
    if (node->course.courseId.compare(course.courseId) > 0) {
        // if no left node
        if (node->left == nullptr) {
            // this node becomes left
            node->left = new Node(course);
        }
        // else
        else {
            // recurse down the left node
            this->addNode(node->left, course);
        }
    }
    // else
    else {
        // if no right node
        if (node->right == nullptr) {
            // this node becomes right
            node->right = new Node(course);
        }
        // else
        else {
            // recurse down the right node
            this->addNode(node->right, course);
        }

    }
}

/**
 * Recursively inserts a new course into the binary search tree.
 *
 * @param node Pointer to the current node being examined.
 * @param course The course to be inserted into the tree.
 */
void BinarySearchTree::insertRecursive(Node* node, Course course) {
    // checks if courseId of the new course is less than the current nodes courseId
    if (node->course.courseId.compare(course.courseId) > 0) {
        //if node.left is null
        if (node->left == nullptr) {
            //set node.left to new Node(course)
            node->left = new Node(course);
        }
        //else
        else {
            // recursively call insertRecursive on the left subtree
            addNode(node->left, course);
        }
    }
    //else
    else {
        //if node.right is null
        if (node->right == nullptr) {
            //set node.right to new node(course)
            node->right = new Node(course);
        }
        //else
        else {
            // recursively call insertRecursive on the right subtree
            addNode(node->right, course);
        }
    }
}

/**
 * Recursively searches for a course in the binary search tree.
 *
 * @param node Pointer to the current node being examined.
 * @param courseId The unique course ID to search for.
 * @return Pointer to the node containing the course if found, otherwise nullptr.
 */
Node* BinarySearchTree::searchRecursive(Node* node, string courseId) {
    //if node is nullptr, course was not found
    if (node == nullptr)
        return nullptr;
    //if current nodes courseID matches search Id, return node
    if (node->course.courseId == courseId)
        return node;
    // if courseID is less than the current nodes courseID, search left subtree
    if (courseId.compare(node->course.courseId) > 0)
        //return searchRecursive(node.left, courseId)
        return searchRecursive(node->left, courseId);
    else
        // else search right subtree
        return searchRecursive(node->right, courseId);

}

/**
 * Performs an in-order traversal of the binary search tree.
 * This traversal visits nodes in ascending order (left, root, right).
 *
 * @param node Pointer to the current node being processed.
 */
void BinarySearchTree::inOrder(Node* node) {
    // if node is not null continue traversal
    if (node != nullptr) {
        // recursively traverse left subtree
        inOrder(node->left);
        // output courseId, title
        cout << node->course.courseId << ", "
            << node->course.title << endl;
        // recursively traverse right subtree
        inOrder(node->right);
    }
}

/**
 * Performs a post-order traversal of the binary search tree.
 * This traversal visits nodes in the following order: left subtree, right subtree, then root.
 *
 * @param node Pointer to the current node being processed.
 */
void BinarySearchTree::postOrder(Node* node) {
    // if node is not null continue traversal
    if (node != nullptr) {
        // recursively traverse left subtree first
        postOrder(node->left);
        // recursively traverse right subtree next
        postOrder(node->right);
        // output courseID, title
        cout << node->course.courseId << " : "
            << node->course.title << endl;
    }
}

/**
 * Perform a pre-order traversal of the binary search tree.
 * In pre-order traversal, the node is processed before its child nodes.
 *
 * @param node The current node being processed.
 */
void BinarySearchTree::preOrder(Node* node) {
    // check if the node is not null
    if (node != nullptr) {
        // output courseID, title
        cout << node->course.courseId << " : "
            << node->course.title << endl;
        // recursively process the left subtree
        preOrder(node->left);
        // recursively process the right subtree
        preOrder(node->right);
    }
}

/**
 * Remove a course from some node (recursive)
 *
 * @param node The current node being examined.
 * @param courseId The ID of the course to be removed.
 * @return The updated node after removal.
 */
Node* BinarySearchTree::removeNode(Node* node, string courseId) {
    // if node is null, return null
    if (node == nullptr) {
        return node;
    }
    // otherwise recurse down the left subtree
    if (courseId.compare(node->course.courseId) < 0) {
        // check for match and if so, remove left node using recursive call
        node->left = removeNode(node->left, courseId);
    }
    // otherwise recurse down the right subtree
    else if (courseId.compare(node->course.courseId) > 0) {
        // check for match and if so, remove right node using recursive call
        node->right = removeNode(node->right, courseId);
    }
    // found node to be deleted
    else {
        // case 1: node is a leaf node with no children
        if (node->left == nullptr && node->right == nullptr) {
            delete node;
            node = nullptr;
        }
        // case 2: node has only a left node
        else if (node->left != nullptr && node->right == nullptr) {
            Node* temp = node;
            node = node->left;
            delete temp;
        }
        // case 3: node has only a right node
        else if (node->left == nullptr && node->right != nullptr) {
            Node* temp = node;
            node = node->right;
            delete temp;
        }
        // case 4: node has two children
        else {
            // create temp node to right
            Node* temp = node->right;
            // while left node is not nullptr keep moving temp left
            while (temp->left != nullptr) {
                temp = temp->left;
            }
            // make node bid (right) equal to temp bid (left)
            node->course = temp->course;
            // remove right node using recursive call
            node->right = removeNode(node->right, temp->course.courseId);
        }
    }
    // return node
    return node;
}

/**
 * Recursively traverses BST to check for missing prerequisites.
 * This function verifies if each course's prerequisites exist in the tree.
 *
 * @param node The current node being examined in the traversal.
 */
void BinarySearchTree::traverseTree(Node* node) {
    // if node is null, return
    if (node == nullptr)
        return;
    // Iterate through each prerequisite of current course
    for (const auto& pr : node->course.prerequisite) {
        // Search for prerequisites in BST
        if (this->Search(pr).courseId == "") {
            //  if not count, print error
            cout << "course " << pr << "has missing prerequisite." << endl;
        }
    }
    // recursively traverse the left subtree
    traverseTree(node->left);
    // recursively traverse the right subtree
    traverseTree(node->right);
}

/**
 * Validates courses in the binary search tree by checking if all prerequisites exist.
 *
 * @param courses The BinarySearchTree containing all the courses to validate.
 */
void BinarySearchTree::validateCourses(BinarySearchTree& courses) {
    // traverse tree from the root to check for missing prerequisites
    traverseTree(courses.root);

}

/**
 * Displays course ID, title, and prerequisites (if any).
 *
 * @param course The Course object containing course information.
 */
void BinarySearchTree::displayCourse(Course course) {
    // print courseID and title
    cout << course.courseId << ", " << course.title << endl;
    // if there are any prereuisites
    if (!course.prerequisite.empty()) {
        // print "Prerequisites: "
        cout << "Prerequisites: ";
        // itereate through prerequisites for course and print each one
        for (size_t i = 0; i < course.prerequisite.size(); i++) {
            // print prereuisite
            cout << course.prerequisite[i];
            // adds comma between each prerequisite
            if (i < course.prerequisite.size() - 1) {
                // comma separator between prerequisites
                cout << ", ";
            }
        }
        // add a new line after adding all prerequisites
        cout << endl;
    }
}

/**
 * Searches for a course in the binary search tree and displays its details.
 *
 * @param courses The BinarySearchTree object containing course data.
 * @param courseId The ID of the course to search for.
 */
void BinarySearchTree::searchCourse(BinarySearchTree& courses, string courseId) {
    // have node correspond to courseId using recursive search
    Node* courseNode = courses.searchRecursive(courses.root, courseId);
    // if the courseNode is not null
    if (courseNode != nullptr) {
        //print out the course information
        displayCourse(courseNode->course);
    }
    // else print that the course was not found.
    else {
        cout << "course not found" << endl;
    }
}

/**
 * Prints all courses in the Binary Search Tree (BST) in alphanumeric order.
 *
 * @param tree The BinarySearchTree object containing the course data.
 */
void BinarySearchTree::printCourse(BinarySearchTree& tree) {
    // print courses in sorted traversal order
    inOrderTraversal(tree.root);
}

/**
 * Performs an in-order traversal of the Binary Search Tree (BST)
 * to print course details in sorted order.
 *
 * @param node The current node being processed in the BST.
 */
void BinarySearchTree::inOrderTraversal(Node* node) {
    // if node is null, return to stop recursion
    if (node == nullptr)
        return;
    // traverse though the left subtree
    inOrderTraversal(node->left);
    // print current nodes course information
    cout << node->course.courseId << ": " << node->course.title;
    // if the course has no prerequisites
    if (node->course.prerequisite.empty()) {
        // print "No prerequisites"
        cout << "No prerequisites" << endl;
    }
    // else print prerequisites
    else {
        cout << "Prerequisite(s): ";
        for (const auto& pr : node->course.prerequisite)
            // print space after every prerequisite
            cout << pr << " " << endl;
    }
    // traverse the right subtree
    inOrderTraversal(node->right);
}


//============================================================================
// Static methods used for testing
//============================================================================

bool BinarySearchTree::validateFile(string filename) {
    //open file
    ifstream inFile(filename);
    // alerts user with errors opening file
    if (!inFile.is_open()) {
        cout << "Error opening file." << endl;
        return false;
    }
    // add comment
    string line;
    // while looking at each line in file
    while (getline(inFile, line)) {
        //partition each parameter between commas
        vector<string> tokens = split(line, ',');
        // if file has less than 2 parameters
        if (tokens.size() < 2) {
            // print “filename does not have enough parameters”
            cout << "filename does not have enough parameters" << endl;
            //close file
            inFile.close();
            return false;
        }
        //close file
        inFile.close();
        return true;
    }
}

/**
 * Loads course data from a CSV file and inserts it into a Binary Search Tree.
 *
 * @param csvPath The file path of the CSV file containing course data.
 * @param courses The Binary Search Tree to store the course data.
 */void loadCourses(string csvPath, BinarySearchTree& courses) {
    // checks to see if the csv file is valid using validater. If not, return.
    if (!courses.validateFile(csvPath))
        return;
    //open csv file. Alerts user if file cannot open.
    ifstream inFile(csvPath);
    if (!inFile.is_open()) {
        cout << "Error opening file" << endl;
        return;
    }
    //
    string line;
    //for each line in validated file
    while (getline(inFile, line)) {
        //Create course newCourse
        Course newCourse;
        // partition each parameter between commas.
        vector<string> tokens = split(line, ',');
        // if there are fewer than 2 parameters, skip this line
        if (tokens.size() < 2)
            continue;
        //set newCourse.courseId to first partitioned parameter
        newCourse.courseId = tokens[0];
        //set newCourse.title to second partitioned parameter
        newCourse.title = tokens[1];
        // for all additional partitioned parameters
        for (size_t i = 2; i < tokens.size(); i++) {
            //  add to newCourse.prerequisite
            if (!tokens[i].empty()) {
                newCourse.prerequisite.push_back(tokens[i]);
            }
        }
        //courses.insert(newCourse)
        courses.Insert(newCourse);
    }
    // close file
    inFile.close();
}

/**
 * Simple C function to convert a string to a double
 * after stripping out unwanted char
 *
 * credit: http://stackoverflow.com/a/24875936
 *
 * @param ch The character to strip out
 */
double strToDouble(string str, char ch) {
    str.erase(remove(str.begin(), str.end(), ch), str.end());
    return atof(str.c_str());
}

/**
 * The one and only main() method
 */
int main(int argc, char* argv[]) {

    // process command line arguments
    string csvPath, courseKey;
    switch (argc) {
    case 2:
        csvPath = argv[1];
        courseKey = "98223";
        break;
    case 3:
        csvPath = argv[1];
        courseKey = argv[2];
        break;
    default:
        csvPath = "CS 300 ABCU_Advising_Program_Input.csv";
        courseKey = "98223";
    }

    // Define a binary search tree to hold all courses
    BinarySearchTree* bst;
    bst = new BinarySearchTree();
    Course course;

    int choice = 0;
    cout << "Welcome to the Course Planner" << endl;
    cout << endl;

    while (choice != 9) {
        cout << "  1. Load Data Structure" << endl;
        cout << "  2. Print Course List" << endl;
        cout << "  3. Print Course" << endl;
        cout << "  9. Exit" << endl;
        cout << endl;
        cout << "What would you like to do? ";
        cin >> choice;

        switch (choice) {

        case 1:

            // tells user program is loading the data structure
            cout << "Loading Data Structure..." << endl;

            // Complete the method call to load the bids
            loadCourses(csvPath, *bst);

            cout << "...Data loaded!" << endl;
            cout << endl;
            break;

        case 2:
            // tells user that they selected a sample schedule
            cout << "Here is a sample schedule: " << endl;
            cout << endl;
            // perform in order traversak if BST to print courses in sorted order
            bst->InOrder();
            cout << endl;
            break;

        case 3:
            // prompts user to enter the course they want to look up
            cout << "What course do you want to know about? ";
            // take user input
            cin >> courseKey;
            // search input in bst
            course = bst->Search(courseKey);
            //if course exists, print details
            if (!course.courseId.empty()) {
                bst->displayCourse(course);
                }
            // if course is not found, print error message
            else {
                cout << "Course '" << courseKey << "' not found. Please re-enter!" << endl;
            }
            break;

            // exits the program
        case 9:
            cout << "Thank you for using the course planner!" << endl;
            cout << endl;
            return 0;

            // default to choice not being a valid option if a case number does not have a menu option.
        default:
            cout << choice << " is not a vaild option" << endl;
            cout << endl;
        }
    }

    return 0;
}
