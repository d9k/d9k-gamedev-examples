package com.foo;

import jmunit.framework.cldc11.TestCase;

public class BarTest extends TestCase {

	public BarTest() {
		super(2, "BarTest");
	}

	public void testReverse(String arg, String expected) {
		Bar bar = new Bar();
		assertEquals(expected, bar.reverseCharacters(arg));
	}

	public void test(int testNum) throws Throwable {
		switch (testNum) {
		case 0:
			testReverse("abc", "cba");
			break;

		case 1:
			testReverse("Able was I ere I saw Elba",
					"ablE was I ere I saw elbA");
		}
	}

}
