// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include <QDataStream>
#include <QtGlobal>
#include <algorithm>

#include "SetSegmentParameters.hpp"
#include <Curve/CurveModel.hpp>
#include <Curve/Segment/CurveSegmentModel.hpp>
#include <iscore/serialization/DataStreamVisitor.hpp>
#include <iscore/model/IdentifiedObjectMap.hpp>
#include <iscore/model/path/Path.hpp>
#include <iscore/model/path/PathSerialization.hpp>

namespace Curve
{
SetSegmentParameters::SetSegmentParameters(
    const Model& curve, SegmentParameterMap&& parameters)
    : m_model{curve}, m_new{std::move(parameters)}
{
  for (auto it = m_new.cbegin(); it != m_new.cend(); ++it)
  {
    const auto& seg = curve.segments().at(it.key());
    m_old.insert(
        it.key(), {seg.verticalParameter(), seg.horizontalParameter()});
  }
}

void SetSegmentParameters::undo(const iscore::DocumentContext& ctx) const
{
  auto& curve = m_model.find(ctx);
  for (auto it = m_old.cbegin(); it != m_old.cend(); ++it)
  {
    auto& seg = curve.segments().at(it.key());

    if (it.value().first)
      seg.setVerticalParameter(*it.value().first);
    if (it.value().second)
      seg.setHorizontalParameter(*it.value().second);
  }

  curve.changed();
}

void SetSegmentParameters::redo(const iscore::DocumentContext& ctx) const
{
  auto& curve = m_model.find(ctx);
  for (auto it = m_new.cbegin(); it != m_new.cend(); ++it)
  {
    auto& seg = curve.segments().at(it.key());

    seg.setVerticalParameter(it.value().first);
    seg.setHorizontalParameter(it.value().second);
  }

  curve.changed();
}

void SetSegmentParameters::serializeImpl(DataStreamInput& s) const
{
  s << m_model << m_old << m_new;
}

void SetSegmentParameters::deserializeImpl(DataStreamOutput& s)
{
  s >> m_model >> m_old >> m_new;
}
}
