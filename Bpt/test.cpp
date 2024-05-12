#include <iostream>
#include <fstream>
class Base {
public:
};

class Derived : public Base {
public:
    int derivedVar = 10;

};
std::fstream s("233");
int main() {
    Base* basePtr = new Derived();
    s.open("233", std::ios::out);
    s.close();

    s.open("233", std::ios::in | std::ios::out | std::ios::binary | std::ios::trunc);
    // 使用 static_cast 进行向下转型
    Derived* derivedPtr = new Derived();
    derivedPtr->derivedVar = 11;
    
    Base* baseP = new Derived(); 
    static_cast<Derived*>(baseP)->derivedVar = 233;
    std::cerr << derivedPtr->derivedVar << std::endl;
    s.seekp(0);
    s.write(reinterpret_cast<const char *>(baseP), sizeof(Derived));
    static_cast<Derived*>(baseP)->derivedVar = 234;
    if(!s.good()) {
        std::cout << 233;
        throw "fuck";
    }
    // 访问派生类特有的变量

    s.seekg(0);
    s.read(reinterpret_cast<char*>(baseP), sizeof(Derived));
    if(!s.good()) {
        throw "fuck";
    }
    std::cerr << static_cast<Derived*>(baseP)->derivedVar << std::endl;

    s.close();
    return 0;
}