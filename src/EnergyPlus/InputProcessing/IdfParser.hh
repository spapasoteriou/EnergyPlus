// EnergyPlus, Copyright (c) 1996-2021, The Board of Trustees of the University of Illinois,
// The Regents of the University of California, through Lawrence Berkeley National Laboratory
// (subject to receipt of any required approvals from the U.S. Dept. of Energy), Oak Ridge
// National Laboratory, managed by UT-Battelle, Alliance for Sustainable Energy, LLC, and other
// contributors. All rights reserved.
//
// NOTICE: This Software was developed under funding from the U.S. Department of Energy and the
// U.S. Government consequently retains certain rights. As such, the U.S. Government has been
// granted for itself and others acting on its behalf a paid-up, nonexclusive, irrevocable,
// worldwide license in the Software to reproduce, distribute copies to the public, prepare
// derivative works, and perform publicly and display publicly, and to permit others to do so.
//
// Redistribution and use in source and binary forms, with or without modification, are permitted
// provided that the following conditions are met:
//
// (1) Redistributions of source code must retain the above copyright notice, this list of
//     conditions and the following disclaimer.
//
// (2) Redistributions in binary form must reproduce the above copyright notice, this list of
//     conditions and the following disclaimer in the documentation and/or other materials
//     provided with the distribution.
//
// (3) Neither the name of the University of California, Lawrence Berkeley National Laboratory,
//     the University of Illinois, U.S. Dept. of Energy nor the names of its contributors may be
//     used to endorse or promote products derived from this software without specific prior
//     written permission.
//
// (4) Use of EnergyPlus(TM) Name. If Licensee (i) distributes the software in stand-alone form
//     without changes from the version obtained under this License, or (ii) Licensee makes a
//     reference solely to the software portion of its product, Licensee must refer to the
//     software as "EnergyPlus version X" software, where "X" is the version number Licensee
//     obtained under this License and may not use a different name for the software. Except as
//     specifically required in this Section (4), Licensee shall not use in a company name, a
//     product name, in advertising, publicity, or other promotional activities any name, trade
//     name, trademark, logo, or other designation of "EnergyPlus", "E+", "e+" or confusingly
//     similar designation, without the U.S. Department of Energy's prior written consent.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND ANY EXPRESS OR
// IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY
// AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR
// CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
// CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
// SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR
// OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
// POSSIBILITY OF SUCH DAMAGE.

#ifndef InputProcessing_IdfParser_hh_INCLUDED
#define InputProcessing_IdfParser_hh_INCLUDED

#include <string>

#include <nlohmann/json.hpp>
#include <unordered_map>

namespace EnergyPlus {
class InputProcessorFixture;
}

class IdfParser
{
public:
    friend class EnergyPlus::InputProcessorFixture;
    using json = nlohmann::json;

    IdfParser() = default;

    json decode(std::string const &idf, json const &schema);

    json decode(std::string const &idf, json const &schema, bool &success, bool const convertHVACTemplate=false);

    std::string encode(json const &root, json const &schema);

    std::string normalizeObjectType(std::string const &objectType);

    std::vector<std::string> const &errors();

    std::vector<std::string> const &warnings();

    bool hasErrors();

    enum class Token : size_t
    {
        NONE = 0,
        END = 1,
        EXCLAMATION = 2,
        COMMA = 3,
        SEMICOLON = 4,
        STRING = 5,
        NUMBER = 6
    };

private:
    size_t cur_line_num = 1;
    size_t index_into_cur_line = 0;
    size_t beginning_of_line_index = 0;
    char s[129];
    std::unordered_map<std::string, std::string> objectTypeMap;
    std::vector<std::string> errors_;
    std::vector<std::string> warnings_;

    void increment_both_index(size_t &index, size_t &line_index);

    void decrement_both_index(size_t &index, size_t &line_index);

    json parse_idf(std::string const &idf, size_t &index, bool &success, json const &schema, bool const convertHVACTemplate = false);

    json parse_object(std::string const &idf, size_t &index, bool &success, json const &schema_loc, json const &obj_loc, int idfObjectCount);

    json parse_value(std::string const &idf, size_t &index, bool &success, json const &field_loc);

    json parse_number(std::string const &idf, size_t &index, bool &success);

    std::string parse_string(std::string const &idf, size_t &index, bool &success);

    void eat_whitespace(std::string const &idf, size_t &index);

    void eat_comment(std::string const &idf, size_t &index);

    Token look_ahead(std::string const &idf, size_t index);

    Token next_token(std::string const &idf, size_t &index);

    std::string &rtrim(std::string &s);

    inline std::string convertToUpper(std::string s)
    {
        size_t len = s.size();
        for (size_t i = 0; i < len; ++i) {
            char c = s[i];
            s[i] = ('a' <= c && c <= 'z') ? c ^ 0x20 : c; // ASCII only, which is fine
        }
        return s;
    }
};

#endif // InputProcessing_IdfParser_hh_INCLUDED
