/*
 * PerformanceMeasurement.java
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
 * The PerformanceMeasurement interface is implemented by classes that want to
 * monitor the performance of a test class. Typically a class might monitor
 * speed or memory usage.
 * 
 * @author C.A. Meijer
 * @since JMUnit 1.2
 */
public interface PerformanceMeasurement {
	/**
	 * Starts the performance monitoring for a test in a TestCase.
	 */
	public void startMeasurement();

	/**
	 * Ends the monitoring of a test in a TestCase.
	 */
	public void endMeasurement();
}
