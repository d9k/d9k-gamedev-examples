package com.foo;

import jmunit.framework.cldc11.TestSuite;

public class Suite extends TestSuite {
	public Suite() {
		super("All Tests");
		add(new FooTest());
		add(new BarTest());
		add(new SpeedTest());
		add(new MemoryTest());
	}
}
