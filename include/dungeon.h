#ifndef __DUNGEON_H
#define __DUNGEON_H

#include <string>
#include <vector>
#include <memory>
#include <sstream>


class Description {
private:
    std::string _display_name;
    std::string _text;
public:
    Description(std::string display_name, std::string text)
        : _display_name(display_name), _text(text) {}

    std::string getDisplayName() { return _display_name; }
    std::string getText() { return _text; }

    static std::unique_ptr<Description> makeDesc(std::string display_name, std::string text)
    {
        return std::make_unique<Description>(display_name, text);
    }

};

class DungeonNode {
private:
    std::string _ident;
    std::unique_ptr<Description> _desc;
public:
    DungeonNode(std::string ident, std::unique_ptr<Description> desc)
        : _ident(ident), _desc(std::move(desc)) {}

    std::string getIdent() { return _ident; }
    std::string getDisplayName() { return _desc->getDisplayName(); }
    std::string getText() { return _desc->getText(); }

    virtual std::ostream& details(std::ostream& os) = 0;

    std::ostream& display(std::ostream& os)
    {
        os << "---------------------" << std::endl;
        os << getIdent() << ", " << getDisplayName() << std::endl;
        os << getText() << std::endl;        
        return details(os);
    }

};

class Item : public DungeonNode {
private:
    int _value;
    bool _is_unique;

public:
    Item(std::string ident, std::unique_ptr<Description> desc, int value, bool is_unique = false)
        : DungeonNode(ident, std::move(desc)), _value(value), _is_unique(is_unique) {}

    int getValue() { return _value; }
    bool isUnique() { return _is_unique; }

    virtual std::ostream& details(std::ostream& os) override {
        os << "Value: " << _value << std::endl;
        os << (_is_unique ? "Unique" : "Common") << std::endl;
    } 

};

class Room : public DungeonNode {
private:
    std::vector<std::string> _exits;
    std::vector<std::string> _items;

public:
    Room(std::string ident, std::unique_ptr<Description> desc)
        : DungeonNode(ident, std::move(desc)) {}

    void addExist(std::string exit_id)
    {
        _exits.push_back(exit_id);
    }

    void addItem(std::string item_id)
    {
        _items.push_back(item_id);
    }

    std::vector<std::string> getExits() { return _exits; }
    std::vector<std::string> getItems() { return _items; }


    virtual std::ostream& details(std::ostream& os) override {
    } 



};

class Dungeon : public DungeonNode {
private:
    std::vector<std::unique_ptr<Room>> _rooms;
    std::vector<std::unique_ptr<Item>> _items;

public:
    Dungeon(std::string ident, std::unique_ptr<Description> desc)
        : DungeonNode(ident, std::move(desc)) {}

    void addRoom(std::unique_ptr<Room> room)
    {
        _rooms.push_back(std::move(room));
    }

    void addItem(std::unique_ptr<Item> item)
    {
        _items.push_back(std::move(item));
    }
    virtual std::ostream& details(std::ostream& os) override {

        for (int i = 0; i < _rooms.size(); i++)
        {
            _rooms[i]->display(os);
            os << "Exits" << std::endl;
            for (auto exit : _rooms[i]->getExits())
            {
                os << exit << std::endl;
            }
            os << "Items" << std::endl;
            for (auto item : _rooms[i]->getItems())
            {
                os << item << std::endl;
            }
        }
    } 

};



#endif // __DUNGEON_H
