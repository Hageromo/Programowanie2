#include <string>
#include <iostream>
#include <vector>
#include <time.h>
#include <algorithm>

using namespace std;

class Room;

class Item
{
    Room* ownerRoom;
    string name;
    int value;
    bool seen = false;
public:
    string getName() { return name; }
    void setSeen(bool seen) { this->seen = seen; }
    bool wasSeen() { return this->seen; }
    void setOwner(Room* ownerRoom) { this->ownerRoom = ownerRoom; }
    Room* getOwner() { return this->ownerRoom; }
    Item(string name, int value);
    ~Item();
};

Item::Item(string name, int value)
{
    this->name = name;
    this->value = value;
}


Item::~Item()
{
}

class Room
{
    string description;
    int x;
    int y;
    static int minX;
    static int maxX;
    static int minY;
    static int maxY;
    Room *neighborN;
    Room *neighborW;
    Room *neighborE;
    Room *neighborS;
    Item *item;
    double f, h;
    double g = 0;
    bool wasEvaluated = false;
    Room *parent;
    static void clearAfterPathfinding(vector <Room*> &rooms);
    static vector <Room*> rebuildPath(vector <Room*> &rooms, Room* &endRoom);
public:
    string getDescription()	{ return description; };
    Room *getNeighborN()			{ return neighborN; };
    Room *getNeighborW()			{ return neighborW; };
    Room *getNeighborE()			{ return neighborE; };
    Room *getNeighborS()			{ return neighborS; };
    vector<Room*> getNeighbors();
    bool hasItem()					{ return (item != nullptr); };
    Item *getItem()					{ return item; };
    bool placeItem(Item* item);

    void assignNeighbors(vector <Room*> &rooms);
    char getSymbol();

    static float getHDistanceBetweenRooms(const Room *roomA, const Room *roomB);


    static Room* findRoomByCoordinates(vector <Room*> &rooms, int x, int y);
    static void findNeighbors(vector <Room*> &rooms);
    static string createMap(vector <Room*> &rooms);
    static vector <Room*> getShortestPathBetweenRooms(vector <Room*> &rooms, Room* startRoom, Room* endRoom);
    Room(int x, int y);
    ~Room();
};
int Room::minX = INT8_MAX;
int Room::minY = INT8_MAX;
int Room::maxX = INT8_MIN;
int Room::maxY = INT8_MIN;

Room::Room(int x, int y)
{
    this->x = x;
    this->y = y;
    if (x < minX)
    {
        minX = x;
    }
    if (y < minY)
    {
        minY = y;
    }
    if (x > maxX)
    {
        maxX = x;
    }
    if (y > maxY)
    {
        maxY = y;
    }
}

Room::~Room()
{
}

Room* Room::findRoomByCoordinates(vector <Room*> &rooms, int x, int y)
{

    if (x < minX || x > maxX || y < minY || y > maxY)
    {
        return nullptr;
    }

    for (auto room : rooms)
    {
        if (room->x == x && room->y == y)
        {
            return room;
        }
    }

    return nullptr;
}

void Room::assignNeighbors(vector <Room*> &rooms)
{
    this->neighborN = findRoomByCoordinates(rooms, this->x, this->y - 1);
    this->neighborW = findRoomByCoordinates(rooms, this->x - 1, this->y);
    this->neighborE = findRoomByCoordinates(rooms, this->x + 1, this->y);
    this->neighborS = findRoomByCoordinates(rooms, this->x, this->y + 1);
}

void Room::findNeighbors(vector <Room*> &rooms)
{
    for (auto room : rooms)
    {
        room->assignNeighbors(rooms);
    }
}

char Room::getSymbol()
{
    if (nullptr != item && item->wasSeen())
    {
        return item->getName()[0];
    }
    return '#';
}

bool Room::placeItem(Item* item)
{
    if (this->item != nullptr)
    {
        return false;
    }
    this->item = item;
    item->setOwner(this);
    return true;
}

