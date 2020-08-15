#include <iostream>
#include <vector>
#include <map>
#include <string>
#include <algorithm>
#include <set>
#include <cassert>
#include <fstream>
using namespace std;

struct Node{
   Node* next;
   Node* prev;
   int value;
   int key;
   Node(Node* p, Node* n, int k, int val):prev(p),next(n),key(k),value(val){};
   Node(int k, int val):prev(NULL),next(NULL),key(k),value(val){};
};

class Cache{
   
   protected: 
   map<int,Node*> mp; //map the key to the node in the linked list
   int cp;  //capacity
   Node* tail; // double linked list tail pointer
   Node* head; // double linked list head pointer
   virtual void set(int, int) = 0; //set function
   virtual int get(int) = 0; //get function

};

class LRUCache : public Cache{
    
    public:
    LRUCache(int capacity)
    {
        cp = capacity;
        
    }

    //Set/insert the value of the key, if present, 
    //otherwise add the key as the most recently used key. 
    //If the cache has reached its capacity, 
    //it should replace the least recently used key with a new key.
    void set( int key , int value){
        auto it = mp.find(key), end = mp.end();
        if(it == end){
            if( mp.size() >= cp)
            {
                Node* tmp = tail;
                //remove tail from mp
                mp.erase(tmp->key);
                tmp->next=nullptr;
                delete tmp;
            }
            Node* nd = new Node(key,value);
            nd->prev= nullptr;
            nd->next= head;
            head = nd;
            mp[key] = nd;
            if( mp.size() ==1){
                tail=head;
                head->prev = head; //particular case
            }
                
        }
        else
        {
            //key already exist
            //put it in the head
            if ( it->second==head){
                //nothing to do. already in the head
            }
            else
            {
                Node* tmp= head , *tmp2;
                while(tmp->next->key != it->second->key)
                {
                    tmp= tmp->next;
                }
                tmp2 = tmp->next->next;
                if (tmp2 != nullptr)
                    tmp2->prev= tmp; 
                tmp->next = tmp2;
                it->second->prev=nullptr;
                head->prev=it->second;
                it->second->next= head;
                head = it->second;
            }
            head->value=value;
        }        
    }

    int get ( int key){
        auto it = mp.find(key);
        if(it == mp.end()){
            return -1;
        }
        if ( it->second==head){
                //nothing to do. already in the head
        }
        else
        {
            Node* tmp= head , *tmp2;
            while(tmp->next != it->second)
            {
                tmp= tmp->next;
            }
            tmp2 = tmp->next->next;
            if ( tmp2 !=nullptr)
                tmp2->prev= tmp; 
            tmp->next = tmp2;
            it->second->prev=nullptr;
            head->prev=it->second;
            it->second->next= head;
            head = it->second;
        }
        return head->value;
    }
};

int main() {
   int n, capacity,i;
   cin >> n >> capacity;
   LRUCache l(capacity);
   for(i=0;i<n;i++) {
      string command;
      cin >> command;
      if(command == "get") {
         int key;
         cin >> key;
         cout << l.get(key) << endl;
      } 
      else if(command == "set") {
         int key, value;
         cin >> key >> value;
         l.set(key,value);
      }
   }
   return 0;
}
