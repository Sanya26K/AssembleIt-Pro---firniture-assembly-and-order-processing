#ifndef SDDS_CUSTOMERORDER_H
#define SDDS_CUSTOMERORDER_H

#include "Station.h"
#include <iostream>

namespace sdds {
    // Item structure
    struct Item {
        std::string m_itemName;
        size_t m_serialNumber = 0;
        bool m_isFilled = false;
        Item(const std::string& src) : m_itemName(src) {};
    };

    class CustomerOrder {
    private:
        std::string m_name{};
        std::string m_product{};
        size_t m_cntItem{ 0 };
        Item** m_lstItem{ nullptr };
        static size_t m_widthField;
    public:
        // Default Constructor
        CustomerOrder() = default;
        // Overloaded Constructor
        CustomerOrder(const std::string& record);
        // Copy Operations
        CustomerOrder(const CustomerOrder&);
        // copy assignment constructor - which is in delete state
        CustomerOrder& operator=(const CustomerOrder&) = delete;
        // Move Semantics
        CustomerOrder(CustomerOrder&& co) noexcept;
        // move assignment constructor
        CustomerOrder& operator=(CustomerOrder&& co) noexcept;
        // Destructor
        virtual ~CustomerOrder();
        // checks if all the items in the order have been filled
        bool isOrderFilled() const;
        // checks if all items have been filled
        bool isItemFilled(const std::string& itemName) const;
        void fillItem(Station& station, std::ostream& other);
        void display(std::ostream& other = std::cout) const;
    };
}

#endif // SDDS_CUSTOMERORDER_H