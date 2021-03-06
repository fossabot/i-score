// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <Scenario/Process/Algorithms/StandardCreationPolicy.hpp>
#include <Scenario/Process/ScenarioModel.hpp>
#include <iscore/tools/RandomNameProvider.hpp>

#include <QByteArray>
#include <boost/iterator/iterator_facade.hpp>
#include <boost/multi_index/detail/hash_index_iterator.hpp>
#include <iscore/tools/IdentifierGeneration.hpp>
#include <vector>

#include "CreateEvent_State.hpp"
#include <Scenario/Commands/Scenario/Creations/CreateState.hpp>
#include <Scenario/Document/Event/EventModel.hpp>
#include <iscore/model/ModelMetadata.hpp>
#include <iscore/serialization/DataStreamVisitor.hpp>
#include <iscore/model/EntityMap.hpp>

namespace Scenario
{
namespace Command
{
CreateEvent_State::CreateEvent_State(
    const Scenario::ProcessModel& scenario,
    Id<TimeNodeModel>
        timeNode,
    double stateY)
    : m_newEvent{getStrongId(scenario.events)}
    , m_createdName{RandomNameProvider::generateRandomName()}
    , m_command{scenario, m_newEvent, stateY}
    , m_timeNode{std::move(timeNode)}
{
}

void CreateEvent_State::undo(const iscore::DocumentContext& ctx) const
{
  m_command.undo(ctx);

  ScenarioCreate<EventModel>::undo(
      m_newEvent, m_command.scenarioPath().find(ctx));
}

void CreateEvent_State::redo(const iscore::DocumentContext& ctx) const
{
  auto& scenar = m_command.scenarioPath().find(ctx);

  // Create the event
  ScenarioCreate<EventModel>::redo(
      m_newEvent,
      scenar.timeNode(m_timeNode),
      {m_command.endStateY() - 0.1, m_command.endStateY() + 0.1},
      scenar);

  scenar.events.at(m_newEvent).metadata().setName(m_createdName);

  // And the state
  m_command.redo(ctx);
}

void CreateEvent_State::serializeImpl(DataStreamInput& s) const
{
  s << m_newEvent << m_createdName << m_command.serialize() << m_timeNode;
}

void CreateEvent_State::deserializeImpl(DataStreamOutput& s)
{
  QByteArray b;
  s >> m_newEvent >> m_createdName >> b >> m_timeNode;

  m_command.deserialize(b);
}
}
}
