package com.foo;

import jmunit.framework.cldc11.TestCase;
import jmunit.framework.cldc11.TimedMeasurement;

public class SpeedTest extends TestCase {

	public void tearDown() {
		removeAllPerformanceMeasurements();
	}

	public SpeedTest() {
		super(3, "SpeedTest");
	}

	public void testNoSpeedRequirements() throws InterruptedException {
		Thread.sleep(100);
	}

	public void testSpeedRequirements() throws InterruptedException {
//		addPerformanceMeasurement(new TimedMeasurement(50, 0));
		addPerformanceMeasurement(new TimedMeasurement(500, 0));
		Thread.sleep(100);
	}

	public void test(int testNum) throws Throwable {
		switch (testNum) {
		case 0:
		case 2:
			testNoSpeedRequirements();
			break;

		case 1:
			testSpeedRequirements();
			break;

		default:
			fail("No such test.");
		}
	}

}
