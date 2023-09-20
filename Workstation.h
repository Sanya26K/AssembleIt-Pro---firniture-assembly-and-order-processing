#ifndef SDDS_WORKSTATION_H
#define SDDS_WORKSTATION_H

#include "Station.h"
#include "CustomerOrder.h"
#include <deque>
#include <iostream>

namespace sdds {

    extern std::deque<CustomerOrder> g_pending;
    extern std::deque<CustomerOrder> g_completed;
    extern std::deque<CustomerOrder> g_incomplete;

    class Workstation : public Station {

    public:
        std::deque<CustomerOrder> m_orders;
        Workstation* m_pNextStation;
        Workstation(const std::string& record);
        void fill(std::ostream& os);
        bool attemptToMoveOrder();
        void setNextStation(Workstation* station);
        Workstation* getNextStation() const;
        void display(std::ostream& os) const;
        Workstation& operator+=(CustomerOrder&& newOrder);
        Workstation(const Workstation&) = delete;
        Workstation& operator=(const Workstation&) = delete;
        Workstation(Workstation&&) = delete;
        Workstation& operator=(Workstation&&) = delete;
    };
}

#endif