string Room::createMap(vector <Room*> &rooms)
{
    int horizontalSpread = (maxX - minX + 2);
    int verticalSpread = (maxY - minY + 1);
    int horizontalOffset =  - minX;
    int verticalOffset =  - minY;
    string map = string(horizontalSpread * verticalSpread, ' ');
    for (auto room : rooms)
    {
        map[(room->x + horizontalOffset) + ((room->y + verticalOffset) * horizontalSpread)] = room->getSymbol();
        int neighborX;
        int neighborY;
        if (room->getNeighborN() == nullptr)
        {
            neighborY = room->y + verticalOffset - 1;
            if (neighborY >= (minY + verticalOffset))
            {
                map[(room->x + horizontalOffset) + (neighborY * horizontalSpread)] = 'X';
            }
        }
        if (room->getNeighborS() == nullptr)
        {
            neighborY = room->y + verticalOffset + 1;
            if (neighborY <= (maxY + verticalOffset))
            {
                map[(room->x + horizontalOffset) + (neighborY * horizontalSpread)] = 'X';
            }
        }
        if (room->getNeighborW() == nullptr)
        {
            neighborX = room->x + horizontalOffset - 1;
            if (neighborX >= (minX + horizontalOffset))
            {
                map[(neighborX) + ((room->y + verticalOffset) * horizontalSpread)] = 'X';
            }
        }
        if (room->getNeighborE() == nullptr)
        {
            neighborX = room->x + horizontalOffset + 1;
            if (neighborX <= (maxX + horizontalOffset))
            {
                map[(neighborX)+((room->y + verticalOffset) * horizontalSpread)] = 'X';
            }
        }
    }
    for (auto i = 1; i <= verticalSpread; i++)
    {
        map[horizontalSpread * i - 1] = '\n';
    }
    return map;
}

vector<Room*> Room::getNeighbors()
{
    vector<Room*> neighbors;
    if (neighborN != nullptr)
    {
        neighbors.push_back(neighborN);
    }
    if (neighborE != nullptr)
    {
        neighbors.push_back(neighborE);
    }
    if (neighborS != nullptr)
    {
        neighbors.push_back(neighborS);
    }
    if (neighborW != nullptr)
    {
        neighbors.push_back(neighborW);
    }
    return neighbors;
}

float Room::getHDistanceBetweenRooms(const Room *roomA, const Room *roomB)
{
    return ((roomA->x * roomB->x) + (roomA->y * roomB->y))^1/2;
}

Room* findClosestRoom(vector <Room*> &openNodes, Room* &room)
{
    Room* closestRoom = openNodes[0];
    float currentShortestDistance = Room::getHDistanceBetweenRooms(room, closestRoom);
    float potentialShortestDistance = 0;
    for (Room* currentRoom : openNodes)
    {
        potentialShortestDistance = Room::getHDistanceBetweenRooms(room, currentRoom);
        if (potentialShortestDistance < currentShortestDistance)
        {
            closestRoom = currentRoom;
        }
    }
    return closestRoom;
}

void Room::clearAfterPathfinding(vector <Room*> &rooms)
{
    for (auto room : rooms)
    {
        room->g = 0;
        room->wasEvaluated = false;
        room->parent = nullptr;
    }
}

vector <Room*> Room::rebuildPath(vector <Room*> &rooms, Room* &endRoom)
{
    vector <Room*> path;
    Room *currentNode = endRoom;
    while (currentNode->parent != nullptr)
    {
        path.insert(path.begin(), currentNode);
        currentNode = currentNode->parent;
    }
    Room::clearAfterPathfinding(rooms);
    return path;
}



vector <Room*> Room::getShortestPathBetweenRooms(vector <Room*> &rooms, Room* startRoom, Room* endRoom)
{

    vector <Room*> openNodes;
    Room* currentNode;
    bool isTentativeBetter;
    float tentativeG;

    openNodes.push_back(startRoom);
    while (openNodes.size() != 0)
    {
        currentNode = findClosestRoom(openNodes, endRoom);
        if (currentNode == endRoom)
        {
            return rebuildPath(rooms, endRoom);
        }
        openNodes.erase(find(openNodes.begin(), openNodes.end(), currentNode));
        currentNode->wasEvaluated = true;

        for (auto neighbor : currentNode->getNeighbors())
        {
            if (neighbor->wasEvaluated)
                continue;
            tentativeG = currentNode->g + 1.0;
            isTentativeBetter = false;
            if (count(openNodes.begin(), openNodes.end(), neighbor) == 0)
            {
                openNodes.push_back(neighbor);
                neighbor->h = Room::getHDistanceBetweenRooms(neighbor, endRoom);
                isTentativeBetter = true;
            }
            else if (tentativeG < neighbor->g)
            {
                isTentativeBetter = true;
            }
            if (isTentativeBetter)
            {
                neighbor->parent = currentNode;
                neighbor->g = tentativeG;
                neighbor->f = neighbor->g + neighbor->f;
            }
        }
    }
    return vector <Room*>();
}


