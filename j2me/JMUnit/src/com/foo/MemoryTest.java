package com.foo;

import jmunit.framework.cldc11.MemoryMeasurement;
import jmunit.framework.cldc11.TestCase;

public class MemoryTest extends TestCase {

	public void test100Bytes() {
		byte[] b = new byte[100];
	}
	
	public void test40Bytes() {
		byte[] b = new byte[40];
	}
	
	public void test60Bytes() {
		byte[] b = new byte[60];
	}
	
	public MemoryTest() {
		super(3, "MemoryTest");
		addPerformanceMeasurement(new MemoryMeasurement(80, false));
	}
	
	public void test(int testNum) throws Throwable {
		switch (testNum) {
		case 0:
			test100Bytes();
			break;
			
		case 1:
			test40Bytes();
			break;
	
		case 2:
			test60Bytes();
			break;
			
		default:
			fail("No such test.");
		}
	}

}
