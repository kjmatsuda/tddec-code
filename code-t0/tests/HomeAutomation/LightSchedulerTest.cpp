//- ------------------------------------------------------------------
//-    Copyright (c) James W. Grenning -- All Rights Reserved         
//-    For use by owners of Test-Driven Development for Embedded C,   
//-    and attendees of Renaissance Software Consulting, Co. training 
//-    classes.                                                       
//-                                                                   
//-    Available at http://pragprog.com/titles/jgade/                 
//-        ISBN 1-934356-62-X, ISBN13 978-1-934356-62-3               
//-                                                                   
//-    Authorized users may use this source code in your own          
//-    projects, however the source code may not be used to           
//-    create training material, courses, books, articles, and        
//-    the like. We make no guarantees that this source code is       
//-    fit for any purpose.                                           
//-                                                                   
//-    www.renaissancesoftware.net james@renaissancesoftware.net      
//- ------------------------------------------------------------------


extern "C"
{
#include "LightScheduler.h"
#include "LightControllerSpy.h"
#include "FakeTimeService.h"
}
#include "CppUTest/TestHarness.h"

static void setTimeTo(int day, int minuteOfDay);
static void checkLightState(int id, int level);

TEST_GROUP(LightScheduler)
{
    void setup()
    {
    	LightController_Create();
    	LightScheduler_Create();
    }

    void teardown()
    {
    	LightController_Destroy();
    	LightScheduler_Destroy();
    }
};

TEST(LightScheduler, NoChangeToLightsDuringInitialization)
{
	LONGS_EQUAL(LIGHT_ID_UNKNOWN, LightControllerSpy_GetLastId());
	LONGS_EQUAL(LIGHT_STATE_UNKNOWN, LightControllerSpy_GetLastState());
}

TEST(LightScheduler, NoScheduleNothingHappens)
{
	FakeTimeService_SetDay(MONDAY);
	FakeTimeService_SetMinute(100);

	LightScheduler_WakeUp();

	LONGS_EQUAL(LIGHT_ID_UNKNOWN, LightControllerSpy_GetLastId());
	LONGS_EQUAL(LIGHT_STATE_UNKNOWN, LightControllerSpy_GetLastState());
}

TEST(LightScheduler, ScheduleOnTodayNotTimeYet)
{
    LightScheduler_ScheduleTurnOn(3, EVERYDAY, 1200);
    FakeTimeService_SetMinute(1199);

    LightScheduler_WakeUp();

    LONGS_EQUAL(LIGHT_ID_UNKNOWN, LightControllerSpy_GetLastId());
    LONGS_EQUAL(LIGHT_STATE_UNKNOWN, LightControllerSpy_GetLastState());
}

TEST(LightScheduler, ScheduleOnEverydayItsTime)
{
    LightScheduler_ScheduleTurnOn(3, EVERYDAY, 1200);
    FakeTimeService_SetDay(MONDAY);
    FakeTimeService_SetMinute(1200);

    LightScheduler_WakeUp();

    LONGS_EQUAL(3, LightControllerSpy_GetLastId());
    LONGS_EQUAL(LIGHT_ON, LightControllerSpy_GetLastState());
}

TEST(LightScheduler, ScheduleOffEverydayItsTime)
{
    LightScheduler_ScheduleTurnOff(3, EVERYDAY, 1200);
    FakeTimeService_SetDay(MONDAY);
    FakeTimeService_SetMinute(1200);

    LightScheduler_WakeUp();

    LONGS_EQUAL(3, LightControllerSpy_GetLastId());
    LONGS_EQUAL(LIGHT_OFF, LightControllerSpy_GetLastState());
}

TEST(LightScheduler, ScheduleWeekEndItsMonday)
{
	LightScheduler_ScheduleTurnOn(3, WEEKEND, 1200);

	setTimeTo(MONDAY, 1200);

	LightScheduler_WakeUp();

	checkLightState(LIGHT_ID_UNKNOWN, LIGHT_STATE_UNKNOWN);
}

static void setTimeTo(int day, int minuteOfDay)
{
	FakeTimeService_SetDay(day);
	FakeTimeService_SetMinute(minuteOfDay);
}

static void checkLightState(int id, int level)
{
	LONGS_EQUAL(id, LightControllerSpy_GetLastId());
	LONGS_EQUAL(level, LightControllerSpy_GetLastState());
}
