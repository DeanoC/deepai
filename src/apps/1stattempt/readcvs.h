//
// Created by Dean Calver on 25/03/2016.
//

#pragma once

namespace Util {

    std::vector<std::string> readCSVRow(const std::string &row);

    std::vector<std::vector<std::string>> readCSV(std::istream &in);

}
