/*
 * TestResult.java
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

import java.util.Vector;

/**
 * The TestResult class collects test results and relays them to TestListeners.
 * 
 * @author C.A. Meijer
 * @since JMUnit 1.1.
 */
public class TestResult {

	// The TestListeners.
	private Vector listeners;

	// An error that has been reported for the current test.
	private Throwable testError;

	/**
	 * The constructor.
	 */
	public TestResult() {
		this.listeners = new Vector();
	}

	/**
	 * Registers a TestListener
	 * 
	 * @param listener
	 *            The listener.
	 * 
	 * @since JMUnit 1.1.
	 */
	public void addListener(TestListener listener) {
		this.listeners.addElement(listener);
	}

	/**
	 * Unregisters all TestListeners
	 * 
	 * @since JMUnit 1.1.
	 */
	public void removeListeners() {
		this.listeners.removeAllElements();
	}

	/**
	 * An error or failure occurred. Note that, unlike JUnit, the TestResult's
	 * API doesn't distinguish between failures and errors. Kent Beck has
	 * written that he's not sure that the distinction between errors and
	 * failures is worthwhile since it complicates the interface. We still draw
	 * the distinction for TestListeners though.
	 * 
	 * @param testClass
	 *            the Class associated with the test.
	 * 
	 * @param test
	 *            The test.
	 * 
	 * @param t
	 *            The error.
	 * 
	 * @since JMUnit 1.1.
	 */
	public void addError(Class testClass, String test, Throwable t) {
		// If we've already reported a test failure for this test, ignore.
		if (this.testError != null) {
			return;
		}

		this.testError = t;
		for (int i = 0; i < this.listeners.size(); i++) {
			TestListener listener = (TestListener) this.listeners.elementAt(i);
			try {
				if (t instanceof AssertionFailedException) {
					listener.addFailure(testClass, test,
							(AssertionFailedException) t);
				} else {
					listener.addError(testClass, test, t);
				}
			} catch (Throwable th) {
				// Ignore. If a listener throws an error we should still keep
				// other listeners informed.
			}
		}
	}

	/**
	 * A test ended.
	 * 
	 * @param testClass
	 *            the Class associated with the test.
	 * 
	 * @param test
	 *            The test.
	 * 
	 * @since JMUnit 1.1.
	 */
	public void endTest(Class testClass, String test) {
		for (int i = 0; i < this.listeners.size(); i++) {
			TestListener listener = (TestListener) this.listeners.elementAt(i);
			try {
				listener.endTest(testClass, test);
			} catch (Throwable t) {
				// Ignore.
			}
		}
	}

	/**
	 * A test started.
	 * 
	 * @param testClass
	 *            the Class associated with the test.
	 * 
	 * @param test
	 *            The test.
	 * 
	 * @since JMUnit 1.1.
	 */
	public void startTest(Class testClass, String test) {
		this.testError = null;
		for (int i = 0; i < this.listeners.size(); i++) {
			TestListener listener = (TestListener) this.listeners.elementAt(i);
			try {
				listener.startTest(testClass, test);
			} catch (Throwable t) {
				// Ignore.
			}
		}
	}

	/**
	 * Clears the results of all the tests.
	 * 
	 * @since JMUnit 1.1.
	 */
	public void clear() {
		for (int i = 0; i < this.listeners.size(); i++) {
			TestListener listener = (TestListener) this.listeners.elementAt(i);
			try {
				listener.clear();
			} catch (Throwable t) {
				// Ignore.
			}
		}
	}
}
