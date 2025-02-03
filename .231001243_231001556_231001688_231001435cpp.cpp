    #include <iostream>
#include <string>
#include <vector>
#include <list>
#include <stack>
#include <algorithm>
using namespace std;

// Forward declaration for course_tree
class CourseBST;
extern CourseBST course_tree;

// Student class
class Student {
public:
    int id;
    string name;
    string email;
    string phone;
    string address;
    string password;
    Student* next;

    Student(int id, string name, string email, string phone, string address, string password)
        : id(id), name(name), email(email), phone(phone), address(address), password(password), next(nullptr) {}
};

// StudentLinkedList class
class StudentLinkedList {
private:
    Student* head;

public:
    StudentLinkedList() : head(nullptr) {}

    void add(int id, string name, string email, string phone, string address, string password) {
        Student* newStudent = new Student(id, name, email, phone, address, password);
        if (head == nullptr) {
            head = newStudent;
        } else {
            Student* temp = head;
            while (temp->next) {
                temp = temp->next;
            }
            temp->next = newStudent;
        }
    }

    void deleteStudent(int id) {
        if (head == nullptr) return;
        if (head->id == id) {
            Student* temp = head;
            head = head->next;
            delete temp;
            return;
        }
        Student* temp = head;
        while (temp->next && temp->next->id != id) {
            temp = temp->next;
        }
        if (temp->next) {
            Student* toDelete = temp->next;
            temp->next = temp->next->next;
            delete toDelete;
        }
    }

    void display() {
        Student* temp = head;
        while (temp) {
            cout << "ID: " << temp->id << ", Name: " << temp->name << ", Email: " << temp->email
                 << ", Phone: " << temp->phone << ", Address: " << temp->address << endl;
            temp = temp->next;
        }
    }

    void sortStudents() {
        if (head == nullptr || head->next == nullptr) {
            return; // No need to sort if the list is empty or has only one element
        }

        bool swapped;
        do {
            swapped = false;
            Student* current = head;
            Student* prev = nullptr;
            //traverse till the end of the list
            while (current->next != nullptr) {
                if (current->name > current->next->name) { // Compare student names
                    // Swap the students
                    Student* nextNode = current->next;
                    if (prev == nullptr) { // Swapping head node
                        head = nextNode;
                    } else {
                        prev->next = nextNode;
                    }
                    current->next = nextNode->next;
                    nextNode->next = current;

                    // Set swapped to true
                    swapped = true;
                    prev = nextNode;
                } else {// no swapping occured
                    prev = current;
                    current = current->next;
                }
            }
            // ensure that sorting continues until no swapping occur
        } while (swapped);
    }
    //youssef

    bool linearSearchStudent(int studentId) {
        Student* temp = head;
        while (temp) {
            if (temp->id == studentId) return true;
            temp = temp->next;
        }
        return false;
    }
};

// Course class
class Course {
public:
    int courseId;
    string courseName;
    int courseCredits;
    string courseInstructor;
    Course* left;
    Course* right;
    stack<int> prerequisites; // Stack of prerequisite course IDs

    Course(int id, string name, int credits, string instructor, stack<int> prereqs = {})
        : courseId(id), courseName(name), courseCredits(credits), courseInstructor(instructor), left(nullptr), right(nullptr), prerequisites(prereqs) {}
};

// CourseBST class
//omar
class CourseBST {
private:
    Course* root;

    Course* addCourse(Course* node, int id, string name, int credits, string instructor, stack<int> prereqs) {
        if (!node) return new Course(id, name, credits, instructor, prereqs);
        if (id < node->courseId)
            node->left = addCourse(node->left, id, name, credits, instructor, prereqs);
        else if (id > node->courseId)
            node->right = addCourse(node->right, id, name, credits, instructor, prereqs);
        return node;
    }

    Course* findMin(Course* node) {
        while (node->left) node = node->left;
        return node;
    }

