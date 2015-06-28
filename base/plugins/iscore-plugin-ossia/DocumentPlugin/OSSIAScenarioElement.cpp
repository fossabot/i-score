#include "OSSIAScenarioElement.hpp"

#include <API/Headers/Editor/Scenario.h>
#include <API/Headers/Editor/TimeConstraint.h>
#include <API/Headers/Editor/TimeEvent.h>
#include <API/Headers/Editor/TimeNode.h>

#include <Process/ScenarioModel.hpp>
#include "iscore2OSSIA.hpp"
OSSIAScenarioElement::OSSIAScenarioElement(const ScenarioModel* element, QObject* parent):
    iscore::ElementPluginModel{parent},
    m_iscore_scenario{element}
{
    m_ossia_scenario = OSSIA::Scenario::create();

    connect(element, &ScenarioModel::constraintCreated,
            this, &OSSIAScenarioElement::on_constraintCreated);
    connect(element, &ScenarioModel::eventCreated,
            this, &OSSIAScenarioElement::on_eventCreated);
    connect(element, &ScenarioModel::timeNodeCreated,
            this, &OSSIAScenarioElement::on_timeNodeCreated);

    connect(element, &ScenarioModel::constraintMoved,
            this, &OSSIAScenarioElement::on_constraintMoved);
    connect(element, &ScenarioModel::eventMoved,
            this, &OSSIAScenarioElement::on_eventMoved);

    connect(element, &ScenarioModel::constraintRemoved,
            this, &OSSIAScenarioElement::on_constraintRemoved);
    connect(element, &ScenarioModel::eventRemoved_before,
            this, &OSSIAScenarioElement::on_eventRemoved);
    connect(element, &ScenarioModel::timeNodeRemoved,
            this, &OSSIAScenarioElement::on_timeNodeRemoved);

    // Create elements for the existing stuff. (e.g. start/ end timenode / event)
    for(auto& timenode : m_iscore_scenario->timeNodes())
    {
        on_timeNodeCreated(timenode->id());
    }
    for(auto& event : m_iscore_scenario->events())
    {
        on_eventCreated(event->id());
    }
    for(auto& constraint : m_iscore_scenario->constraints())
    {
        on_constraintCreated(constraint->id());
    }
}



iscore::ElementPluginModel* OSSIAScenarioElement::clone(
        const QObject* element,
        QObject* parent) const
{
    qDebug() << "TODO: " << Q_FUNC_INFO;
    return nullptr;
}


iscore::ElementPluginModelType OSSIAScenarioElement::elementPluginId() const
{
    return staticPluginId();
}

void OSSIAScenarioElement::serialize(const VisitorVariant&) const
{
    qDebug() << "TODO: " << Q_FUNC_INFO;
}

void OSSIAScenarioElement::on_constraintCreated(const id_type<ConstraintModel>& id)
{
    auto& cst = m_iscore_scenario->constraint(id);

    Q_ASSERT(m_ossia_timeevents.find(cst.startEvent()) != m_ossia_timeevents.end());
    auto& ossia_sev = m_ossia_timeevents.at(cst.startEvent());
    Q_ASSERT(m_ossia_timeevents.find(cst.endEvent()) != m_ossia_timeevents.end());
    auto& ossia_eev = m_ossia_timeevents.at(cst.endEvent());

    auto ossia_cst = OSSIA::TimeConstraint::create(
                         ossia_sev->event(),
                         ossia_eev->event(),
                         iscore::convert::time(cst.defaultDuration()),
                         iscore::convert::time(cst.minDuration()),
                         iscore::convert::time(cst.maxDuration()));

    auto elt = new OSSIAConstraintElement{ossia_cst, cst, &cst};
    m_ossia_constraints.insert({id, elt});

    m_ossia_scenario->addConstraint(ossia_cst);

    cst.pluginModelList.add(elt);
}

void OSSIAScenarioElement::on_eventCreated(const id_type<EventModel>& id)
{
    auto& ev = m_iscore_scenario->event(id);

    Q_ASSERT(m_ossia_timenodes.find(ev.timeNode()) != m_ossia_timenodes.end());
    auto ossia_tn = m_ossia_timenodes.at(ev.timeNode());

    std::shared_ptr<OSSIA::TimeEvent> ossia_ev;
    // If it's the first, it's already here
    if(ossia_tn->timeNode()->timeEvents().size() == 1)
    {
        ossia_ev = ossia_tn->timeNode()->timeEvents().front();
    }
    else
    {
        ossia_ev = *ossia_tn->timeNode()->emplace(ossia_tn->timeNode()->timeEvents().begin());
    }

    // Pass the element in ctor
    auto elt = new OSSIAEventElement{ossia_ev, &ev, &ev};
    m_ossia_timeevents.insert({id, elt});

    ev.pluginModelList.add(elt);
}

void OSSIAScenarioElement::on_timeNodeCreated(const id_type<TimeNodeModel>& id)
{
    auto& tn = m_iscore_scenario->timeNode(id);

    // Note : this also default-creates an event.

    std::shared_ptr<OSSIA::TimeNode> ossia_tn;
    if(id == m_iscore_scenario->startEvent().timeNode())
    {
        ossia_tn = m_ossia_scenario->getStartNode();
    }
    else if(id == m_iscore_scenario->endEvent().timeNode())
    {
        ossia_tn = m_ossia_scenario->getEndNode();
    }
    else
    {
        ossia_tn = OSSIA::TimeNode::create();
    }
    auto elt = new OSSIATimeNodeElement{ossia_tn, tn, &tn};

    m_ossia_scenario->addTimeNode(elt->timeNode());
    m_ossia_timenodes.insert({id, elt});

    tn.pluginModelList.add(elt);
}

void OSSIAScenarioElement::on_constraintMoved(const id_type<ConstraintModel>& id)
{
    qDebug(Q_FUNC_INFO);

}

void OSSIAScenarioElement::on_eventMoved(const id_type<EventModel>& id)
{
    qDebug(Q_FUNC_INFO);

}

void OSSIAScenarioElement::on_constraintRemoved(const id_type<ConstraintModel>& id)
{
    auto it = m_ossia_constraints.find(id);
    auto cst = (*it).second;
    m_ossia_scenario->removeConstraint(cst->constraint());

    m_ossia_constraints.erase(it);
    // Deletion will be part of the ConstraintModel* delete.
}

void OSSIAScenarioElement::on_eventRemoved(const id_type<EventModel>& id)
{
    auto ev_it = m_ossia_timeevents.find(id);
    OSSIAEventElement* ev = (*ev_it).second;

    auto tn_it = m_ossia_timenodes.find(m_iscore_scenario->event(id).timeNode());
    OSSIATimeNodeElement* tn = (*tn_it).second;

    m_ossia_timeevents.erase(ev_it);

    // TODO how to remove it in OSSIA?
    // TODO if this was the last event, we certainly have to remove the timenode, too.
}

void OSSIAScenarioElement::on_timeNodeRemoved(const id_type<TimeNodeModel>& id)
{
    auto tn_it = m_ossia_timenodes.find(id);
    OSSIATimeNodeElement* tn = (*tn_it).second;

    m_ossia_scenario->removeTimeNode(tn->timeNode());
    // Deletion will be part of the TimeNodeModel* delete.

    m_ossia_timenodes.erase(tn_it);
}