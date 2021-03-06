// This is an open source non-commercial project. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "ScenarioValidity.hpp"

namespace Scenario
{

ScenarioValidityChecker::~ScenarioValidityChecker()
{
}

bool ScenarioValidityChecker::validate(const iscore::DocumentContext& ctx)
{
  auto scenars = ctx.document.model().findChildren<Scenario::ProcessModel*>();
  for (auto scenar : scenars)
  {
    checkValidity(*scenar);
  }

  return true;
}

void ScenarioValidityChecker::checkValidity(const ProcessModel& scenar)
{
#if defined(ISCORE_DEBUG)
  for (const ConstraintModel& constraint : scenar.constraints)
  {
    auto ss = scenar.findState(constraint.startState());
    ISCORE_ASSERT(ss);
    auto es = scenar.findState(constraint.endState());
    ISCORE_ASSERT(es);

    ISCORE_ASSERT(ss->nextConstraint() == constraint.id());
    ISCORE_ASSERT(es->previousConstraint() == constraint.id());
  }

  for (const StateModel& state : scenar.states)
  {
    auto ev = scenar.findEvent(state.eventId());
    ISCORE_ASSERT(ev);
    ISCORE_ASSERT(ev->states().contains(state.id()));

    if (state.previousConstraint())
    {
      auto cst = scenar.findConstraint(*state.previousConstraint());
      ISCORE_ASSERT(cst);
      ISCORE_ASSERT(cst->endState() == state.id());
    }

    if (state.nextConstraint())
    {
      auto cst = scenar.findConstraint(*state.nextConstraint());
      ISCORE_ASSERT(cst);
      ISCORE_ASSERT(cst->startState() == state.id());
    }

    auto num = ossia::count_if(scenar.events, [&] (auto& ev) {
        return ossia::contains(ev.states(), state.id());
    });
    ISCORE_ASSERT(num == 1);
  }

  for (const EventModel& event : scenar.events)
  {
    auto tn = scenar.findTimeNode(event.timeNode());
    ISCORE_ASSERT(tn);
    ISCORE_ASSERT(tn->events().contains(event.id()));

    // ISCORE_ASSERT(!event.states().empty());
    for (auto& state : event.states())
    {
      auto st = scenar.findState(state);
      ISCORE_ASSERT(st);
      /*
      if(st->eventId() != event.id())
      {
          st->setEventId(event.id());
      }*/
      ISCORE_ASSERT(st->eventId() == event.id());
    }

    auto num = ossia::count_if(scenar.timeNodes, [&] (auto& ev) {
        return ossia::contains(ev.events(), event.id());
    });
    ISCORE_ASSERT(num == 1);
  }

  for (const TimeNodeModel& tn : scenar.timeNodes)
  {
    ISCORE_ASSERT(!tn.events().empty());
    for (auto& event : tn.events())
    {
      auto ev = scenar.findEvent(event);
      ISCORE_ASSERT(ev);
      ISCORE_ASSERT(ev->timeNode() == tn.id());
    }
  }
#endif
}
}
