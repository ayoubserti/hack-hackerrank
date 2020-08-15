/*
    pointer distance implemntation of Doubly linked list
    an Abstract data type implementation
*/
#include <iostream> 

template<typename T>
class double_list
{
    

    struct node{
        T            value_;
        node* ptrdiff_;
        node()
        :ptrdiff_{nullptr}
        {}

        node* next( node* prev ){
            return (node*)((uint64_t)prev ^ (uint64_t)ptrdiff_);
        }

        bool hasNext( node* prev){
            return  prev != ptrdiff_;
        }

        node* prev( node* next ){
            return (node*)((uint64_t)next ^ (uint64_t)ptrdiff_);
        }

        bool hasPrev( node* next){
             return  next!=ptrdiff_;
        }

        void delete_all(node* prev){
            if( !hasNext(prev)){
                if ( prev != nullptr)
                    prev->ptrdiff_ = (node*)((uint64_t)(prev->ptrdiff_) ^ (uint64_t)this); //recalculate the ptr distance of the previous
                delete this;
            }
            else
            {
               next(prev)->delete_all( this);
            }
        }

        void insert_after(node* elem)
        {
            // this is a tail
            ptrdiff_ = elem; // xor with nullptr
            elem->ptrdiff_= (node*)((uint64_t)(elem->ptrdiff_) ^(uint64_t)this);
        }

        void delete_side(node* elem, bool next){
             
        }

        
    };

    node*   head_;
    node*   tail_;
    size_t  size_;

public:

//ctor
    double_list()
    {
        head_ = tail_ = nullptr;
        size_ = 0;
    }

//Element accessor
    T front() const{
        return head_->value_;
    }

    T back() const {
        return tail_->value_;
    }

//capacity
    bool empty() const {
        return head_ == nullptr;
    }

    size_t size() const{
        return size_;
    }

//modifiers
    void clear(){
        if ( head_ != nullptr){
            head_->delete_all(nullptr);
            delete head_;
        }
        head_ = nullptr;
        tail_ = nullptr;
        size_ = 0;
    }

    void insert( T elem){
        //delayed
    }

    void push_back( T elem)
    {
        //insert after tail
        if ( head_ == nullptr)
        {
            //first element
            tail_ = head_ = new node();
            head_->value_ = elem;
            head_->ptrdiff_ = nullptr; // 0 ^0
        }
        else if ( size_ == 1)
        {
            tail_ = new node();
            tail_->value_ = elem;
            head_->ptrdiff_ = tail_;
            tail_->ptrdiff_ = head_;
        }
        else
        {
            node* e = new node();
            e->value_ = elem;
            tail_->insert_after(e);
            tail_ = e;
        }
        size_++;
        
    }

    void pop_back( ){
        //remove from back
        if ( tail_ != nullptr)
        {
            node* new_tail = tail_->prev(nullptr);
            new_tail->ptrdiff_= (node*)((uint64_t)new_tail->ptrdiff_ ^ (uint64_t)tail_);
            delete tail_;
            tail_ = new_tail;
            size_--;
        }
    }

    void push_front( T elem){

        //insert before head
        if ( head_ == nullptr)
        {
            //first element
            tail_ = head_ = new node();
            head_->value_ = elem;
            head_->ptrdiff_ = nullptr; // 0 ^0
        }
        else if ( size_ == 1)
        {
            head_ = new node();
            head_->value_ = elem;
            head_->ptrdiff_ = tail_;
            tail_->ptrdiff_ = head_;
        }
        else
        {
            node* e = new node();
            e->value_ = elem;
            e->insert_after(head_);
            head_= e;
        }
        size_++;
    }

    void pop_front(){
        //remove from front
        if ( head_ != nullptr)
        {
            node* new_head = head_->next(nullptr);
            new_head->ptrdiff_=(node*)((uint64_t)new_head->ptrdiff_ ^ (uint64_t)head_) ;
            delete head_;
            head_ = new_head;
            size_--;
        }
    }

    void print()
    {
        node* tmp = nullptr,
              *n = head_;

       
        while(n != nullptr)
        {
            std::cout << n->value_  << " ";
            node* t = n;
            n = n->next(tmp);
            tmp = t;
        }
        std::cout << std::endl;
    }

     class iterator
        {
            public:
                typedef iterator self_type;
                typedef T value_type;
                typedef T& reference;
                typedef T* pointer;
                typedef std::forward_iterator_tag iterator_category;
                typedef int difference_type;
                iterator(double_list<T>::node*  ptr) : ptr_(ptr), prev_(nullptr) { }
                self_type operator++() { 
                    self_type i = *this;
                    //ptr_++;
                    double_list<T>::node* tmp = ptr_;
                    ptr_ = ptr_->next(prev_);
                    prev_ = tmp;
                    return i; }
                self_type operator++(int junk) { 
                    double_list<T>::node* tmp = ptr_;
                    ptr_ = ptr_->next(prev_);
                    prev_ = tmp;
                     return *this; }
                reference operator*() { return ptr_->value_; }
                pointer operator->() { return &(ptr_->value_); }
                bool operator==(const self_type& rhs) { return ptr_ == rhs.ptr_; }
                bool operator!=(const self_type& rhs) { return ptr_ != rhs.ptr_; }
            private:
                double_list<T>::node* ptr_;
                double_list<T>::node* prev_;
        };

    iterator begin(){
        return iterator(head_);
    }

    iterator end()
    {
        return iterator(nullptr); // not correct
    }

};


using namespace std;

int main(int, char**){

    double_list<int> ll;
    ll.push_back(100);
    ll.push_back(200);

    ll.push_front(8765);
    
    ll.pop_front();
    ll.print();
    

    ll.push_front(865);
    ll.push_front(10987);
    ll.pop_back();
    cout << ll.size() << endl;
    ll.print();
    
    for ( auto&& it : ll )
    {
        cout << it << " ";   
    }
    cout << endl;
    return EXIT_SUCCESS;
}