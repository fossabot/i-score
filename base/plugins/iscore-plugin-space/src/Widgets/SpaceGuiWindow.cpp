#include "SpaceGuiWindow.hpp"

#include <iscore/widgets/MarginLess.hpp>

#include "AreaTab.hpp"
#include "SpaceTab.hpp"
#include "ComputationsTab.hpp"
SpaceGuiWindow::SpaceGuiWindow(iscore::CommandStack &stack, const SpaceProcess &space, QWidget* parent) :
    QWidget{parent}
{
    auto lay = new MarginLess<QGridLayout>;

    QTabWidget* tabs = new QTabWidget;
    lay->addWidget(tabs);

    this->setLayout(lay);

    tabs->addTab(new AreaTab{stack, space, this}, tr("Areas"));
    tabs->addTab(new SpaceTab{this}, tr("Space"));
    tabs->addTab(new ComputationsTab{this}, tr("Computation"));

}