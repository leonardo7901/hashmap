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

int main()
{
    int numKeys;
    std::cout << "Inserisci il numero di chiavi da generare: ";
    std::cin >> numKeys;
    // Genera un set di coppie chiave-valore casuali
    std::unordered_map<std::string, std::string> cpp_map;
    map_t c_map = map_create_default();
    std::vector<std::string> arr;

    for (int i = 0; i < numKeys; i++)
    {
        std::string key = generate_random_string(16);
        arr.push_back(key);
    }

    auto start_cpp = std::chrono::high_resolution_clock::now();
    for (std::string &str : arr)
    {
        cpp_map[str] = str;
    }
    auto end_cpp = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> cpp_duration = end_cpp - start_cpp;
    std::cout << "Tempo impiegato per la mappa C++ in inserimento: " << cpp_duration.count() << " secondi" << std::endl;

    auto start_c = std::chrono::high_resolution_clock::now();
    char *c_str;
    for (std::string &str : arr)
    {
        c_str = (char *)str.c_str();
        map_add(&c_map, c_str, c_str);
    }
    auto end_c = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> c_duration = end_c - start_c;
    std::cout << "Tempo impiegato per la mappa C in inserimento: " << c_duration.count() << " secondi" << std::endl;

    // Misura il tempo di iterazione e ricerca per la mappa C
    map_status_t iter_status = init_iterator();
    obj_t *c_element;
    start_c = std::chrono::high_resolution_clock::now();
    while ((c_element = map_iterate(&c_map, &iter_status)) != NULL)
    {
        // Esegui operazioni con gli elementi della mappa C
    }
    end_c = std::chrono::high_resolution_clock::now();
    c_duration = end_c - start_c;

    // Misura il tempo di iterazione e ricerca per la mappa C++
    start_cpp = std::chrono::high_resolution_clock::now();
    for (auto &pair : cpp_map)
    {
        // Esegui operazioni con gli elementi della mappa C++
    }
    end_cpp = std::chrono::high_resolution_clock::now();
    cpp_duration = end_cpp - start_cpp;

    // Stampa i tempi misurati
    std::cout << "Tempo impiegato per la mappa C in ricerca: " << c_duration.count() << " secondi" << std::endl;
    std::cout << "Tempo impiegato per la mappa C++ in ricerca: " << cpp_duration.count() << " secondi" << std::endl;

    // Dealloca le risorse
    map_destroy(&c_map);

    return 0;
}
