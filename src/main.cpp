#include "inventory.h"

#include <iostream>
#include <string>

void printUsage()
{
    std::cout << "Inventory Manager v1.0\n"
              << "Commands:\n"
              << "  add <name> <category> <qty> <price>  - Add an item\n"
              << "  remove <name>                        - Remove an item\n"
              << "  find <name>                          - Find an item by name\n"
              << "  category <category>                  - List items in category\n"
              << "  list                                 - List all items\n"
              << "  total                                - Show total count and value\n"
              << "  quit                                 - Exit\n";
}

int main()
{
    Inventory inventory;

    printUsage();
    std::cout << "\n> ";

    std::string command;

    while(std::getline(std::cin, command))
    {
        if(command == "quit" || command == "exit")
        {
            break;
        }
        else if(command == "list")
        {
            auto items=inventory.getAllItems();

            if(items.empty())
            {
                std::cout << "(empty)\n";
            }
            else
            {
                for(const auto &item : items)
                {
                    std::cout << "  " << item.toString() << "\n";
                }
            }
        }
        else if(command == "total")
        {
            std::cout << "Items: " << inventory.getTotalItemCount()
                      << "  Value: $" << inventory.getTotalValue() << "\n";
        }
        else if(command.substr(0, 4) == "add ")
        {
            // Simple parsing: add name category qty price
            std::string rest=command.substr(4);
            std::string name, category;
            int qty=0;
            double price=0.0;

            size_t pos=rest.find(' ');

            if(pos != std::string::npos)
            {
                name=rest.substr(0, pos);
                rest=rest.substr(pos + 1);
                pos=rest.find(' ');

                if(pos != std::string::npos)
                {
                    category=rest.substr(0, pos);
                    rest=rest.substr(pos + 1);
                    pos=rest.find(' ');

                    if(pos != std::string::npos)
                    {
                        qty=std::stoi(rest.substr(0, pos));
                        price=std::stod(rest.substr(pos + 1));
                    }
                }
            }

            Item item{name, category, qty, price};

            if(inventory.addItem(item))
            {
                std::cout << "Added: " << item.toString() << "\n";
            }
            else
            {
                std::cout << "Failed to add item.\n";
            }
        }
        else if(command.substr(0, 7) == "remove ")
        {
            std::string name=command.substr(7);

            if(inventory.removeItem(name))
            {
                std::cout << "Removed: " << name << "\n";
            }
            else
            {
                std::cout << "Not found: " << name << "\n";
            }
        }
        else if(command.substr(0, 5) == "find ")
        {
            std::string name=command.substr(5);
            auto item=inventory.findItem(name);

            if(item.has_value())
            {
                std::cout << "  " << item->toString() << "\n";
            }
            else
            {
                std::cout << "Not found: " << name << "\n";
            }
        }
        else if(command.substr(0, 9) == "category ")
        {
            std::string cat=command.substr(9);
            auto items=inventory.findItemsByCategory(cat);

            if(items.empty())
            {
                std::cout << "No items in category: " << cat << "\n";
            }
            else
            {
                for(const auto &item : items)
                {
                    std::cout << "  " << item.toString() << "\n";
                }
            }
        }
        else
        {
            std::cout << "Unknown command. Type 'quit' to exit.\n";
        }

        std::cout << "> ";
    }

    return 0;
}
