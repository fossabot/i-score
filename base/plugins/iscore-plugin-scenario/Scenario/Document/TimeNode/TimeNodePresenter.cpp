// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <Scenario/Document/TimeNode/TimeNodeModel.hpp>
#include <Scenario/Document/TimeNode/TimeNodeView.hpp>
#include <Scenario/Document/TimeNode/TriggerView.hpp>
#include <iscore/widgets/GraphicsItem.hpp>

#include "TimeNodePresenter.hpp"
#include <iscore/model/ModelMetadata.hpp>
#include <iscore/selection/Selectable.hpp>

#include <iscore/tools/Todo.hpp>

class QObject;
#include <iscore/model/Identifier.hpp>

namespace Scenario
{
TimeNodePresenter::TimeNodePresenter(
    const TimeNodeModel& model,
        QGraphicsItem* parentview,
        QObject* parent)
    : QObject{parent}
    , m_model{model}
    , m_view{new TimeNodeView{*this, parentview}}
    , m_triggerView{new TriggerView{m_view}}
{
  con(m_model.selection, &Selectable::changed, this,
      [=](bool b) { m_view->setSelected(b); });

  con(m_model, &TimeNodeModel::newEvent, this,
      &TimeNodePresenter::on_eventAdded);

  con(m_model.metadata(), &iscore::ModelMetadata::ColorChanged, this,
      [=](const iscore::ColorRef& c) { m_view->changeColor(c); });
  con(m_model.metadata(), &iscore::ModelMetadata::LabelChanged, this,
      [=](const auto& t) { m_view->setLabel(t); });
  con(m_model, &TimeNodeModel::activeChanged, this, [=] {
    m_view->setTriggerActive(m_model.active());
    m_triggerView->setVisible(m_model.active());
    m_triggerView->setToolTip(m_model.expression().toString());
  });
  m_view->changeColor(m_model.metadata().getColor());
  m_view->setLabel(m_model.metadata().getLabel());
  m_view->setTriggerActive(m_model.active());
  // TODO find a correct way to handle validity of model elements.
  // extentChanged is updated in scenario.

  m_triggerView->setVisible(m_model.active());
  m_triggerView->setPos(-7.5, -25.);

  m_triggerView->setToolTip(m_model.expression().toString());
  con(m_model, &TimeNodeModel::triggerChanged, this,
      [&](const State::Expression& t) { m_triggerView->setToolTip(t.toString()); });

  connect(
      m_triggerView, &TriggerView::pressed,
              &m_model, [=] (QPointF sp) {
      m_model.triggeredByGui();
      pressed(sp);
  });
}

TimeNodePresenter::~TimeNodePresenter()
{
}

const Id<TimeNodeModel>& TimeNodePresenter::id() const
{
  return m_model.id();
}

const TimeNodeModel& TimeNodePresenter::model() const
{
  return m_model;
}

TimeNodeView* TimeNodePresenter::view() const
{
  return m_view;
}

void TimeNodePresenter::on_eventAdded(const Id<EventModel>& eventId)
{
  emit eventAdded(eventId, m_model.id());
}
}
