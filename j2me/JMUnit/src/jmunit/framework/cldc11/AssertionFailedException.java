/*
 * AssertionFailedException.java
 *  
 * Copyright 2006 Brunno Silva
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package jmunit.framework.cldc11;

/**
 * A sub-class of RuntimeException used by the framework to manage a fail in a
 * assertion.
 * 
 * @author Brunno Silva
 * @since JMUnit 1.0
 */
public final class AssertionFailedException extends RuntimeException {
	/**
	 * The default constructor.
	 * 
	 * @since JMUnit 1.0
	 */
	public AssertionFailedException() {
	}

	/**
	 * A constructor that takes a message that describes the exception in more
	 * detail.
	 * 
	 * @param message
	 *            An informative description of the exception.
	 * 
	 * @since JMUnit 1.1
	 */
	public AssertionFailedException(String message) {
		super(message);
	}

}