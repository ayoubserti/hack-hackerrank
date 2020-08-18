/*
    binary tree 
    T value type
    C comparator function
    defaulty it's a binary search tree
    the compare function object allow to change the way key are stored in the tree
*/
#include <iostream>
#include <functional>

template<class T ,  class Compare= std::less<T> >
struct Btree
{
    T value_;
    Btree* left_;
    Btree* right_;
    Compare comparator_;      
    public:
    Btree(const T& val): value_{val}, left_{nullptr}, right_{nullptr}{}

    Btree* insert( const T& elem){
        
        Btree* node = nullptr;
        if ( comparator_(elem,value_))
        {
            if ( left_ == nullptr) 
                node =left_ = new Btree(elem);
            else
                node = left_->insert(elem);
        }
        else
        {
            if ( right_ == nullptr)
               node = right_ = new Btree(elem);
            else
                node = right_->insert(elem);
        } 
        return node;
    }
    
    void preorder(std::function<void(const T&)>&& traversal ){
        //NLR
        traversal(value_);
        
        if ( left_ != nullptr)
            left_->preorder(std::move(traversal));

        if ( right_ != nullptr)
            right_->preorder(std::move(traversal));
    }

    void inorder(std::function<void(const T&)>&& traversal ){
        //LNR
        if ( left_ != nullptr)
            left_->inorder(std::move(traversal));
        
        traversal(value_);

        if ( right_ != nullptr)
            right_->inorder(std::move(traversal));
    }

    void postorder(std::function<void(const T&)>&& traversal ){
        //LRN
        if ( left_ != nullptr)
            left_->postorder(std::move(traversal));

        if ( right_ != nullptr)
            right_->postorder(std::move(traversal));
        
        traversal(value_);
    }

};


template<class T>
class EvenOdd
{
    public:
    bool operator()(const T& lhs, const T& rhs ){

        if ( rhs % 2 == 0)
            return true;
        return false;
    }
};

int main( int , char**)
{
    Btree<int> root(20);

    Btree<int>* e1 = root.insert(10);
    Btree<int>* e2 = root.insert(30);
    e1->insert(109)->insert(765);
    e2->insert(5)->insert(987);
    e1->insert(76);
    e2->insert(0)->insert(3);

    root.inorder([](const int& v){
        std::cout << v << " ";
    });
    std::cout << std::endl;

    root.preorder([](const int& v){
        std::cout << v << " ";
    });
    std::cout << std::endl;

    root.postorder([](const int& v){
        std::cout << v << " ";
    });
    std::cout << std::endl;



    // even - odd tree
    {
        Btree<int ,EvenOdd<int>> root1(0);
        root1.insert(76);
        root1.insert(875);
        root1.insert(1);
        root1.insert(5);
        root1.insert(6);

        root1.preorder([](const int& v){
            std::cout << v << " ";
        });
        std::cout << std::endl;

    }
    


    return EXIT_SUCCESS;
}