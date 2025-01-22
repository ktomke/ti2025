#include "Tries.hpp"
#include <string>
#include <memory>
#include <iostream>

static const size_t ALPH_SIZE = 64; // a-z : 26, A-Z:26, 0-9: 10, $:1, 0:1 -> 26+26+10+1+1=64

class FixedSizeArrayTrie : public Trie {
    private:
        struct Node {
            private:
                // This char to nbr function translates a char to its position in the children array.
                // It can deal with both the 0 byte and the $ letter and as long as they do not appear mixed
                // in an input file, this should not lead to unexpected behaviour.
                int char_to_nbr(char c) const {
                    if (c >= 'a' && c <= 'z') return c - 'a';
                    if (c >= 'A' && c <= 'Z') return c - 'A' + 26;
                    if (c >= '0' && c <= '9') return c - '0' + 52;
                    if (c == '$') return 62;
                    if (c == 0 ) return 63;
                    throw std::invalid_argument("Unsupported character");
                }

            public: 
                std::string comp_edge_label;
                Node** children = (Node**) malloc(ALPH_SIZE * sizeof(Node*));

                Node(std::string edge_label) {
                    for (size_t i = 0; i < ALPH_SIZE; i++)
                    {
                        children[i] = nullptr;
                    }
                    
                    comp_edge_label = edge_label;
                }

                ~Node() { free(children); }

                // This function adds a child to the node and stores a pointer to it.
                void add_child(Node* child_ptr) {
                    int8_t char_nbr = char_to_nbr(child_ptr->comp_edge_label[0]);
                    if (children[char_nbr] == nullptr)
                    {
                        children[char_nbr] = child_ptr;
                    }
                    
                }

                // This function checks if there is an edge to a child, that begins with a given letter.
                Node* find_child(char letter) {
                    return children[char_to_nbr(letter)];
                }

                // This functions delets the child, whoms edge starts with the given letter.
                void delete_child(char letter) {
                    children[char_to_nbr(letter)] = nullptr;
                }
        };

        Node *root = new Node("");

    public:

        bool insert(std::string &elem) override {
            size_t matched_characters = 0;
            Node* current_node = root;
            Node* parent_node;
            Node* new_leave_node;
            Node* next_node;
            char first_letter;

            while (true)        
            {
                first_letter = elem[matched_characters];
                next_node = current_node->find_child(first_letter);

                if (next_node == nullptr)
                {   
                    // There is no edge with the first letter of our unmatched suffix. Therefore, the word is not
                    // in the trie and we can add it here.
                    current_node->add_child(new Node(elem.substr(matched_characters)));
                    return 1;
                }
                else
                {
                    // We now step into the next node and compare the suffix of our word with the edge of that node.
                    parent_node = current_node;
                    current_node = next_node;

                    size_t lcp = lcp_function(elem.substr(matched_characters), current_node->comp_edge_label);
                    size_t suffix_length = elem.length() - matched_characters;
                    size_t edge_length = current_node->comp_edge_label.length();

                    if (lcp == suffix_length)
                    {   
                        // This means, we reached a leave and therefore the element was alredy in the trie.
                        return 0;
                    } 
                    else if (lcp == edge_length)        
                    {
                        // This means, we can not yet make a decicion weather or not the word is in the trie.
                        matched_characters = matched_characters + lcp;
                    }
                    else {
                        // This means, that lcp is smaller then both edge_length and suffix_length, so we now that 
                        // the word is not in the trie and we can insert it here.
                        //
                        // For that, we now have to split the current_node at lcp and make two child nodes.
                        // One of the two new nodes will be the a leave (no children) and on its edge we write the 
                        // unmatched suffix of our word. The other will have the unmatched suffix of the edge label 
                        // and its children are going to be the children of the current node.

                        // Create new leave and name it accordingly.
                        new_leave_node = new Node(elem.substr(matched_characters + lcp));

                        // Create new intermediate node and name it accordingly.
                        current_node = new Node(current_node->comp_edge_label.substr(0,lcp));

                        // Rename the label of the next_node accordingly.
                        next_node->comp_edge_label = next_node->comp_edge_label.substr(lcp);

                        // Make this new intermediate node the child of the parent_node.
                        parent_node->delete_child(first_letter);
                        parent_node->add_child(current_node);

                        // Set the children of the new intermediate node.
                        current_node->add_child(next_node);
                        current_node->add_child(new_leave_node);

                        return 1;
                    }
                    
                }
                
            }

            return 0;
        }

        bool contains(std::string& elem) const override {
            size_t matched_characters = 0;
            Node* current_node = root;
            Node* next_node;
            char first_letter;

            while (true)        
            {
                first_letter = elem[matched_characters];
                next_node = current_node->find_child(first_letter);

                if (next_node == nullptr)
                {   
                    // There is no edge with the first letter of our unmatched suffix. Therefore, the word is not
                    // in the trie and we can return false. 
                    return 0;
                }
                else
                {
                    // We now step into the next node and compare the suffix of our word with the edge of that node.
                    current_node = next_node;

                    size_t lcp = lcp_function(elem.substr(matched_characters), current_node->comp_edge_label);
                    size_t suffix_length = elem.length() - matched_characters;
                    size_t edge_length = current_node->comp_edge_label.length();

                    if (lcp == suffix_length)
                    {   
                        // This means, we reached a leave and therefore the element is contained in the trie.
                        return 1;
                    } 
                    else if (lcp == edge_length)        
                    {
                        // This means, we can not yet make a decicion weather or not the word is contained in the trie.
                        matched_characters = matched_characters + lcp;
                    }
                    else {
                        // This means, that lcp is smaller then both edge_length and suffix_length, so we now that 
                        // the word is not contained in the trie.
                        return 0;
                    }
                    
                }
                
            }

            return 0;
        }

        bool delete_elem(std::string& elem) override{
            size_t matched_characters = 0;
            Node* current_node = root;
            Node* parent_node;
            Node* next_node;
            char first_letter;

            while (true)        
            {
                first_letter = elem[matched_characters];
                next_node = current_node->find_child(first_letter);

                if (next_node == nullptr)
                {   
                    // There is no edge with the first letter of our unmatched suffix. Therefore, the word is not
                    // in the trie and we can return false.   
                    return 0;
                }
                else
                {
                    // We now step into the next node and compare the suffix of our word with the edge of that node.
                    parent_node = current_node;
                    current_node = next_node;

                    size_t lcp = lcp_function(elem.substr(matched_characters), current_node->comp_edge_label);
                    size_t suffix_length = elem.length() - matched_characters;
                    size_t edge_length = current_node->comp_edge_label.length();

                    if (lcp == suffix_length)
                    {   
                        // This means, we reached a leave and therefore the element was contained in the trie.
                        // We now have to remove this leave and then we are done.

                        // delete current_node;
                        parent_node->delete_child(first_letter);

                        return 1;
                    } 
                    else if (lcp == edge_length)        
                    {
                        // This means, we can not yet make a decicion weather or not the word is in the trie.
                        matched_characters = matched_characters + lcp;
                    }
                    else {
                        // This means, that lcp is smaller then both edge_length and suffix_length, so we now that 
                        // the word is not contained in the trie.
                        return 0;
                    }
                    
                }
                
            }

            return 0;
        }
};