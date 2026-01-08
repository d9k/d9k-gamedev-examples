package ru.d9k;

import javax.microedition.lcdui.Display;
import javax.microedition.lcdui.Form;
import javax.microedition.midlet.MIDlet;

public class PngAnimationMIDlet extends MIDlet {
    protected void startApp() {
//        Form f = new Form("ru.d9k.PngAnimationMIDlet");
        Form f = new Form("PngAnimationMIDlet");
        f.append("Your MIDlet seems to run.");
        Display.getDisplay(this).setCurrent(f);
    }

    protected void pauseApp() {
    }

    protected void destroyApp(boolean unconditional) {
    }
}
