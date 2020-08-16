#ifndef __PLAYER_H
#define __PLAYER_H


class Player {
protected:
    std::string _playerName;
    std::string _currentRoom;
    // [quatity, item_ident]
    std::vector<std::tuple<int, std::string>> _items;

    std::tuple<int, int, std::string> findItem(std::string item_id)
    {
        int index = 0;
        for (auto item : _items)
        {
            auto [qty, id] = item;
            if (id == item_id)
            {
                return std::tuple<int, int, std::string>(index, qty, id);
            }
            index++;
        }
        std::tuple<int, int, std::string>(-1,-1,"");
    }

public:
    Player(std::string playerName, std::string startRoom) : _playerName(playerName), _currentRoom(startRoom) {}

    std::string getCurrentRoom()
    {
        return _currentRoom;
    }

    std::string getPLayerName()
    {
        return _playerName;
    }

    void setCurrentRoom(std::string newRoom)
    {
        _currentRoom = newRoom;
    }

    void addItem(std::string item)
    {
        auto [index, qty, id] = findItem(item);
        if (index > -1)
        {
            _items.at(index) = std::tuple<int, std::string>(qty+1, id);
        } else {
            _items.push_back(std::tuple<int, std::string>(1, id));
        }
    }

    void removeItem(std::string item)
    {
        auto [index, qty, id] = findItem(item);
        if (index > -1 && qty > 0)
        {
            _items.at(index) = std::tuple<int, std::string>(qty-1, id);
        } 
    }

    friend std::ostream& operator <<(std::ostream& os, const Player& player)
    {
        os << "Player: " << player._playerName << std::endl;
        os << "Items" << std::endl;
        for (auto [qty, item] : player._items)
        {
            os << "\t" << qty << ": " << item << std::endl;
        }
        return os;        
    }



};




#endif // __PLAYER_H