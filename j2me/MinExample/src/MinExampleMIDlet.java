
import javax.microedition.midlet.*;
import javax.microedition.lcdui.*;

/*
 * This is the main class of the hello world demo.
 */

public class MinExampleMIDlet extends MIDlet implements CommandListener
{
  private Command exitCommand;
  private TextBox tb;

  /**
  * This is constructor of class HelloWorld.
  * The Command objects appear as buttons.
  */
  public MinExampleMIDlet()
  {
    exitCommand = new Command("Exit", Command.EXIT, 1);
    tb = new TextBox("Hello world MIDlet", "Hello World!", 25, 0);
  }
  //  implementation of MIDlet
  /**
  * Start the application.
  */
  protected void startApp()
  {
    tb.addCommand(exitCommand);
    tb.setCommandListener(this);
    Display.getDisplay(this).setCurrent(tb);
  }
  /**
  * This method is called to notify the MIDlet to enter a paused
  * state.  The MIDlet should use this opportunity to release
  * shared resources.
  */
  protected void pauseApp() {}

  /**
  * If the MIDlet was using resources, it should release
  * them in this method.
  */
  protected void destroyApp(boolean bool) {}

  public void commandAction(Command cmd, Displayable disp)
  {
    if (cmd == exitCommand) {
      destroyApp(false);
      notifyDestroyed();
    }
  }
}