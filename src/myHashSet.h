#ifndef MY_HASH_SET_H
#define MY_HASH_SET_H

#include <vector>
#include <iostream>
#include <string>
#include <cassert>
#include <sstream>

using namespace std;

//---------------------
// Define Hash class
//---------------------
// To use Hash ADT,
// the class "Data" should at least overload the "()" and "==" operators.
//
// "operator ()" is to generate the hash key (size_t)
// that will be % by _numBuckets to get the bucket number.
// ==> See "bucketNum()"
//
// "operator ==" is to check whether there has already been
// an equivalent "Data" object in the Hash.
// Note that Hash does not allow equivalent nodes to be inserted
//
class Hash
{
public:
   Hash(size_t b = 0) : _numBuckets(0), _buckets(NULL) { if (b != 0) init(b); }
   ~Hash() { reset(); }

   void init(size_t b) { _numBuckets = b; _buckets = new vector<string>[b]; }

   void reset() {
      _numBuckets = 0;
      if (_buckets) { delete [] _buckets; _buckets = NULL; }
   }

   void clear() {
      for (size_t i = 0; i < _numBuckets; ++i) _buckets[i].clear();
   }

   size_t numBuckets() const { return _numBuckets; }

   vector<string>& operator [] (size_t i) { return _buckets[i]; }
   const vector<string>& operator [](size_t i) const { return _buckets[i]; }

   // check if d is in the hash...
   // if yes, return true;
   // else return false;
   bool check(const string& d) const {
     size_t num = this->get_key(d);
     for (size_t i = 0; i < _buckets[num].size(); ++i) {
       if (d == _buckets[num][i])
        return true;
     }
     return false;
   }

   // return true if inserted successfully (i.e. d is not in the hash)
   // return false is d is already in the hash ==> will not insert
   bool insert(const string& d) {
     if (check(d)) return false;
     _buckets[this->get_key(d)].push_back(d);
     return true;
   }

   void debbug() const {
     for (size_t i = 0; i < _numBuckets; ++i) {
       if (_buckets[i].size()) cout << "_buckets[" << i << "] size: ";
       if (_buckets[i].size()) cout << _buckets[i].size() << endl;
     }
   }

private:
   size_t            _numBuckets;
   vector<string>*   _buckets;

	inline int get_key(const string& pat) const {
	    assert(pat.length() < 64);
	    size_t value;
	    stringstream ss(pat);
	    ss >> value;
	    return value % _numBuckets;
	    int key = 0;
	    for (int i = 0; i < (int)pat.size(); ++i) {
            if (pat[i] == '1') ++key;
        }
        return key;
    }
};

#endif // MY_HASH_SET_H