    Course* dropCourse(Course* node, int id) {
        if (!node) return node;
        if (id < node->courseId) {
            node->left = dropCourse(node->left, id);
        } else if (id > node->courseId) {
            node->right = dropCourse(node->right, id);
        } else {
            if (!node->left) {
                Course* temp = node->right;
                delete node;
                return temp;
            } else if (!node->right) {
                Course* temp = node->left;
                delete node;
                return temp;
            }
            //if node has both left and right children:
            Course* temp = findMin(node->right);
            node->courseId = temp->courseId;
            node->courseName = temp->courseName;
            node->courseCredits = temp->courseCredits;
            node->courseInstructor = temp->courseInstructor;
            node->prerequisites = temp->prerequisites;
            node->right = dropCourse(node->right, temp->courseId);
        }
        return node;
    }

    void inorderPrint(Course* node) {
        if (node) {
            inorderPrint(node->left);
            cout << "Course ID: " << node->courseId << ", Course Name: " << node->courseName
                 << ", Course Credits: " << node->courseCredits << ", Course Instructor: " << node->courseInstructor << endl;
            inorderPrint(node->right);
        }
    }

public:
    CourseBST() : root(nullptr) {}

    void addCourse(int id, string name, int credits, string instructor, stack<int> prereqs = {}) {
        root = addCourse(root, id, name, credits, instructor, prereqs);
    }

    void dropCourse(int id) {
        root = dropCourse(root, id);
    }

    void displayCourses() {
        inorderPrint(root);
    }

    Course* searchCourse(int id) {
        Course* temp = root;
        while (temp) {
            if (temp->courseId == id) {
                return temp;
            }
            if (id < temp->courseId) {
                temp = temp->left;
            } else {
                temp = temp->right;
            }
        }
        return nullptr;
    }

    void inorderVector(Course* node, vector<Course*>& courses) {
        if (node) {
            inorderVector(node->left, courses);
            courses.push_back(node);
            inorderVector(node->right, courses);
        }
    }

    void sortCourses() {
        vector<Course*> courses;
        inorderVector(root, courses);
        sort(courses.begin(), courses.end(), [](Course* a, Course* b) {
            return a->courseName < b->courseName; // Sort by course name
        });

        // Clear the tree and reinsert sorted courses
        root = nullptr;
        for (Course* course : courses) {
            addCourse(course->courseId, course->courseName, course->courseCredits, course->courseInstructor, course->prerequisites);
        }
    }//end of omar

//ibarahim 
    bool binarySearchCourse(int courseId) {
        vector<Course*> courses;
        inorderVector(root, courses);
        int left = 0;
        int right = courses.size() - 1;
        while (left <= right) {
            //to get the mid 
            int mid = left + (right - left) / 2;
            if (courses[mid]->courseId == courseId) {
                return true;
            } else if (courses[mid]->courseId < courseId) {
                left = mid + 1;
            } else {
                right = mid - 1;
            }
        }
        return false;
    }
};

// Global variable for CourseBST instance
CourseBST course_tree;

// Enrollment class
class Enrollment {
public:
    int studentId;
    int courseId;
    Enrollment* next;
    Enrollment* prev;

    Enrollment(int stid, int coid) : studentId(stid), courseId(coid), prev(nullptr), next(nullptr) {}
};

// EnrollmentDLL class
class EnrollmentDLL {
private:
    Enrollment* head;

public: 
    EnrollmentDLL() : head(nullptr) {}

    void add(int studentId, int courseId) {
        if (!hasCompletedPrerequisites(studentId, courseId)) {
            cout << "Student has not completed all prerequisites for course ID: " << courseId << endl;
            return;
        }

        Enrollment* newEnrollment = new Enrollment(studentId, courseId);
        if (!head) {
            head = newEnrollment;
        } else {    
            Enrollment* temp = head;
            while (temp->next) {
                temp = temp->next;
            }
            temp->next = newEnrollment;
            newEnrollment->prev = temp;
        }
    }

