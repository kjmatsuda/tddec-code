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

static ScheduledLightEvent scheduledEvent;

static void scheduleEvent(int id, Day day, int minuteOfDay, int event);
static void processEventDueNow(Time *pTime, ScheduledLightEvent *pEvent);
static void operateLight(ScheduledLightEvent *pEvent);

void LightScheduler_Create(void)
{
	scheduledEvent.id = UNUSED;
}

void LightScheduler_Destroy(void)
{

}

int LightScheduler_ScheduleTurnOn(int id, Day day, int minuteOfDay)
{
	scheduleEvent(id, day, minuteOfDay, TURN_ON);
}

int LightScheduler_ScheduleTurnOff(int id, Day day, int minuteOfDay)
{
	scheduleEvent(id, day, minuteOfDay, TURN_OFF);
}

void LightScheduler_WakeUp(void)
{
	Time time;
	TimeService_GetTime(&time);

	processEventDueNow(&time, &scheduledEvent);
}

static void scheduleEvent(int id, Day day, int minuteOfDay, int event)
{
	scheduledEvent.minuteOfDay = minuteOfDay;
	scheduledEvent.event = event;
	scheduledEvent.id = id;
	scheduledEvent.day = day;
}

static void processEventDueNow(Time *pTime, ScheduledLightEvent *pEvent)
{
	if (pEvent->id == UNUSED)
	{
		return;
	}
	if (pEvent->day != EVERYDAY)
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
