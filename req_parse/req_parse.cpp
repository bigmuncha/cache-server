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

std::string result_message(node **table, std::vector<std::string>& vec ){

    std::string result;
    if(vec[0] == "get"){
        result = get(table,vec[1].c_str());
    }else if(vec[0] == "set"){
        if(set(table,vec[1].c_str(),vec[2].c_str()) == 0){
            result = "Error, can not set this key\n";
        }else{
            result = "OK, key is a set\n";
        }
    }
    return result;

}
