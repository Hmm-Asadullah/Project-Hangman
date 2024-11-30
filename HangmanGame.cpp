#include<iostream>
#include <string>
#include<fstream>
#include <ctime>
using namespace std;
//Function prototypes
void registerUser();
void loginUser();
int wordLength(string& word);
void playGame(const string& username);
string chooseWord();
void updateGameRecords(const string& username, bool won, int score);
//Main Function
int main()
{
	srand(time(0));
	cout << "Welcome to Hangman Game!\n";
	do
	{
		//Printing interface
		cout << "1. Register\n";
		cout << "2. Login\n";
		cout << "3. Quit\n";
		cout << "Enter your choice = ";
		int choice;
		cin >> choice;
		cin.ignore();
		if (choice == 1)
		{
			//Calling register user function for user registeration
			registerUser();
		}
		else if (choice == 2)
		{
			loginUser();
		}
		else if (choice == 3)
		{
			cout << "\nThank you for playing Hangman!\n";
			break;
		}
		else
		{
			cout << "Invalid choice. Please try again.\n";
		}
	} while (true);
	return 0;
}
//Register user function
void registerUser()
{
	//Getting user Credentials
	string username, password, fileUsername, filePassword;
	bool flag = true;
	cout << "Enter username = ";
	getline(cin, username);
	cout << "Enter password = ";
	cin >> password;
	//Making and opening users.txt file in append mode
	ofstream file("D:\\users.txt", ios::app);
	ifstream f1("D:\\users.txt");
	if (file.is_open())
	{
		//Checking and comparing with already registered users
		while (f1 >> fileUsername >> filePassword)
		{
			if ((fileUsername == username) || (filePassword == password))
			{
				cout << "\nA user with that username or password already exists. Try with different credentials!\n\n";
				flag = false;
				break;
			}
		}
		if (flag)
		{
			//Adding user credentials to file
			file << username << "\t" << password << "\n";
			file.close();
			cout << "User registered successfully!\n";
		}
	}
	else
	{
		cout << "Error opening file for registration.\n";
	}
}
//Login user function
void loginUser()
{
	string username, password, fileUsername, filePassword;
	bool flag = false;
	cout << "Enter username = ";
	getline(cin, username);
	cout << "Enter password = ";
	cin >> password;
	ifstream file("D:\\users.txt");
	if (file.is_open())
	{
		//Comparing entered credentials with registered credentials
		while (file >> fileUsername >> filePassword)
		{
			if ((fileUsername == username) && (filePassword == password))
			{
				flag = true;
				break;
			}
		}
		file.close();
	}
	else
	{
		cout << "Error opening file for login.\n";
	}
	//Checking if the user login is successfull or not
	if (flag)
	{
		cout << "Login successful!\n";
		playGame(username);
	}
	else
	{
		cout << "Invalid username or password. Please try again!\n";
	}
}
//Choose word function
string chooseWord()
{
	const int MAX_WORDS = 1000;
	string words[MAX_WORDS];
	int wordCount = 0;
	ifstream file("D:\\words.txt");
	if (!file.is_open())
	{
		cout << "Error opening words file.\n";
		exit(1);
	}
	while (file >> words[wordCount])
	{
		wordCount++;
		if (wordCount >= MAX_WORDS)
		{
			cout << "Exceeded maximum word limit.\n";
			break;
		}
	}
	file.close();
	if (wordCount == 0)
	{
		cout << "Words file is empty.\n";
		exit(1);
	}
	//Getting random word from file
	int randomIndex = rand() % wordCount;
	return words[randomIndex];
}
//Word length Fumction
int wordLength(string& word)
{
	//Getting word length
	int length = 0;
	for (int i = 0; word[i] != '\0'; i++)
	{
		length++;
	}
	return length;
}
//Play game function
void playGame(const string& username)
{
	string word = chooseWord();
	string guessedWord(wordLength(word), '_');
	int triesLeft = 6;
	bool won = false;
	while (triesLeft > 0 && !won)
	{
		char guess;
		if (triesLeft == 6)
		{
			cout << "________________\n|\t\t|\n|\n|\n|\n\|\n";
		}
		cout << "Current word= " << guessedWord << "\n";
		cout << "Tries left= " << triesLeft << "\n";
		cout << "Enter your guess= ";
		cin >> guess;
		//Checking capital alphabets
		if (guess >= 'A' && guess <= 'Z')
		{
			guess = guess + 'a' - 'A';
		}
		bool correctGuess = false;
		for (int i = 0; i < wordLength(word); i++)
		{
			if (word[i] == guess && guessedWord[i] == '_')
			{
				guessedWord[i] = guess;
				correctGuess = true;
			}
		}
		if (!correctGuess)
		{
			triesLeft--;
			//Printing Hangman
			if (triesLeft == 5)
			{
				cout << "________________\n|\t\t|\n|\t\tO\n|\n|\n|\n";
			}
			else if (triesLeft == 4)
			{
				cout << "________________\n|\t\t|\n|\t\tO\n|\t\t|\n|\n|\n";
			}
			else if (triesLeft == 3)
			{
				cout << "________________\n|\t\t|\n|\t\tO\n|              /|\n|\n|\n";
			}
			else if (triesLeft == 2)
			{
				cout << "________________\n|\t\t|\n|\t\tO\n|              /|\\\n|\n|\n";
			}
			else if (triesLeft == 1)
			{
				cout << "________________\n|\t\t|\n|\t\tO\n|              /|\\\n|              /\n|\n";
			}
		}
		if (guessedWord == word)
		{
			won = true;
		}
	}
	//Won condition
	if (won)
	{
		cout << "Congratulations! You guessed the word right. Word = " << word << "\n";
		int score = triesLeft;
		updateGameRecords(username, true, score);
	}
	//Lost condition
	else
	{
		cout << "Game Over!\nThe word was= " << word << "\n";
		cout << "________________\n|\t\t|\n|\t\tO\n|              /|\\\n|              / \\\n|\n";
		updateGameRecords(username, false, 0);
	}
	//Asking from user to paly agian or not
	cout << "Do you want to play again? (Y/N)= ";
	char choice;
	cin >> choice;
	if (choice == 'Y' || choice == 'y')
	{
		playGame(username);
	}
}
//Update records function
void updateGameRecords(const string& username, bool won, int score)
{
	ofstream file("D:\\game_records.txt", ios::app);
	if (file.is_open())
	{
		//Updating records of user in file
		file << username << "\t \t " << (won ? "Won" : "Lost") << "\t \t " << score << "\n";
		file.close();
	}
	else
	{
		cout << "Error opening game records file.\n";
	}
}