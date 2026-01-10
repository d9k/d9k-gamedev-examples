/*
 * TimedMeasurement.java
 *  
 * Copyright 2008 C.A. Meijer
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
package jmunit.framework.cldc11;

/**
 * The TimedMeasurement class monitors the running time of a test. A test fails
 * if it takes too long to run.
 * 
 * @author C.A. Meijer
 * @since JMUnit 1.2
 */
public class TimedMeasurement implements PerformanceMeasurement {

	private static final int JTWI_MIN_CLOCK_RESOLUTION = 40;

	private long startTime;

	private long maxRunningTime;

	private int clockResolution;

	/**
	 * Constructor. This assumes that the clock resolution is 40 milliseconds
	 * which is the worst-case resolution on a JTWI-compliant device.
	 * 
	 * @param maxRunningTime
	 *            The maximum allowed running time for the test (in
	 *            milliseconds).
	 */
	public TimedMeasurement(long maxRunningTime) {
		this(maxRunningTime, JTWI_MIN_CLOCK_RESOLUTION);
	}

	/**
	 * Constructor that allows the clock resolution to be specified. If the
	 * resolution is given as zero, the TimedMeasurement waits until the next
	 * clock tick before allowing a test to start running.
	 * 
	 * @param maxRunningTime
	 *            The maximum allowed running time for the test (in
	 *            milliseconds).
	 * 
	 * @param clockResolution
	 *            The uncertainty associated with the system clock (in
	 *            milliseconds).
	 */
	public TimedMeasurement(long maxRunningTime, int clockResolution) {
		this.maxRunningTime = maxRunningTime;
		this.clockResolution = clockResolution;
		startMeasurement();
	}

	/**
	 * Starts monitoring.
	 */
	public void startMeasurement() {
		this.startTime = System.currentTimeMillis();
		if (this.clockResolution == 0) {
			while (System.currentTimeMillis() == this.startTime)
				;
			this.startTime = System.currentTimeMillis();
		}
	}

	/**
	 * Ends monitoring.
	 */
	public void endMeasurement() {
		long runningTime = System.currentTimeMillis() - this.startTime;
		Assertion.assertTrue("Test took too long: " + runningTime + " ms.",
				runningTime <= this.maxRunningTime + this.clockResolution);
	}

}
