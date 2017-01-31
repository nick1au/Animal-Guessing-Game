/* Nicholas Lau, Win Aung
March 9, 2016 - LAST EDITED: March 13, 2016
Animal Guessing Game: This game uses linked lists and recursion to create a game where the program guesses the player's animal.
If correct, the program says yay, and if not, the program will ask the player for the animal and a question to identify it.
After, it will realign the questions with the two new techniques and then save it into a text file for future use.
*/

#include <iostream>
#include <fstream>
#include <cstdlib>

using namespace std;
namespace comp_assignment_3
{
struct TreeNode
{
    string text;
    TreeNode *yes_ans, *no_ans;
};
typedef TreeNode* TreeNodePtr;
}
using namespace comp_assignment_3;

TreeNodePtr create_default_tree();

void write_to_file(TreeNodePtr start,ofstream& out_file);
void read_from_file(TreeNodePtr& start,ifstream& in_file);
void choice(TreeNodePtr& start, TreeNodePtr& last_accessed);
bool ask_if_animal(TreeNodePtr start);
void add_new_question(TreeNodePtr& start);
void build_question(TreeNodePtr& start);
void ready();
void get_tree(ifstream&);
bool play_again();
int get_answer();

int main()
{
    TreeNodePtr head;
    TreeNodePtr last_accessed;

    ofstream out_file;
    ifstream in_file;

    int answer = 0;
    cout << "Welcome to Animal Guessing Game." << endl;
    bool loop_play;
    ready();
    do
    {
        cout << "Do you want to load the questions from a text file. If not, we'll create one for you." << endl;
        answer = get_answer();
        if(answer == 1)
        {
            get_tree(in_file);
            read_from_file(head, in_file);
            in_file.close();
            cin.ignore(1000, '\n');
        }
        else
        {
            if (answer == 0)
            {
                cout << "Creating a question tree for you..";
                head = create_default_tree();
            }

            else
                cout << "NOT A VALID ANSWER." << endl;
        }
    }while(answer == -1);

    while (loop_play == true)
    {
        choice(head, last_accessed);

        if (ask_if_animal(last_accessed) == true)
        {
            cout << "YAY! I GOT IT RIGHT!!!!" << endl;
        }
        else
        {
            cout << "OH NO! I GOT IT WRONG!!!" << endl;
            build_question(last_accessed);
        }

        string filename;
        cout << "Saving your questions. Input a filename." << endl;
        cin >> filename;

        out_file.open(filename);
        write_to_file(head,out_file);
        out_file.close();

        loop_play = play_again();
    }

    cout << "Closing. Goodbye!" << endl;


    return 0;
}
/*Asks if player wants to play again. If invalid response, recursion occurs.
INPUT: nothing.
OUTPUT: TRUE or FALSE, depending on user choice.
*/
bool play_again()
{
    string input;
    cout << "That was fun! Do you want to play again?" << endl;
    cin >> input;
    cin.ignore();
    if (input == "yes")
        return true;
    else
    {
        if (input == "no")
            return false;
        else
        {
            cout << "Invalid answer. " << endl;
            play_again();
        }
    }
}
/*Gets the filename with the tree.
INPUT: Ifstream
OUTPUT: The file in the ifstream.
*/
void get_tree(ifstream& in_file)
{
    string filename;

    cout << "Input file name.Follow by (.txt)."<<endl;
    cin >> filename;
    in_file.open(filename);

    if (in_file.fail())
    {
        cout << "Invalid. ";
        get_tree(in_file);
    }
    else
    {
        return;
    }
}

/*Asks if the user is ready.
INPUT: NOTHING
OUTPUT: Returns to program when user enters yes. Exit the program if no or any other input.
*/
void ready()
{
    cout << "Are you ready? Yes? No?" << endl;
    if (get_answer() == 1)
        cout <<"Enjoy the Game." << endl;
    else
    {
        cout << "You either entered 'No' or other invalid answers.Bye!See you Again.";
        exit(0);
    }

}

