/**********************************************************************************
 *File: 	Assignment5.hpp
 *Author: 	Easa El Sirgany (easaemad14@gmail.com)
 *
 *Description:  C++ header file containing required macros, classes, functions,
 *		etc. utilized by the corresponding .cpp file. 
 *********************************************************************************/
#include <string>
#include <iostream> //For cin and cout
#include <cctype> //For character checking
#include <cstdlib> //For input manipulation
#include <cstring> //For memset and strncpy


//Room for 127 chars and a terminator
#define MAX_LINE_LENGTH 127


//Macros (yeah, I'm fancy!)
//flush_buffer may be overkill since only one line, but it looks better
#define flush_buffer(B, S) memset(B, '\0', S)
//Get clean line (flush and then getline) showing off CPP knowledge
#define get_next_line(B, S) \
	flush_buffer(B, S); std::cin.getline(B, S)
//Check debug macro
/* In school, they drill these somewhat silly rules into your head, like gotos
 * should never be used and that every statement should be on a separate line
 * for readability. There is a good reason to have some of these rules, and in
 * general they serve a purpose. Unfortunately, they do not teach people to think,
 * but rather to memorize shit and some of these people see my code and regurgitate
 * all the stupid rules that they memorized from their programming class! 
 *
 * So yeah, I am gonna use a goto AND I'm gonna put it on the same line as my if
 * statement. If this bothers you, please see the .jpg file associated with this
 * project. That should make you feel better! */
#define check(A) if(!A) goto error


//Variables
struct course{
	std::string number;
	std::string name;
	struct course *next;
};


//Classes
class LinkedList
{
private:
	int index, length;
	struct course *head;

public:
	LinkedList(){
		length = 0;
		head = NULL;
	}

	~LinkedList(){
		struct course *tmp;
		if(head){
			if(head->next){
				for(tmp=head->next; head->next; tmp=head->next){
					head->next = tmp->next;
					delete tmp;
				}
			}
			delete head;
		}
	}

	void insertElement(const char to_add[]){
		/* See HashTable::insertElement for complete details on the format
		 * of the user input (I built this from the top down, so started at
		 * main() and this is the end of the logic train, choo choo!). */
		struct course *newCourse = new course;

		newCourse->number.assign(to_add, 6);
		newCourse->name.assign(to_add, 7, MAX_LINE_LENGTH); //Don't worry!
		if(head) newCourse->next = head;
		else newCourse->next = NULL;
		head = newCourse;
		this->length++;
	}

	void searchElement(const char to_find[]){
		/* The requirements of this function were not fully elaborated and
		 * I do not have enough information from the test cases to deduce
		 * the answer to this question, but when we are deleting elems, it
		 * is noted in the .txt file that we shall delete all instances (or
		 * duplicates, if you will), so my implementation will only search
		 * until either the class is found or we come to end of list. */
		bool found = false;
		std::string number, name;

		struct course *tmp; //We're not allocating, just looking at data
		number.assign(to_find, 6);

		if(this->head){
			for(tmp=head; tmp->next && !found; tmp=tmp->next){
				if(tmp->number.compare(number)==0){
					found=true;
					name = tmp->name;
				}
			}
			//The loop breaks at last elem (need to check that too)
			if(tmp->number.compare(number)==0){
				found=true;
				name = tmp->name;
			}
		}

		//Now, if found==true, print we did find, else print not found
		if(found){
			std::cout << "The course " << number << " has ";
			std::cout << "the title: " << name << std::endl;
		}
		else{
			std::cout << "The course " << number;
			std::cout << " not found" << std::endl;
		}
	}
					
