/*
 * TestListener.java
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
 * The TestListener interface defines methods that a class that wants to know
 * the results of a test must implement.
 * 
 * @author C.A. Meijer
 * @since JMUnit 1.1
 */
public interface TestListener {

	/**
	 * An error occurred.
	 * 
	 * @param testClass
	 *            The Class associated with the test.
	 * 
	 * @param test
	 *            The test that reported an error.
	 * 
	 * @param t
	 *            The error.
	 * 
	 * @since JMUnit 1.1.
	 */
	public void addError(Class testClass, String test, Throwable t);

	/**
	 * An failure occurred.
	 * 
	 * @param testClass
	 *            The Class associated with the test.
	 * 
	 * @param test
	 *            The test that reported a failure.
	 * 
	 * @param t
	 *            The failure.
	 * 
	 * @since JMUnit 1.1.
	 */
	public void addFailure(Class testClass, String test,
                           AssertionFailedException t);

	/**
	 * A test ended.
	 * 
	 * @param testClass
	 *            The Class associated with the test.
	 * 
	 * @param test
	 *            The test that ended.
	 * 
	 * @since JMUnit 1.1.
	 */
	public void endTest(Class testClass, String test);

	/**
	 * A test started.
	 * 
	 * @param testClass
	 *            The Class associated with the test.
	 * 
	 * @param test
	 *            The test that started.
	 * 
	 * @since JMUnit 1.1.
	 */
	public void startTest(Class testClass, String test);

	/**
	 * The test results have been cleared.
	 * 
	 * @since JMUnit 1.1.
	 */
	public void clear();
}
