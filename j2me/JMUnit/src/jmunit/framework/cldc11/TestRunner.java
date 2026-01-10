/*
 * TestRunner.java
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

import java.util.Date;
import java.util.Vector;

/**
 * The TestRunner class runs a test immediately on launching of a MIDlet. The
 * MIDlet shuts down shortly after the tests finish. This class should be used
 * 
 * @author C.A. Meijer
 * @since JMUnit 1.1.
 */
public abstract class TestRunner extends Test {

	// A delay period
	private int delay;

	/**
	 * Constructor.
	 * 
	 * @param delay
	 *            The delay in milliseconds after which the MIDlet should close
	 *            down.
	 * 
	 * @since JMUnit 1.1.
	 */
	public TestRunner(int delay) {
		this("TestRunner", delay);
	}

	/**
	 * Constructor.
	 * 
	 * @param name
	 *            The test name.
	 * 
	 * @param delay
	 *            The delay in milliseconds after which the MIDlet should close
	 *            down.
	 * 
	 * @since JMUnit 1.1.
	 */
	public TestRunner(String name, int delay) {
		super(name);
		if (delay > 0) {
			this.delay = delay;
		}
	}

	/**
	 * A TestRunner needs to run some wrapped Test. This method must return the
	 * test that needs to run.
	 * 
	 * Note: It would be better to pass the test to this class via the
	 * TestRunner constructor. While that works in the emulator it does not work
	 * on the Sony-Ericsson W200i (and, presumably other MIDs). This is a
	 * workaround.
	 * 
	 * @return The test to run on startup.
	 * 
	 * @since JMUnit 1.1.
	 */
	protected abstract Test getNestedTest();

	/**
	 * We override the doStart method to run the tests immediately.
	 * 
	 * @since JMUnit 1.1.
	 */
	public void doStart() {
		new Thread(new Runnable() {
			public void run() {
				// Run the tests immediately.
				test();

				// Pause before exiting; give the viewer a chance
				// to see the test results.
				try {
					Thread.sleep(delay);
				} catch (InterruptedException e) {
					// Ignore.
				}

				// Exit.
				notifyDestroyed();
			}
		}).start();
	}

	/**
	 * Counts the number of tests.
	 * 
	 * @return The number of test cases.
	 * 
	 * @since JMUnit 1.1.
	 */
	public int countTestCases() {
		return this.getNestedTest().countTestCases();
	}

	/**
	 * Runs and reports the tests.
	 * 
	 * @param results
	 *            The TestResult where the results are gathered.
	 * 
	 * @since JMUnit 1.1.
	 */
	public void run(TestResult results) {
		results.removeListeners();
		results.addListener(new GuiListener(this));
		results.addListener(new XmlTestResultsListener(this));
		results.clear();
		this.getNestedTest().run(results);
	}

	// An XmlTestResultListener writes an XML document to the standard output
	// when all tests have finished running. The document can be parsed by
	// the <junitreport> task.
	private static class XmlTestResultsListener implements TestListener {

		// The Test that launched the runner.
		private Test test;

		// The number of completed tests.
		private int numCompletedTests;

		// The XML elements in the document.
		private Vector xmlElements;

		// An exception reported by a test.
		private Throwable t;

		// The start time of the current test case.
		private long testCaseStartTime;

		// The start time of the whole test suite.
		private long testSuiteStartTime;

		// The number of errors and failures.
		private int numErrors, numFailures;

		public XmlTestResultsListener(Test test) {
			this.test = test;
			this.testSuiteStartTime = System.currentTimeMillis();
			this.xmlElements = new Vector();
		}

		public void addError(Class testClass, String test, Throwable t) {
			this.t = t;
			this.numErrors++;
		}

		public void addFailure(Class testClass, String test,
				AssertionFailedException t) {
			this.t = t;
			this.numFailures++;
		}

		public void clear() {
			this.xmlElements.removeAllElements();
			this.testSuiteStartTime = System.currentTimeMillis();
			this.numErrors = 0;
			this.numFailures = 0;
		}

		private String convertTimeToString(long time) {
			String timeString = "" + time;
			while (timeString.length() < 4) {
				timeString = "0" + timeString;
			}
			timeString = timeString.substring(0, timeString.length() - 3) + "."
					+ timeString.substring(timeString.length() - 3);

			return timeString;
		}

		public void endTest(Class testClass, String test) {
			long runningTime = (System.currentTimeMillis() - this.testCaseStartTime);
			String testCase = "<testcase classname=\"" + testClass.getName()
					+ "\" name=\"" + test + "\" time=\""
					+ convertTimeToString(runningTime) + "\">";
			this.xmlElements.addElement(testCase);
			if (this.t != null) {
				this.xmlElements.addElement(this.t);
			}
			this.xmlElements.addElement("</testcase>");
			this.numCompletedTests++;

			if (this.numCompletedTests == this.test.countTestCases()) {
				this.xmlElements.insertElementAt(
						"<?xml version=\"1.0\" encoding=\"UTF-8\"?>", 0);
				long suiteRunningTime = System.currentTimeMillis()
						- this.testSuiteStartTime;
				String suiteElement = "<testsuite errors=\"" + this.numErrors
						+ "\" failures=\"" + this.numFailures + "\" name=\""
						+ this.test.name + "\" tests=\""
						+ this.test.countTestCases() + "\" time=\""
						+ convertTimeToString(suiteRunningTime)
						+ "\" timestamp=\"" + new Date() + "\">";
				this.xmlElements.insertElementAt(suiteElement, 1);
				this.xmlElements.addElement("</testsuite>");
				System.out.println("<XmlTestResultsListener>");
				for (int i = 0; i < this.xmlElements.size(); i++) {
					Object element = this.xmlElements.elementAt(i);
					if (element instanceof Throwable) {
						Throwable t = (Throwable) element;
						String type = t instanceof AssertionFailedException ? "failure"
								: "error";
						System.out.println("<" + type + ">");
						t.printStackTrace();
						System.out.println("</" + type + ">");
					} else {
						System.out.println(element);
					}
				}
				System.out.println("</XmlTestResultsListener>");
			}
		}

		public void startTest(Class testClass, String test) {
			this.testCaseStartTime = System.currentTimeMillis();
			this.t = null;
		}

	}
}
