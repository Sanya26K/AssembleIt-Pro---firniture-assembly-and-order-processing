#include "Utilities.h"
#include <iostream>
#include <stdexcept>
#include <algorithm>
#include <cctype>
#include <string>

namespace sdds {
    void Utilities::setFieldWidth(size_t newWidth) {
        m_widthField = newWidth;
    }

    size_t Utilities::getFieldWidth() const {
        return m_widthField;
    }

    std::string Utilities::extractToken(const std::string& str, size_t& next_pos, bool& more) {
        std::string token;

        if (next_pos == std::string::npos) {
            more = false;
            return "";
        }

        size_t delimiter_pos = str.find(m_delimiter, next_pos);

        if (delimiter_pos != std::string::npos) {
            token = str.substr(next_pos, delimiter_pos - next_pos);
            next_pos = delimiter_pos + 1;
            more = true;
        }
        else {
            token = str.substr(next_pos);
            next_pos = std::string::npos;
            more = false;
        }

        token = trim(token);

        if (token.empty()) {
            more = false;
            throw std::runtime_error("Error: Empty token found.");
        }

        if (token.find(m_delimiter) != std::string::npos) {
            more = false;
            throw std::runtime_error("Error: Token contains the delimiter character.");
        }
        if (token.length() > m_widthField) {
            m_widthField = token.length();
        }
        return token;
    }

    char Utilities::m_delimiter = ',';

    void Utilities::setDelimiter(char newDelimiter) {
        m_delimiter = newDelimiter;
    }

    char Utilities::getDelimiter() {
        return m_delimiter;
    }

    std::string trim(const std::string& str) {
        std::string result = str;
        // Strip leading spaces
        result.erase(result.begin(), std::find_if(result.begin(), result.end(), [](int ch) {
            return !std::isspace(ch);
            }));
        // Strip trailing spaces
        result.erase(std::find_if(result.rbegin(), result.rend(), [](int ch) {
            return !std::isspace(ch);
            }).base(), result.end());
        return result;
    }
}