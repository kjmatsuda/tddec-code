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
    int minuteOfDay;
    int event;
} ScheduledLightEvent;

static ScheduledLightEvent scheduledEvent;

static void scheduleEvent(int id, Day day, int minuteOfDay, int event);

void LightScheduler_Create(void)
{
	scheduledEvent.id = UNUSED;
}

void LightScheduler_Destroy(void)
{

}

int LightScheduler_ScheduleTurnOn(int id, Day day, int minuteOfDay)
{
	scheduledEvent.id = id;
	scheduledEvent.minuteOfDay = minuteOfDay;
	scheduledEvent.event = TURN_ON;
}

int LightScheduler_ScheduleTurnOff(int id, Day day, int minuteOfDay)
{
	scheduledEvent.id = id;
	scheduledEvent.minuteOfDay = minuteOfDay;
	scheduledEvent.event = TURN_OFF;
}

void LightScheduler_WakeUp(void)
{
	Time time;
	TimeService_GetTime(&time);

	if (scheduledEvent.id == UNUSED)
	{
		return;
	}

	if (time.minuteOfDay != scheduledEvent.minuteOfDay)
	{
		return;
	}

	if (scheduledEvent.event == TURN_ON)
	{
		LightController_TurnOn(scheduledEvent.id);
	}
	else if (scheduledEvent.event == TURN_OFF)
	{
		LightController_TurnOff(scheduledEvent.id);
	}
}

static void scheduleEvent(int id, Day day, int minuteOfDay, int event)
{
	scheduledEvent.minuteOfDay = minuteOfDay;
}
