// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "InterpolationView.hpp"
#include <QGraphicsSceneMouseEvent>
#include <iscore/model/Skin.hpp>
#include <Process/Style/ScenarioStyle.hpp>
#include <QPainter>
namespace Interpolation
{
View::View(QGraphicsItem* parent) : Process::LayerView{parent}
{
  setZValue(1);
  setFlags(ItemClipsToShape |
           ItemClipsChildrenToShape | ItemIsSelectable | ItemIsFocusable);
  setAcceptDrops(true);

  m_textcache.setFont(ScenarioStyle::instance().Medium8Pt);
  m_textcache.setCacheEnabled(true);
}

View::~View()
{
}

void View::setDisplayedName(const QString& s)
{
  m_textcache.setText(s);
  m_textcache.beginLayout();
  QTextLine line = m_textcache.createLine();
  line.setPosition(QPointF{0., 0.});

  m_textcache.endLayout();

  update();
}

void View::paint_impl(QPainter* painter) const
{
#if !defined(ISCORE_IEEE_SKIN)
  if (m_showName)
  {
    painter->setPen(ScenarioStyle::instance().ConstraintHeaderSeparator);
    m_textcache.draw(painter, QPointF{5., 8.});
  }
#endif
}

void View::dropEvent(QGraphicsSceneDragDropEvent* event)
{
  emit dropReceived(*event->mimeData());
}
}
