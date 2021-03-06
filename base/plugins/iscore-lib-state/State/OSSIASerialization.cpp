// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <State/ValueSerializationImpl.hpp>
#include <State/DomainSerializationImpl.hpp>


template <>
ISCORE_LIB_STATE_EXPORT void
DataStreamReader::read(const ossia::domain& n)
{
  readFrom((const ossia::domain_base_variant&)n);
}


template <>
ISCORE_LIB_STATE_EXPORT void
DataStreamWriter::write(ossia::domain& n)
{
  writeTo((ossia::domain_base_variant&)n);
}



template <>
ISCORE_LIB_STATE_EXPORT void
JSONObjectReader::read(const ossia::domain& n)
{
  readFrom((const ossia::domain_base_variant&)n);
}


template <>
ISCORE_LIB_STATE_EXPORT void
JSONObjectWriter::write(ossia::domain& n)
{
  writeTo((ossia::domain_base_variant&)n);
}


template <>
ISCORE_LIB_STATE_EXPORT void
JSONObjectReader::read(const ossia::value& n)
{
  readFrom((const ossia::value_variant_type&)n.v);
}


template <>
ISCORE_LIB_STATE_EXPORT void
JSONObjectWriter::write(ossia::value& n)
{
  // REMOVEME in i-score 2.0 temporary check for the old case.
  auto it = obj.constFind(strings.Type);
  if(it == obj.constEnd())
    writeTo((ossia::value_variant_type&)n.v);
  else
    n = State::convert::fromQJsonValue(
            obj[strings.Value], it->toString());
}

template <>
ISCORE_LIB_STATE_EXPORT void
JSONValueReader::read(const ossia::value& n)
{
  val = iscore::marshall<JSONObject>(n);
}

template <>
ISCORE_LIB_STATE_EXPORT void
JSONValueWriter::write(ossia::value& n)
{
  n = iscore::unmarshall<ossia::value>(val.toObject());
}

template <>
ISCORE_LIB_STATE_EXPORT void
JSONValueReader::read(const ossia::impulse& n)
{
}

template <>
ISCORE_LIB_STATE_EXPORT void
JSONValueWriter::write(ossia::impulse& n)
{
  val = QJsonValue{};
}

template <>
ISCORE_LIB_STATE_EXPORT void
JSONValueReader::read(const std::array<float, 2>& n)
{
  val = toJsonValue(n);
}
template <>
ISCORE_LIB_STATE_EXPORT void
JSONValueReader::read(const std::array<float, 3>& n)
{
  val = toJsonValue(n);
}
template <>
ISCORE_LIB_STATE_EXPORT void
JSONValueReader::read(const std::array<float, 4>& n)
{
  val = toJsonValue(n);
}

template <>
ISCORE_LIB_STATE_EXPORT void
JSONValueWriter::write(std::array<float, 2>& n)
{
  fromJsonValue(val, n);
}
template <>
ISCORE_LIB_STATE_EXPORT void
JSONValueWriter::write(std::array<float, 3>& n)
{
  fromJsonValue(val, n);
}
template <>
ISCORE_LIB_STATE_EXPORT void
JSONValueWriter::write(std::array<float, 4>& n)
{
  fromJsonValue(val, n);
}


/// Instance bounds ///

template <>
ISCORE_LIB_STATE_EXPORT void
DataStreamReader::read(const ossia::net::instance_bounds& n)
{
  m_stream << n.min_instances << n.max_instances;
}


template <>
ISCORE_LIB_STATE_EXPORT void
DataStreamWriter::write(ossia::net::instance_bounds& n)
{
  m_stream >> n.min_instances >> n.max_instances;
}

template <>
ISCORE_LIB_STATE_EXPORT void
JSONValueReader::read(const ossia::net::instance_bounds& b)
{
  QJsonObject obj;
  obj[strings.Min] = b.min_instances;
  obj[strings.Max] = b.max_instances;
  val = std::move(obj);
}

template <>
ISCORE_LIB_STATE_EXPORT void
JSONValueWriter::write(ossia::net::instance_bounds& n)
{
  const auto& obj = val.toObject();
  n.min_instances = obj[strings.Min].toInt();
  n.max_instances = obj[strings.Max].toInt();
}
