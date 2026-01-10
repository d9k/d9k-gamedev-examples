/*
 * TestCase.java
 *  
 * Copyright 2006 Brunno Silva
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

import java.util.Vector;

/**
 * The principal class in the framework. All your test classes must extend this
 * one. It's purpose is to encapsulate all the tests methods of the application
 * or a specific group of them. The framework then use it to execute all.
 * 
 * @author Brunno
 * @since JMUnit 1.0
 */
public abstract class TestCase extends Test {
	private int totalOfTests;

	// A PerformanceMeasurement collaborator that wants to monitor the
	// performance of a test.
	private Vector perfMeasurements;

	// Are performance measurements running?
	private boolean perfMeasurementsRunning;

	/**
	 * The default constructor. It just transmits the necessary informations to
	 * the superclass.
	 * 
	 * @param totalOfTests
	 *            the total of test methods present in the class.
	 * @param name
	 *            this testcase's name.
	 * 
	 * @since JMUnit 1.0
	 */
	public TestCase(int totalOfTests, String name) {
		super(name);
		this.totalOfTests = totalOfTests;
		this.perfMeasurements = new Vector();
	}

	/**
	 * This method stores all the test methods invocation. The developer must
	 * implement this method with a switch-case. The cases must start from 0 and
	 * increase in steps of one until the number declared as the total of tests
	 * in the constructor, exclusive. For example, if the total is 3, the cases
	 * must be 0, 1 and 2. In each case, there must be a test method invocation.
	 * 
	 * @param testNumber
	 *            the test to be executed.
	 * @throws Throwable
	 *             anything that the executed test can throw.
	 * 
	 * @since JMUnit 1.0
	 */
	public abstract void test(int testNumber) throws Throwable;

	/**
	 * A empty method used by the framework to initialize the tests. If there's
	 * 5 test methods, the setUp is called 5 times, one for each method. The
	 * setUp occurs before the method's execution, so the developer can use it
	 * to any necessary initialization. It's necessary to override it, however.
	 * 
	 * @throws Throwable
	 *             anything that the initialization can throw.
	 * 
	 * @since JMUnit 1.0
	 */
	public void setUp() throws Throwable {

	}

	/**
	 * A empty mehod used by the framework to release resources used by the
	 * tests. If there's 5 test methods, the tearDown is called 5 times, one for
	 * each method. The tearDown occurs after the method's execution, so the
	 * developer can use it to close something used in the test, like a
	 * InputStream or the RMS. It's necessary to override it, however.
	 * 
	 * @since JMUnit 1.0
	 */
	public void tearDown() {

	}

	/**
	 * Runs a test and collects its result in a TestResult instance.
	 * 
	 * @param result
	 *            The TestResult collecting the results.
	 * 
	 * @since JMunit 1.1
	 */
	public final void run(TestResult result) {
		for (int i = 0; i < this.totalOfTests; i++) {
			String testName = this.name + "#" + i;
			result.startTest(this.getClass(), testName);
			try {
				setUp();
				startPerformanceMeasurements();
				test(i);
				endPerformanceMeasurements();
			} catch (Throwable e) {
				result.addError(this.getClass(), testName, e);
			}
			try {
				tearDown();
			} catch (Throwable e) {
				result.addError(this.getClass(), testName, e);
			}
			result.endTest(this.getClass(), testName);
		}
	}

	/**
	 * Returns the number of tests in TestCase.
	 * 
	 * @since JMunit 1.1
	 */
	public int countTestCases() {
		return this.totalOfTests;
	}

	/**
	 * Adds a <code>PerformanceMeasurement</code> that wants to measure the
	 * performance of one or more tests in this TestCase.
	 * 
	 * @param perfMeasurement
	 *            The PerformanceMeasurement object to observe the test.
	 * 
	 * @since JMunit 1.2
	 */
	protected final void addPerformanceMeasurement(PerformanceMeasurement perfMeasurement) {
		this.perfMeasurements.addElement(perfMeasurement);
	}

	/**
	 * Removes a <code>PerformanceMeasurement</code> that was observing the
	 * performance of a test.
	 * 
	 * @param perfMeasurement
	 *            The PerformanceMeasurement object to remove.
	 * 
	 * @since JMunit 1.2
	 */
	protected final void removePerformanceMeasurement(
			PerformanceMeasurement perfMeasurement) {
		this.perfMeasurements.removeElement(perfMeasurement);
	}

	/**
	 * Removes all <code>PerformanceMeasurement</code> objects that were
	 * observing the performance of a test.
	 * 
	 * @since JMunit 1.2
	 */
	protected final void removeAllPerformanceMeasurements() {
		this.perfMeasurements.removeAllElements();
	}

	/**
	 * This method starts the performance measurements.
	 */
	protected final void startPerformanceMeasurements() {
		this.perfMeasurementsRunning = true;
		for (int i = 0; i < this.perfMeasurements.size(); i++) {
			((PerformanceMeasurement) this.perfMeasurements.elementAt(i))
					.startMeasurement();
		}
	}

	/**
	 * This method ends performance measurements. This method should only be
	 * called once. Calling this method more than once will be ignored.
	 */
	protected final void endPerformanceMeasurements() {
		if (!this.perfMeasurementsRunning) {
			return;
		}
		this.perfMeasurementsRunning = false;
		for (int i = this.perfMeasurements.size() - 1; i >= 0; i--) {
			((PerformanceMeasurement) this.perfMeasurements.elementAt(i))
					.endMeasurement();
		}
	}
}