#include "LineManager.h"
#include <fstream>
#include <algorithm>
#include "Utilities.h"
#include "Workstation.h"
#include <map>

namespace sdds {
    LineManager::LineManager(const std::string& file, const std::vector<Workstation*>& stations) : m_cntCustomerOrder(0), m_firstStation(nullptr) {
        std::ifstream input(file);
        if (!input) {
            throw std::string("Unable to open [") + file + "] file.";
        }

        std::string record;
        
        while (std::getline(input, record)) {
            size_t delimiterPos = record.find('|');
            if (delimiterPos != std::string::npos) {
                std::string stationName = record.substr(0, delimiterPos);
                std::string nextStationName = record.substr(delimiterPos + 1);

                //std::cout << stationName << " - " << nextStationName << std::endl;

                auto station = std::find_if(stations.begin(), stations.end(), [&stationName](Workstation* ws) {
                    return ws->getItemName() == stationName;
                    });

                auto nextStation = std::find_if(stations.begin(), stations.end(), [&nextStationName](Workstation* ws) {
                    return ws->getItemName() == nextStationName;
                    });

                if (station != stations.end() && nextStation != stations.end()) {
                    (*station)->setNextStation(*nextStation);
                    m_activeLine.push_back(*station);
                }
            }
            else {
                auto station = std::find_if(stations.begin(), stations.end(), [&record](Workstation* ws) {
                    return ws->getItemName() == record;
                    });

                if (station != stations.end()) {
                    (*station)->setNextStation(nullptr);
                    m_activeLine.push_back(*station);
                }
            }
        }

        std::string first = "Bed";

        auto station = std::find_if(stations.begin(), stations.end(), [&first](Workstation* ws) {
            return ws->getItemName() == first;
            });
        m_firstStation = *station;
        m_cntCustomerOrder = g_pending.size();
    }


    void LineManager::reorderStations() {
        Workstation* station = m_firstStation;
        std::vector<Workstation*> orderedActiveline;

        while (station != nullptr) {
            orderedActiveline.push_back(station);
            station = station->getNextStation();
        }

        m_activeLine = orderedActiveline;
        orderedActiveline.clear();
    }

    bool LineManager::run(std::ostream& os) {
        static size_t iteration = 0; // to keep track of iteration
        iteration++;

        os << "Line Manager Iteration: " << iteration << std::endl;
        if (!g_pending.empty()) {
            *m_firstStation += std::move(g_pending.front());  // move to first
            g_pending.pop_front();
        }

        for_each(m_activeLine.begin(), m_activeLine.end(), [&](Workstation* ws) { 
            ws->fill(os); // calling fill operation on each 
            });

        for_each(m_activeLine.begin(), m_activeLine.end(), [&](Workstation* ws) {
            ws->attemptToMoveOrder(); // moving order
            });

        size_t totalOrder = g_completed.size() + g_incomplete.size();
        return  totalOrder == m_cntCustomerOrder;

    }

    void LineManager::display(std::ostream& os) const {
        for (auto station : m_activeLine) {
            //std::cout << station->getItemName() << " -- " << station->getNextStation() << std::endl;
            station->display(os);
        }
    }
}
