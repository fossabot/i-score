// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <Scenario/Document/Constraint/ConstraintModel.hpp>
#include <Scenario/Process/ScenarioModel.hpp>

#include <QMap>
#include <QObject>
#include <QString>
#include <QtAlgorithms>
#include <algorithm>
#include <cstddef>
#include <vector>

#include "AddressBarItem.hpp"
#include "ClickableLabelItem.hpp"
#include <iscore/model/ModelMetadata.hpp>
#include <iscore/model/path/ObjectIdentifier.hpp>

class QPainter;
class QStyleOptionGraphicsItem;
class QWidget;

namespace Scenario
{
AddressBarItem::AddressBarItem(
    const iscore::DocumentContext& ctx,
    QGraphicsItem* parent)
  : QGraphicsItem{parent}
  , m_ctx{ctx}
{
  this->setFlag(QGraphicsItem::ItemHasNoContents, true);
}

void AddressBarItem::setTargetObject(ObjectPath&& path)
{
  qDeleteAll(m_items);
  m_items.clear();

  m_currentPath = std::move(path);

  double currentWidth = 0.;

  int i = -1;
  for (auto& identifier : m_currentPath)
  {
    i++;
    if (!identifier.objectName().contains("ConstraintModel"))
      continue;

    auto thisPath = m_currentPath;
    auto& pathVec = thisPath.vec();
    pathVec.resize(i + 1);
    ConstraintModel& thisObj = thisPath.find<ConstraintModel>(m_ctx);

    QString txt = thisObj.metadata().getName();

    auto lab = new ClickableLabelItem{
        thisObj.metadata(),
        [&](ClickableLabelItem* item) { emit constraintSelected(thisObj); },
        txt, this};

    lab->setIndex(i);
    connect(
        lab, &ClickableLabelItem::textChanged, this, &AddressBarItem::redraw);

    m_items.append(lab);
    lab->setPos(currentWidth, 0.);
    currentWidth += 5. + lab->boundingRect().width();

    auto sep = new SeparatorItem{this};
    sep->setPos(currentWidth, 0.);
    currentWidth += 5. + sep->boundingRect().width();
    m_items.append(sep);
  }

  prepareGeometryChange();
  m_width = currentWidth;
}

double AddressBarItem::width() const
{
  return m_width;
}

QRectF AddressBarItem::boundingRect() const
{
  return {};
}

void AddressBarItem::paint(
    QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
{
}

void AddressBarItem::redraw()
{
  double currentWidth = 0.;
  for (auto obj : m_items)
  {
    obj->setPos(currentWidth, 0.);
    currentWidth += 10. + obj->boundingRect().width();
  }

  emit needRedraw();
}
}