	void deleteElement(const char to_delete[]){
		bool exists = false;
		struct course *tmp, *del; //We'll need two to keep track of pos
		std::string number, name;

		number.assign(to_delete, 6);
		name.assign(to_delete, 7, MAX_LINE_LENGTH);

		if(this->head){
			/* This bit is actually a little tricky. Let's say that you
			 * had your head pointing to an element that you wanted to
			 * delete. Okay, no problem, let's delete the head and set
			 * head to whatever head->next points to! Obviously we will
			 * check that head->next actually points to something, but
			 * what if it points to another instance of the same class?
			 * 
			 * So we want to be able to recursively delete the class,
			 * and I am going to put this in bold font because it needs
			 * that much attention, WHICH REQUIRES CHECKING THE NAME
			 * AND NUMBER ELEMENTS OF A POSSIBLY NULL POINTER! Yeah,
			 * I think the logic on this is ugly too, but that's how it
			 * came up in my head. And unfortunately for me, I don't
			 * have the luxury of blaming my subconsious like the
			 * fathers of melodic death metal In Flames
			 * (http://www.azlyrics.com/lyrics/inflames/alias.html).
			 * 
			 * So here's my trick: we will traverse the list until we
			 * find a class that doesn't match what we want to delete
			 * and set that as the head, or until we can't traverse
			 * anymore (NULL), at which point, we just delete the whole
			 * list (KILL IT WITH FIRE). 
			 *
			 * Now you might be thinking, "Don't you think you are
			 * overdoing it Easa? With the thinking and the 30 lines of
			 * comments!" And you would have a point, but remember our
			 * hash function uses the mod of the course number, so any
			 * course with the same number, regardless of the field of
			 * study, will have the same index! Therefore, any
			 * duplicates are guaranteed to have the same index in our
			 * HashTable! But I will agree, 30 lines is ridiculous! */
			if(this->head->name.compare(name)==0 &&
			this->head->number.compare(number)==0){
				for(tmp=this->head; tmp->next &&
				(tmp->name.compare(name)==0 &&
				tmp->number.compare(number)==0);
				tmp=tmp->next){} //Just traverse

				if(!tmp->next){ //KILL IT WITH FIRE!
					while(this->head->next){
						exists=true;
						del = this->head->next;
						if(del->next)
							head->next = del->next;
						else head->next = NULL;
						delete del;
						this->length--;
					}
					exists=true;
					delete this->head;
					this->head = NULL;
				}
				else{ //Delete until you get to tmp
					while(head!=tmp){
						exists=true;
						del=this->head;
						this->head=this->head->next;
						delete del;
						this->length--;
					}
				}
			}
		}
		/* Round 2! Honestly, this one isn't nearly as bad, but what I
		 * learned from this one is that the flow of a for loop in C and
		 * C++ will not check the condition after executing the statement
		 * and before the increment. This means, if you delete the last
		 * element of your list, your will run into a segmentation fault
		 * trying to move your pointer to the next element your just de-
		 * allocated. So don't do that ;) */
		if(this->head){ //Can become NULL after while loop
			for(tmp=this->head; tmp->next;){
				if(tmp->next->name.compare(name)==0 &&
				tmp->next->number.compare(number)==0){
					exists = true;
					del = tmp->next;
					if(del->next) tmp->next=del->next;
					else tmp->next = NULL;
					delete del;
					this->length--;
					if(tmp->next) tmp=tmp->next;
				}
				else tmp=tmp->next;
			}
		}

		if(exists){
			std::cout << "Course Number " << number << " with ";
			std::cout << "Title " << name << " is removed\n";
		}
		else{
			std::cout << "Course Number " << number << " with ";
			std::cout << "Title " << name << " not found\n";
		}
	}

	void displayList(){
		struct course *tmp;

		std::cout << "index: " << this->index << ", ";
		std::cout << "linked list size: " << this->length << std::endl;
		if(!this->head)
			std::cout << "The list is empty\n\n";
		else{
			for(tmp=this->head; tmp->next; tmp=tmp->next){
				std::cout << "Course Number: " << tmp->number;
				std::cout << ", Course Title: " << tmp->name;
				std::cout << std::endl;
			}
			//Don't forget to print the last element in the list!
			std::cout << "Course Number: " << tmp->number;
			std::cout << ", Course Title: " << tmp->name;
			std::cout << std::endl << std::endl;
		}
	}

	void setIndex(const unsigned int num){
		this->index = num;
	}
};


//HashTable with chaining (array of LinkedLists; see the constructor)
class HashTable
{
private:
	LinkedList *head;
	unsigned int size;

public:
	HashTable(); //Default constructor shouldn't be utilized!

	HashTable(const unsigned int length){
		//Since we are using an array of LinkedLists, allocate length lists
		this->head = new LinkedList[length];
		size = length;

		//LinkedList constructor sets the default length and head ptr,
		//but not index
		for(unsigned int i=0; i<length; i++)
			head[i].setIndex(i);
	}

	~HashTable(){
		delete[] head; //LinkedList destructor handles this for us :)
	}

