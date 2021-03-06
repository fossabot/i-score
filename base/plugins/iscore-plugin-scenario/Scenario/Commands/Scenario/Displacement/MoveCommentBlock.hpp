#pragma once

#include <Scenario/Commands/ScenarioCommandFactory.hpp>
#include <iscore/command/Command.hpp>
#include <iscore/model/path/Path.hpp>
#include <iscore/model/Identifier.hpp>
#include <iscore/tools/std/Optional.hpp>

#include <Process/TimeValue.hpp>

namespace Scenario
{
class CommentBlockModel;
namespace Command
{
class MoveCommentBlock final : public iscore::Command
{
  ISCORE_COMMAND_DECL(
      ScenarioCommandFactoryName(), MoveCommentBlock, "Move a comment block")
public:
  MoveCommentBlock(
      const ProcessModel& scenarPath,
      Id<CommentBlockModel>
          id,
      TimeVal newDate,
      double newY);

  void update(unused_t, unused_t, TimeVal newDate, double newYPos)
  {
    m_newDate = std::move(newDate);
    m_newY = newYPos;
  }
  // Command interface

  void undo(const iscore::DocumentContext& ctx) const override;
  void redo(const iscore::DocumentContext& ctx) const override;

protected:
  void serializeImpl(DataStreamInput&) const override;
  void deserializeImpl(DataStreamOutput&) override;

private:
  Path<ProcessModel> m_path;
  Id<CommentBlockModel> m_id;
  TimeVal m_oldDate, m_newDate;
  double m_oldY{}, m_newY{};
};
}
}