/* Builds the new question if the guessed animal wasn't right. It first asks for the animal and question, and then reorganizes tree.
Original animal is moved to the yes_ans treenode, the new animal to the no_ans and the new question to the head of the two branches.
INPUT: TreenodePtr at which it was last accessed
OUTPUT: Reassigned last accessed node with new link pointers.
*/
void build_question(TreeNodePtr& start)
{
    string new_a;
    string new_q;
    cout << "WHAT'S YOUR ANIMAL?" << endl;

    getline(cin, new_a);
    cin.clear();
    cout << "OK. Help my by adding a question! Type a question for which yes is a " << new_a << " and no for a " << start->text << "." << endl;
    getline(cin,new_q);
    cin.clear();
    //cout << new_q;

    TreeNodePtr yes_ans = new TreeNode;
    yes_ans->text = new_a;
    yes_ans->yes_ans=NULL;
    yes_ans->no_ans=NULL;

    TreeNodePtr no_ans = new TreeNode;
    no_ans->text = start->text;
    no_ans->yes_ans = NULL;
    no_ans -> no_ans = NULL;

    start->text = new_q;
    start->yes_ans = yes_ans;
    start->no_ans = no_ans;
    return;
}

/*Asks if animal is right.
INPUT: TreeNodePtr at where it was last accessed
OUTPUT: TRUE if guessed correctly and FALSE if not.
*/
bool ask_if_animal(TreeNodePtr start)
{
    int user_decision;
    cout << "Is your animal a(n) " << start->text << "?" << endl;
    user_decision = get_answer();
    if (user_decision == 0)
        return false;
    else
    {
        if (user_decision == 1)
            return true;
        else
        {
            cout << "Invalid answer.";
            ask_if_animal(start);
        }
    }
}
/*Allows user to navigate tree by saying yes or no to identify animal.
INPUT: TreeNodePtr start and last accessed
OUTPUT: Referenced TreeNodePtr's
*/
void choice(TreeNodePtr& start, TreeNodePtr& last_accessed)
{
    int user_choice;
    last_accessed = start;
    if (start->yes_ans == NULL && start->no_ans == NULL)
        return;
    else
    {
        cout << "Question " << start->text << endl;
        user_choice = get_answer();
        //cout << "ANSWER = " << user_choice << endl;
        if (user_choice == 0) // NO -> no_ans
        {
            choice(start->no_ans, last_accessed);
        }
        else
        {
            if (user_choice == 1) // YES -> yes_ans
            {
                choice(start->yes_ans, last_accessed);
            }
            else
            {
                cout << "Not a valid answer. ";
                choice(start, last_accessed);
            }
        }

    }
}

/* Writes the entire tree into a text file for future use
INPUT: TreeNodePtr at where it was lasted accessed and ofstream
OUTPUT: The text file.
*/
void write_to_file(TreeNodePtr start, ofstream& out_file)
{
    if (start == NULL)
        out_file << "*" << endl;
    else
    {

        out_file << start->text << endl;
        write_to_file(start->yes_ans, out_file);
        write_to_file(start->no_ans, out_file);
    }
    return;
}

/* Reads the file in the ifstream and builds the tree.
INPUT: TreeNodePtr start for the linked list tree and ifstream infile for the tree data
OUTPUT: The completed TreeNodePtr start tree
*/
void read_from_file(TreeNodePtr& start,ifstream& in_file)
{
    string input;

    getline(in_file, input);


    if (input != "*" )
    {
        start = new TreeNode;
        start->text = input;
        read_from_file(start->yes_ans, in_file);
        read_from_file(start->no_ans, in_file);
    }
    else
    {
        //cout << "NULL";
        start = NULL;
    }
}

/* Creates the default tree linked list
INPUT: NONE
OUTPUT: A usable tree TreeNodePtr defined as head in main.
*/
TreeNodePtr create_default_tree()
{
    TreeNodePtr yes = new TreeNode;
    TreeNodePtr no = new TreeNode;
    TreeNodePtr root = new TreeNode;

    no->text = "Bird";
    no->yes_ans = NULL;
    no->no_ans = NULL;

    yes->text = "Pikachu";
    yes->yes_ans = NULL;
    yes->no_ans = NULL;

    root -> text = "Does your animal have 4 legs?";
    root ->yes_ans = yes;
    root -> no_ans = no;

    return root;

}

/* Gets the answer to a question by getting the input if it is yes or no
INPUT: None
OUTPUT: An int that represents yes, no or an invalid answer.
*/
int get_answer()
{
    string answer;
    cin.clear();
    getline(cin,answer);
    //cout << "GET_ANS = " << answer;
    if(answer == "yes" || answer == "YES" || answer == "Yes")
    {
        return 1;
    }

    else
    {
        if(answer == "No" || answer == "NO" || answer == "no")
        {
            return 0;
        }
        else
        {
            return -1;
        }
    }
}
