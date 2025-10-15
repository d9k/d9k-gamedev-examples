package ru.d9k;
import java.io.IOException;
import javax.microedition.lcdui.Image;

public class ImagesManager {
   public Image[] getPigRightArray() throws IOException {
		 int len = 4;
		 Image[] result = new Image[len];

		 for (int ix = 0; ix < len; ix++) {
			 int fileNumber = ix + 1;
			 System.out.println("ix=" + ix +", fileNumber=" + fileNumber);
		   result[ix] = Image.createImage("/characters/pig-right-" + fileNumber + ".png");
//				try {
//					Thread.sleep(10);
//				} catch (InterruptedException e) {
//				}
//			 result[ix] = Image.createImage("/characters/pig-right-1.png");
		 }

		 return result;
	 }
}
