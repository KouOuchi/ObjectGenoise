#pragma once

#include <iostream>
#include <fstream>

#include <boost/utility.hpp>
#include <boost/iostreams/filtering_streambuf.hpp>
#include <boost/iostreams/filter/gzip.hpp>
#include <boost/iostreams/copy.hpp>

using namespace boost::property_tree;
using namespace boost::property_tree::xml_parser;

namespace og
{
namespace core
{

class xml_stream : boost::noncopyable
{
public:
	xml_stream() : indent_(2)
  {}
  virtual ~xml_stream() {}

  void write_to_file(std::string _xml_path, ptree& _pt, bool _gzip = true)
  {
    std::ofstream os;
    os.open(_xml_path.c_str(), std::ofstream::out | std::ofstream::binary);

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
    is.open(_xml_path.c_str(), std::ifstream::in | std::ofstream::binary);

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

