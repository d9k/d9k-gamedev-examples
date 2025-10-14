package ru.d9k;

import javax.microedition.lcdui.Display;
import javax.microedition.midlet.MIDlet;
import java.io.IOException;

/*
 * This is the main class of the hello world demo.
 */
public class PngAnimationMIDlet extends MIDlet {

	private GameCanvas gameCanvas;

	/**
	 * This is constructor of class HelloWorld. The Command objects appear as
	 * buttons.
	 */
	public PngAnimationMIDlet() {

	}
	//  implementation of MIDlet

	/**
	 * Start the application.
	 */
	protected void startApp() {
		try {
			gameCanvas = new GameCanvas();
			Display.getDisplay(this).setCurrent(gameCanvas);
			new Thread(gameCanvas).start();
		} catch (IOException e) {

		}
	}

	/**
	 * This method is called to notify the MIDlet to enter a paused state. The
	 * MIDlet should use this opportunity to release shared resources.
	 */
	protected void pauseApp() {
	}

	/**
	 * If the MIDlet was using resources, it should release them in this method.
	 */
	protected void destroyApp(boolean bool) {
	}
}

