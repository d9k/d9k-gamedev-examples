/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package ru.d9k;
import javax.microedition.lcdui.Image;

/**
 *
 * @author d9k
 */
public class PngAnimation {
  private final Image[] imagesFrames;
	private final int framesCount;
	private int currentFrame = 0;
	private int elapsedSinceLastFrameMs = 0;
	public int delayMs = 450;

	public PngAnimation(Image[] imagesFrames) {
		this.imagesFrames = imagesFrames;
		this.framesCount = imagesFrames.length;
	}

	public void reset() {
		this.elapsedSinceLastFrameMs = 0;
		this.currentFrame = 0;
	}

	public void increaseTime(int deltaMs) {
		this.elapsedSinceLastFrameMs += deltaMs;
		if (this.elapsedSinceLastFrameMs > this.delayMs) {
			this.nextFrame();
		}
	}

	public void nextFrame() {
		this.currentFrame++;
		if (currentFrame >= framesCount) {
			currentFrame = 0;
		}
		this.elapsedSinceLastFrameMs = 0;
	}

	public Image currentImage() {
		return this.imagesFrames[this.currentFrame];
	}
}
