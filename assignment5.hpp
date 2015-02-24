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
#define MAX_LINE_LENGTH 128

//Macros (yeah, I'm fancy!)
//flush_buffer may be overkill since only one line, but good habit
#define flush_buffer(B, S) memset(B, '\0', S)

#define get_next_line(B, S) flush_buffer(B, S), std::cin.getline(B, S)


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

		char course_number[7]; //Don't forget NULL terminator!
		char course_name[MAX_LINE_LENGTH-6]; //-7+1 for '\0'
		struct course *newCourse = new course;

		strncpy(course_number, to_add, 6);
		//I am not sure if there is an easier way to do this bit:
		for(int i=7; to_add[i]!='\0'; i++){
			course_name[i-7] = to_add[i];
		}

		newCourse->number = course_number;
		newCourse->name = course_name;
		if(head) newCourse->next = head->next;
		else newCourse->next = NULL;
		head->next = newCourse;
	}

	void searchElement(const char to_find[]){
		/* The requirements of this function were not fully elaborated and
		 * I do not have enough information from the test cases to deduce
		 * the answer to this question, but when we are deleting elems, it
		 * is noted in the .txt file that we shall delete all instances (or
		 * duplicates, if you will), so my implementation will only search
		 * until either the class is found or we come to end of list. */
		bool found = false;
		char course_number[7];
		char course_name[MAX_LINE_LENGTH-6];
		struct course *tmp; //We're not allocating, just looking at data

		strncpy(course_number, to_find, 6);
		for(int i=7; to_find[i]!='\0'; i++){
			course_name[i-7] = to_find[i];
		}

		if(this->head){
			for(tmp=head->next; tmp->next && !found; tmp=tmp->next){
				if(tmp->number.compare(course_number)==0 &&
				tmp->name.compare(course_name)==0)
					found=true;
			}
		}

		//Now, if found==true, print we did find, else print not found
		if(found){
			std::cout << "The course " << course_number << " has ";
			std::cout << " the title: " << course_name << std::endl;
		}
		else{
			std::cout << "The course " << course_number;
			std::cout << " not found" << std::endl;
		}
	}
					
	void deleteElement(const char to_delete[]){
		bool exists = false;
		char course_number[7];
		char course_name[MAX_LINE_LENGTH-6];
		struct course *tmp, *del; //We'll need two to keep track of pos

		strncpy(course_number, to_delete, 6);
		for(int i=7; to_delete[i]!='\0'; i++){
			course_name[i-7] = to_delete[i];
		}

		if(this->head){
			for(tmp=head; tmp->next; tmp=tmp->next){
				if(tmp->next->name.compare(course_name)==0 &&
				tmp->next->number.compare(course_number)==0){
					exists = true;
					del = tmp->next;
					if(del->next) tmp->next = del->next;
					else tmp->next = NULL;
					delete del;
				}
			}
		}

		if(exists){
			std::cout << "Course Number " << course_number << " with ";
			std::cout << "Title " << course_name << " is removed\n";
		}
		else{
			std::cout << "Course Number " << course_number << " with ";
			std::cout << "Title " << course_name << " not found\n";
		}
	}

	void displayList(){
		struct course *tmp;

		std::cout << "index: " << this->index << ", ";
		std::cout << "linked list size: " << this->length << std::endl;
		if(!this->head)
			std::cout << "The list is empty\n\n";
		else{
			for(tmp=this->head->next; tmp->next; tmp=tmp->next){
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
		 * For my hash function, I will use the first six characters (the
		 * three letters and three numbers) casted to an integer in hopes
		 * to preserve an efficient table. */

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
		if(course[MAX_LINE_LENGTH-1] != '\0') goto error;
		if(!isalpha(course[0])) goto error; //Everything begins alpha!

		unsigned int pos;
		char to_cast[7]; //Don't forget the NULL bit!

		strncpy(to_cast, course, 6);
		pos = hash((unsigned long)to_cast);
		head[pos].insertElement(course);

	error:
		//nothing was allocated
		std::cout << "\n============================================";
		std::cout << "Your input buffer was corrupt when passed to\n";
		std::cout << "HashTable::insertElement! Please try again.\n";
		std::cout << "============================================\n";
	}

	void searchElement(const char course[MAX_LINE_LENGTH]){
		/* This uses the same prescrived input as the last function
		 * (HashTable::insertElement), so we will use the same error
		 * checking method. We will also have to cast the course (like we
		 * did in the last function) to find the hash to search. Then we
		 * just pass the buffer to LinkedList::insertElement to do all of
		 * the work! */
		if(course[MAX_LINE_LENGTH-1] != '\0') goto error;
		if(!isalpha(course[0])) goto error;

		unsigned int pos;
		char to_cast[7];

		strncpy(to_cast, course, 6);
		pos = hash((unsigned long)to_cast);
		head[pos].searchElement(course);

	error:
		std::cout << "\n============================================";
		std::cout << "Your input buffer was corrupt when passed to\n";
		std::cout << "HashTable::searchElement! Please try again.\n";
		std::cout << "============================================\n";
	}

	void deleteElement(const char course[MAX_LINE_LENGTH]){
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
		if(course[MAX_LINE_LENGTH-1] != '\0') goto error;
		if(!isalpha(course[0])) goto error;

		unsigned int pos;
		char to_cast[7];

		strncpy(to_cast, course, 6);
		pos = hash((unsigned long)to_cast);
		head[pos].deleteElement(course);

	error:
		std::cout << "\n============================================";
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

	unsigned int hash(const unsigned long casted){
		return (unsigned int)(casted % this->size); //Yeah, that simple!
	}
};
