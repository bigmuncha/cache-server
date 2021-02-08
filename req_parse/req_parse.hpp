#ifndef __REQ_PARSE_H_
#define __REQ_PARSE_H_
#include <string>
#include <boost/regex.hpp>

#include "../hash_table/hash_table.hpp"
#include <unordered_map>

std::vector<std::string> request_parse(std::string);
std::string result_message(std::unordered_map<std::string, std::string>&,std::vector<std::string>&);

#endif // __REQ_PARSE_H_
