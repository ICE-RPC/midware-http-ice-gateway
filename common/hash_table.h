
#include "libcuckoo/cuckoohash_map.hh"
#include "libcuckoo/city_hasher.hh"

namespace utils {

template<typename KEY, typename VALUE>
class SeqHashTable {
public:
    
    bool insert(const KEY& key, const VALUE& val) {
        return hash_table_.insert_or_assign(key, val);
    }
    bool find(const KEY& key, VALUE& val) {
        return hash_table_.find(key, val);
    }
    bool contains(const KEY& key) {
        return hash_table_.contains(key);
    }
    bool update(const KEY& key, VALUE& val) {
        return hash_table_.update(key, val);
    }
    bool erase(const KEY& key) {
        int res = hash_table_.erase(key);
        return res==1?true:false;
    }
    void clear() {
        hash_table_.clear();
    }
    std::size_t size() {
        return hash_table_.size();
    }

private:
    cuckoohash_map<KEY, VALUE, CityHasher<KEY>> hash_table_;
};

}

