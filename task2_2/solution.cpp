#include <iostream>

class StringPointer {
private:
    std::string *s;
public:
    std::string *operator->() 
    {
        //if (s == NULL) s = new std::string();
        return s;
    }
    std::string& operator*() 
    {   
        //if (s == NULL) s = new std::string();
        return *s;
    }
    StringPointer(std::string *pointer)
    {
        s = (pointer == NULL) ? new std::string() : pointer;
    }
    ~StringPointer() 
    {
        //delete s;
    }
};

int main()
{
    std::string s1 = "Hello, world!";

    StringPointer sp1(&s1);
    StringPointer sp2(NULL);

    std::cout << sp1->length() << std::endl;
    std::cout << *sp1 << std::endl;
    std::cout << sp2->length() << std::endl;
    std::cout << *sp2 << std::endl;
    
    return 0;
}