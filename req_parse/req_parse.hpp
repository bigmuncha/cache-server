#ifndef __REQ_PARSE_H_
#define __REQ_PARSE_H_
#include <string>
#include <boost/regex.hpp>

#include "../hash_table/hash_table.hpp"


std::vector<std::string> request_parse(std::string);
char* result_message(node **,std::vector<std::string>&);

#endif // __REQ_PARSE_H_
