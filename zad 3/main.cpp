#include <iostream>
#include <string>
#include <vector>

using namespace std;

class Skills
{
    static vector <string> skills_;
public:
    Skills();
    ~Skills();
    static vector <string> generatSkills(int seed, unsigned int amount);
};

vector <string> Skills::skills_ = { "1", "2", "3", "4", "5", "6", "7", "8", "9", "10" };


Skills::Skills()
{
}

vector <string> Skills::generatSkills(int seed, unsigned int amount)
{
    srand(seed);
    vector <string> skills;
    vector <bool> wasUsed(skills_.size());
    int randomIndex;
    while (skills.size() < amount)
    {
        randomIndex = rand() % skills_.size();
        if (wasUsed[randomIndex])
        {
            continue;
        }
        skills.insert(skills.begin(), skills_[randomIndex]);
        wasUsed[randomIndex] = true;
    }
    return skills;
}

class Player
{
    string name;
    string surname;
    vector<string> skills;
    short int height;
public:
    Player(string name, string surname, int height, vector<string> skills);
    Player(string name, string surname, int height, int seed);
    Player(const Player &source);
    ~Player();

    string getName() { return name; };
    string getSurname() { return surname; };
    int getHeight() { return height; };

    string getSkills();
    string getDescription();

    Player rerollSkills(int seed);
};


Player::Player(string name, string surname, int height, vector<string> skills)
{
    this->name = name;
    this->surname = surname;
    this->skills = skills;
    this->height = height;
}

Player::Player(string name, string surname, int height, int seed)
{
    this->name = name;
    this->surname = surname;
    this->skills = Skills::generatSkills(seed, 5);
    this->height = height;
}

Player::Player(const Player &source)
{
    this->name = source.name;
    this->surname = source.surname;
    this->skills = source.skills;
    this->height = source.height;
}

Player::~Player() {}

string Player::getSkills()
{
    string concatenatedSkills;
    for (string skill : skills)
    {
        concatenatedSkills += skill + " ";
    }
    return concatenatedSkills;
}

string Player::getDescription()
{
    string description;
    description += "    Name of player: " + getName() + " " + getSurname() + "\n";
    description += "    Height: " + to_string(getHeight()) + "cm\n";
    description += "    Skills: " + getSkills() + "\n";
    return description;
}

Player Player::rerollSkills(int seed)
{
    this->skills = Skills::generatSkills(seed, 5);
    return *this;
}
class Team
{
    string name;
    vector <Player> players;
public:
    Team(const string name);
    Team(const Team &source);

    Player operator [](int i) const { return players[i]; }
    Player & operator [](int i) { return players[i]; }

    string getName() { return name; };

    string getDescription();

    void addPlayer(Player playerToAdd);
    void changePlayer(Player player, int playerIndex);
};
Team::Team(const string name)
{
    this->name = name;
}

Team::Team(const Team &source)
{
    this->name = source.name;
    for (Player playerToAdd : source.players)
    {
        addPlayer(playerToAdd);
    }
}

void Team::addPlayer(Player playerToAdd)
{
    this->players.push_back(playerToAdd);
}

void Team::changePlayer(Player player, int playerIndex)
{
    this->players[playerIndex] = player;
}

string Team::getDescription()
{
    string description = "  Team's name: " + getName() + "\n";
    description += "   Player list:\n";
    for (Player player : players)
    {
        description += player.getDescription();
    }
    return description;
}



int main() {

	Team First("Alfa");
	First.addPlayer(Player("Michael", "First", 180, 1));
	First.addPlayer(Player("Jacob", "Second", 190, 2));
	First.addPlayer(Player("Codi", "Third", 177, 3));
	First.addPlayer(Player("Pete", "Fourth", 169, 4));
	First.addPlayer(Player("Adam", "Fifth", 179, 5));

	Team Second("Beta");
	Second.addPlayer(Player("Bob", "One", 156, 6));
	Second.addPlayer(Player("Malcolm", "Two", 177, 7));
	Second.addPlayer(Player("Francis", "Three", 168, 8));
	Second.addPlayer(Player("Peter", "Four", 187, 9));
	Second.addPlayer(Player("Harry", "Five", 190, 10));

	Team Third(Second);

	cout << First.getDescription()<<endl;
	cout << Second.getDescription()<<endl;
	cout << Third.getDescription()<<endl;

		Third[0] = Third[0].rerollSkills(3);

	cout << First.getDescription();
	cout << Second.getDescription();
	cout << Third.getDescription();


    return 0;
}