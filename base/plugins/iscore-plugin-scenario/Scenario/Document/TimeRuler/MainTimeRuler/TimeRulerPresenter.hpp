#pragma once

#include <Scenario/Document/TimeRuler/AbstractTimeRuler.hpp>
#include <Process/TimeValue.hpp>

class TimeRulerView;

class TimeRulerPresenter : public AbstractTimeRuler
{
        Q_OBJECT
    public:
        explicit TimeRulerPresenter(TimeRulerView* view, QObject *parent = 0);
        ~TimeRulerPresenter();

};