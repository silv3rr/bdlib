/*
 * Copyright (c) 2006-2014, Bryan Drewery <bryan@shatow.net>
 * All rights reserved.
 * 
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTIRBUTORS ''AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
 * WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE FOR ANY
 * DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
 * (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 * LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
 * ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
 * SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/* HashTable.h
 */
#ifndef _BD_HASHTABLE_H
#define _BD_HASHTABLE_H 1

#include <unordered_map>
#include <vector>
#include "bdlib.h"
BDLIB_NS_BEGIN


/**
  * @class HashTable
  * @brief HashTable data structure
  * @todo replace()
  * @todo iterators
  * @todo resizing/growing
  *
  */
template <class Key, class Value>
class HashTable {
  private:
    typedef typename std::unordered_map<Key, Value> map_type;
  public:
    typedef typename map_type::value_type		value_type;
    typedef typename map_type::iterator			iterator;
    typedef typename map_type::const_iterator		const_iterator;
  private:
    static const size_t default_list_size = 100;
    typedef void (*hash_table_block)(const Key, Value, void *param);
    const Value empty_value{};

    map_type map;

  public:
    HashTable() : map(default_list_size) {} ;
    explicit HashTable(size_t capacity_in) : map(capacity_in) {};
    HashTable(const HashTable<Key, Value>& table) : map(table.map) {}
    HashTable(HashTable<Key, Value>&& table) : map(std::move(table.map)) {
      table.map.clear();
    }
    HashTable(std::initializer_list<value_type> list) : map(list) {}

    ~HashTable() {}

    inline iterator begin() { return map.begin(); };
    inline const_iterator cbegin() const { return map.cbegin(); };
    inline const_iterator begin() const { return this->cbegin(); };
    inline iterator end() { return map.end(); };
    inline const_iterator cend() const { return map.cend(); };
    inline const_iterator end() const { return this->cend(); };
    inline iterator find(const Key& key) {
      return map.find(key);
    }
    inline const_iterator find(const Key& key) const {
      return map.find(key);
    }

    inline void clear() {
      map.clear();
    }

    /**
     * @brief A ruby style block which will yield to the passed callback for each Key/Value pair.
     * @param block The block to execute for each element
     * @param param An optional parameter to pass to the block.
     * @return How many iterations were called
     */
    int each(hash_table_block block, void* param = nullptr) {
      int n = 0;
      if (!size()) return n;

      // Make a list of KeyValues to yield from.
      // Don't yield in this loop as the block may actually modify (this), thus making this iterator stale
      std::vector<value_type> items;
      items.reserve(size());
      for (const auto& item : map) {
        items.push_back(item);
      }

      // Now yield on our temporary, so (this) isn't a factor.
      for (const auto& item : items) {
        ++n;
        block(item.first, item.second, param);
      }
      return n;
    }

    friend void swap(HashTable<Key, Value>& a, HashTable<Key, Value>& b) {
      using std::swap;

      swap(a.map, b.map);
    }

    HashTable& operator=(HashTable<Key, Value> table) {
      swap(*this, table);
      return *this; 
    }

    /**
     * @brief Create an array from an initializer list
     * @param list An initializer_list
     */
    HashTable& operator=(std::initializer_list<value_type> list) {
      for (const auto& item : list) {
        (*this)[item.key()] = item.value();
      }
      return *this;
    }

    inline size_t size() const __attribute__((pure)) {
      return map.size();
    }
    inline size_t capacity() const __attribute__((pure)) {
      return map.max_size();
    }
    inline bool isEmpty() const __attribute__((pure)) {
      return map.empty();
    }
    inline explicit operator bool() const __attribute__((pure)) {
      return !isEmpty();
    }

    inline bool insert(const Key& key, const Value& value) {
      if (contains(key)) return false;
      map[key] = value;
      return true;
    }

    inline bool insert(Key&& key, Value&& value) {
      if (contains(key)) return false;
      map[std::move(key)] = std::move(value);
      return true;
    }

    inline bool contains(const Key& key) const __attribute__((pure)) {
      if (isEmpty()) return false;
      return map.find(key) != std::end(map);
    };

    inline bool remove(const Key& key) {
      if (isEmpty()) return false;
      return map.erase(key) > 0 ? true : false;
    };

    inline Value getValue(const Key& key) const __attribute__((pure)) {
      auto result = map.find(key);
      if (result == std::end(map))
        return Value();
      return result->second;
    };

    /**
      * @brief Associate array type accessor (rvalue)
      * @param key The key to search for
      */
    inline const Value operator[](const Key& key) const __attribute__((pure)) {
      return getValue(key);
    }

    /**
      * @brief Associate array type accessor (lvalue)
      * @param key The key to search for
      * @sa find_or_insert_key 
      * If the key is not in the table, it is inserted, and the value set to the rvalue given.
      */
    Value& operator[](const Key& key) {
      return map[key];
    }
    Value& operator[](Key&& key) {
      return map[std::move(key)];
    }
};

BDLIB_NS_END

#endif /* _BD_HASHTABLE_H */ 
/* vim: set sts=2 sw=2 ts=8 et: */
