#include <iostream>

class CacheCounter{
    private:
        __uint64_t total_access;
        __uint64_t total_hit;
        __uint64_t high_access_bits;
        __uint64_t high_hit_bits;
    public:
        CacheCounter(){
            total_access = 0;
            total_hit = 0;
        }
        void access(bool hit = 0){
            total_access++;
            if(hit){
                total_hit++;
            }
        }
        void print(){
            std::cout << "Total Access: " << total_access << std::endl;
            std::cout << "Total Hit: " << total_hit << std::endl;
            std::cout << "Hit Rate: " << (double)total_hit / total_access << std::endl;
        }
        __uint64_t get_total_access(){
            return total_access;
        };
        __uint64_t get_total_hit(){
            return total_hit;
        };
        void reset(){
            total_access = 0;
            total_hit = 0;
        };
};