/*
 * Screen.java
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

import javax.microedition.lcdui.Canvas;
import javax.microedition.lcdui.Font;
import javax.microedition.lcdui.Graphics;

/**
 * The purpose of this class is generate all the framework's interface. It's a
 * sub-class of canvas that prints the necessary informations in the simulator
 * screen.
 * 
 * @author Brunno Silva
 * @since JMUnit 1.0
 */
public final class Screen extends Canvas {
	private static final String passed = "Pass:";
	private static final String failed = "Fail:";
	private static final String error = "Error:";
	private static final String total = "Total:";
	private static final String time = "Time:";
	private final Font font = Font.getFont(Font.FACE_SYSTEM, Font.STYLE_BOLD,
			Font.SIZE_LARGE);
	private final int width = getWidth();
	private final int height = getHeight();
	private final int verticalAlignment = (width / 8);
	private final int verticalResultAlignment = ((width * 6) / 10);
	private final int barPosition = (height / 3);
	private final int barWidth = ((width * 3) / 4);
	private final int barHeight = (height / 20);
	private final int passedHeight = ((height * 45) / 100);
	private final int failedHeight = ((height * 55) / 100);
	private final int errorHeight = ((height * 65) / 100);
	private final int totalHeight = ((height * 75) / 100);
	private final int elapsedHeight = ((height * 85) / 100);
	private final int jmunitX = ((width - font.stringWidth(JMUnit.getVersion())) / 2);
	private final int jmunitY = (height / 8);
	private final int anchor = Graphics.TOP | Graphics.LEFT;
	private String name;
	private GuiListener result;
	private int nameX;
	private int nameY;

	/**
	 * The default constructor. It initializes some variables and configures the
	 * interface's commands.
	 * 
	 * @param name
	 *            The name of the test.
	 * @param result
	 *            The GuiListener that maintains information about the tests
	 *            that are being run.
	 * 
	 * @since JMUnit 1.1
	 */
	public Screen(String name, GuiListener result) {
		this.name = name;
		this.result = result;
		nameX = ((width - font.stringWidth(name)) / 2);
		nameY = (height / 5);
	}

	/**
	 * The paint method of Screen is the responsible for painting the interface.
	 * It overrides the paint method of Canvas. It's called by the framework
	 * when it's started and in ever test method execution. Currently, it draws
	 * the whole screen in all the cases, but in future versions, the paint
	 * method is going to change only some parts, for perfomance tuning.
	 * 
	 * @since JMUnit 1.0
	 */
	protected void paint(Graphics graphics) {
		graphics.setColor(255, 255, 255);
		graphics.fillRect(0, 0, width, height);

		if ((result.getNumberErrors() == 0)
				&& (result.getNumberFailures() == 0)) {
			graphics.setColor(0, 255, 0);
		} else {
			graphics.setColor(255, 0, 0);
		}

		if (result.getNumberTests() != 0) {
			graphics.fillRect(verticalAlignment, barPosition,
					(barWidth * result.getNumberCompletedTests())
							/ result.getNumberTests(), barHeight);
		}

		graphics.setColor(0, 0, 0);
		graphics.drawRect(verticalAlignment, barPosition, barWidth, barHeight);
		graphics.setFont(font);
		graphics.drawString(JMUnit.getVersion(), jmunitX, jmunitY, anchor);
		graphics.drawString(name, nameX, nameY, anchor);
		graphics.drawString(passed, verticalAlignment, passedHeight, anchor);
		graphics.drawString(failed, verticalAlignment, failedHeight, anchor);
		graphics.drawString(error, verticalAlignment, errorHeight, anchor);
		graphics.drawString(total, verticalAlignment, totalHeight, anchor);
		graphics.drawString(time, verticalAlignment, elapsedHeight, anchor);
		graphics.drawString(String.valueOf(result.getNumberSuccessful()),
				verticalResultAlignment, passedHeight, anchor);
		graphics.drawString(String.valueOf(result.getNumberFailures()),
				verticalResultAlignment, failedHeight, anchor);
		graphics.drawString(String.valueOf(result.getNumberErrors()),
				verticalResultAlignment, errorHeight, anchor);
		graphics.drawString(String.valueOf(result.getNumberTests()),
				verticalResultAlignment, totalHeight, anchor);
		graphics.drawString(String.valueOf(result.getRunningTime()) + "ms",
				verticalResultAlignment, elapsedHeight, anchor);
	}
}