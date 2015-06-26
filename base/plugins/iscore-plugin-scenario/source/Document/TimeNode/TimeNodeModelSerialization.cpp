#include "TimeNodeModel.hpp"


template<>
void Visitor<Reader<DataStream>>::readFrom(const TimeNodeModel& timenode)
{
    readFrom(static_cast<const IdentifiedObject<TimeNodeModel>&>(timenode));

    readFrom(timenode.metadata);
    readFrom(timenode.pluginModelList);

    m_stream << timenode.m_date
             << timenode.m_y
             << timenode.m_events
             << timenode.m_eventHasPreviousConstraint;


    insertDelimiter();
}

template<>
void Visitor<Writer<DataStream>>::writeTo(TimeNodeModel& timenode)
{
    writeTo(timenode.metadata);
    timenode.pluginModelList = iscore::ElementPluginModelList{*this, &timenode};

    m_stream >> timenode.m_date
             >> timenode.m_y
             >> timenode.m_events
             >> timenode.m_eventHasPreviousConstraint;


    checkDelimiter();
}

template<>
void Visitor<Reader<JSONObject>>::readFrom(const TimeNodeModel& timenode)
{
    readFrom(static_cast<const IdentifiedObject<TimeNodeModel>&>(timenode));
    m_obj["Metadata"] = toJsonObject(timenode.metadata);

    m_obj["Date"] = toJsonValue(timenode.date());
    m_obj["Y"] = timenode.y();
    m_obj["Events"] = toJsonArray(timenode.m_events);
    m_obj["EventsPreviousConstraintMap"] = toJsonMap(timenode.m_eventHasPreviousConstraint);

    m_obj["PluginsMetadata"] = toJsonValue(timenode.pluginModelList);
}

template<>
void Visitor<Writer<JSONObject>>::writeTo(TimeNodeModel& timenode)
{
    timenode.metadata = fromJsonObject<ModelMetadata>(m_obj["Metadata"].toObject());

    timenode.m_date = fromJsonValue<TimeValue> (m_obj["Date"]);
    timenode.m_y = m_obj["Y"].toDouble();

    fromJsonValueArray(m_obj["Events"].toArray(), timenode.m_events);
    timenode.m_eventHasPreviousConstraint = fromJsonMap<id_type<EventModel>,bool>(m_obj["EventsPreviousConstraintMap"].toArray());

    Deserializer<JSONValue> elementPluginDeserializer(m_obj["PluginsMetadata"]);
    timenode.pluginModelList = iscore::ElementPluginModelList{elementPluginDeserializer, &timenode};
}
