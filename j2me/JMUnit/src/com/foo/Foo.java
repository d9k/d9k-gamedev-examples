package com.foo;

public class Foo {
	
	private boolean empty = true;
	
	public void add(Object o) {
		this.empty = false;
	}
	
	public int add(int i, int j) {
		return i + j;
	}

	public boolean isEmpty() {
		return empty;
	}

	public void setEmpty(boolean empty) {
		this.empty = empty;
	}
}
