// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
/*
#include <QtTest/QtTest>
#include <core/QNamedObject>
#include <core/command/CommandStack.hpp>
#include <iscore/command/Command.hpp>
using namespace iscore;


class FakeParent : public QObject
{

} m_globalParent;

struct FakeModel : public QNamedObject
{
    FakeModel(QObject* parent) : QNamedObject {parent, "FakeModel"}
    {
    }

    int modelId {};
    int value {};
};

class FakeCommand : public Command
{
    public:
        FakeCommand(int modelId) :
            Command {"", "FakeCommand", "" },
        m_modelId {modelId}
        {

        }

        virtual void undo(const iscore::DocumentContext& ctx)
        {
            auto children = m_globalParent.findChildren<FakeModel*>
("FakeModel");

            for(auto& model : children)
            {
                if(model->modelId == m_modelId)
                {
                    model->value--;
                }
            }
        }

        virtual void redo(const iscore::DocumentContext& ctx)
        {
            auto children = m_globalParent.findChildren<FakeModel*>
("FakeModel");

            for(auto& model : children)
            {
                if(model->modelId == m_modelId)
                {
                    model->value++;
                }
            }
        }

        virtual int id() const
        {
            return 1;
        }

    private:
        int m_modelId {};

        // Command interface
    protected:
        void serializeImpl(DataStreamInput&) override
        {
        }
        void deserializeImpl(DataStreamOutput&) override
        {
        }
};

class TestCommand: public QObject
{
        Q_OBJECT
    public:
        TestCommand() : QObject {}
        {
            m_model = new FakeModel{&m_globalParent};
        }

    private slots:
        void CommandTest()
        {
            QVERIFY(m_model->value == 0);
            auto cmd = new FakeCommand {0};
            m_commandQueue.push(cmd);
            QVERIFY(m_model->value == 1);
            m_commandQueue.undo(ctx);
            QVERIFY(m_model->value == 0);
            m_commandQueue.redo(ctx);
            QVERIFY(m_model->value == 1);
        }

    private:
        CommandQueue m_commandQueue;
        FakeModel* m_model {};
};

QTEST_MAIN(TestCommand)
#include "test_command.moc"

*/