void randomizeItems(vector <Room*> &rooms, vector <Item *> items)
{
    srand((unsigned int)time(NULL));
    int randomRoomIndex;
    bool result;
    while (items.size() > 0)
    {
        randomRoomIndex = rand() % rooms.size();
        result = rooms[randomRoomIndex]->placeItem(items.back());
        if (result)
            items.pop_back();
    }
}

vector <Room*> loadMap()
{
    vector <Room*> rooms;
    rooms.push_back(new Room(1, 1));
    rooms.push_back(new Room(2, 1));
    rooms.push_back(new Room(4, 1));
    rooms.push_back(new Room(6, 1));
    rooms.push_back(new Room(7, 1));
    rooms.push_back(new Room(8, 1));
    rooms.push_back(new Room(2, 2));
    rooms.push_back(new Room(3, 2));
    rooms.push_back(new Room(4, 2));
    rooms.push_back(new Room(5, 2));
    rooms.push_back(new Room(6, 2));
    rooms.push_back(new Room(8, 2));
    rooms.push_back(new Room(1, 3));
    rooms.push_back(new Room(2, 3));
    rooms.push_back(new Room(4, 3));
    rooms.push_back(new Room(6, 3));
    rooms.push_back(new Room(7, 3));
    rooms.push_back(new Room(8, 3));

    Room::findNeighbors(rooms);

    return rooms;
}

void findFastestWayToGetItems(vector <Room*> rooms, vector <Item *> items)
{
    vector <int> itemsIndex;
    int x = 0;
    for (auto item : items)
    {
        itemsIndex.push_back(x++);
    }
    int minimalStepCount = INT8_MAX;
    vector <int> bestItemsIndex;
    vector <Room*> shortPath;
    int stepCount = 0;
    int previousItem = 0;
    do
    {
        stepCount = 0;
        previousItem = -1;
        for (auto i : itemsIndex)
        {
            if (previousItem == -1)
            {
                shortPath = Room::getShortestPathBetweenRooms(rooms, rooms[0], items[i]->getOwner());
                stepCount += shortPath.size();
            }
            else
            {
                shortPath = Room::getShortestPathBetweenRooms(rooms, items[previousItem]->getOwner(), items[i]->getOwner());
                stepCount += shortPath.size();
            }
            previousItem = i;

        }


        if (stepCount < minimalStepCount)
        {
            bestItemsIndex = itemsIndex;
            minimalStepCount = stepCount;
        }
    } while (next_permutation(itemsIndex.begin(), itemsIndex.end()));
    cout << "Minimalna ilość kroków do znalezienia przedmiotu: " << minimalStepCount << endl;
    cout << "Kolejność przedmiotów: " << endl;
    for (auto i : bestItemsIndex)
    {
        cout << items[i]->getName() << endl;
    }
    cout << endl;
}

void takeSteps(vector <Room*> rooms, vector <Item *> items, string steps)
{
    int stepCounter = 0;
    Room * currentRoom = rooms[0];
    for (auto character : steps)
    {
        if (currentRoom->getItem() != nullptr && !currentRoom->getItem()->wasSeen())
        {
            cout << "Nazwa przedmiotu: " << currentRoom->getItem()->getName() << endl;
            cout << "Krok[" << stepCounter << "]" << endl;
            currentRoom->getItem()->setSeen(true);
        }
        stepCounter++;
        switch (character)
        {
            case 'n':
                currentRoom = currentRoom->getNeighborN();
                break;
            case 's':
                currentRoom = currentRoom->getNeighborS();
                break;
            case 'w':
                currentRoom = currentRoom->getNeighborW();
                break;
            case 'e':
                currentRoom = currentRoom->getNeighborE();
                break;
            default:
                break;
        }
    }
    if (currentRoom->getItem() != nullptr)
    {
        cout << "Nazwa przedmiotu: " << currentRoom->getItem()->getName() << endl;
        cout << "Krok[" << stepCounter << "]" << endl;
    }

}

int main()
{
    vector <Room*> rooms = loadMap();
    vector <Item *> items;

    items.push_back(new Item("Obraz", 10));
    items.push_back(new Item("Doniczka", 20));
    items.push_back(new Item("Klawiatura", 50));
    randomizeItems(rooms, items);

    cout << Room::createMap(rooms) << endl;
    takeSteps(rooms, items, "essweneesnnseeneewwssee");

    cout << Room::createMap(rooms) << endl;
    findFastestWayToGetItems(rooms, items);

}