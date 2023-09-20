#include "Station.h"
#include "Utilities.h"
#include <iomanip>


namespace sdds {
    // Initialize the static member variables
    size_t Station::m_widthField = 0;
    size_t Station::id_generator = 0;

    Station::Station(const std::string& record) {
        Utilities util;
        size_t next_pos = 0;
        bool more = true;

        m_id = ++id_generator;
        m_itemName = util.extractToken(record, next_pos, more);
        m_serialNumber = std::stoi(util.extractToken(record, next_pos, more));
        m_quantity = std::stoi(util.extractToken(record, next_pos, more));

        if (m_widthField < util.getFieldWidth())
            m_widthField = util.getFieldWidth();

        m_description = util.extractToken(record, next_pos, more);
    }

    const std::string& Station::getItemName() const {
        return m_itemName;
    }

    size_t Station::getNextSerialNumber() {
        return m_serialNumber++;
    }

    size_t Station::getQuantity() const {
        return m_quantity;
    }

    void Station::updateQuantity() {
        if (m_quantity > 0)
            --m_quantity;
    }

    void Station::display(std::ostream& os, bool full) const {
        os << std::setfill('0') << std::right << std::setw(3) << m_id << " | ";
        os << std::setw(m_widthField) << std::left << std::setfill(' ') << m_itemName << " | ";
        os << std::setw(6) << std::right << std::setfill('0') << m_serialNumber << " | ";

        if (full) {
            os << std::setw(4) << std::right << std::setfill(' ') << m_quantity << " | ";
            os << m_description;
        }

        os << std::endl;
    }
}