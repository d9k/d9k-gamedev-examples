package com.foo;

public class Bar {
	public String reverseCharacters(String s) {
		StringBuffer buf = new StringBuffer();
		for (int i = s.length() - 1; i >= 0; i--) {
			buf.append(s.charAt(i));
		}
		return buf.toString();
	}
}
