#pragma once

#include <memory>
#include "transform.hpp"

#include "cpptoml.h"

class OptionsParser
{
public:
  OptionsParser(const std::string&);
  ~OptionsParser(void) {}

  
  Params * parse(void);

private:
  std::string m_file_name;
  std::shared_ptr<cpptoml::table> m_data;
};
