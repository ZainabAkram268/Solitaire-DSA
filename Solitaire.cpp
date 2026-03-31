#include <iostream>
#include <iomanip> //For setw(n)
#include <string>
#include <windows.h>
#include <cstdlib> // For rand() and srand() to generate random numbers used for shuffling
#include <ctime>   // For getting time
#include <thread> // For std::this_thread
#include <chrono> // For std::chrono::seconds for stopping the system, used after error messages
using namespace std;
template<class type> class mystack;
template<class type>
class DoubleList
{
	friend class mystack<type>;
	friend class game;
private:
	class node;
	node* head;
	node* tail;
	class ListIterator;
public:
	DoubleList()
	{
		head = new node(card(-1, "dummy"), 0, 0);
		tail = new node(card(-1, "dummy"), head, 0);
		head->next = tail;
	}
	int getSize()const
	{
		if (isEmpty())
			return 0;
		else
		{
			int i = 1;
			for (Iterator ite = begin(); ite.current->next->next != nullptr; ++ite, ++i);
			return i;
		}
	}
	bool isEmpty()const
	{
		return (head->next == tail);

	}
	typedef ListIterator Iterator;
	node* sublistStart(int s)
	{
		Iterator ite2 = begin();
		for (int i = 0; ite2 != end() && i != s; ite2++, i++);   // to reach that point
		return ite2.getCurrent();
	}
	Iterator begin() const
	{
		return head->next;
	}
	Iterator end() const
	{
		return tail->prev;
	}
	void InsertAtEnd(type value)
	{
		Iterator ite = end();
		node* tmp = ite.current;
		node* newNode = new node(value, tmp, tmp->next);
		tmp->next->prev = newNode;
		tmp->next = newNode;
	}
	void InsertNodeAtEnd(node* ptr)
	{
		if (ptr)
		{
			Iterator ite = end();
			node* tmp = ite.current;
			ptr->next = tmp->next;
			ptr->prev = tmp;
			tmp->next->prev = ptr;
			tmp->next = ptr;
		}
	}
	~DoubleList()
	{
		node* current = head->next;
		while (current != tail) {
			node* nextNode = current->next; // Store next node
			delete current; // Delete current node
			current = nextNode; // Move to the next node
		}

		// Delete the dummy head and tail nodes
		delete head;
		delete tail;
	}
};
															//Iterator class
template<class type> 
class DoubleList<type>::ListIterator
{
private:
	node* current;
public:
	friend class DoubleList;
	ListIterator(node* n = 0)
	{
		current = n;
	}
	node* getCurrent()
	{
		return current;
	}
	ListIterator & operator++ ()
	{
		if (current)
			current = current->next;
		return *this;
	}
	ListIterator operator++ (int)
	{
		ListIterator old = *this;
		++(*this);
		return old;
	}
	ListIterator & operator-- ()
	{
		if (current)
			current = current->prev;
		return *this;
	}
	ListIterator operator--(int)
	{
		ListIterator old = *this;
		--(*this);
		return old;
	}
	type& operator* ()
	{
		return current->data;
	}
	bool operator== (const ListIterator & RHS) const
	{
		return (current == RHS.current);
	}
	bool operator != (const ListIterator & RHS) const
	{
		return (current != RHS.current);
	}

};
										//Node class
template<class type>
class DoubleList<type>::node
{
public:
	node(){};
	node(type val, node* pptr = 0, node* nptr = 0)
	{
		data = val;
		prev = pptr;
		next = nptr;

	}
	type data;
	node* next;
	node* prev;
};
										//Stack class implemented thorugh linked list
