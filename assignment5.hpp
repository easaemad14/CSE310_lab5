/********************************************************************************************
 *File: 	Assignment5.hpp
 *Author: 	Easa El Sirgany (easaemad14@gmail.com)
 *
 *Description:  C++ header file containing required macros, classes, functions, etc. utilized
 *		by the corresponding .cpp file. 
 *******************************************************************************************/
#include <string>
#include <iostream> //For cin and cout
#include <cctype> //For character checking
#include <cstdlib> //For input manipulation
#include <cstring> //For memset


#define MAX_LINE_LENGTH 128

//Macros (yeah, I'm fancy!)
#define hash(elem) \
	int hash(char *elem){ \
	}

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

	void insertElement();
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

		//LinkedList constructor sets the default length and head ptr, but not index
		for(unsigned int i=0; i<length; i++)
			head[i].setIndex(i);
	}

	~HashTable(){
		delete[] head; //LinkedList destructor handles this for us :)
	}

	void insertElement(); //Add the hashed element at the beginning of the list
	void searchElement();
	void deleteElement();
};
