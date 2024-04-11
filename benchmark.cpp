#include <iostream>
#include <unordered_map>
#include <vector>
#include <chrono>
#include "map.h" // Assicurati che questo includa la tua definizione di hashmap in C

// Funzione per generare una stringa casuale di lunghezza data
std::string generate_random_string(int length)
{
    static const char alphanum[] =
        "0123456789"
        "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
        "abcdefghijklmnopqrstuvwxyz";

    std::string str;
    for (int i = 0; i < length; ++i)
    {
        str.push_back(alphanum[rand() % (sizeof(alphanum) - 1)]);
    }
    return str;
}

int main(int argc, char* argv[])
{
    //int numKeys;
    //std::cout << "Inserisci il numero di chiavi da generare: ";
    //std::cin >> numKeys;

    int numKeys = std::atoi(argv[1]);
    std::cout << numKeys << ",";
    // Genera un set di coppie chiave-valore casuali
    std::unordered_map<std::string, std::string> cpp_map;
    map_t c_map = map_create_default();
    std::vector<std::string> arr;

    //Tempo in inserimento
    for (int i = 0; i < numKeys; i++)
    {
        std::string key = generate_random_string(8);
        arr.push_back(key);
    }

    auto start_cpp = std::chrono::high_resolution_clock::now();
    for (std::string &str : arr)
    {
        cpp_map[str] = str;
    }
    auto end_cpp = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> cpp_duration = end_cpp - start_cpp;

    auto start_c = std::chrono::high_resolution_clock::now();
    char *c_str;
    for (std::string &str : arr)
    {
        c_str = (char *)str.c_str();
        map_add(&c_map, c_str, c_str);
    }
    auto end_c = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> c_duration = end_c - start_c;
    std::cout << c_duration.count() << ",";
    std::cout << cpp_duration.count() << ",";

    // Misura il tempo di iterazione e ricerca per la mappa C
    start_c = std::chrono::high_resolution_clock::now();
    for (std::string &str : arr)
    {
        map_find(&c_map, (char *)str.c_str());
    }
    end_c = std::chrono::high_resolution_clock::now();
    c_duration = end_c - start_c;

    // Misura il tempo di iterazione e ricerca per la mappa C++
    start_cpp = std::chrono::high_resolution_clock::now();
    for (std::string &str : arr)
    {
        cpp_map.find(str);
    }
    end_cpp = std::chrono::high_resolution_clock::now();
    cpp_duration = end_cpp - start_cpp;

    // Stampa i tempi misurati
    std::cout << c_duration.count() << ",";
    std::cout << cpp_duration.count() << ",";

    //std::cout << "Verifying correctness ";
    for (auto &str : arr)
    {
        std::string s = cpp_map[str];
        std::string x = std::string(map_find(&c_map, (char *)str.c_str()));
        if (s != x)
        {
            std::cout << "NOT OK,";
            map_destroy(&c_map);
            return 1;
        }
    }
    std::cout << "OK,";

    int i = numKeys / 2;
    for (std::string &str : arr)
    {
        if (--i <= 0)
        {
            break;
        }
        map_delete(&c_map, (char *)str.c_str());
    }
    i = numKeys / 2;
    for (std::string &str : arr)
    {
        if (--i <= 0)
        {
            break;
        }
        cpp_map.erase(str);
    }

    i = numKeys / 2;
    //std::cout << "Verifying correctness after some delete";
    for (auto &str : arr)
    {
        if (--i <= 0)
        {
            break;
        }
        char *f = map_find(&c_map, (char *)str.c_str());
        if (f != NULL && cpp_map.count(str) != 0)
        {
            std::cout << "NOT OK,";
            map_destroy(&c_map);
            return 1;
        }
    }
    std::cout << "OK,";
    i = numKeys / 2;

    // Misura il tempo di eliminazione per la mappa C
    start_c = std::chrono::high_resolution_clock::now();
    for (std::string &str : arr)
    {
        if (--i <= 0)
        {
            break;
        }
        map_delete(&c_map, (char *)str.c_str());
    }
    end_c = std::chrono::high_resolution_clock::now();
    c_duration = end_c - start_c;
    i = numKeys / 2;
    // Misura il tempo di eliminazione per la mappa C++
    start_cpp = std::chrono::high_resolution_clock::now();
    for (std::string &str : arr)
    {
        if (--i <= 0)
        {
            break;
        }
        cpp_map.erase(str);
    }
    end_cpp = std::chrono::high_resolution_clock::now();
    cpp_duration = end_cpp - start_cpp;
    std::cout << c_duration.count() << ",";
    std::cout << cpp_duration.count() << std::endl;

    // Dealloca le risorse
    map_destroy(&c_map);

    return 0;
}