template<class type>
class mystack
{
	friend class DoubleList<type>;
	typename DoubleList<type>::node* top;
	int size;
public:
	mystack()
	{
		size = 0;
		top = new typename DoubleList<type>::node(type(), nullptr, nullptr);//dummy node pointing to the top element
	}
	void push(typename DoubleList<type>::node* ptr)
	{
		if (ptr)
		{
			if (top->next)
			{
				ptr->next = top->next;
				top->next->prev = ptr;
			}
			top->next = ptr;
			ptr->prev = top;
			size++;						//updating the size 
		}

	}
	type& Top()							//return by reference so that changes can be held
	{
		if (!IsEmpty())
			return top->next->data;
		else
			return top->data;
		
	}
	typename DoubleList<type>::node* pop()			// popping node instead of data , avoiding deletion as per requirements
	{
		if (!IsEmpty() && size>1)
		{
			typename DoubleList<type>::node* tmp = top->next;
			top->next = tmp->next;
			tmp->next->prev = top;
			size--;
			tmp->next = tmp->prev = NULL;
			return tmp;

		}
		else if (!IsEmpty() && size == 1)          // if size == 1 then just making top->next NULL
		{
			typename DoubleList<type>::node* tmp = top->next;
			top->next = NULL;
			tmp->next = tmp->prev = NULL;
			size--;
			return tmp;
		}
		return nullptr;
	}
	bool IsEmpty()
	{
		return size == 0;
	}
	int getSize()
	{
		return size;
	}
	typename DoubleList<type>::node* sublistStart(int s = 0)
	{
		return top->next;

	}
	~mystack()
	{ // Start at the top real node (top->next) and delete all nodes
		DoubleList<type>::node* current = top->next;
		while (current != nullptr) {
			DoubleList<type>::node* nextNode = current->next; // Store next node
			delete current; // Delete current node
			current = nextNode; // Move to the next node
		}

		// Delete the dummy top node
		delete top;
	}
};
											//Command class
class command
{
	char com;
	string source;
	string dest;
	int number;
public:
	command(){};
	command(char c, string s = "\0", string d = "\0", int num = 0)
	{
		com = c;
		source = s;
		dest = d;
		number = num;

	}
	char getcom()
	{
		return com;
	}
	string getsource()
	{
		return source;
	}
	string getdest()
	{
		return dest;
	}
	int getnumber()
	{
		return number;
	}
	void swapSrcAndDest()
	{
		swap(source, dest);
	}
	
};

string ranks[13] = { "A", "2", "3", "4", "5", "6", "7", "8", "9", "10", "J", "Q", "K" };
class card
{
	string rank;
	string suit;
	int value;
	bool faceup;
	friend class game;
	friend ostream& operator<<(ostream& out, card& c)
	{
		c.print();
		return out;
	}
public:
	card(){ };
	void setfaceup(bool i){
		faceup = i;
	}
	card(int v , string s='\0')
	{
		suit = s;
		value = v;
		rank = ranks[v - 1];
		faceup = 0;
	}
	void print()
	{
		char* heart = "\x03";  //♥
		char* diamond = "\x04"; //♦
		char* clubs = "\x05";  //♣
		char* spades = "\x06"; //♠
		if (faceup)
		{
			if (suit == "heart" || suit == "diamond")
			{// Get handle to the console
				HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
				// Set the text color to red
				SetConsoleTextAttribute(hConsole, FOREGROUND_RED);
				cout << rank;
				if (suit == "heart")
					cout << heart << " ";
				else
					cout << diamond << " ";
				// Reset to default color
				SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
			}
			else
			{
				cout << rank;
				if (suit == "spades")
					cout << spades << " ";
				else if (suit == "clubs")
					cout << clubs << " ";
			}

		}
		else
			cout << "[ ]";
	}
	bool alternateColors(card& cd)
	{
		if (suit == "heart" ||suit == "diamond")
		{
			if (cd.suit == "heart" || cd.suit == "diamond")
				return false;
		}
		else if (suit == "clubs" ||suit == "spades")
		{
			if (cd.suit == "clubs" || cd.suit == "spades")
				return false;
		}
		return true;
	}
	bool isKing()
	{
		return value == 13;
	}
	bool isAce()
	{
		return value == 1;
	}
	bool getFace()
	{
		return faceup;
	}

};
										//Game class
