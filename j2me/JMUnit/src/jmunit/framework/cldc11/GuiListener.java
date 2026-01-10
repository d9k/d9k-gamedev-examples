/*
 * GuiListener.java
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

import javax.microedition.lcdui.Command;
import javax.microedition.lcdui.CommandListener;
import javax.microedition.lcdui.Display;
import javax.microedition.lcdui.Displayable;

/**
 * The GuiListener class displays the results of running a test as a green/red
 * bar together with the number of tests, failures and errors and the running
 * time.
 * 
 * @author C.A. Meijer
 * @since JMUnit 1.1.
 */
public class GuiListener implements TestListener, CommandListener {

	// The MIDlet that contains the tests that need to be run.
	private Test testMidlet;

	// The number of errors reported.
	private int numErrors;

	// The number of failures reported.
	private int numFailures;

	// The number of tests that have been run.
	private int numCompleted;

	// The Screen that shows a graphical view of the results.
	private Screen screen;

	// A list containing the errors and failures reported.
	private ExceptionList list;

	// The time that the tests started running.
	private long startTime;

	// The time that the tests stopped running.
	private long endTime;

	// This command allows a user to exit the MIDlet. It is attached to screen.
	private Command cmdScreenExit = new Command("Exit", Command.EXIT, 0);

	// This command allows a user to start running the tests. It is attached to
	// screen.
	private Command cmdScreenTest = new Command("Test", Command.OK, 1);

	// This command is attached to the list. If the user clicks it, the display
	// reverts to the screen.
	private Command cmdListOk = new Command("OK", Command.OK, 0);

	// A flag indicating whether the tests are running.
	private boolean running;

	/**
	 * 
	 * @param testMidlet
	 */
	public GuiListener(Test testMidlet) {
		this.testMidlet = testMidlet;

		// Create the screen for displaying the results graphically.
		this.screen = new Screen(testMidlet.getName(), this);
		this.screen.setCommandListener(this);
		this.screen.addCommand(this.cmdScreenExit);
		this.screen.addCommand(this.cmdScreenTest);
		Display.getDisplay(testMidlet).setCurrent(this.screen);

		// Create the list for displaying exceptions and errors.
		this.list = new ExceptionList(this);
		this.list.addCommand(this.cmdListOk);
		this.list.setCommandListener(this);
	}

	/**
	 * Appends the error to the list of failed test cases.
	 * 
	 * @param testClass
	 *            The Class associated with the test.
	 * 
	 * @param test
	 *            The test.
	 * 
	 * @param t
	 *            The error.
	 */
	public void addError(Class testClass, String test, Throwable t) {
		this.numErrors++;
		this.list.addError(test, t);
	}

	/**
	 * Appends the failure to the list of failed test cases.
	 * 
	 * @param testClass
	 *            The Class associated with the test.
	 * 
	 * @param test
	 *            The test.
	 * 
	 * @param t
	 *            The failure.
	 */
	public void addFailure(Class testClass, String test,
			AssertionFailedException t) {
		this.numFailures++;
		this.list.addFailure(test, t);
	}

	/**
	 * Refreshes the display to indicate that a test has finished running.
	 * 
	 * @param testClass
	 *            The Class associated with the test.
	 * 
	 * @param test
	 *            The test case that finished running.
	 */
	public void endTest(Class testClass, String test) {
		this.endTime = System.currentTimeMillis();
		this.numCompleted++;
		this.screen.repaint();

		if (this.numCompleted == this.testMidlet.countTestCases()) {
			this.running = false;

			// If there were any failures or errors, display them.
			if (this.numErrors + this.numFailures > 0) {
				this.list.displayResults(this.testMidlet);
			}
		}
	}

	/**
	 * The GuiListener ignores the fact that a test has started running.
	 */
	public void startTest(Class testClass, String test) {
	}

	/**
	 * Returns the total number of tests in the test suite.
	 * 
	 * @return The total number of tests.
	 */
	public int getNumberTests() {
		return this.testMidlet.countTestCases();
	}

	/**
	 * Returns the number of errors reported.
	 * 
	 * @return The number of errors.
	 */
	public int getNumberErrors() {
		return this.numErrors;
	}

	/**
	 * Returns the number of tests that have been run.
	 * 
	 * @return The number of tests.
	 */
	public int getNumberCompletedTests() {
		return this.numCompleted;
	}

	/**
	 * Returns the number of failures reported.
	 * 
	 * @return The number of failures.
	 */
	public int getNumberFailures() {
		return this.numFailures;
	}

	/**
	 * Returns the number successful tests run.
	 * 
	 * @return The number of successful tests.
	 */
	public int getNumberSuccessful() {
		return this.numCompleted - this.numErrors - this.numFailures;
	}

	/**
	 * Returns how long the tests have taken to run.
	 * 
	 * @return The running time.
	 */
	public long getRunningTime() {
		return this.endTime - this.startTime;
	}

	/**
	 * Responds to a user command.
	 * 
	 * @param cmd
	 *            The command.
	 * 
	 * @param disp
	 *            The display.
	 */
	public void commandAction(Command cmd, Displayable disp) {
		// If the user presses the OK button on the list, revert to the
		// graphical display of the test results.
		if (cmd.equals(this.cmdListOk)) {
			Display.getDisplay(this.testMidlet).setCurrent(this.screen);
			return;
		}

		// If the user want to exit, destroy the MIDlet.
		if (cmd.equals(this.cmdScreenExit)) {
			this.testMidlet.notifyDestroyed();
			return;
		}

		// The user must want run the tests. Ignore the request if the tests are
		// already running.
		if (!this.running) {
			new Thread(new Runnable() {
				public void run() {
					testMidlet.test();
				}
			}).start();
		}
	}

	/**
	 * Clears the status of any previous test runs.
	 */
	public void clear() {
		numErrors = 0;
		numFailures = 0;
		numCompleted = 0;
		startTime = System.currentTimeMillis();
		endTime = startTime;
		running = true;
		list.clearAll();
		screen.repaint();
	}
}
