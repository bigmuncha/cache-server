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

std::string result_message(std::unordered_map<std::string, std::string>& table,
                           std::vector<std::string>& vec ){
    std::string result;
    if(vec[0] == "get"){
        if(vec.size()!=2){
            result = "ERROR WRONG REQUEST\n";
        }else if (table.find(vec[1]) == table.end() ){
            result =  "THIS KEY NOT VALID\n";
        }else{
            result =  table[vec[1]];
        }
    }else if(vec[0] == "set"){
        if(vec.size() != 3){
            result = "ERROR WRONG REQUEST\n";
        }else if(table.find(vec[1]) == table.end()){
            table[vec[1]] = vec[2];
            result =  "KEY IS SET\n";
        }else{
            result =  "CANNOT SET THIS KEY\n";
        }
    }else if(vec[0] == "all"){
        for(auto &a:table){
            result = result + a.second + ' ';
        }
    }else {
        result =  "ERROR WRONG REQUEST\n";
    }
    return result;
}
