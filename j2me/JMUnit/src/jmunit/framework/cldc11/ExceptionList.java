/*
 * ExceptionList.java
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

import javax.microedition.lcdui.Display;
import javax.microedition.lcdui.List;
import javax.microedition.midlet.MIDlet;
import java.util.Vector;

/**
 * The ExceptionList class displays errors and failures reported to a
 * GuiListener. The class was heavily influenced by the List used in J2MEUnit
 * for displaying test results.
 * 
 * @author C.A. Meijer
 * @since JMUnit 1.1.
 */
public class ExceptionList extends List {

	private GuiListener listener;

	// A vector that contains errors.
	Vector errors;

	// A vector that contains failures.
	Vector failures;

	/**
	 * Constructor.
	 * 
	 * @param listener
	 *            The GuiListener associated with this list.
	 */
	public ExceptionList(GuiListener listener) {
		super(JMUnit.getVersion(), List.IMPLICIT);
		this.listener = listener;
		this.errors = new Vector();
		this.failures = new Vector();
	}

	/**
	 * Removes all messages displayed by the list.
	 */
	public void clearAll() {
		while (this.size() != 0) {
			this.delete(0);
		}
		this.errors.removeAllElements();
		this.failures.removeAllElements();
	}

	// A method that appends a String to the list without an image.
	private void append(String s) {
		super.append(s, null);
	}

	// A method that appends a list of failures or errors to the list.
	private void displayExceptions(String type, Vector v) {
		if (v.size() == 0) {
			return;
		}

		String message = "There was 1" + type + ":";
		if (v.size() != 1) {
			message = "There were " + v.size() + type + "s:";
		}
		append(message);
		for (int i = 1; i <= v.size(); i++) {
			append("" + i + ")");

			// The exception is stored as 2 Strings: the test name and a
			// message.
			String[] s = (String[]) v.elementAt(i - 1);
			append(s[0]);
			append(s[1]);
		}
	}

	/**
	 * Displays the list of errors and failures.
	 */
	public void displayResults(MIDlet midlet) {
		append("Test Results:");
		append("Passed: " + this.listener.getNumberSuccessful());
		append("Failures: " + this.listener.getNumberFailures());
		append("Errors: " + this.listener.getNumberErrors());

		displayExceptions(" error", this.errors);
		displayExceptions(" failure", this.failures);

		append("");
		append("Display based on J2MEUnit.");

		Display.getDisplay(midlet).setCurrent(this);
	}

	// A method to add a failure or error to the failures or errors Vector.
	private void addFailureError(String test, Throwable t, Vector v) {
		String message = t.getClass().getName();
		if (t.getMessage() != null) {
			message = t.getMessage();
		}
		v.addElement(new String[] { test, message });
	}

	/**
	 * Adds an error to be displayed on the list.
	 * 
	 * @param test
	 *            The test that reported an error.
	 * 
	 * @param t
	 *            The error.
	 */
	public void addError(String test, Throwable t) {
		this.addFailureError(test, t, this.errors);
	}

	/**
	 * Adds a failure to be displayed on the list.
	 * 
	 * @param test
	 *            The test that reported an error.
	 * 
	 * @param t
	 *            The failure
	 */
	public void addFailure(String test, Throwable t) {
		this.addFailureError(test, t, this.failures);
	}
}
