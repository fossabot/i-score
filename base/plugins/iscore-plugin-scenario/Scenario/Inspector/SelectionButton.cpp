// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <QBoxLayout>
#include <QIcon>
#include <iscore/selection/SelectionDispatcher.hpp>
#include <iscore/widgets/MarginLess.hpp>

#include "SelectionButton.hpp"
#include <iscore/selection/Selection.hpp>
#include <iscore/widgets/SetIcons.hpp>

SelectionButton::SelectionButton(
    const QString& text,
    Selection target,
    iscore::SelectionDispatcher& disp,
    QWidget* parent)
    : QPushButton{text, parent}, m_dispatcher{disp}
{
  auto icon = makeIcons(":/icons/next_on.png", ":/icons/next_off.png");

  setObjectName("SelectionButton");
  setIcon(icon);
  setStyleSheet(
      "margin: 5px;"
      "margin-left: 10px;"
      "text-align: left;"
      "text-decoration: underline;"
      "border: none;");
  setFlat(true);

  connect(this, &QPushButton::clicked, this, [=]() {
    m_dispatcher.setAndCommit(target);
  });
}
