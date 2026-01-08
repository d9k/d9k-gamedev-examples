package ru.d9k;
import java.io.IOException;
import javax.microedition.lcdui.Image;

public class ImagesManager {
   public Image[] getPigRightArray() throws IOException {
		 int len = 4;
//		 int len = 2;
		 Image[] result = new Image[len];

		 for (int ix = 0; ix < len; ix++) {
			 int fileNumber = ix + 1;
			 System.out.println("ix=" + ix +", fileNumber=" + fileNumber);

			 /**
				* TODO Without Thread.sleep() there is runtime error on Kemulator
				* (https://github.com/shinovon/KEmulator/issues/270)
				* when built with NetBeans project.
				* (No error with IDEA + Kemulator integration)
				*/

//				try {
//					Thread.sleep(10);
//				} catch (InterruptedException e) {
//				}
		   result[ix] = Image.createImage("/characters/pig-right-" + fileNumber + ".png");
//			 result[ix] = Image.createImage("/characters/pig-right-1.png");
		 }

		 return result;
	 }
}
