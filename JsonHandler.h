#pragma once

#include <vector>
#include <fstream>
#include <sstream>
#include "include/rapidjson/rapidjson.h"
#include "include/rapidjson/document.h"
#include "include/rapidjson/stringbuffer.h"

class JsonHandler
{
public:
  static void GetValues(const rapidjson::Value& array, std::vector<double> vec)
  {
    if (array.IsArray())
    {
      for (rapidjson::SizeType i = 0, n = array.Size(); i < n; ++i)
      {
        vec.push_back(array[i].GetDouble());
      }
    }
  }
};

