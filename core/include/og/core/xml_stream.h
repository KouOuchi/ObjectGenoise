#pragma once

#include <iostream>
#include <fstream>

#include <boost/utility.hpp>
#include <boost/iostreams/filtering_streambuf.hpp>
#include <boost/iostreams/filter/gzip.hpp>
#include <boost/iostreams/copy.hpp>
#include <boost/filesystem.hpp>
#include <boost/locale.hpp>

namespace og
{
namespace core
{
using namespace boost::property_tree;
using namespace boost::property_tree::xml_parser;
namespace fs = boost::filesystem;

class xml_stream : boost::noncopyable
{
public:
  xml_stream() : indent_(2) {}

  virtual ~xml_stream() {}

  void write_to_file(std::string _xml_path, ptree& _pt, bool _gzip = true)
  {
    std::ofstream os;
    fs::path file_path(_xml_path);

    os.open(file_path.native().c_str(), std::ofstream::out | std::ofstream::binary);

    boost::iostreams::filtering_streambuf<boost::iostreams::output> filter;

    if(_gzip)
    {
      //add gzip to filter
      filter.push(boost::iostreams::gzip_compressor());
    }

    // add stream to filter
    filter.push(os);
    std::ostream stream(&filter);

    // write to stream
    write_xml(
      stream,
      _pt,
      xml_writer_make_settings<std::string>(' ', indent_));
  }

  void read_from_file(std::string _xml_path, ptree* _pt, bool _gzip = true)
  {
    std::ifstream is;
    fs::path file_path(_xml_path);

    is.open(file_path.native().c_str(), std::ifstream::in | std::ofstream::binary);

    boost::iostreams::filtering_streambuf<boost::iostreams::input> filter;

    if(_gzip)
    {
      //add gzip to filter
      filter.push(boost::iostreams::gzip_decompressor());
    }

    // add stream to filter
    filter.push(is);
    std::istream stream(&filter);

    // read from stream
    read_xml(
      stream,
      *_pt);
  }

private:
  const int indent_;
};

} //namespace core
} //namespace og

