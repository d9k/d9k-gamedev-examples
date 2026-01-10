import jmunit.framework.cldc11.Test;
import jmunit.framework.cldc11.TestRunner;
import com.foo.Suite;

public class JMUnitMIDlet extends TestRunner {

    private Test nestedTest;

    /**
     * Fix for freej2me emulator: it can't find startApp() in grandparent class (TestRunner extends Test), Test::startApp()
     * */
    public void startApp() {
        createTestResult();

        // Call the template method.
        doStart();
    }

    public JMUnitMIDlet() {
//        super(3000);
        super(15000);
        this.nestedTest = new Suite();
    }

    protected Test getNestedTest() {
        return this.nestedTest;
    }

}
