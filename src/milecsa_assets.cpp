//
// Created by denn on 27/10/2018.
//

#include "milecsa_assets.hpp"

milecsa::token::token() {}

milecsa::token::token(const milecsa::token &t):
        code(t.code), precision(t.precision), name(t.name) {}

milecsa::token::token(unsigned short code, unsigned short precision, const std::string &name) :
        code(code), precision(precision), name(name)  {}

milecsa::token milecsa::assets::XDR  = {0,2,std::string("XDR tokens")};
milecsa::token milecsa::assets::MILE = {1,5,std::string("Mile tokens")};
milecsa::token milecsa::assets::NILL = {(unsigned short)-1,0,std::string("Nothing tokens")};
