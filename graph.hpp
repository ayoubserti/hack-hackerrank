/*
    graph implementation
    Directed graph
*/

#include <vector>
#include <functional>
#include <memory>
#include <queue>

template<class T >
class Graph
{
    public:
    struct Node
    {
        T   value_;
        std::vector<std::shared_ptr<Node>> adjacent_;
        bool  visited_;
        Node(const T& val):value_{val}, visited_{false} {}

        std::shared_ptr<Node> add(const T& e) {
            std::shared_ptr<Node> elem(new Node(e));
            adjacent_.push_back(elem);
            return elem;
        }

        virtual ~Node() = default;
    };
protected:
    std::shared_ptr<Node> root_;
public:

    Graph( const T&  r ){ 
        root_.reset(new Node(r));
    }

    std::shared_ptr<Node> getRoot() { return root_;}

    void DFS( std::shared_ptr<Node> root,std::function<void(std::shared_ptr<Node>)>&& visitor){
        if ( root == nullptr ) return;
        visitor(root);
        root->visited_ = true;
        for ( auto&& it : root->adjacent_)
        {
            if( it->visited_ == false){
                DFS(it, std::move(visitor));
            }
        }
    }

    void unvisitAll() {
        _unvisit(root_);
    }


    void BFS( std::shared_ptr<Node> root,std::function<void(std::shared_ptr<Node>)>&& visitor){
        std::queue<std::shared_ptr<Node> > Q;
        root->visited_ = true;
        Q.push(root);
        while (!Q.empty()){
            auto elem = Q.front();
            Q.pop();
            visitor(elem);
            for ( auto&& it : elem->adjacent_){
                if ( it->visited_ == false){
                    it->visited_ = true;
                    Q.push(it);
                }
            }
        }
    }

    virtual ~Graph() = default;


    private:
    void _unvisit( std::shared_ptr<Node> node){
        node->visited_ = false;
        for( auto&& it : node->adjacent_)
            if (it->visited_ == true)
                _unvisit(it);
    }
};