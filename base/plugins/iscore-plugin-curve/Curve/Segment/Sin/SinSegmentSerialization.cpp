// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <QJsonObject>
#include <QJsonValue>
#include <iscore/serialization/DataStreamVisitor.hpp>
#include <iscore/serialization/JSONVisitor.hpp>

#include "SinSegment.hpp"

template <>
void DataStreamReader::read(const Curve::PeriodicSegmentData& segmt)
{
  m_stream << segmt.freq << segmt.ampl;
}


template <>
void DataStreamWriter::write(Curve::PeriodicSegmentData& segmt)
{
  m_stream >> segmt.freq >> segmt.ampl;
}


template <>
void JSONObjectReader::read(const Curve::PeriodicSegmentData& segmt)
{
  obj["Freq"] = segmt.freq;
  obj["Ampl"] = segmt.ampl;
}


template <>
void JSONObjectWriter::write(Curve::PeriodicSegmentData& segmt)
{
  segmt.freq = obj["Freq"].toDouble();
  segmt.ampl = obj["Ampl"].toDouble();
}