	void insertElement(const char course[MAX_LINE_LENGTH]){
		/* The prescribed input, per the txt document attached, is in the
		 * format of "XXXYYY/Class name", where X represents the three
		 * letters delegating the field of study, Y representing the three
		 * digit course number, and the class name following the '/'.
		 *
		 * For my hash function, I will use the three digits of the class
		 * number to preserve an efficient table. */

		/* This next line is a line that you should see in all of my
		 * functions to begin error checking. What this is doing is
		 * making sure that the last element in my array is the null
		 * terminator. Let's say I allowed the user to input some data,
		 * and I incorrectly (one of the many things that I have learned
		 * about myself is that I love writing code with bugs in it. I know
		 * this makes no sense, because I am just going to take them out at
		 * the end, but I just can't help myself sometimes I guess!) read
		 * the user input into the buffer (not implementing a limit on the
		 * amount of data that I stored). This is actually a common
		 * security flaw known as buffer overflow! */
		check(course[MAX_LINE_LENGTH] == '\0'); //See my fancy macro
		check(isalpha(course[0])); //See the format comment above!
		check(strlen(course) > 7); //Don't want out of bounds throws!

		unsigned int pos;
		char to_cast[4]; //Don't forget NULL Terminator!
		for(int i=3; i<6; i++){ //get the three numbers
			to_cast[i-3] = course[i];
		}

		pos = hash(to_cast);
		//A little more error checking never hurt anyone
		if(pos >= this->size) goto hash_error;
		head[pos].insertElement(course);

		return; //Don't want to fallthrough to errors!

	error:
		//nothing was allocated
		std::cout << "\n==============================================\n";
		std::cout << "Your input buffer was incorrect when passed \n";
		std::cout << "to HashTable::insertElement! Please try again.\n";
		std::cout << "==============================================\n";
		return; //Same
	hash_error:
		std::cout << "\n===========================================\n";
		std::cout << "HashTable::hash() returned an index that is\n";
		std::cout << "out of bounds! This needs to be addressed!\n";
		std::cout << "===========================================\n";
	}

	void searchElement(const char course[]){
		/* This uses the same prescrived input as the last function
		 * (HashTable::insertElement), so we will use the same error
		 * checking method. We will also have to cast the course (like we
		 * did in the last function) to find the hash to search. Then we
		 * just pass the buffer to LinkedList::insertElement to do all of
		 * the work! */
		check(course[MAX_LINE_LENGTH] == '\0');
		check(isalpha(course[0]));
		check(strlen(course) > 5);

		unsigned int pos;
		char to_cast[4];

		for(int i=3; i<6; i++){
			to_cast[i-3] = course[i];
		}
		pos = hash(to_cast);
		head[pos].searchElement(course);

		return;

	error:
		std::cout << "\n============================================\n";
		std::cout << "Your input buffer was corrupt when passed to\n";
		std::cout << "HashTable::searchElement! Please try again.\n";
		std::cout << "============================================\n";
	}

	void deleteElement(const char course[]){
		/* More fun stuff! Let's start with some good old fashioned error
		 * checking, and then let's remove some courses from the hash
		 * table.
		 *
		 * Based off of the requirements in the text file, it states that
		 * we are to remove all instances of a class, but again it doesn't
		 * seem to explain in great detail. Therefore, I am making the
		 * executive decision to only print if the course is removed or not
		 * if there are one or more courses with the same number and name
		 * (I am also deciding that a course number and name both have to
		 * match with what was passed, otherwise it is probably user input
		 * error. In a real life situation, I would make this more robust
		 * since it should be assumed that errors of input are common). */
		check(course[MAX_LINE_LENGTH] == '\0');
		check(isalpha(course[0]));
		check(strlen(course) > 7);

		unsigned int pos;
		char to_cast[4];

		for(int i=3; i<6; i++){
			to_cast[i-3]=course[i];
		}
		pos = hash(to_cast);
		head[pos].deleteElement(course);

		return;

	error:
		std::cout << "\n============================================\n";
		std::cout << "Your input buffer was corrupt when passed to\n";
		std::cout << "HashTable::deleteElement! Please try again!\n";
		std::cout << "============================================\n";
	}

	void displayTable(){
		if(!this->head) return; //Empty tree

		for(unsigned int i=0; i<this->size; i++){
			head[i].displayList();
		}
	}

	unsigned int hash(char cast[]){
		return (atoi(cast) % this->size); //Yeah, that simple!
	}
};
