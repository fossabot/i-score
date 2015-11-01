#pragma once

#include <Scenario/Commands/ScenarioCommandFactory.hpp>
#include <iscore/command/SerializableCommand.hpp>

#include <iscore/tools/ModelPath.hpp>

class EventModel;
class TimeNodeModel;

namespace Scenario
{
    namespace Command
    {
        class SplitTimeNode : public iscore::SerializableCommand
        {
                ISCORE_SERIALIZABLE_COMMAND_DECL(ScenarioCommandFactoryName(), SplitTimeNode, "SplitTimeNode")
            public:
                SplitTimeNode(
                    Path<TimeNodeModel>&& path,
                    QVector<Id<EventModel> > eventsInNewTimeNode);
                void undo() const override;
                void redo() const override;

            protected:
                virtual void serializeImpl(QDataStream&) const override;
                virtual void deserializeImpl(QDataStream&) override;

            private:
                Path<TimeNodeModel> m_path;
                QVector<Id<EventModel> > m_eventsInNewTimeNode;

                Id<TimeNodeModel> m_originalTimeNodeId;
                Id<TimeNodeModel> m_newTimeNodeId;
         };
    }
}