/*****************************************************************************
 * VCLib                                                                     *
 * Visual Computing Library                                                  *
 *                                                                           *
 * Copyright(C) 2021-2025                                                    *
 * Visual Computing Lab                                                      *
 * ISTI - Italian National Research Council                                  *
 *                                                                           *
 * All rights reserved.                                                      *
 *                                                                           *
 * This program is free software; you can redistribute it and/or modify      *
 * it under the terms of the Mozilla Public License Version 2.0 as published *
 * by the Mozilla Foundation; either version 2 of the License, or            *
 * (at your option) any later version.                                       *
 *                                                                           *
 * This program is distributed in the hope that it will be useful,           *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of            *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the              *
 * Mozilla Public License Version 2.0                                        *
 * (https://www.mozilla.org/en-US/MPL/2.0/) for more details.                *
 ****************************************************************************/

#ifndef CMD_OPT_PARSER_H
#define CMD_OPT_PARSER_H

#include <iostream>
#include <optional>
#include <stdint.h>
#include <string>
#include <unordered_map>
#include <utility>
#include <vector>

using CmdOption = std::pair<std::string, uint32_t>;

class CmdOptionParser
{
    std::unordered_map<std::string, uint32_t> options;

public:
    CmdOptionParser(std::initializer_list<CmdOption> lst)
    {
        for (const CmdOption& option : lst) {
            if(option.first.find("-") != 0) {
                std::cerr << "Error: attempted to declare option without any '-' in front" << std::endl;
                exit(1);
            }
            options.insert(option);
        }
    };

    /**
     * Parses the options.
     *
     * @returns A map of options with their parameters and the remaining vector
     * (all the arguments that were not options or option parameters)
     */
    std::pair<
        std::unordered_map<std::string, std::vector<std::string>>,
        std::vector<std::string>>
    parseOptions(int argc, char** argv)
    {
        std::vector<std::string>                                  argVec;
        std::unordered_map<std::string, std::vector<std::string>> ret_val;
        for (size_t i = 1; i < argc; i++) {
            argVec.push_back(std::string(argv[i]));
        }
        std::vector<std::string>::iterator it = argVec.begin();
        while (it != argVec.end()) {
            if (it->rfind("-", 0) == 0) {
                auto search_res = options.find(*it);
                if (search_res == options.end()) {
                    std::cerr << "Error: unknown option " << *it << std::endl;
                    exit(1);
                }
                it = argVec.erase(it);
                std::vector<std::string> pars;
                for (size_t i = 0; i < search_res->second; i++) {
                    if (it == argVec.end() || it->rfind('-', 0) == 0) {
                        std::cerr << "Error: missing parameters for option "
                                  << search_res->first << std::endl;
                        exit(1);
                    }
                    pars.push_back(*it);
                    it = argVec.erase(it);
                }
                ret_val.insert(std::pair(search_res->first, pars));
                continue;
            }
            it++;
        }
        return std::pair(ret_val, argVec);
    }
};

#endif
