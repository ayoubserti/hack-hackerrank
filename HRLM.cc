#include <cmath>
#include <cstdio>
#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <string>
#include <map>
#include <memory>
#include <assert.h>
using namespace std;

class Tag : public enable_shared_from_this<Tag>{
    string name_;
    map<string,string> attribute_value_;
    vector<shared_ptr<Tag> > inner_tags_;
    shared_ptr<Tag>     parent_;

    enum StateMachine
    {
        eBegin,
        eTagName,
        eAttributeName,
        eEqualSymbol,
        eEndTagAtts,
        eTagAttValue,
        eBeginTagEnding,
        eEndTag
    };

public:
    Tag(const string& name, shared_ptr<Tag> parent):name_(""),parent_(parent){}


    void read( istream& instream ){
        _read(instream,eBegin);
    }


    string get_value( const string& path) 
    {
        string result= "Not Found!";
        int index = path.find(".");
        if ( index == -1)
        {
            string tag = path.substr(0, path.find('~'));
            string attribute = path.substr(path.find('~')+1);
            if ( tag == name_)
            {
                result = attribute_value_[attribute];        
            }
           
            if (result == "") result = "Not Found!";
        }
        else {
            int ind1;

            string tag = path.substr(index+1);
            ind1 = tag.find(".");
            if ( ind1 == -1)
            {
                ind1= tag.find("~");
                tag = tag.substr(0,ind1);
            }
            else
            {
                tag = path.substr(0,ind1);
            }
            
            auto it = std::find_if(inner_tags_.begin(),inner_tags_.end(),[tag]( shared_ptr<Tag> const &child){
                return child->name_ == tag;
            });
            if(it != inner_tags_.end()){
               result=  (*it)->get_value(path.substr(index+1));
            }
        }
        return result;
    }

private:

    /**
     * _read function read from a stream starting from a state 
    */
    void _read( istream& instream , StateMachine  inState){
        StateMachine state = inState;
        string att ="";
        bool reading = true;
        string tmp;
        
        while(!instream.eof() )
        {
            if ( reading)
                instream >> tmp;
            reading = true;

            if ( state == eBegin )
            {
                // it's the begining, obviously the first character must be '<'
                if ( tmp[0] == '<'){
                    name_ = tmp.substr(1);
                    state = eTagName;
                }
                else
                {
                    //some case the < was already read
                    name_ = tmp;
                    state = eTagName;
                }
            }
           else if (state == eTagName)
            {
                long ind1 = tmp.find("="); 
                if (  ind1 != -1)
                {
                    //'=' found,
                    // there is two case: att= or att="value"
                    att = tmp.substr(0,ind1);
                    if ( tmp.size() ==ind1+1 )
                    {
                        //a space between = and value
                        state = eEqualSymbol;
                    }
                    else {
                        // case att="value"
                        long ind2 = tmp.find("\"");
                        string value = tmp.substr(ind2+1,tmp.find("\"")-ind2);
                        attribute_value_[att] = value;
                        if ( tmp.find(">"))
                        {
                            state= eEndTagAtts;
                        }
                        else
                        {
                            state = eTagAttValue;
                        }        
                    }
                }
                else {
                    att = tmp;
                    state = eAttributeName;
                }
            }
            else if ( state == eAttributeName)
            {
                if ( tmp == "=")
                {
                    state = eEqualSymbol;
                }
                else if ( tmp[0]=='=')
                {
                    
                    string value = tmp.substr(tmp.find("\"")+1);
                    if ( value.find(">") != -1)
                    {
                        attribute_value_[att] = value.substr(0,value.size()-1);
                        state = eTagAttValue;
                    }
                    else
                    {
                        state = eEndTagAtts;
                        tmp = tmp.substr(value.find(">")+1);
                        reading = false;
                        continue;
                    }
                    
                }
            }
            else if ( state == eEqualSymbol)
            {
                 string value = tmp.substr(tmp.find("\"")+1,tmp.size()-3);
                 attribute_value_[att] = value;
                 state = eTagAttValue;
                 if ( tmp.find(">"))
                    state= eEndTagAtts;
            }
            else if ( state == eTagAttValue)
            {
                //maybe another attribute or a > symbol
                if ( tmp[0]== '>'){
                    state = eEndTagAtts;
                }
                else
                {
                    //so it's another attribute value
                    long ind1 = tmp.find("="); 
                    if (  ind1 != -1)
                    {
                        //'=' found,
                        // there is two case: att= or att="value"
                        att = tmp.substr(0,ind1);
                        if ( tmp.size() ==ind1+1 )
                        {
                            //a space between = and value
                            state = eEqualSymbol;
                        }
                        else {
                            // case att="value"
                            long ind2 = tmp.find("\"");
                            string value = tmp.substr(ind2+1,tmp.find("\"")-ind2);
                            attribute_value_[att] = value;
                            if ( tmp.find(">"))
                            {
                                state= eEndTagAtts;
                            }
                            else
                            {
                                state = eTagAttValue;
                            }        
                        }
                    }
                    else {
                        att = tmp;
                        state = eAttributeName;
                    }
                }
                
            }
            else if (state == eEndTagAtts)
            {
                if ( (tmp[0] == '<') && (tmp[1] == '/'))
                {
                    //end of tag
                    assert(name_ == tmp.substr(2,tmp.size()-3));
                    if( tmp.find(">") == -1)
                    {
                        state = eBeginTagEnding;
                    }
                    else {
                        state = eEndTag;
                        break;
                    }
                    
                }else if ( tmp[0] == '<'){
                    //read subTag
                    shared_ptr<Tag> subTag(  new Tag("",shared_from_this()));
                    
                    inner_tags_.push_back(subTag);
                    if ( tmp.size() > 1)
                    {
                      subTag->name_ = tmp.substr(1);   
                      subTag->_read(instream, StateMachine::eTagName);
                    }
                    else
                    {
                        subTag->_read(instream, StateMachine::eBegin);
                    }
                    

                }

            }
            else if ( eEndTag)
            {
                break;
            }
            else if ( eBeginTagEnding)
            {

            }
            else{
                assert( false && " should never come here");
            }
        
        }
    }


    void _read2( istream& instream)
    {
        //example of input
        /*
            <tag1 value="HelloWorld" >
                <tag2 name = "Name1">
                </tag2>
            </tag1>
        */
        

    }

};

int main() {
    /* Enter your code here. Read input from STDIN. Print output to STDOUT */  
    int N, Q;
    cin >> N >> Q;
    shared_ptr<Tag> root(new Tag("",nullptr));
    root->read(cin);
    while(Q>0){
        string path;
        cin >> path;
        cout << root->get_value(path) << endl;
        Q--;
    }
    return 0;
}
