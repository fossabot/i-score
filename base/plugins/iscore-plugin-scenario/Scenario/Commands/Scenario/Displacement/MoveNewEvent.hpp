#pragma once

#include <iscore/tools/std/Optional.hpp>

#include "MoveEventOnCreationMeta.hpp"
#include <Process/ExpandMode.hpp>
#include <Process/TimeValue.hpp>
#include <Scenario/Commands/ScenarioCommandFactory.hpp>
#include <iscore/command/Command.hpp>
#include <iscore/model/path/Path.hpp>
#include <iscore/model/Identifier.hpp>

#include <iscore_plugin_scenario_export.h>

struct DataStreamInput;
struct DataStreamOutput;

/*
 * Used on creation mode, when mouse is pressed and is moving.
 * In this case, both vertical and horizontal move are allowed
 */

namespace Scenario
{
class EventModel;
class ConstraintModel;
class ProcessModel;
namespace Command
{
class ISCORE_PLUGIN_SCENARIO_EXPORT MoveNewEvent final
    : public iscore::Command
{
  ISCORE_COMMAND_DECL(
      ScenarioCommandFactoryName(), MoveNewEvent, "Move a new event")
public:
  MoveNewEvent(
      const Scenario::ProcessModel& scenarioPath,
      Id<ConstraintModel> constraintId,
      Id<EventModel> eventId,
      TimeVal date,
      const double y,
      bool yLocked);
  MoveNewEvent(
      const Scenario::ProcessModel& scenarioPath,
      Id<ConstraintModel>  constraintId,
      Id<EventModel> eventId,
      TimeVal date,
      const double y,
      bool yLocked,
      ExpandMode);

  void undo(const iscore::DocumentContext& ctx) const override;
  void redo(const iscore::DocumentContext& ctx) const override;

  void update(
      Scenario::ProcessModel& s,
      unused_t,
      const Id<EventModel>& id,
      const TimeVal& date,
      const double y,
      bool yLocked)
  {
    m_cmd.update(s, id, date, y, ExpandMode::Scale);
    m_y = y;
    m_yLocked = yLocked;
  }

  const Path<Scenario::ProcessModel>& path() const
  {
    return m_path;
  }

protected:
  void serializeImpl(DataStreamInput&) const override;
  void deserializeImpl(DataStreamOutput&) override;

private:
  Path<Scenario::ProcessModel> m_path;
  Id<ConstraintModel> m_constraintId{};

  MoveEventOnCreationMeta m_cmd;
  double m_y{};
  bool m_yLocked{true}; // default is true and constraints are on the same y.
};
}
}