    void view(int studentId) {
        Enrollment* temp = head;
        while (temp) {
            if (temp->studentId == studentId) {
                cout << "Student ID: " << temp->studentId << ", Course ID: " << temp->courseId << endl;
            }
            temp = temp->next;
        }
    }
//youssef
    bool hasCompletedPrerequisites(int studentId, int courseId) {
    Course* course = course_tree.searchCourse(courseId);
    if (!course) {
        cout << "Course not found!" << endl;
        return false;
    }

    stack<int> prereqs = course->prerequisites;



    //a copy
    stack<int> tempStack = prereqs;
    while (!tempStack.empty()) {
        cout << tempStack.top() << " ";
        tempStack.pop();
    }
    cout << endl;

    
    while (!prereqs.empty()) {
        int prereq = prereqs.top();   
        prereqs.pop();                

        cout << "Checking prerequisite: " << prereq << endl;

        if (!isEnrolled(studentId, prereq)) {
            cout << "Student " << studentId << " has NOT completed prerequisite: " << prereq << endl;
            return false;
        }
    }

    cout << "Student " << studentId << " has completed all prerequisites for course " << courseId << endl;
    return true;
}

//ibrahim
    bool isEnrolled(int studentId, int courseId) {
        Enrollment* temp = head;
        while (temp) {
            if (temp->studentId == studentId && temp->courseId == courseId) {
                return true;
            }
            temp = temp->next;
        }
        return false;
    }
};4

// StackNode class
class StackNode {
public:
    int data;
    StackNode* next;

    StackNode(int d) : data(d), next(nullptr) {}
};

// Stack class
class Stack {
private:
    StackNode* top;

public:
//top = -1
    Stack() : top(nullptr) {}

    void push(int value) {
        //adding a new element in the top of the stack
        StackNode* newNode = new StackNode(value);
        newNode->next = top;
       
        top = newNode;
    }

    int pop() {
        if (isEmpty()) {
            cout << "Stack is empty." << endl;
            return -1;
        }
        StackNode* temp = top;
        top = top->next;
        int poppedValue = temp->data;
        delete temp;
        return poppedValue;
    }

    bool isEmpty() {
        return top == nullptr;
    }

    int peek() {
        if (isEmpty()) {
            cout << "Stack is empty." << endl;
            return -1;
        }
        return top->data;
    }
};

// QueueNode class
class QueueNode {
public:
    int data;
    QueueNode* next;

    QueueNode(int d) : data(d), next(nullptr) {}
};

// Queue class
class Queue {
private:
    QueueNode* front;
    QueueNode* rear;

public:
    Queue() : front(nullptr), rear(nullptr) {}

    void enqueue(int value) {
        QueueNode* newNode = new QueueNode(value);
        if (rear == nullptr) {
            front = rear = newNode;
            return;
        }
        rear->next = newNode;
        rear = newNode;
    }

    int dequeue() {
        if (isEmpty()) {
            cout << "Queue is empty." << endl;
            return -1;
        }
        QueueNode* temp = front;
        front = front->next;
        int dequeuedValue = temp->data;
        delete temp;
        if (front == nullptr) {
            rear = nullptr;
        }
        return dequeuedValue;
    }

    bool isEmpty() {
        return front == nullptr;
    }

    int peek() {
        if (isEmpty()) {
            cout << "Queue is empty." << endl;
            return -1;
        }
        return front->data;
    }
};

// HashTable class  
class HashTable {
private:
    vector<list<pair<int, Course*>>> table;
    int size;

    int hashFunction(int key) {
        return key % size;
    }

public:
    HashTable(int size) : size(size) {
        table.resize(size);
    }

    void insert(int key, Course* value) {
        int index = hashFunction(key);
        table[index].push_back(make_pair(key, value));
    }

    Course* search(int key) {
        int index = hashFunction(key);
        for (auto& [k, v] : table[index]) {
            if (k == key) {
                return v;
            }
        }
        return nullptr;
    }
};

