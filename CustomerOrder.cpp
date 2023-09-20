#include "CustomerOrder.h"
#include "Utilities.h"
#include <iostream>
#include <iomanip>
#include <algorithm>
using std::endl;

namespace sdds {
    size_t CustomerOrder::m_widthField = 0;

    // Overloaded Constructor - Extract the tokens from the string and populate the current instance
    CustomerOrder::CustomerOrder(const std::string& record) {
        Utilities util;

        size_t next_pos = 0;
        bool more = true;

        size_t copyIdx{};

        m_name = util.extractToken(record, next_pos, more);
        m_product = util.extractToken(record, next_pos, more);
        copyIdx = next_pos;
        // Counting the number of items
        while (more) {
            util.extractToken(record, next_pos, more);
            m_cntItem++;
        }
        // Reset position
        next_pos = copyIdx;
        more = true;

        // Dynamically allocating memory for the array of pointers
        m_lstItem = new Item * [m_cntItem];
        for (size_t i = 0; i < m_cntItem; i++) {
            m_lstItem[i] = new Item(util.extractToken(record, next_pos, more));
        }

        // Setting the field width
        if (m_widthField < util.getFieldWidth()) {
            m_widthField = util.getFieldWidth();
        }
    }

    // Copy constructor
    CustomerOrder::CustomerOrder(const CustomerOrder& other) { 
        throw "Copy operation not allowed"; 
    }


    // Move Semantics
    CustomerOrder::CustomerOrder(CustomerOrder&& other) noexcept {
        *this = std::move(other); // moving item
    }

    CustomerOrder& CustomerOrder::operator=(CustomerOrder&& other) noexcept {
        if (this != &other) {
            for (size_t i = 0; i < m_cntItem; i++) {
                // Deallocating memory for every items
                delete m_lstItem[i];
            }
            // Deallocating memory for array of pointers
            delete[] m_lstItem;

            // moving items
            m_name = other.m_name;
            other.m_name = "";
            m_product = other.m_product;
            other.m_product = "";
            m_cntItem = other.m_cntItem;
            other.m_cntItem = 0;
            m_lstItem = other.m_lstItem;
            other.m_lstItem = nullptr;
        }
        return *this;
    }

    // Destructor
    CustomerOrder::~CustomerOrder() {
        for (size_t i = 0; i < m_cntItem; i++) {
            delete m_lstItem[i]; // deleting every items in array
        }
        
        delete[] m_lstItem; // delete array of pointer
    }

    // checks if order is filled
    bool CustomerOrder::isOrderFilled() const {
        bool flag = true;
        for (size_t i = 0; i < m_cntItem && flag; i++) {
            if (!m_lstItem[i]->m_isFilled) {
                flag = false;
            }
        }
        return flag;
    }

    // check if all items have been filled
    bool CustomerOrder::isItemFilled(const std::string& itemName) const {
        bool flag = true;
        for (size_t i = 0; i < m_cntItem && flag; i++) {
            if (m_lstItem[i]->m_itemName == itemName) {
                if (!m_lstItem[i]->m_isFilled) {
                    flag = false;
                }
            }
        }
        return flag;
    }

    
    void CustomerOrder::fillItem(Station& station, std::ostream& os) {
        bool filled = false;

        //std::cout << m_cntItem << std::endl;

        for (size_t i = 0; i < m_cntItem && !filled; i++) {

            //std::cout << m_lstItem[i]->m_itemName << " - " << station.getItemName() << " - " << station.getQuantity() << std::endl;
            if ((m_lstItem[i]->m_itemName == station.getItemName()) && (station.getQuantity() > 0) && !m_lstItem[i]->m_isFilled) {
                station.updateQuantity();
                m_lstItem[i]->m_serialNumber = station.getNextSerialNumber();
                m_lstItem[i]->m_isFilled = true;
                filled = true;
                os << "    Filled " << m_name << ", " << m_product << " [" << m_lstItem[i]->m_itemName << "]" << endl;
            }
            else if ((m_lstItem[i]->m_itemName == station.getItemName()) && (station.getQuantity() == 0)) {
                os << "    Unable to fill " << m_name << ", " << m_product << " [" << m_lstItem[i]->m_itemName << "]" << endl;
            }
        }
    }

    // Displays current object
    void CustomerOrder::display(std::ostream& os) const {
        os << m_name << " - " << m_product << endl;
        for (size_t i = 0; i < m_cntItem; i++) {
            os << std::setfill('0') << "[" << std::right << std::setw(6) << this->m_lstItem[i]->m_serialNumber << "] ";
            os << std::left << std::setfill(' ') << std::setw(m_widthField) << m_lstItem[i]->m_itemName << " - " << (m_lstItem[i]->m_isFilled ? "FILLED" : "TO BE FILLED");
            os << std::endl;
        }
    }
}