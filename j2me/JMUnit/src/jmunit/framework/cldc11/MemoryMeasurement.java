/*
 * MemoryMeasurement.java
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
 * The MemoryMeasurement class monitors how much memory is used by a test.
 * 
 * @author C.A. Meijer
 * @since JMUnit 1.2
 */
public class MemoryMeasurement implements PerformanceMeasurement {

	private long maxMemory;

	private long initFreeMemory;

	private boolean runGc;

	/**
	 * Constructor.
	 * 
	 * @param maxMemory
	 *            The maximum number of bytes that the test may put on the heap.
	 */
	public MemoryMeasurement(long maxMemory) {
		this(maxMemory, true);
	}

	/**
	 * 
	 * @param maxMemory
	 *            The maximum number of bytes that the test may put on the heap.
	 * @param runGarbageCollector
	 *            If true the GC is run before and after a test starts.
	 * 
	 */
	public MemoryMeasurement(long maxMemory, boolean runGarbageCollector) {
		this.runGc = runGarbageCollector;
		this.maxMemory = maxMemory;
		startMeasurement();
	}

	/**
	 * Starts monitoring.
	 */
	public void startMeasurement() {
		if (this.runGc) {
			System.gc();
		}
		this.initFreeMemory = Runtime.getRuntime().freeMemory();
	}

	/**
	 * Ends monitoring.
	 */
	public void endMeasurement() {
		if (this.runGc) {
			System.gc();
		}
		long memUsed = this.initFreeMemory - Runtime.getRuntime().freeMemory();
		Assertion.assertTrue("Test used too much memory: " + memUsed
				+ " bytes.", memUsed < this.maxMemory);
	}

}
