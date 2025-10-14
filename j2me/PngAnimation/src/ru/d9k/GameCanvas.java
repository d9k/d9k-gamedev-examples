package ru.d9k;

import java.io.IOException;
import javax.microedition.lcdui.Canvas;
import javax.microedition.lcdui.Graphics;
import javax.microedition.lcdui.Image;

class GameCanvas extends Canvas implements Runnable {

	private boolean running = true;

	// Переменные для FPS
	private long lastTime = 0;
	private long deltaTime;
	private int fps;
	private int frameCount;
	private long fpsTime;

  private int screenWidth, screenHeight;

	private final int MIN_FRAME_TIME_MS = 4;
	private final int MAX_FRAME_TIME_MS = 200;

	// Игровые переменные в процентах от пикселя
	private int x = 50*100, y = 50*100;
	private int speedXPxPInS = 80*100, speedYPxPInS = 30*100;

	private final Image pig1;

	public GameCanvas() throws IOException {
		screenWidth = getWidth();
		screenHeight = getHeight();

		// setFullScreenMode(true);
		lastTime = System.currentTimeMillis();

		pig1 = Image.createImage("/characters/pig-right-1.png");
	}

	public void run() {
		while (running) {
			// Вычисление delta time
			long currentTime = System.currentTimeMillis();
			if (lastTime == 0) {
				lastTime = currentTime;
				continue;
			}
			deltaTime = currentTime - lastTime;

			// Обновление логики игры с учетом deltaTime

			if (deltaTime < MIN_FRAME_TIME_MS) {
				try {
				  Thread.sleep(MIN_FRAME_TIME_MS - deltaTime);
				} catch (InterruptedException e) {
				}
				continue;
			}

			if (deltaTime > MAX_FRAME_TIME_MS) {
				deltaTime = MAX_FRAME_TIME_MS;
			}

			lastTime = currentTime;

			// Подсчет FPS
			frameCount++;
			fpsTime += deltaTime;

			if (fpsTime >= 1000) {
				fps = frameCount;
				frameCount = 0;
				fpsTime = 0;
			}

			update(deltaTime);

			// Отрисовка
			repaint();
			//serviceRepaints();
		}
	}

	private void update(long deltaMs) {
		// Преобразуем delta в секунды для удобства
		//float deltaSeconds = deltaMs / 1000.0f;

		// Обновление позиции с учетом времени
		int dx = (int) (speedXPxPInS * deltaMs / 1000);
		int dy = (int) (speedYPxPInS * deltaMs / 1000);

		x += dx;
		y += dy;

		System.out.println("dx=" + dx+ " dy=" + dy + " deltaMs=" + deltaMs);

		// Teleport on edges
		if (x < -10 * 100) {
			x = screenWidth * 100;
		}
		if (x > screenWidth * 100) {
			x = 0;
		}
		if (y < -10 * 100) {
			y = screenHeight * 100;
		}
		if (y > screenHeight * 100) {
			y = 0;
		}
	}

	protected void paint(Graphics g) {
		// Очистка экрана
		g.setColor(0x000000);
		g.fillRect(0, 0, screenWidth, screenHeight);

		// Отрисовка объекта
		//g.setColor(0xFFFFFF);
		//g.fillRect(x, y, 20, 20);

		// Отображение FPS
		g.setColor(0x00FF00);
		g.drawString("FPS: " + fps, 0, 0, Graphics.TOP | Graphics.LEFT);
		g.drawImage(pig1, x / 100, y / 100, Graphics.TOP | Graphics.LEFT);
		//g.drawString("Delta: " + deltaTime + "ms", 5, 20, Graphics.TOP | Graphics.LEFT);
	}

	public void stop() {
		running = false;
	}
}