// Main function
int main() {
    StudentLinkedList student_list;
    EnrollmentDLL enrollment_history;
    Queue waiting_list;
    HashTable course_hash_table(15);

    int choice;
    while (true) {
        cout << "\nUniversity Management System\n";
        cout << "1. Add Student\n";
        cout << "2. Delete Student\n";
        cout << "3. Display Students\n";
        cout << "4. Add Course\n";
        cout << "5. Drop Course\n";
        cout << "6. Display Courses\n";
        cout << "7. Add Enrollment\n";
        cout << "8. View Enrollment\n";
        cout << "9. Sort Students\n";
        cout << "10. Sort Courses\n";
        cout << "11. Add to Waitlist\n";
        cout << "12. Remove from Waitlist\n";
        cout << "13. Search Course by ID\n";
        cout << "14. Search Student by ID\n";
        cout << "15. Exit\n";
        cout << "Enter your choice: ";
        cin >> choice;
        if (choice == 15) break;

        switch (choice) {
            case 1: {
                int id;
                string name, email, phone, address, password;
                cout << "Enter student ID: ";
                cin >> id;
                cout << "Enter name: ";
                cin.ignore();
                getline(cin, name);
                cout << "Enter email: ";
                getline(cin, email);
                cout << "Enter phone: ";
                getline(cin, phone);
                cout << "Enter address: ";
                getline(cin, address);
                cout << "Enter password: ";
                getline(cin, password);
                student_list.add(id, name, email, phone, address, password);
                
                break;
            }
            case 2: {
                int id;
                cout << "Enter student ID to delete: ";
                cin >> id;
                student_list.deleteStudent(id);
                break;
            }
            case 3:
                student_list.display();
                break;
            case 4: {
                int id, credits, numPrereqs;
                string name, instructor;
                stack<int> prereqs;
                cout << "Enter course ID: ";
                cin >> id;
                cout << "Enter course name: ";
                cin.ignore();
                getline(cin, name);
                cout << "Enter course credits: ";
                cin >> credits;
                cout << "Enter course instructor: ";
                cin.ignore();
                getline(cin, instructor);
                cout << "Enter number of prerequisites: ";
                cin >> numPrereqs;
                for (int i = 0; i < numPrereqs; ++i) {
                    int prereqId;
                    cout << "Enter prerequisite course ID " << (i + 1) << ": ";
                    cin >> prereqId;
                    prereqs.push(prereqId);
                }
                course_tree.addCourse(id, name, credits, instructor, prereqs);
                break;
            }
            case 5: {
                int id;
                cout << "Enter course ID to drop: ";
                cin >> id;
                course_tree.dropCourse(id);
                break;
            }
            case 6:
                course_tree.displayCourses();
                break;
            case 7: {
                int student_id, course_id;
                cout << "Enter student ID: ";
                cin >> student_id;
                cout << "Enter course ID: ";
                cin >> course_id;
                enrollment_history.add(student_id, course_id);
                break;
            }
            case 8: {
                int student_id;
                cout << "Enter student ID to view enrollment: ";
                cin >> student_id;
                enrollment_history.view(student_id);
                break;
            }
            case 9:
                student_list.sortStudents();
                cout << "Students sorted by name." << endl;
                break;
            case 10:
                course_tree.sortCourses();
                cout << "Courses sorted by name." << endl;
                break;
            case 11: {
                int student_id;
                cout << "Enter student ID to add to waitlist: ";
                cin >> student_id;
                waiting_list.enqueue(student_id);
                break;
            }
            case 12:
                waiting_list.dequeue();
                break;
            case 13: {
                int course_id;
                cout << "Enter course ID to search: ";
                cin >> course_id;
                if (course_tree.binarySearchCourse(course_id)) {
                    cout << "Course found with ID: " << course_id << endl;
                } else {
                    cout << "Course not found with ID: " << course_id << endl;
                }
                break;
            }
            case 14: {
                int student_id;
                cout << "Enter student ID to search: ";
                cin >> student_id;
                bool found = student_list.linearSearchStudent(student_id);
                if (found) {
                    cout << "Student found with ID: " << student_id << endl;
                } else {
                    cout << "Student not found with ID: " << student_id << endl;
                }
                break;
            }
            default:
                cout << "Invalid choice. Please try again." << endl;
        }
    }

    return 0;
}