//Group project Assessment II
#include <iostream>
#include <string>
#include <stack>
#include <vector>
#include <limits>
using namespace std;

// Node class to represent each line in the text editor
class Node {
public:
    string data;
    Node* prev;
    Node* next;

    // Constructor
    Node(string line) {
        data = line;
        prev = nullptr;
        next = nullptr;
    }
};

// TextEditor class to manage the doubly linked list
class TextEditor {
private:
    Node* head;
    Node* tail;
    stack<vector<string>> undoStack; // For undo
    stack<vector<string>> redoStack; // For redo

    // Save current state of the text editor for undo/redo
    vector<string> captureState() {
        vector<string> state;
        Node* temp = head;
        while (temp != nullptr) {
            state.push_back(temp->data);
            temp = temp->next;
        }
        return state;
    }

    // Restore the text editor to a previous state undo/redo
    void restoreState(const vector<string>& state) {
        Node* temp = head;
        while (temp != nullptr) {
            Node* next = temp->next;
            delete temp;
            temp = next;
        }
        head = tail = nullptr;

        for (const string& line : state) {
            insertAtEnd(line, false);
        }
    }

public:
    // Constructor 
    //this initializes the head and tail pointers to null and create an empty text editor
    TextEditor() {
        head = nullptr;
        tail = nullptr;
    }

    // Destructor
    //deletes all the line of the text editor
    ~TextEditor() {
        Node* temp = head;
        while (temp != nullptr) {
            Node* next = temp->next;
            delete temp;
            temp = next;
        }
    }

    // function to Insert at the beginning
    void insertAtBeginning(const string& line, bool saveState = true) {
        //for undo and redo functionality
        if (saveState) {
            undoStack.push(captureState());
            while (!redoStack.empty()) redoStack.pop();//this line clears the redo stack
            //redo can only be done after undo
        }

        Node* newNode = new Node(line);
        if (head == nullptr) {
            head = tail = newNode;
        } else {
            //newNode->next = head;
            //head->prev = newNode;
            head = newNode;
        }
    }

    // function to Insert at a specific position
    void insertAtPosition(const string& line, int position, bool saveState = true) {
         //for undo and redo functionality
        if (saveState) {
            undoStack.push(captureState());
            while (!redoStack.empty()) redoStack.pop();
        }

        if (position <= 1) {
            insertAtBeginning(line, false);
            return;
        }

        Node* newNode = new Node(line);
        Node* temp = head;
        int currentPos = 1;

        while (temp != nullptr && currentPos < position - 1) {
            temp = temp->next;
            currentPos++;
        }

        if (temp == nullptr) {
            insertAtEnd(line, false);
        } else {
            newNode->next = temp->next;
            newNode->prev = temp;
            if (temp->next != nullptr) {
                temp->next->prev = newNode;
            } else {
                tail = newNode;
            }
            temp->next = newNode;
        }
    }

    // function to Insert at the end
    void insertAtEnd(const string& line, bool saveState = true) {
         //for undo and redo functionality
        if (saveState) {
            undoStack.push(captureState());
            while (!redoStack.empty()) redoStack.pop();
        }

        Node* newNode = new Node(line);
        if (head == nullptr) {
            head = tail = newNode;
        } else {
            tail->next = newNode;
            newNode->prev = tail;
            tail = newNode;
        }
    }

    // function to Delete at a specific position
    void deleteAtPosition(int position) {
        if (head == nullptr || position <= 0) {
            cout << "Invalid position or the list is empty" << endl;
            return;
        }
         //for undo and redo functionality
        undoStack.push(captureState());
        while (!redoStack.empty()) redoStack.pop();

        Node* temp = head;
        int currentPos = 1;

        while (temp != nullptr && currentPos < position) {
            temp = temp->next;
            currentPos++;
        }

        if (temp == nullptr) {
            cout << "Position is out of range" << endl;
            return;
        }

        if (temp->prev != nullptr) {
            temp->prev->next = temp->next;
        } else {
            head = temp->next;
        }

        if (temp->next != nullptr) {
            temp->next->prev = temp->prev;
        } else {
            tail = temp->prev;
        }

        delete temp;
        cout << "Line deleted at position " << position << endl;
    }

