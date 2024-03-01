#pragma once

#include <iostream>
#include <sstream>
#include <vector>
#include <unordered_set>
#include <stdexcept>
#include <regex>
#include <stack>
#include <iterator>

namespace strman {
    std::string trim(std::string str) {
        int start = 0;
        int end = str.length();

        for (start; start < str.length(); start++) {
            if (str[start] != ' ') break;
        }

        for (end; end >= 0; end--) {
            if (str[end] != ' ') break;
        }

        return str.substr(start, end);
    }

    std::vector<std::string> split(std::string str, std::string delimiter, bool trim_token = true) {
        size_t pos_start = 0, pos_end, delim_len = delimiter.length();
        std::string token;
        std::vector<std::string> result;

        while ((pos_end = str.find(delimiter, pos_start)) != std::string::npos) {
            token = str.substr(pos_start, pos_end - pos_start);
            pos_start = pos_end + delim_len;
            result.push_back(trim_token ? trim(token) : token);
        }

        result.push_back(trim_token ? trim(str.substr(pos_start)) : str.substr(pos_start));
        return result;
    }

    std::vector<std::string> split(std::string str, std::vector<std::string> delimiters, bool trim_token = true) {
        std::stack<std::string> stack;
        std::vector<std::string> final_result;

        stack.emplace(str);

        while (!stack.empty()) {
            auto top = stack.top();
            stack.pop();

            bool atomic_string = true;
            for (auto& delim : delimiters) {
                if (!atomic_string) break;

                auto results = split(top, delim, trim_token);
                if (results.size() > 1) {
                    atomic_string = false;
                }
                else {
                    continue;
                }

                for (auto& result : results) {
                    stack.emplace(result);
                }
            }

            if (atomic_string) {
                final_result.emplace_back(top);
            }
        }

        return final_result;
    }

    std::string split_get(std::string str, std::string delimiter, size_t index) {
        size_t pos_start = 0, pos_end, delim_len = delimiter.length(), count = -1;
        std::string token;

        while ((pos_end = str.find(delimiter, pos_start)) != std::string::npos && count != index) {
            token = str.substr(pos_start, pos_end - pos_start);
            pos_start = pos_end + delim_len;
            count++;
        }

        return token;
    }

    std::string split_replace(std::string str, std::string delimiter, size_t index, std::string replace_with) {
        size_t pos_start = 0, pos_end, delim_len = delimiter.length(), count = 0;

        std::string result;
        std::string token;

        while ((pos_end = str.find(delimiter, pos_start)) != std::string::npos) {
            token = str.substr(pos_start, pos_end - pos_start);
            pos_start = pos_end + delim_len;

            if (count == index) {
                token = replace_with;
            }

            result += token + ',';
            count++;
        }

        if (index == count) {
            return result + replace_with;
        }
        result += str.substr(pos_start);

        return result;
    }

    std::vector<std::string> read_array(std::string str) {
        std::regex regex("'[a-z\\s\\-]*'");
        auto begin = std::sregex_iterator (str.begin(), str.end(), regex);
        auto end = std::sregex_iterator();
        std::vector<std::string> elements;

        for (auto iter = begin; iter != end; ++iter) {
            std::smatch match = *iter;
            std::string match_str = match.str();

            elements.push_back(match_str.substr(1, match_str.length() - 2));
        }
        
        return elements;
    }

    bool check_empty_delimiter(std::string str, std::string delimiter) {
        size_t pos_start = 0, pos_end, delim_len = delimiter.length();
        std::string token;

        while ((pos_end = str.find(delimiter, pos_start)) != std::string::npos) {
            token = str.substr(pos_start, pos_end - pos_start);
            pos_start = pos_end + delim_len;

            if (token.length() == 0) {
                return true;
            }
        }

        return false;
    }
}

