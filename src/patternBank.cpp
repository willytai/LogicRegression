#include "patternBank.h"
#include "RandPatGen.h"
#include <cassert>
#include <cmath>
#include <iostream>
#include <cstdlib>

extern Pattern::Generator Gen;
using std::cout;
using std::endl;

namespace LogicRegression
{

   void PatternBank::random_sample() {
     cout << "[PatBank] sampling patterns ..." << endl;
     std::vector<std::string> sample;
     for (int i = 0; i < (int)_patterns.size(); ++i) {
        int unkown = this->count_X(_patterns[i]);
        int expand_num = ( unkown == 0 ? 0 : std::log10( std::pow(2, unkown) ) / std::log10( 1.1 ) );
        cout << "sampling " << expand_num << " patterns with " << unkown << "unkown inputs" << endl;
        for (int j = 0; j < expand_num; ++j) {
           std::string newPat(_patterns[i]);
           for (int bit = 0; bit < (int)newPat.size(); ++bit) {
               if (newPat[bit] != 'X') continue;
               newPat[bit] = ( Gen(2) == 1 ? '1' : '0' );
           }
           sample.push_back(newPat);
        }
     }
     _patterns.swap(sample);
     exit(0);
   }

   void PatternBank::insert(const std::string& pattern) {
    std::vector<std::string>::iterator str_it = _patterns.begin();
      for (; str_it != _patterns.end(); ++str_it) {
         status stat = check_stat(*str_it, pattern);
         // visualize(stat, *str_it, pattern);
         if (stat == IDENTICAL) return;
         if (stat == MERGE)     return merge(str_it, pattern);
      }
      _patterns.push_back(pattern);
   }

   status PatternBank::check_stat(const std::string& base, const std::string& pattern) {
      int length = base.length(); assert(base.length() == pattern.length());
      bool idendital_check = true;
      for (int i = 0; i < length; ++i) {
         const char& base_char = base[i];
         const char& patt_char = pattern[i];
         if ( base_char != patt_char ) idendital_check = false;
         if ( (base_char == '1' && patt_char == '0') || (base_char == '0' && patt_char == '1') ) return NONE;
      }
      if (idendital_check) return IDENTICAL;
      return MERGE;
   }

   void PatternBank::merge(std::vector<std::string>::iterator& str_it, const std::string& pattern) {
      const std::string& base = *str_it;
      std::string merged(pattern.size(), 'X');
      int newKey = 0;
      for (int i = 0; i < (int)pattern.length(); ++i) {
         if (base[i]    != 'X') merged[i] = base[i];
         if (pattern[i] != 'X') merged[i] = pattern[i];
         if (merged[i] == '1') ++newKey;
      }
      *str_it = merged;
   }

   int PatternBank::count_X(const std::string& pat) {
       int count = 0;
       for (int i = 0; i < (int)pat.length(); ++i) {
           if (pat[i] == 'X') ++count;
       }
       return count;
   }

   void PatternBank::visualize(status stat, std::string s1, std::string s2) {
       std::cout << s1 << std::endl << s2 << std::endl;
       if (stat == NONE) {
           std::cout << "NONE" << std::endl;
       }
       else if (stat == IDENTICAL) {
           std::cout << "IDENTICAL" << std::endl;
       }
       else {
           std::cout << "MERGE" << std::endl;
           exit(0);
       }
   }

   void PatternBank::show() const {
      for (int i = 0; i < (int)_patterns.size(); ++i) {
         std::cout << _patterns[i] << std::endl;
      }
      std::cout << _patterns.size() << std::endl;
      exit(0);
   }
}
