/*
 * Test.java
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

/**
 * The Test is a abstract class that has the main implementation to create a
 * executing test class or a utility class to execute others. The MIDlet methods
 * as startApp are localized here.
 * 
 * @author Brunno Silva
 * @since JMUnit 1.0
 */
public abstract class Test extends Assertion {
	/**
	 * The name of the test.
	 */
	protected String name;

	/**
	 * The TestResult object that will handle test result updates.
	 */
	protected TestResult results;

	/**
	 * The constructor.
	 * 
	 * @param name
	 *            the name of the executing class.
	 * 
	 * @since JMUnit 1.0
	 */
	public Test(String name) {
		if (name == null) {
			this.name = "JMUnit Test";
		} else {
			this.name = name;
		}
	}

	// Create the test results.
//	private void createTestResult() {
	protected void createTestResult() {
		if (results == null) {
			results = new TestResult();
			results.addListener(new ConsoleListener());
			results.addListener(new GuiListener(this));
		}
	}

	/**
	 * The startApp puts the screen in the simulator interface.
	 * 
	 * @since JMUnit 1.0
	 */
//	public final void startApp() {
	public void startApp() {
		createTestResult();

		// Call the template method.
		doStart();
	}

	/**
	 * The doStart method is a template method that is called when an app is
	 * started. A class should override it if it needs to do something special
	 * when a MIDlet starts.
	 * 
	 * @since JMUnit 1.1
	 */
	public void doStart() {
	}

	/**
	 * It's an empty method.
	 * 
	 * @since JMUnit 1.0
	 */
	public final void pauseApp() {

	}

	/**
	 * It's an empty method.
	 * 
	 * @param unconditional
	 *            an irrelevant boolean.
	 * @since JMUnit 1.0
	 */
	public final void destroyApp(boolean unconditional) {
	}

	/**
	 * This convenience method runs all the tests and creates a TestResult if
	 * necessary.
	 * 
	 * @since JMUnit 1.0
	 */
	public final void test() {
		createTestResult();
		results.clear();
		run(results);
	}

	/**
	 * This method gets the names of the test.
	 * 
	 * @return The test name.
	 * 
	 * @since JMUnit 1.1.
	 */
	public final String getName() {
		return this.name;
	}

	/**
	 * Counts the number of test cases that will be run by this test.
	 * 
	 * @since JMUnit 1.1
	 */
	public abstract int countTestCases();

	/**
	 * Runs a test and collects its result in a TestResult instance.
	 * 
	 * @since JMunit 1.1
	 */
	public abstract void run(TestResult result);
}
