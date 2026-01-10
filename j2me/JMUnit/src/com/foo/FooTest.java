package com.foo;

import jmunit.framework.cldc11.TestCase;

public class FooTest extends TestCase {

	// The OUT.
	private Foo foo;
	
	public void setUp() {
		this.foo = new Foo();
	}
	
	public FooTest() {
		super(3, "FooTest");
	}

	public void test(int testNumber) throws Throwable {
		switch (testNumber) {
		case 0:
			testIsEmpty();
			break;

		case 1:
			testAdd(3, 5, 8);
			break;

		case 2:
			testAdd(2, 1, 3);
			break;

		default:
			break;
		}
	}

	public void testIsEmpty() {
		assertTrue(foo.isEmpty());
		foo.add(new Object());
		assertFalse(foo.isEmpty());
	}

	public void testAdd(int addend, int augend, int expected) {
		int actual = foo.add(addend, augend);
		assertEquals("testAdd", expected, actual);
	}
}
