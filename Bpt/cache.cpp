#include<unordered_map>

template <typename T>
class Ptr {
public:
    Ptr(BufferPoolManager* bpm, page_id_t page_id) : bpm_(bpm), page_id_(page_id) {
        // Check if the page is in the cache
        auto it = cache.find(page_id);
        if (it != cache.end()) {
            // Page is in the cache, increase the reference count
            content = it->second.first;
            ++(it->second.second);
        } else {
            // Page is not in the cache, load it and add it to the cache
            content = static_cast<T*>(bpm_->FetchPage(page_id)->GetData());
            cache[page_id] = {content, 1};
        }
    }

    ~Ptr() {
        // Decrease the reference count, remove the page from the cache if the count is 0
        auto it = cache.find(page_id_);
        if (it != cache.end() && --(it->second.second) == 0) {
            cache.erase(it);
        }
    }

    T* operator->() {
        return content;
    }

private:
    BufferPoolManager* bpm_;
    page_id_t page_id_;
    T* content;

    static std::unordered_map<page_id_t, std::pair<T*, int>> cache;
};

// Initialize the cache
template <typename T>
std::unordered_map<page_id_t, std::pair<T*, int>> Ptr<T>::cache = {};