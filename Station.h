#ifndef STATION_H
#define STATION_H

#include <string>
#include <iostream>

namespace sdds {
    class Station {
        int m_id;
        std::string m_itemName;
        std::string m_description;
        size_t m_serialNumber;
        size_t m_quantity;
        static size_t m_widthField;
        static size_t id_generator;

    public:
        Station(const std::string& record);

        const std::string& getItemName() const;
        size_t getNextSerialNumber();
        size_t getQuantity() const;
        void updateQuantity();
        void display(std::ostream& os, bool full) const;
    };
}

#endif // STATION_H
