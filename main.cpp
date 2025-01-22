#include <iostream>
#include <fstream>
#include <filesystem>
#include <sys/time.h>
#include <sys/resource.h>
#include <chrono>
#include "Tries.hpp"
#include "FixedSize.cpp"
#include "VariableSizeTrie.cpp"
#include "HashTableTrie.cpp"

static const bool DEBUG_OUTPUT = true;

int main(int argc, char* argv[]) {

    // Some variables for messurments and the command line output at the end of the test.
    int const byte_mebiByte_conversion_rate = 1049000;

    std::string const name = "Tomke_Klein";
    std::string trie_variant;
    long trie_construction_memory;
    long trie_contruction_time;
    long querry_time;

    std::chrono::_V2::system_clock::time_point start;
    std::chrono::_V2::system_clock::time_point end;

    struct rusage usage;
    getrusage(RUSAGE_SELF, &usage);



    // This section parses the programm arguments and checks if they are valid.
    // It also chooses the requested trie_variant and creates file handles for the input, output and querry file.

    if (argc != 4) throw std::invalid_argument("Unsupported number of arguments: " + (argc-1) );

    std::string version = argv[1];
    if (version.find("-version=") == std::string::npos)
    {
        throw std::invalid_argument("Unsupported argument at first position: " + (std::string) argv[1]);
    }

    version = version.substr(version.find("=") + 1);
    std::int8_t version_nbr = version[0] - 48;

    if (version_nbr > 3 || version_nbr < 1 ) throw std::invalid_argument("Unsupported version number: " + version_nbr);

    std::ifstream input(argv[2]);
    std::ifstream querry(argv[3]); 
    std::ofstream output("result_" + (std::string) argv[2]);

    if (!input) {
        std::cerr << "The input_file " << argv[2] << " does not exist!" << std::endl;
        return 1;
    }
    if (!querry) {
        std::cerr << "The querry_file " << argv[3] << " does not exist!" << std::endl;
        return 1;
    }

    std::unique_ptr<Trie> trie;
    if (version_nbr == 1)
    {
        trie = std::make_unique<FixedSizeArrayTrie>();
        trie_variant = "fixed_size_array_trie";
    }
    if (version_nbr == 2)
    {
        trie = std::make_unique<VariableSizeArrayTrie>();
        trie_variant = "variable_size_array_trie";
    }
    if (version_nbr == 3)
    {
        trie = std::make_unique<HashTableTrie>();
        trie_variant = "hash_table_trie";
    }



    // TRIE CONSTRUCTION
    
    std::string line;
    bool result;

    if(DEBUG_OUTPUT) std::cout << "Building trie:" << std::endl; // <---- print command

    start = std::chrono::high_resolution_clock::now(); // begin timer
        
    

    while (getline (input, line))
    {
        result = trie->insert(line);

        if(DEBUG_OUTPUT) std::cout << "inserted: " << line << " successfull: " << result << std::endl; // <---- print command
    }

    

    end = std::chrono::high_resolution_clock::now(); // end timer

    trie_contruction_time = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    trie_construction_memory = usage.ru_maxrss / byte_mebiByte_conversion_rate;


    // QUERRYS

     if(DEBUG_OUTPUT) std::cout << "Running Querries:" << std::endl; // <---- print command
    int16_t sep_pos;
    std::string word;
    std::string querry_type;

    start = std::chrono::high_resolution_clock::now(); // begin timer

    while (getline (querry, line))
    {
        sep_pos = line.find(" ");
        word = line.substr(0, sep_pos);
        querry_type = line.substr(sep_pos + 1);

         if(DEBUG_OUTPUT) std::cout << "querry type: " << querry_type << " for word: " << word; // <---- print command

        if (querry_type == "c")         result = trie->contains(word);
        else if (querry_type == "d")    result = trie->delete_elem(word);
        else if (querry_type == "i")    result = trie->insert(word);
        else throw std::invalid_argument("Unsupported querry type: " + querry_type);

         if(DEBUG_OUTPUT) std::cout << " - result: " << result << std::endl; // <---- print command
        
        if (result)
        {
            output << "true" << std::endl; 
        } else {
            output << "false" << std::endl;
        }
        
    }

    end = std::chrono::high_resolution_clock::now(); // end timer
    querry_time = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();

    // Making the output

    std::cout << "RESULT"
            << " name=" << name
            << " trie_variant=" << trie_variant
            << " trie_construction_time=" << trie_contruction_time << "ms"
            << " trie_construction_memory=" << trie_construction_memory << "MiB"
            << " querry_time=" << querry_time << "ms" << std::endl;

    input.close();
    querry.close();
    output.close();
    
    return 0;
}
