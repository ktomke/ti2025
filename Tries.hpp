#pragma once

#include <string>

class Trie {
    public: 
        virtual ~Trie() = default;
        virtual bool contains(std::string& elem) const =0;
        virtual bool delete_elem(std::string& elem) =0;
        virtual bool insert(std::string& elem) =0;

        size_t lcp_function(const std::string& str1, const std::string& str2) const {
            size_t minLength = std::min(str1.length(), str2.length());
            size_t i = 0;

            while (i < minLength && str1[i] == str2[i]) {
                i++;
            }

            return i;
        }
};

