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
#define flush_buffer(B, S) memset(B, '\n', S)

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
		memset(course_number, '\n', 7);
		char course_name[MAX_LINE_LENGTH-6]; //-7+1 for '\n'
		memset(course_name, '\n', MAX_LINE_LENGTH-6);
		struct course *newCourse = new course;

		strncpy(course_number, to_add, 6);
		//I am not sure if there is an easier way to do this bit:
		for(int i=7; to_add[i]!='\n'; i++){
			course_name[i] = to_add[i];
		}

		newCourse->number = course_number;
		newCourse->name = course_name;
		if(head) newCourse->next = head->next;
		else newCourse->next = NULL;
		head->next = newCourse;
	}

	void searchElement();
	void deleteElement();

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
		//Come back to check EOF!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
		if(course[MAX_LINE_LENGTH-1] != '\n') goto error;

		unsigned int pos;
		char to_cast[7]; //Don't forget the NULL bit!

		memset(to_cast, '\n', 7);
		strncpy(to_cast, course, 6);
		pos = hash((unsigned long)to_cast);
		head[pos].insertElement(course);

	error:
		//nothing was allocated (not that it matters with c++)
		std::cout << "\n==========================================";
		std::cout << "you input buffer was corrupt when passed to\n";
		std::cout << "hashtable::insertelement! please try again.\n";
		std::cout << "===========================================\n";
	}

	void searchElement();
	void deleteElement();

	unsigned int hash(const unsigned long casted){
		return (unsigned int)(casted % this->size); //Yeah, that simple!
	}
};
