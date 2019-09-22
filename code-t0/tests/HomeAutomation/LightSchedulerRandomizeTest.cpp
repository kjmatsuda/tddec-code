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
		// ランダム分生成関数のプロダクトコード版を退避し、テストダブル版を設定
		// UT_PTR_SETはCppUTestで提供されているマクロで、これで設定した関数ポインタは
		// teardownで自動的に元の値に戻される
		UT_PTR_SET(RandomMinute_Get, FakeRandomMinute_Get);
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
