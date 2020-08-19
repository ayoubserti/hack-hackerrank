#include <iostream>
#include "graph.hpp"

int main(int , char**){

    Graph<int> myGraph(0); //graph begin with node 0
    auto root = myGraph.getRoot();

    std::shared_ptr<Graph<int>::Node> a1 = root->add(1);    
    std::shared_ptr<Graph<int>::Node> a4 = root->add(4);
    auto a3 =  a1->add(3);
    auto a2 =  a3->add(2);
    std::shared_ptr<Graph<int>::Node> a5 = root->add(5);
    a1->adjacent_.push_back(a4);
    a2->adjacent_.push_back(a1);
    a3->adjacent_.push_back(a4);
    

    myGraph.DFS(root,[](auto a){
            std::cout << a->value_ << " ";
    });

    std::cout << std::endl;


    myGraph.unvisitAll();

    myGraph.BFS(root,[](auto a){
            std::cout << a->value_ << " ";
    });

    std::cout << std::endl;


    return EXIT_SUCCESS;
}