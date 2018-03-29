#pragma once
class Game
{
public:
	Game(int pTurn, int pRound);

	int setTurn(int turn);
	//Precondition: The index of the turn order is passed in turn
	//Postcondition: Value is stored in turn.

	int getTurn();
	//Precondition: None
	//Postcondition: Current value of turn is returned.

	int setRound(int round);
	//Precondition: The index of the round is passed in turn
	//Postcondition: Value is stored in turn.

	int getRound();
	//Precondition: None
	//Postcondition: Current value of turn is returned.

	int setEndLevel(int level);
	//Precondition: A value of the end level is passed in turn
	//Postcondition: Value is stored in turn.

	int getEndLevel();
	//Precondition: None
	//Postcondition: Current value of the end level is returned.

	int incTurn(int order[], int number);
		// Precondition: The order of the entities is stored in array order with the index of the first entity
		//				stored in order[0]. The number of entities is stored in number. The current round is 
		//				stored in round.
		// Postcondition: The index of the entity to go next is returned. Round is updated as appropirate. A -1 is returned if the conditions
		//				for the value of endLevel is greater than or equal to the end game threshold.

	int incTurn2(int order[], int number);


private:
	int turn; // Index of current Player or company
	int round; // Current round
	bool marketSuccess; //Indicates player did not pass
	int numberPasses; //Number of passes in turn
	int playerOrder[maxPlayers];
	int companyOrder[maxCompanies];
	int endLevel = 2;
};

