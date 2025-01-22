#include "Tries.hpp"
#include <iostream>
#include <unordered_map>

class HashTableTrie : public Trie {
    private:
        struct Node {
            private:
                int8_t nbr_children = 0;
                std::unordered_map<char, Node*> children;

            public: 
                std::string comp_edge_label;

                Node(std::string edge_label) { comp_edge_label = edge_label; }

                // This function adds a child to the node and stores a pointer to it.
                void add_child(Node* child_ptr) {
                    char first_letter = child_ptr->comp_edge_label[0];
                    if(children.find(first_letter) == children.end()) {
                        children[first_letter] = child_ptr;
                    }
                }

                // This function checks if there is an edge to a child, that begins with a given letter.
                Node* find_child(char letter) {
                    if (children.find(letter) != children.end())
                    {
                        return children[letter];
                    }
                    return nullptr;
                }

                // This functions delets the child, whoms edge starts with the given letter.
                void delete_child(char letter) {
                    children.erase(letter);
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