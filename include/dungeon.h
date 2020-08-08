#ifndef __DUNGEON_H
#define __DUNGEON_H

#include <string>
#include <vector>
#include <memory>
#include <sstream>

enum class ItemType {
    Common,
    Unique
};

enum class NodeType {
    Dungeon,
    Room,
    Item,
    Description
};

class DungeonNode {
protected:
    std::string _ident;
    std::string _display_name;
    std::string _description;
    NodeType _node_type;
public:
    DungeonNode(std::string ident, std::string display_name, std::string description, NodeType node_type)
        : _ident(ident), _display_name(display_name), _description(description), _node_type(node_type) {}



    std::string getIdent() { return _ident; }
    std::string getDisplayName() { return _display_name; }
    std::string getDescription() { return _description; }
    NodeType getNodeType() { return _node_type; } 

    virtual void display(std::ostream& os)
    {
        os << _ident << std::endl;
        os << _display_name << std::endl;
        os << _description << std::endl;
    }   
};

class Item : public DungeonNode {
protected:
   int _value;
   ItemType _type; 
public:
    Item(std::string ident, std::string display_name, std::string description, int value, ItemType type)
        : DungeonNode(ident, display_name, description, NodeType::Item), _value(value), _type(type) {}    

    int getValue() { return _value; }
    ItemType getType() { return _type; }

    virtual void display(std::ostream& os) override 
    {
        os << "----------------------------------------------" << std::endl;
        os << "Item" << std::endl;
        os << "----------------------------------------------" << std::endl;
        DungeonNode::display(os);
        os << "----------------------------------------------" << std::endl;
    }
};

class Room : public DungeonNode {
protected:
    std::vector<std::string> _exits;
    std::vector<std::string> _items;

public:
    Room(std::string ident, std::string display_name, std::string description)
        : DungeonNode(ident, display_name, description, NodeType::Room) {}    

    Room(std::string ident, DungeonNode* node) : DungeonNode(ident, node->getDisplayName(), node->getDescription(), NodeType::Room) {}

    void addExit(std::string exit)
    {
        _exits.push_back(exit);
    }

    void addExits(std::vector<std::string> exits)
    {
        for (auto exit : exits) _exits.push_back(exit);
    }

    std::vector<std::string> getExits() { return _exits; }

    void addItem(std::string item)
    {
        _items.push_back(item);
    }

    void addItems(std::vector<std::string> items)
    {
        for (auto item : items) _items.push_back(item);
    }


    std::vector<std::string> getItems() { return _items; }

    virtual void display(std::ostream& os) override 
    {
        os << "----------------------------------------------" << std::endl;
        os << "Room" << std::endl;
        os << "----------------------------------------------" << std::endl;
        DungeonNode::display(os);
        os << "----------------------------------------------" << std::endl;
        os << "Items" << std::endl;
        os << "----------------------------------------------" << std::endl;
        for (auto item : _items)
        {
            os << item << std::endl;
        }
        os << "Exits" << std::endl;
        os << "----------------------------------------------" << std::endl;
        for (auto exit : _exits)
        {
            os << exit << std::endl;
        }
        os << "----------------------------------------------" << std::endl;
    }


};

class Dungeon : public DungeonNode {
protected:
    std::vector<Room*> _rooms;
    std::vector<Item*> _items;

public:
    Dungeon(std::string ident, std::string display_name, std::string description)
        : DungeonNode(ident, display_name, description, NodeType::Dungeon) {}    

    void addRoom(Room* room)
    {
        _rooms.push_back(room);
    }

    std::vector<Room*> getRooms()
    {
        return _rooms;
    }

    void addItem(Item* item)
    {
        _items.push_back(item);
    }

    std::vector<Item*> getItems()
    {
        return _items;
    }

    virtual void display(std::ostream& os) override 
    {
        os << "----------------------------------------------" << std::endl;
        os << "Dungeon" << std::endl;
        os << "----------------------------------------------" << std::endl;
        DungeonNode::display(os);
        os << "----------------------------------------------" << std::endl;
        os << "Rooms" << std::endl;
        os << "----------------------------------------------" << std::endl;
        for (auto room : _rooms)
        {
            room->display(os);
        }
    }

};












#endif // __DUNGEON_H
