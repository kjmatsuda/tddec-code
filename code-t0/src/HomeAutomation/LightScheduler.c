/***
 * Excerpted from "Test-Driven Development for Embedded C",
 * published by The Pragmatic Bookshelf.
 * Copyrights apply to this code. It may not be used to create training material, 
 * courses, books, articles, and the like. Contact us if you are in doubt.
 * We make no guarantees that this code is fit for any purpose. 
 * Visit http://www.pragmaticprogrammer.com/titles/jgade for more book information.
***/
/*- ------------------------------------------------------------------ -*/
/*-    Copyright (c) James W. Grenning -- All Rights Reserved          -*/
/*-    For use by owners of Test-Driven Development for Embedded C,    -*/
/*-    and attendees of Renaissance Software Consulting, Co. training  -*/
/*-    classes.                                                        -*/
/*-                                                                    -*/
/*-    Available at http://pragprog.com/titles/jgade/                  -*/
/*-        ISBN 1-934356-62-X, ISBN13 978-1-934356-62-3                -*/
/*-                                                                    -*/
/*-    Authorized users may use this source code in your own           -*/
/*-    projects, however the source code may not be used to            -*/
/*-    create training material, courses, books, articles, and         -*/
/*-    the like. We make no guarantees that this source code is        -*/
/*-    fit for any purpose.                                            -*/
/*-                                                                    -*/
/*-    www.renaissancesoftware.net james@renaissancesoftware.net       -*/
/*- ------------------------------------------------------------------ -*/


#include "LightScheduler.h"
#include "LightController.h"
#include "TimeService.h"
#include <stdlib.h>
#include <string.h>

enum
{
    TURN_ON, TURN_OFF, DIM, RANDOM_ON, RANDOM_OFF
};

enum
{
    MAX_EVENTS = 128, UNUSED = -1
};

typedef struct
{
    int id;
    Day day;
    int minuteOfDay;
    int event;
} ScheduledLightEvent;

static ScheduledLightEvent scheduledEvents[MAX_EVENTS];

static void scheduleEvent(int id, Day day, int minuteOfDay, int event);
static void processEventDueNow(Time *pTime, ScheduledLightEvent *pEvent);
static void operateLight(ScheduledLightEvent *pEvent);
static int DoesLightRespondToday(Day eventDay, Day currentDay);

void LightScheduler_Create(void)
{
	int ii = 0;

	for (ii = 0; ii < MAX_EVENTS; ii++)
	{
		scheduledEvents[ii].id = UNUSED;
	}

	TimeService_SetPeriodicAlarmInSeconds(60, LightScheduler_WakeUp);
}

void LightScheduler_Destroy(void)
{
	TimeService_CancelPeriodicAlarmInSeconds(60, LightScheduler_WakeUp);
}

void LightScheduler_ScheduleTurnOn(int id, Day day, int minuteOfDay)
{
	scheduleEvent(id, day, minuteOfDay, TURN_ON);
}

void LightScheduler_ScheduleTurnOff(int id, Day day, int minuteOfDay)
{
	scheduleEvent(id, day, minuteOfDay, TURN_OFF);
}

void LightScheduler_WakeUp(void)
{
	int ii = 0;
	Time time;
	TimeService_GetTime(&time);

	for (ii = 0; ii < MAX_EVENTS; ii++)
	{
		processEventDueNow(&time, &scheduledEvents[ii]);
	}
}

static void scheduleEvent(int id, Day day, int minuteOfDay, int event)
{
	int ii = 0;

	for (ii = 0; ii < MAX_EVENTS; ii++)
	{
		if (scheduledEvents[ii].id == UNUSED) {
			scheduledEvents[ii].minuteOfDay = minuteOfDay;
			scheduledEvents[ii].event = event;
			scheduledEvents[ii].id = id;
			scheduledEvents[ii].day = day;
		}
	}
}

static void processEventDueNow(Time *pTime, ScheduledLightEvent *pEvent)
{
	if (pEvent->id == UNUSED)
	{
		return;
	}
	if (!DoesLightRespondToday(pEvent->day, pTime->dayOfWeek))
	{
		return;
	}
	if (pEvent->minuteOfDay != pTime->minuteOfDay)
	{
		return;
	}

	operateLight(pEvent);
}

static void operateLight(ScheduledLightEvent *pEvent)
{
	if (pEvent->event == TURN_ON)
	{
		LightController_TurnOn(pEvent->id);
	}
	else if (pEvent->event == TURN_OFF)
	{
		LightController_TurnOff(pEvent->id);
	}
}

static int DoesLightRespondToday(Day eventDay, Day currentDay)
{
	if (eventDay == EVERYDAY)
	{
		return TRUE;
	}

	if (eventDay == WEEKEND)
	{
		if (currentDay == SATURDAY || currentDay == SUNDAY)
		{
			return TRUE;
		}
	}

	if (eventDay == WEEKDAY)
	{
		if (currentDay >= MONDAY && currentDay <= FRIDAY)
		{
			return TRUE;
		}
	}

	if (eventDay == currentDay)
	{
		return TRUE;
	}

	return FALSE;
}
