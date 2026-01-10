/*
 * TestSuite.java
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
 * The TestSuite class is responsible for execute many TestCases. As it extends
 * Test, it can be used as a MIDlet in a simulator. To use it, it's necessary to
 * create a subclass with a super() declaration in the constructor. The method
 * add(TestCase testCase) must be used in the constructor of the subclass,
 * adding the TestCases objects that are necessary to be runned. When everthing
 * is coded, the TestSuite can be used in the simulator.
 * 
 * @author Brunno Silva
 * @since JMUnit 1.0
 */
public class TestSuite extends Test {

	private Vector tests = new Vector();

	/**
	 * The default constructor. As such TestSuite can be added to MIDlet list as
	 * is and it will take list of classes to test from JMUnitTestClasses
	 * property.
	 * 
	 * @since JMUnit 1.0
	 */
	public TestSuite() {
		super("Default test suite");
		String prop = getAppProperty("JMUnitTestClasses");
		String[] classes;

		if (prop != null && !prop.equals("")) {
			classes = parseTestClassProperty(prop);

			for (int i = 0; i < classes.length; i++) {
				try {
					add((Test) Class.forName(classes[i]).newInstance());
					System.out.println("clazz: " + classes[i]);
				} catch (Exception e) {
					e.printStackTrace();
				}
			}
		}
	}

	/**
	 * 
	 * It must be called by the subclass constructor with a name parameter. It's
	 * also responsability of the subclass to add TestCase objects to be
	 * executed in it's constructor.
	 * 
	 * @param name
	 *            the name of the TestSuite.
	 * 
	 * @since JMUnit 1.0
	 */
	public TestSuite(String name) {
		super(name);
	}

	private String[] parseTestClassProperty(String property) {
		final String delimiter = " ";

		Vector classes = new Vector();

		while (property.length() > 0) {
			int i = property.indexOf(delimiter);
			if (i > 0) {
				classes.addElement(property.substring(0, i));
				property = property.substring(property.indexOf(delimiter) + 1);
			} else {
				classes.addElement(property);
				property = "";
			}
		}

		String[] result = new String[classes.size()];
		for (int i = 0; i < classes.size(); i++) {
			result[i] = (String) classes.elementAt(i);
			System.out.println("result " + i + ": " + result[i]);
		}

		return result;
	}

	/**
	 * The purpose of this method is store Tests.
	 * 
	 * Note: JMUnit 1.0 allowed only TestCases to be added.
	 * 
	 * @param test
	 *            the Test to be added.
	 * 
	 * @since JMUnit 1.1
	 */
	public final void add(Test test) {
		this.tests.addElement(test);
	}

	/**
	 * Counts the number of test cases that will be run by this test.
	 * 
	 * @since JMUnit 1.1
	 */
	public int countTestCases() {
		int total = 0;
		for (int i = 0; i < this.tests.size(); i++) {
			Test test = (Test) this.tests.elementAt(i);
			total += test.countTestCases();
		}
		return total;
	}

	/**
	 * Runs a test and collects its result in a TestResult instance.
	 * 
	 * @since JMunit 1.1
	 */
	public void run(TestResult result) {
		for (int i = 0; i < this.tests.size(); i++) {
			Test test = (Test) this.tests.elementAt(i);
			test.run(result);
		}
	}

	/**
	 * Removes all tests from the TestSuite.
	 * 
	 * @since JMunit 1.1
	 */
	public void removeAll() {
		this.tests.removeAllElements();
	}
}