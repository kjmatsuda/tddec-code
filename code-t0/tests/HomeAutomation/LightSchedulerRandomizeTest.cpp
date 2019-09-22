extern "C"
{
#include "LightScheduler.h"
#include "LightControllerSpy.h"
#include "FakeTimeService.h"
#include "FakeRandomMinute.h"
}
#include "CppUTest/TestHarness.h"

static void setTimeTo(int day, int minuteOfDay);
static void checkLightState(int id, int level);

TEST_GROUP(LightSchedulerRandomize)
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

TEST(LightSchedulerRandomize, TurnsOnEarly)
{
	FakeRandomMinute_SetFirstAndIncrement(-10, 5);
	LightScheduler_ScheduleTurnOn(4, EVERYDAY, 600);
	LightScheduler_Randomize(4, EVERYDAY, 600);

	setTimeTo(MONDAY, 600-10);

	LightScheduler_WakeUp();

	checkLightState(4, LIGHT_ON);
}

static void setTimeTo(int day, int minuteOfDay)
{
	FakeTimeService_SetDay(day);
	FakeTimeService_SetMinute(minuteOfDay);
}

static void checkLightState(int id, int level)
{
	if (id == LIGHT_ID_UNKNOWN)
	{
		LONGS_EQUAL(id, LightControllerSpy_GetLastId());
		LONGS_EQUAL(level, LightControllerSpy_GetLastState());
	}
	else
	{
		LONGS_EQUAL(level, LightControllerSpy_GetLightState(id));
	}
}
