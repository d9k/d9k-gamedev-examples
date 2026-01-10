/*
 * ConsoleListener.java
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
 * The ConsoleListener class listens to test results and writes any failures and
 * errors to the console.
 * 
 * @author C.A. Meijer
 * @since JMUnit 1.1.
 */
public class ConsoleListener implements TestListener {

	/**
	 * Prints the details of an error.
	 * 
	 * @param testClass
	 *            The Class associated with the test.
	 * 
	 * @param test
	 *            The test that reported an error.
	 * 
	 * @param t
	 *            The error.
	 */
	public void addError(Class testClass, String test, Throwable t) {
		System.out.println(test);
		t.printStackTrace();
	}

	/**
	 * Prints the details of a failure.
	 * 
	 * @param testClass
	 *            The Class associated with the test.
	 * 
	 * @param test
	 *            The test that reported a failure.
	 * 
	 * @param t
	 *            The failure.
	 */
	public void addFailure(Class testClass, String test,
			AssertionFailedException t) {
		addError(testClass, test, t);
	}

	/**
	 * The ConsoleListener ignores any notifications that a test has ended.
	 * 
	 * @param testClass
	 *            The Class associated with the test.
	 * 
	 * @param test
	 *            The test that finished running.
	 */
	public void endTest(Class testClass, String test) {
	}

	/**
	 * The ConsoleListener ignores any notifications that a test has started.
	 * 
	 * @param testClass
	 *            The Class associated with the test.
	 * 
	 * @param test
	 *            The test that started running.
	 */
	public void startTest(Class testClass, String test) {
	}

	/**
	 * The ConsoleListener ignores any notifications that the results have been
	 * cleared.
	 * 
	 */
	public void clear() {
	}

}
