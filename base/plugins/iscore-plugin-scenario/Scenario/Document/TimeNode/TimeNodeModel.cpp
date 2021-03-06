// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include <Process/Style/ScenarioStyle.hpp>
#include <QtGlobal>
#include <Scenario/Document/Event/EventModel.hpp>

#include "TimeNodeModel.hpp"
#include <Process/TimeValue.hpp>
#include <Scenario/Document/VerticalExtent.hpp>
#include <Scenario/Process/ScenarioInterface.hpp>
#include <iscore/document/DocumentInterface.hpp>
#include <iscore/model/ModelMetadata.hpp>
#include <iscore/model/IdentifiedObject.hpp>
#include <iscore/model/Identifier.hpp>

namespace Scenario
{
TimeNodeModel::TimeNodeModel(
    const Id<TimeNodeModel>& id,
    const VerticalExtent& extent,
    const TimeVal& date,
    QObject* parent)
    : Entity{id, Metadata<ObjectKey_k, TimeNodeModel>::get(), parent}
    , m_extent{extent}
    , m_date{date}
{
  metadata().setInstanceName(*this);
  metadata().setColor(ScenarioStyle::instance().TimenodeDefault);

  m_expression.push_back(State::Expression{
      State::Relation{State::RelationMember{ossia::value(true)},
                      ossia::expressions::comparator::EQUAL,
                      State::RelationMember{ossia::value(false)}},
      &m_expression});
}

TimeNodeModel::TimeNodeModel(
    const TimeNodeModel& source, const Id<TimeNodeModel>& id, QObject* parent)
    : Entity{source, id, Metadata<ObjectKey_k, TimeNodeModel>::get(), parent}
    , m_extent{source.m_extent}
    , m_date{source.m_date}
    , m_expression{source.expression()}
    , m_active{source.active()}
    , m_events(source.m_events)
{

}

void TimeNodeModel::addEvent(const Id<EventModel>& eventId)
{
  m_events.push_back(eventId);

  auto scenar = dynamic_cast<ScenarioInterface*>(parent());
  if (scenar)
  {
    // There may be no scenario when we are cloning without a parent.
    // TODO this addEvent should be in an outside algorithm.
    auto& theEvent = scenar->event(eventId);
    theEvent.changeTimeNode(this->id());
  }

  emit newEvent(eventId);
}

bool TimeNodeModel::removeEvent(const Id<EventModel>& eventId)
{
  if (m_events.indexOf(eventId) >= 0)
  {
    m_events.remove(m_events.indexOf(eventId));
    emit eventRemoved(eventId);
    return true;
  }

  return false;
}

const TimeVal& TimeNodeModel::date() const
{
  return m_date;
}

void TimeNodeModel::setDate(const TimeVal& date)
{
  m_date = date;
  emit dateChanged(m_date);
}

const QVector<Id<EventModel>>& TimeNodeModel::events() const
{
  return m_events;
}

void TimeNodeModel::setEvents(const QVector<Id<EventModel>>& events)
{
  m_events = events;
}

const VerticalExtent& TimeNodeModel::extent() const
{
  return m_extent;
}

void TimeNodeModel::setExtent(const VerticalExtent& extent)
{
  if (extent != m_extent)
  {
    m_extent = extent;
    emit extentChanged(m_extent);
  }
}

void TimeNodeModel::setExpression(const State::Expression& expression)
{
  if (m_expression == expression)
    return;
  m_expression = expression;
  emit triggerChanged(m_expression);
}

bool TimeNodeModel::active() const
{
  return m_active;
}

void TimeNodeModel::setActive(bool active)
{
  if (active == m_active)
    return;
  m_active = active;
  emit activeChanged();
}
}
