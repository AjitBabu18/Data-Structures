#include <string>
#include <vector>
#include <cstdint>
#include <iostream>

#include "random.h"

using namespace std;

// If the condition is not true, report an error and halt.
#define EXPECT(condition, message) do { if (!(condition)) expect_failed(message); } while (0)

void expect_failed(const string& message);

class TabulationHash {
    /*
     * Hash function for hashing by tabulation.
     *
     * The 32-bit key is split to four 8-bit parts. Each part indexes
     * a separate table of 256 randomly generated values. Obtained values
     * are XORed together.
     */

    unsigned num_buckets;
    uint32_t tables[4][256];

public:
    TabulationHash(unsigned num_buckets, RandomGen *random_gen)
    {
      this->num_buckets = num_buckets;
      for (int i=0; i<4; i++)
          for (int j=0; j<256; j++)
              tables[i][j] = random_gen->next_u32();
    }

    uint32_t hash(uint32_t key)
    {
        unsigned h0 = key & 0xff;
        unsigned h1 = (key >> 8) & 0xff;
        unsigned h2 = (key >> 16) & 0xff;
        unsigned h3 = (key >> 24) & 0xff;
        return (tables[0][h0] ^ tables[1][h1] ^ tables[2][h2] ^ tables[3][h3]) % num_buckets;
    }
};

class CuckooTable {
    /*
     * Hash table with Cuckoo hashing.
     *
     * We have two hash functions, which map 32-bit keys to buckets of a common
     * hash table. Unused buckets contain 0xffffffff.
     */

    const uint32_t UNUSED = 0xffffffff;

    // The array of buckets
    vector<uint32_t> table;
    unsigned num_buckets;

    // Hash functions and the random generator used to create them
    TabulationHash *hashes[2];
    RandomGen *random_gen;

public:

    CuckooTable(unsigned num_buckets)
    {
        // Initialize the table with the given number of buckets.
        // The number of buckets is expected to stay constant.

        this->num_buckets = num_buckets;
        table.resize(num_buckets, UNUSED);

        // Obtain two fresh hash functions.
        random_gen = new RandomGen(42);
        for (int i=0; i<2; i++)
            hashes[i] = new TabulationHash(num_buckets, random_gen);
    }

    ~CuckooTable()
    {
        for (int i=0; i<2; i++)
            delete hashes[i];
        delete random_gen;
    }

    bool lookup(uint32_t key)
    {
        // Check if the table contains the given key. Returns True or False.
        unsigned h0 = hashes[0]->hash(key);
        unsigned h1 = hashes[1]->hash(key);
        return (table[h0] == key || table[h1] == key);
    }

    void _hash() {
        vector<uint32_t> x = std::move(table);

        for (;;) {
            bool end = true;
            table.clear();
            table.resize(num_buckets, UNUSED);

            for (int i=0; i<2; i++) {
                delete hashes[i];
                hashes[i] = nullptr; // just in case of an exception
                hashes[i] = new TabulationHash(num_buckets, random_gen);
            }

            for (auto &&value : x) {
                if (value != UNUSED)
                    if (_insert(value) != UNUSED) {
                        end = false;
                        break;
                    }
            }

            if (end)
                break;
        }
    }
    
    uint32_t _insert(uint32_t key) {
        static auto log = [](uint32_t x){
            uint32_t output = 0;
            while (x /= 2)
                output++;
            return output;
        };

        uint32_t hash0 = hashes[0]->hash(key);
        uint32_t hash1 = hashes[1]->hash(key);

        if (table[hash0] == key || table[hash1] == key)
            return UNUSED;

        if (table[hash0] == UNUSED || table[hash1] == UNUSED) {
            table[(table[hash0] == UNUSED) ? hash0 : hash1] = key;
            return UNUSED;
        }

        std::swap(table[hash0], key);

        uint32_t counter = 6 * log(num_buckets);
        uint32_t old_hash = hash0;

        while (--counter > 0 && key != UNUSED) {
            uint32_t h = hashes[0]->hash(key);

            if (h == old_hash)
                h = hashes[1]->hash(key);

            std::swap(table[h], key);
            old_hash = h;
        }

        return key;
    }

    void insert(uint32_t key)
    {
        // Insert a new key to the table. Assumes that the key is not present yet.
        EXPECT(key != UNUSED, "Keys must differ from UNUSED.");

        // TODO: Implement

        while ((key = _insert(key)) != UNUSED)
            _hash();
    }

};