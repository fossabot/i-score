// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <Scenario/Process/Algorithms/VerticalMovePolicy.hpp>
#include <Scenario/Process/ScenarioModel.hpp>

#include <algorithm>

#include "MoveNewState.hpp"
#include <Scenario/Document/State/StateModel.hpp>
#include <iscore/serialization/DataStreamVisitor.hpp>
#include <iscore/model/path/Path.hpp>
#include <iscore/model/path/PathSerialization.hpp>

#include <iscore/model/Identifier.hpp>

namespace Scenario
{
namespace Command
{
MoveNewState::MoveNewState(
    const Scenario::ProcessModel& scenar,
    Id<StateModel>
        stateId,
    double y)
    : m_path(scenar)
    , m_stateId{std::move(stateId)}, m_y{y}
{
  m_oldy = scenar.state(m_stateId).heightPercentage();
}

void MoveNewState::undo(const iscore::DocumentContext& ctx) const
{
  auto& scenar = m_path.find(ctx);
  auto& state = scenar.state(m_stateId);
  state.setHeightPercentage(m_oldy);
  if(auto prev = state.previousConstraint())
  {
    scenar.constraints.at(*prev).setHeightPercentage(m_oldy);
  }
  if(auto next = state.nextConstraint())
  {
    scenar.constraints.at(*next).setHeightPercentage(m_oldy);
  }

  updateEventExtent(state.eventId(), scenar);
}

void MoveNewState::redo(const iscore::DocumentContext& ctx) const
{
  auto& scenar = m_path.find(ctx);
  auto& state = scenar.state(m_stateId);
  state.setHeightPercentage(m_y);
  if(auto prev = state.previousConstraint())
  {
    scenar.constraints.at(*prev).setHeightPercentage(m_y);
  }
  if(auto next = state.nextConstraint())
  {
    scenar.constraints.at(*next).setHeightPercentage(m_y);
  }

  updateEventExtent(state.eventId(), scenar);
}

void MoveNewState::serializeImpl(DataStreamInput& s) const
{
  s << m_path << m_stateId << m_oldy << m_y;
}

void MoveNewState::deserializeImpl(DataStreamOutput& s)
{
  s >> m_path >> m_stateId >> m_oldy >> m_y;
}
}
}