class game
{
	DoubleList<card> tableau[7];
	mystack<card> stockpile;
	mystack<card> wastepile;
	mystack<card> foundation[4];
	mystack<command> undo;
	mystack<command> redo;
public:
	void prepareDeck()
	{
		for (int i = 1; i <= 13; i++)      //making deck i.e all cards of all suits and ranks combinations
		{
			DoubleList<card>::node* heartNode = new DoubleList<card>::node(card(i, "heart"));
			stockpile.push(heartNode);

			DoubleList<card>::node* diamondNode = new DoubleList<card>::node(card(i, "diamond"));
			stockpile.push(diamondNode);

			DoubleList<card>::node* spadeNode = new DoubleList<card>::node(card(i, "spades"));
			stockpile.push(spadeNode);

			DoubleList<card>::node* clubNode = new  DoubleList<card>::node(card(i, "clubs"));
			stockpile.push(clubNode);
		}
	}
	void shuffleDeck()  //As we cannot shuffle in the stack because of the only availabe pop and push functions
	{					// first i will use array to copy it then shuffle it in an array , then again push into the stack
		int size = stockpile.getSize();
		card* arr = new card[size];
		for (int i = 0; i < size; i++)    //copying into an array for shuffling
		{
			DoubleList<card>::node* poppedNode = stockpile.pop();
			arr[i] = poppedNode->data;
		}
		srand(time(0));   // generates random numbers according to time
		for (int i = 0; i < size; i++)    
		{
			int j = rand() % (i + 1);      // random number is taken modulus with index+1 so that it generates a random index eg when i=4, 12432%5=2
			swap(arr[i], arr[j]);		//swapping
		}
		for (int i = 0; i < size; i++)    //pushing into stack
		{
			DoubleList<card>::node* newNode = new DoubleList<card>::node(arr[i]);
			stockpile.push(newNode);
		}
		delete[] arr;   //deletion of array

	}
	void LoadTableau()
	{
		for (int i = 0; i < 7; i++)
		{
			for (int j = i; j < 7; j++)
			{
				DoubleList<card>::node* poppedNode = stockpile.pop();
				if (j == i)					//only the last card's face will be up
					poppedNode->data.faceup = 1;
				tableau[j].InsertNodeAtEnd(poppedNode);
			}

		}
	}
	void PrintTableau()
	{
		for (int i = 0; i < 7; i++)         //printing sizes
			cout << setw(5) << "Column:" << i + 1 << "    ";
		cout << endl;
		int maxcol = 0;
		for (int i = 0; i < 7; i++)         //printing sizes and finding max column
		{
			if (maxcol < tableau[i].getSize())
				maxcol = tableau[i].getSize();
			cout << setw(2) << tableau[i].getSize();
			if (tableau[i].getSize() == 1)
				cout << " card    ";
			else
				cout << " cards    ";
		}
		cout << endl;
		//printing vertically
		for (int i = 0; i < maxcol; i++)       // to print till longest column  
		{
			for (int j = 0; j < 7; j++)         // to print seven columns
			if (tableau[j].getSize() > i)
			{
				// Print the card in the current row of this column
				DoubleList<card>::Iterator ite = tableau[j].begin();
				for (int k = 0; k < i; k++)
				{
					ite++;
				}
				cout << *ite;
				if ((*ite).faceup == true && (*ite).value == 10)
					cout << setw(9);										//using setw() for properly aligned display
				else
					cout << setw(10);
				if ((*ite).faceup == true && (*ite).value != 10)
					cout << "   ";
				else if ((*ite).faceup == false)
					cout << "   ";
				else if ((*ite).faceup == true && (*ite).value == 10)
					cout << " ";
			}
			else
				cout << setw(12) << "             "; //adjusting spaces
			cout << endl;
		}
	}
	void printpiles()
	{
		HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
		SetConsoleTextAttribute(hConsole, FOREGROUND_RED);
		cout << "\n					SOLITAIRE\n\n";
		SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
		cout << "Stockpile         Wastepile	Foundation:1	Foundation:2	Foundation:3	Foundation:4\n";
		cout << "[    ]            ";
		if (wastepile.getSize())   //Printing wastepile
		{
			card& topCard = wastepile.Top();
			topCard.setfaceup(1);
			topCard.print();
			cout << "	        ";
		}
		else
			cout << "[     ]	";
		for (int i = 0; i < 4; i++)			//Printing foundations
		{
			if (foundation[i].getSize())
			{
				card& topCard = foundation[i].Top();
				topCard.setfaceup(1);
				topCard.print();
				cout << "	       ";
			}
			else
				cout << "[     ]		";
		}
		cout << endl;
		cout << stockpile.getSize();				//Printing number of  cards
		if (stockpile.getSize() <= 1)
			cout << " card            ";
		else
			cout << " cards          ";

		cout << wastepile.getSize();
		if (wastepile.getSize() <= 1)
			cout << " card	";
		else
			cout << " cards	";
		for (int i = 0; i < 4; i++)
		{
			cout << foundation[i].getSize();
			if (foundation[i].getSize() <= 1)
				cout << " card          ";
			else
				cout << " cards	         ";

		}
		cout << endl << endl;
	}
	void Restockpile()
	{				//Restocking piles i.e. pushing from wastepile back to stockpile when it becomes empty
		while (!wastepile.IsEmpty())
			stockpile.push(wastepile.pop());
	}
	void sCommand()
	{
		string ans;
		if (stockpile.getSize() > 0)				//Draws a card from stockpileS
		{
			DoubleList<card>::node* poppedNode = stockpile.pop();
			poppedNode->data.setfaceup(1);
			wastepile.push(poppedNode);
		}
		else						// if stockipile is empty, restocks it
		{
			cout << "Do you want to restock stockpile and draw?(Type yes or no)\n";
			cin >> ans;
			if (ans == "yes")
			{
				Restockpile();
			}
		}
	}
	void sUndoCommand()
	{//						If undo the draw command,move back to stockpile
		DoubleList<card>::node* poppedNode = wastepile.pop();
		stockpile.push(poppedNode);
	}
	void wCommand(string d, int num)//   Source is wastepile
	{
		mystack<card>* Wcolumn = new mystack<card>(wastepile);
		if (d[0] == 'c')			//Wastepile to columns
		{
			int cIndex = d[1] - '1';// converting char to int
			if (isColumnMoveValid(Wcolumn, tableau[cIndex], num))// Checking validity of command
			{
				moveStackToColumn(wastepile, tableau[cIndex], num);
			}
		}
		else if (d[0] == 'f')		//Wastepile to foundations
		{
			int fIndex = d[1] - '1';
			if (isMoveToFoundationValid(wastepile, foundation[fIndex], num))// Checking validity of command
			{
				moveStackToStack(wastepile, foundation[fIndex], num);
			}
		}
	}
	void cCommand(string s, string d, int num)//   Source is column
	{
		int csIndex = s[1] - '1';
		DoubleList<card>* Scolumn = new DoubleList<card>(tableau[csIndex]);
		if (d[0] == 'c')		//Columns to columns
		{
			int cdIndex = d[1] - '1';
			if (isColumnMoveValid(Scolumn, tableau[cdIndex], num))// Checking validity of command
			{
				moveColumnToColumn(tableau[csIndex], tableau[cdIndex], num);
			}
		}
		else if (d[0] == 'f')		//Columns to foundations
		{
			int fIndex = d[1] - '1';
			if (isMoveToFoundationValid(tableau[csIndex], foundation[fIndex], num))// Checking validity of command
			{
				moveColumnToStack(tableau[csIndex], foundation[fIndex], num);
			}

		}
	}
	void fCommand(string s, string d, int num)
	{			//Foundations to Columns 
		int fIndex = s[1] - '1';
		int cdIndex = d[1] - '1';
		mystack<card>* Scolumn = new mystack<card>(foundation[fIndex]);
		if (isColumnMoveValid(Scolumn, tableau[cdIndex], num))// Checking validity of command
		{
			moveStackToColumn(foundation[fIndex], tableau[cdIndex], num);
		}
	}
	void mCommand(command& c)// Move command
	{
		string s = c.getsource();
		string d = c.getdest();
		int num = c.getnumber();
		if (s == "w")
		{
			if (num == 1)
				wCommand(d, num);
			else
			{
				cout << "\nERROR:				!!!Cannot move more than one card!!!\n\n";
				this_thread::sleep_for(std::chrono::seconds(1));
			}

		}
		else if (s[0] == 'c')
		{
			cCommand(s, d, num);
		}
		else if (s[0] == 'f')
		{
			if (num == 1)
				fCommand(s, d, num);
			else
			{
				cout << "\nERROR:				!!!Cannot move more than one card!!!\n\n";
				this_thread::sleep_for(std::chrono::seconds(1));
			}
		}

	}
	void mUndoCommand(command& c)
	{// Not checking validity just undo 
		string s = c.getsource();
		string d = c.getdest();
		int num = c.getnumber();
		
		if (s == "w" && num == 1)
		{
			if (d[0] == 'c')			//Wastepile to columns
			{
				int cIndex = d[1] - '1';
				tableau[cIndex].tail->prev->data.setfaceup(0);// facing down previous card which was faced up when card(s) were moved
				moveStackToColumn(wastepile, tableau[cIndex], num);
			}
			else if (d[0] == 'f')		//Wastepile to foundations
			{
				int fIndex = d[1] - '1';
				moveStackToStack(wastepile, foundation[fIndex], num);
			}
		}
		else if (s[0] == 'c')
		{
			int csIndex = s[1] - '1';
			if (d[0] == 'c')		//Columns to columns
			{
				int cdIndex = d[1] - '1';
				tableau[cdIndex].tail->prev->data.setfaceup(0);
				moveColumnToColumn(tableau[csIndex], tableau[cdIndex], num);
			}
			else if (d[0] == 'f')		//Columns to foundations
			{
				int fIndex = d[1] - '1';
				moveColumnToStack(tableau[csIndex], foundation[fIndex], num);

			}
			else if (d[0] == 'w')		//Columns to wastepile
			{
				moveColumnToStack(tableau[csIndex],wastepile, num);
			}
		}
		else if (s[0] == 'f' && num == 1)		//Foundations to Columns 
		{
			int fIndex = s[1] - '1';
			if (d[0] == 'c')
			{
				int cdIndex = d[1] - '1';
				tableau[cdIndex].tail->prev->data.setfaceup(0);
				moveStackToColumn(foundation[fIndex], tableau[cdIndex], num);
			}
			if (d[0] == 'w')					//Foundations to Wastepile 
			{
				moveStackToStack(foundation[fIndex], wastepile, num);
			}
		}
	}
	void processCommand(command& c)
	{
		if (c.getcom() != 'w')// as a check to unknown command
		{
			if (c.getcom() == 's')
			{
				sCommand();
			}
			else if (c.getcom() == 'm')
			{
				mCommand(c);
			}
			else if (c.getcom() == 'z')
			{
				undoCommand();
			}
		}
		else
		{
			cout << "				    	!!!Unknown Command!!!\n";
			this_thread::sleep_for(std::chrono::seconds(1));
		}
		
	}
	template<class SourceType>
	bool isColumnMoveValid(SourceType*& s, DoubleList<card>& d, int n)
	{
		int size = s->getSize() - n;  // to get from where we have to move the linked list of cards
		if (size < 0)
		{
			cout << "\nERROR:			 !!!Invalid Numbers of cards to be moved!!!\n\n";  //Invalid number of cards check
			this_thread::sleep_for(std::chrono::seconds(1));
			return false;
		}
		if (n == 0)												//Number of cards missing
		{
			cout << "\nERROR:				!!!Please enter numbers of cards!!!\n\n";
			this_thread::sleep_for(std::chrono::seconds(1));
			return false;
		}
		DoubleList<card>::node* sourceSublist = s->sublistStart(size);
		DoubleList<card>::node* destTop = d.tail->prev; 
		//						FACE UP CHECKING
		if (sourceSublist->data.getFace() == 0)
			return false;
		else
		{
			if (d.getSize() > 0)
			{//                  ALTERNATIVE COLORS
				if (!(destTop->data.alternateColors(sourceSublist->data)))
				{
					cout << "\nERROR:				!!!Choose alternative colors!!!\n\n";
					this_thread::sleep_for(std::chrono::seconds(1));
					return false;
				}

				//                  DESCENDING ORDER
				if (destTop->data.value != sourceSublist->data.value + 1)
				{
					cout << "\nERROR:			!!!Choose card keeping in mind the descending order!!!\n\n";
					this_thread::sleep_for(std::chrono::seconds(1));
					return false;
				}
			}
			else//				EMPTY LIST AND SOURCELIST TOP KING CHECK
			{
				if (!sourceSublist->data.isKing())
				{
					cout << "\nERROR:				!!!Only king can be placed at base!!!\n\n";
					this_thread::sleep_for(std::chrono::seconds(1));
					return false;
				}
			}
		}


		return true;
	}
	template<typename SourceType>
	bool isMoveToFoundationValid(SourceType& s, mystack<card>& d, int n = 0)
	{
		card destTop = d.Top();
		int size = s.getSize() - n;  // to get from where we have to move the linked list of cards
		if (size < 0)
		{
			cout << "\nERROR:				!!!Invalid Numbers of cards to be moved!!!\n\n";
			this_thread::sleep_for(std::chrono::seconds(1));
			return false;
		}
		if (n == 0)
		{
			cout << "\nERROR:				!!!Numbers of cards missing!!!\n\n";
			this_thread::sleep_for(std::chrono::seconds(1));
			return false;
		}
		DoubleList<card>::node* sourceSublist = s.sublistStart(size);

		//						FACE UP CHECKING
		if (sourceSublist->data.getFace()==0)
			return false;
		else
		{
			if (d.getSize() > 0)
			{//                 SAME SUITS
				if (destTop.suit != sourceSublist->data.suit)
				{
					cout << "\nERROR:				!!!Suits not matched!!!\n\n";
					this_thread::sleep_for(std::chrono::seconds(1));
					return false;
				}

				//                  ASCENDING ORDER
				if (destTop.value + 1 != sourceSublist->data.value)
				{
					cout << "\nERROR:			!!!Choose card keeping in mind the ascending order!!!\n\n";
					this_thread::sleep_for(std::chrono::seconds(1));
					return false;
				}
			}
			else//				EMPTY LIST AND FOUNDATION TOP ACE CHECK
			{
				if (!sourceSublist->data.isAce())
				{
					cout << "\nERROR:				!!!Only Ace can be placed at base!!!\n\n";
					this_thread::sleep_for(std::chrono::seconds(1));
					return false;
				}
			}
		}
		return true;
	}
	void moveColumnToColumn(DoubleList<card>& s, DoubleList<card>& d, int n = 0)
	{
		int size = s.getSize() - n;  // to get from where we have to move the linked list of cards
		DoubleList<card>::Iterator ite = s.begin();
		for (int i = 0; ite != s.end() && i != size; ite++, i++);   // to reach that point
		if (size > 0)
		{
			DoubleList<card>::Iterator ite1 = ite;   // facing up of the card below the sublist
			ite1--;
			(*ite1).faceup = 1;
		}

		DoubleList<card>::node* startingPos = ite.getCurrent();
		startingPos->prev->next = s.tail; // updating prevs and nexts of source list for removal of sublist
		s.tail->prev = startingPos->prev; //the last card is still pointing to s.tail
		startingPos->prev = NULL;

		d.tail->prev->next = startingPos;   //attaching to destination list
		startingPos->prev = d.tail->prev;

		while (startingPos != nullptr && startingPos->next != s.tail)// to reach sublist's end
			startingPos = startingPos->next;


		d.tail->prev = startingPos;
	}
	void moveStackToColumn(mystack<card>& s, DoubleList<card>& d, int n)
	{	// just popping and inserting node
		DoubleList<card>::node* poppedNode = s.pop();
		d.InsertNodeAtEnd(poppedNode);
	}
	void moveStackToStack(mystack<card>& s, mystack<card>& d, int n)
	{// popping and pushing
		DoubleList<card>::node* poppedNode = s.pop();
		d.push(poppedNode);
	}
	void moveColumnToStack(DoubleList<card>& s, mystack<card>& d, int n)
	{
		DoubleList<card>::Iterator ite = s.end();
		DoubleList<card>::node* iteptr = ite.getCurrent();
		DoubleList<card>::Iterator ite1 = ite;   // facing up of the card below the card being moved
		ite1--;
		if (ite1.getCurrent() != nullptr) // Ensure it's not null
		{
			(*ite1).faceup = 1; 
		}

		iteptr->prev->next = s.tail;// taking the last card to push
		s.tail->prev = iteptr->prev;
		iteptr->next = iteptr->prev = NULL;
		d.push(iteptr);
	}
	void addUndoCommand(command c1)
	{// moving to undo stack whenever a command is entered except undo(z) itself or wrong command
		if (c1.getcom() != 'z' && c1.getcom() != 'w')
		{
			DoubleList<command>::node* comNode = new DoubleList<command>::node(c1);
			undo.push(comNode);
		}
	}
void undoCommand()
{
	DoubleList<command>::node* poppedNode = undo.pop();
	if (poppedNode)
	{
		command c = poppedNode->data;
		if (c.getcom() == 'm')
		{
			c.swapSrcAndDest();// swaping source and destination to undo
			mUndoCommand(c);
		}
		if (c.getcom() == 's')
			sUndoCommand();
	}
	else
	{	cout << "				    	!!!No command to undo left!!!\n";
		this_thread::sleep_for(std::chrono::seconds(1));
	}

}
command makeCommand(const char* entered)
{
	char operation;
	char src[10], dest[10];
	int num;

	operation = entered[0];   // the first letter entered will be the operation char
	if (operation == 'm')
	{
		int i, j = 0;
		for (i = 2; entered[i] != ','; i++, j++) //i=2 so that we start copying after m and space
			src[j] = entered[i];				// e.g src is "c1"
		src[j] = '\0';
		i++;
		j = 0;
		for (; entered[i] != ','&& entered[i] != '\0'; i++, j++)			//after first comma	reads and stores dest	
			dest[j] = entered[i];
		dest[j] = '\0';
		i++;
		num = atoi(&entered[i]);					//stores number of moves
		return command(operation, src, dest, num);
	}
	else if (operation == 's' || operation == 'z')
	{
		return command(operation);
	}
	else
		return command('w');// wrong command
}
bool gamewon()
{
	for (int i = 0; i < 4; i++)// if all the foundations top is king, then game is won
	{
		if (foundation[i].Top().value != 13)
			return false;
	}
	return true;
}
void loadGame()
{
	prepareDeck();
	shuffleDeck();
	LoadTableau();
	printpiles();
	PrintTableau();
}
void playGame()
{
	HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
	char entered[100] = "\0";
	while (!gamewon())
	{
		cout << "Enter Command: ";
		cin.getline(entered, 20);
		if (strcmp(entered,"quit")==0)
			break;
		else
		{
			// Set the text color to red
			SetConsoleTextAttribute(hConsole, FOREGROUND_RED);
			command c1 = makeCommand(entered);
			addUndoCommand(c1);
			processCommand(c1);
			// Reset to default color
			SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
			//system("cls");// clearing the screen after processing command
			printpiles();
			PrintTableau();
		}
	}
	if (gamewon())
	{
		SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN);
		cout << "				!!!CONGRATULATIONS GAME WON!!!\n\n";
		SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
	}
	else	
	{
		SetConsoleTextAttribute(hConsole, FOREGROUND_GREEN);
		cout << "				!!!GAME QUIT!!!\n";
		SetConsoleTextAttribute(hConsole, FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE);
	}
}
};
int main()
{
	game solitaire;
	solitaire.loadGame();
	solitaire.playGame();


	system("pause");
	return 0;
}