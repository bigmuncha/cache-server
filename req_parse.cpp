#include "req_parse.hpp"


std::vector<std::string> request_parse(std::string request){

    boost::regex re("(\\w+)");
    boost::sregex_iterator words_begin = boost::sregex_iterator(
        request.begin(), request.end(), re);
    boost::sregex_iterator words_end = boost::sregex_iterator();

    //boost::smatch sm;
    std::vector<std::string> result;
    for(boost::sregex_iterator i = words_begin;i!=words_end;++i){
        boost::smatch a = *i;
        result.push_back(a.str());
    }
    return result;

}
