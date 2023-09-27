#include <iostream>
#include "dictionary.h"

int main()
{
    dictionary<int, float>* dic = new map<int, float>();

    try {
        dic->get(2);
    }
    //catch (dictionary<int, float>::not_found_exception_type& e) {
    catch (std::remove_pointer<decltype(dic)>::type::not_found_exception_type& e) {
        std::cout << e.get_key();
    }
}
