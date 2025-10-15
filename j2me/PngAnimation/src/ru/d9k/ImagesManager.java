/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */
package ru.d9k;
import java.io.IOException;
import javax.microedition.lcdui.Image;

/**
 *
 * @author d9k
 */
public class ImagesManager {
   public Image[] getPigRightArray() throws IOException {
		 Image[] result = new Image[4];

		 for (int ix = 0; ix < 4; ix++) {
			 int fileNumber = ix + 1;
			 System.out.println("ix=" + ix +", fileNumber=" + fileNumber);
		   result[ix] = Image.createImage("/characters/pig-right-" + fileNumber + ".png");
		 }

		 return result;
	 }
}
