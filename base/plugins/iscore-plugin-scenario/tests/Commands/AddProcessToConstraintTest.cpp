// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <Process/ProcessList.hpp>
#include <QtTest/QtTest>
#include <Scenario/Document/Constraint/ConstraintModel.hpp>
#include <Scenario/Document/Constraint/Rack/RackModel.hpp>
#include <Scenario/Document/Event/EventModel.hpp>
#include <Scenario/Process/ScenarioModel.hpp>

#include <Scenario/Commands/Constraint/AddProcessToConstraint.hpp>
#include <Scenario/Commands/Constraint/AddRackToConstraint.hpp>
#include <Scenario/Commands/Constraint/RemoveProcessFromConstraint.hpp>
#include <Scenario/Process/ScenarioFactory.hpp>

using namespace iscore;
using namespace Scenario::Command;

class AddProcessToConstraintTest : public QObject
{
  Q_OBJECT

private slots:
  void CreateCommandTest()
  {
    NamedObject* obj = new NamedObject{"obj", qApp};
    ProcessList* plist = new ProcessList{obj};
    plist->registerProcess(new ScenarioFactory);

    ConstraintModel* cstrModel = new ConstraintModel{
        Id<ConstraintModel>{1}, Id<ConstraintViewModel>{0}, qApp};

    AddRackToConstraint rackCmd(ObjectPath{{"ConstraintModel", {1}}});
    rackCmd.redo(ctx);

    AddProcessToConstraint cmd{{{"ConstraintModel", {1}}}, "Scenario"};

    cmd.redo(ctx);
    QCOMPARE((int)cstrModel->processes().size(), 1);
    cmd.undo(ctx);
    QCOMPARE((int)cstrModel->processes().size(), 0);
    cmd.redo(ctx);
    QCOMPARE((int)cstrModel->processes().size(), 1);

    // Delete them else they stay in qApp !
    delete cstrModel;
    delete obj;
  }
};

QTEST_MAIN(AddProcessToConstraintTest)
#include "AddProcessToConstraintTest.moc"