    // function to Edit a line at a specific position
    void editAtPosition(int position, const string& newLine) {
        if (head == nullptr || position <= 0) {
            cout << "Invalid position or the list is empty" << endl;
            return;
        }
         //for undo and redo functionality
        undoStack.push(captureState());
        while (!redoStack.empty()) redoStack.pop();

        Node* temp = head;
        int currentPos = 1;

        while (temp != nullptr && currentPos < position) {
            temp = temp->next;
            currentPos++;
        }

        if (temp == nullptr) {
            cout << "Position is out of range" << endl;
            return;
        }

        temp->data = newLine;
        cout << "Line edited at position " << position << endl;
    }

    // function to Undo the last operation
    void undo() {
        if (undoStack.empty()) {
            cout << "Nothing to undo" << endl;
            return;
        }

        redoStack.push(captureState());
        vector<string> prevState = undoStack.top();
        undoStack.pop();
        restoreState(prevState);
        cout << "Undo successful" << endl;
    }

    // function to Redo the last undone operation
    void redo() {
        if (redoStack.empty()) {
            cout << "Nothing to redo" << endl;
            return;
        }

        undoStack.push(captureState());
        vector<string> nextState = redoStack.top();
        redoStack.pop();
        restoreState(nextState);
        cout << "Redo successful" << endl;
    }

    // function to Display the content of the text editor
    void display() {
        Node* temp = head;
        int lineNumber = 1;
        while (temp != nullptr) {
            cout << lineNumber << ": " << temp->data << endl;
            temp = temp->next;
            lineNumber++;
        }
    }

    // function to Search for a keyword
    void search(const string& keyword) {
        Node* temp = head;
        int lineNumber = 1;
        bool found = false;

        while (temp != nullptr) {
            if (temp->data.find(keyword) != string::npos) {
                cout << "Keyword found at line " << lineNumber << ": " << temp->data << endl;
                found = true;
            }
            temp = temp->next;
            lineNumber++;
        }

        if (!found) {
            cout << "Keyword not found" << endl;
        }
    }
};

// main function to demonstrate the text editor functionality
int main() {
    TextEditor editor;
    int choice;
    string line, keyword;
    int position;
//menu for user interaction
    do {
        cout << "\nText Editor Menu:\n";
        cout << "1. Insert at beginning\n";
        cout << "2. Insert at End\n";
        cout << "3. Insert at Specific Position\n";
        cout << "4. Delete at Specific Position\n";
        cout << "5. Edit at Specific Position\n";
        cout << "6. Search for a keyword\n";
        cout << "7. Display the content\n";
        cout << "8. Undo\n";
        cout << "9. Redo\n";
        cout << "10. Exit\n";
        cout << "Enter your choice: ";

        //this handles inavlid input from the users 
        if (!(cin >> choice)) {
            cin.clear(); //clear error flag 
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            cout << "Invalid input. Please enter a number." << endl;
            continue; //continue to the next iteration
        }
       //insert at the beginning
        switch (choice) {
            case 1:
                cout << "Enter the line to insert at the beginning: ";
                cin.ignore();
                getline(cin, line);
                editor.insertAtBeginning(line);
                break;

       //insert at the end
            case 2:
                cout << "Enter the line to insert at the end: ";
                cin.ignore();
                getline(cin, line);
                editor.insertAtEnd(line);
                break;

        //for undo and redo functionality
            case 3:
                cout << "Enter the line to insert: ";
                cin.ignore();
                getline(cin, line);
                cout << "Enter the position to insert at: ";
                cin >> position;
                editor.insertAtPosition(line, position);
                break;
                
        //delete at specific position
            case 4:
                cout << "Enter the position to delete: ";
                cin >> position;
                editor.deleteAtPosition(position);
                break;

        //edit at specific position        

            case 5:
                cout << "Enter the position to edit: ";
                cin >> position;
                cout << "Enter the new line: ";
                cin.ignore();
                getline(cin, line);
                editor.editAtPosition(position, line);
                break;
        //search for a keyword
            case 6:
                cout << "Enter the keyword to search: ";
                cin.ignore();
                getline(cin, keyword);
                editor.search(keyword);
                break;
        //display the content
            case 7:
                cout << "Text Editor Content:\n";
                editor.display();
                break;

        //undo the last operation        

            case 8:
                editor.undo();
                break;

        //redo the last undone operation        

            case 9:
                editor.redo();
                break;
        //exit the program        

            case 10:
                cout << "Exiting the text editor. Goodbye!" << endl;
                break;

            default:
                cout << "Invalid choice. Please try again." << endl;
        }
    } while (choice != 10);

    return 0;
}