//indexer_v1.hpp

#include <string>
#include <map>
#include <shared_mutex>

class Indexer {
public:    
    int get(std::string const& s) {

      {
        std::shared_lock<std::shared_timed_mutex> lock(mut_);
        auto it = index_.find(s);
        if(it != index_.end()) {
            return it->second;
        }
      }

      {
        std::unique_lock<std::shared_timed_mutex> lock(mut_);
        index_[s] = i;
        ++i;
      }
      return i - 1;
    }

    int size() {
        return index_.size();
    }

private:
    int i = 0;
    std::map<std::string, int> index_;
    std::shared_timed_mutex mut_;
};