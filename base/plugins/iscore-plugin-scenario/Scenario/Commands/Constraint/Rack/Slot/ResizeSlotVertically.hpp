#pragma once
#include <Scenario/Commands/ScenarioCommandFactory.hpp>
#include <iscore/command/SerializableCommand.hpp>
#include <iscore/tools/ModelPath.hpp>

#include <QString>
class SlotModel;
namespace Scenario
{
    namespace Command
    {
        /**
         * @brief The ResizeSlotVerticallyCommand class
         *
         * Changes a slot's vertical size
         */
        class ResizeSlotVertically : public iscore::SerializableCommand
        {
                ISCORE_SERIALIZABLE_COMMAND_DECL(ScenarioCommandFactoryName(), ResizeSlotVertically, "ResizeSlotVertically")
            public:
                ResizeSlotVertically(
                    Path<SlotModel>&& slotPath,
                    double newSize);

                void undo() const override;
                void redo() const override;

                void update(const Path<SlotModel>&, double size)
                {
                    m_newSize = size;
                }

            protected:
                virtual void serializeImpl(QDataStream&) const override;
                virtual void deserializeImpl(QDataStream&) override;

            private:
                Path<SlotModel> m_path;

                double m_originalSize {};
                double m_newSize {};
        };
    }
}